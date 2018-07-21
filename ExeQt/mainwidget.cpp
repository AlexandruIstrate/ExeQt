/**************************************************************************
 *
 * Copyright (c) 2018 Alexandru Istrate
 *
 * This file is subject to the terms and conditions defined in the
 * file 'LICENSE', which is part of this source code package.
 *
**************************************************************************/

#include "mainwidget.h"

#include <QDebug>
#include <QStringLiteral>
#include <QDomDocument>
#include <QThread>

#include <QMenu>
#include <QImage>
#include <QMessageBox>
#include <QCloseEvent>
#include <QDesktopWidget>

#include <QJsonDocument>
#include <QJsonObject>

#include "ui_mainwidget.h"
#include "addtab.h"
#include "addgroupdialog.h"
#include "groupconfigure.h"
#include "addgroupdialog.h"
#include "authmanager.h"
#include "logindialog.h"
#include "settingsdialog.h"
#include "stylemanager.h"
#include "task.h"
#include "remotecontrol.h"
#include "remoteauthorizations.h"
#include "networkmanager.h"

#include "constants.h"
#include "common.h"

MainWidget* MainWidget::s_Instance = nullptr;

MainWidget::MainWidget(QWidget* parent) :
	QWidget(parent), ui(new Ui::MainWidget),
	m_ShouldQuit { false }
{
	s_Instance = this;
	NetworkManager::init();
	TaskManager::init();
	ActionServer::start();

	ui->setupUi(this);
	setStyleSheet(StyleManager::instance()->getStyle());

	setupNetwork();
	setupActions();
	setupUI();
	setupSignalsAndSlots();

	loadFromFile();

	m_RemoteControlDialog = new RemoteControl(this);
}

MainWidget::~MainWidget()
{
	TaskManager::terminate();
	NetworkManager::terminate();
	ActionServer::shutdown();

	delete m_RemoteControlDialog;
	delete ui;
}

QIcon MainWidget::getTabIcon(int index)
{
	return m_ActionTabs.at(index)->getIcon().icon;
}

void MainWidget::addNewActionGroup()
{
	AddGroupDialog* addDialog = new AddGroupDialog(this);
	if (addDialog->exec() == QDialog::DialogCode::Rejected)
		return;

	ActionTab* newTab = new ActionTab(addDialog->getName(), addDialog->getIcon(), this);
	addTab(newTab);

	ui->tabGroup->setCurrentWidget(newTab);
	ui->tabGroup->setTabEnabled(getTabCount() - 1, false);
}

void MainWidget::removeActionGroup(int index)
{
	ui->tabGroup->widget(index)->deleteLater();
	ui->tabGroup->removeTab(index);

	m_ActionTabs[index]->removeTrayIcon();
	m_ActionTabs.erase(m_ActionTabs.begin() + index);

	ui->tabGroup->setCurrentIndex(getSelectedTabIndex() - 1);

	if (getTabCount() == 1)
		ui->tabGroup->setTabEnabled(getTabCount() - 1, true);
}

QString MainWidget::getTagName() const
{
	return QString("mainWidget");
}

void MainWidget::readProperties(Bundle& bundle)
{
	if (!checkBundle(bundle))
		return;

	for (int i = 0; i < bundle.getChildrenCount(); ++i)
	{
		ActionTab* tab = new ActionTab("", AppIcon(), this);
		tab->readProperties(bundle.childAt(i));
		addTab(tab);
	}

	m_TrayIcon->showMessage(tr("Ready to Go"), tr("Loaded %1 groups!").arg(bundle.getChildrenCount()));
	TaskManager::instance()->run();
}

void MainWidget::writeProperties(Bundle& bundle)
{
	for (ActionTab* tab : m_ActionTabs)
	{
		Bundle child(tab->getTagName());
		tab->writeProperties(child);

		bundle.addChild(child);
	}
}

void MainWidget::closeEvent(QCloseEvent* event)
{
	hide();
	event->ignore();
}

void MainWidget::setupNetwork()
{
	m_RequestManager = new RequestManager();
	connect(m_RequestManager, SIGNAL(requestFinished(QNetworkReply*, bool)), this, SLOT(onRequestFinished(QNetworkReply*, bool)));
}

void MainWidget::setupActions()
{
	m_AddTabAction = new QAction(QIcon(":/assets/images/app-actions/add.png"), tr("Add Group"), this);
	connect(m_AddTabAction, SIGNAL(triggered()), this, SLOT(onAddTab()));

	m_RemoveTabAction = new QAction(QIcon(":/assets/images/app-actions/delete.png"), tr("Remove Group"), this);
	connect(m_RemoveTabAction, SIGNAL(triggered()), this, SLOT(onRemoveTab()));

	m_ConfigureAction = new QAction(QIcon(":/assets/images/app-actions/configure.png"), tr("Configure..."), this);
	connect(m_ConfigureAction, SIGNAL(triggered()), SLOT(onConfigure()));

	m_ConfigureGroupsAction = new QAction(QIcon(":/assets/images/app-actions/configure-action-groups.png"), tr("Configure Groups..."), this);
	connect(m_ConfigureGroupsAction, SIGNAL(triggered()), this, SLOT(onConfigureActionGroups()));

	m_SyncAction = new QAction(QIcon(":/assets/images/app-actions/cloud-connect.png"), tr("Sync..."), this);
	connect(m_SyncAction, SIGNAL(triggered()), this, SLOT(onSyncSelected()));

	m_RemoteControlAction = new QAction(QIcon(":/assets/images/app-actions/remote.png"), tr("Remote Control"), this);
	connect(m_RemoteControlAction, SIGNAL(triggered()), this, SLOT(onRemoteControlSelected()));

	m_AuthAction = new QAction(QIcon(":/assets/images/app-actions/auth.png"), tr("Authorizations"), this);
	connect(m_AuthAction, SIGNAL(triggered()), this, SLOT(onAuthorizationsSelected()));

	m_SettingsAction = new QAction(QIcon(":/assets/images/app-actions/settings.png"), tr("Settings..."), this);
	connect(m_SettingsAction, SIGNAL(triggered()), this, SLOT(onSettingsSelected()));

	m_QuitAction = new QAction(QIcon(":/assets/images/app-actions/quit.png"), tr("Quit"), this);
	connect(m_QuitAction, SIGNAL(triggered()), this, SLOT(onQuit()));
}

void MainWidget::setupSignalsAndSlots()
{
	connect(m_TrayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(onIconActivated(QSystemTrayIcon::ActivationReason)));
	connect(ui->tabGroup, SIGNAL(tabBarClicked(int)), this, SLOT(onTabSelected(int)));

	connect(ui->btnGroupConfigure, SIGNAL(clicked()), m_ConfigureGroupsAction, SLOT(trigger()));

	connect(NetworkManager::instance(), &NetworkManager::actionsCanUpdate, this, &MainWidget::onActionsCanUpdate);
}

void MainWidget::setupUI()
{
	setupTrayIcon();
	setupTabs();
	AddGroupDialog::initIcons();

	setGeometry(QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter, size(), qApp->desktop()->availableGeometry()));
}

void MainWidget::setupTrayIcon()
{
	QMenu* trayMenu = new QMenu(this);
	trayMenu->addAction(m_AddTabAction);
	trayMenu->addAction(m_RemoveTabAction);
	trayMenu->addAction(m_ConfigureAction);
	trayMenu->addAction(m_ConfigureGroupsAction);
	trayMenu->addMenu(createNetworkMenu());
	trayMenu->addAction(m_SettingsAction);
	trayMenu->addSeparator();
	trayMenu->addAction(m_QuitAction);

	m_TrayIcon = new QSystemTrayIcon(this);
	m_TrayIcon->setContextMenu(trayMenu);
	m_TrayIcon->setIcon(QIcon(":/assets/images/app-icon.png"));
	m_TrayIcon->show();
}

QMenu* MainWidget::createNetworkMenu()
{
	QMenu* menu = new QMenu(tr("Network"));
	menu->addAction(m_SyncAction);
	menu->addAction(m_RemoteControlAction);
	menu->addAction(m_AuthAction);

	return menu;
}

void MainWidget::setupTabs()
{
	AddTab* addTab = new AddTab(this);
	ui->tabGroup->setTabEnabled(ui->tabGroup->indexOf(addTab), false);
	ui->tabGroup->addTab(addTab, "+");
	connect(addTab, SIGNAL(createActionGroup()), m_AddTabAction, SLOT(trigger()));
}

void MainWidget::insertTab(ActionTab* tab)
{
	ui->tabGroup->insertTab(getTabCount() - 1, tab, tab->getIcon().icon, tab->getName());
	ui->tabGroup->setTabEnabled(getTabCount() - 1, false);
}

void MainWidget::addTab(ActionTab* tab)
{
	insertTab(tab);
	m_ActionTabs.push_back(tab);

	NetworkManager::instance()->requestActionUpdate();
}

void MainWidget::removeTab(ActionTab* tab)
{
	m_ActionTabs.removeOne(tab);
	tab->deleteLater();

	NetworkManager::instance()->requestActionUpdate();
}

void MainWidget::openGroupConfigureMenu()
{
	GroupConfigure* conf = new GroupConfigure(this);
	conf->exec();

	reloadTabs();
}

void MainWidget::reloadTabs()
{
	while (getTabCount() > 1)
		ui->tabGroup->removeTab(0);

	for (ActionTab* tab : m_ActionTabs)
		insertTab(tab);

	int selectedTab = getSelectedTabIndex();
	ui->tabGroup->setCurrentIndex(selectedTab < getTabCount() ? selectedTab : getTabCount() - 1);
}

void MainWidget::removeTabs()
{
	for (ActionTab* tab : m_ActionTabs)
		removeTab(tab);

	ui->tabGroup->clear();
}

void MainWidget::saveToFile()
{
	Bundle bundle(getTagName());
	writeProperties(bundle);

	bundle.saveToFile(Common::getSaveFilePath());
}

void MainWidget::loadFromFile()
{
	Bundle bundle = Bundle::fromFile(Common::getSaveFilePath());
	readProperties(bundle);
}

int MainWidget::getTabCount()
{
	return ui->tabGroup->count();
}

int MainWidget::getSelectedTabIndex()
{
	return ui->tabGroup->currentIndex();
}

QString MainWidget::parseResponse(const QString& jsonText)
{
	QJsonDocument doc(QJsonDocument::fromJson(jsonText.toUtf8()));

	QJsonObject jsonObj = doc.object();
	int flag = jsonObj[Constants::JSON_PROPERTY_FLAG].toInt();
	QString message = jsonObj[Constants::JSON_PROPERTY_MESSSAGE].toString();

	if (flag != Constants::FLAG_OK)
		QMessageBox::critical(this, tr("Upload Failed"), message);

	return message;
}

void MainWidget::quitLater()
{
	m_ShouldQuit = true;
}

void MainWidget::onIconActivated(QSystemTrayIcon::ActivationReason reason)
{
	switch (reason)
	{
	case QSystemTrayIcon::ActivationReason::Trigger:
		break;

	case QSystemTrayIcon::ActivationReason::DoubleClick:
		show();
		break;

	case QSystemTrayIcon::ActivationReason::MiddleClick:
		break;

	default:
		break;
	}
}

void MainWidget::onTabSelected(int index)
{
	if (index == ui->tabGroup->count() - 1 || index == -1) // Add a new tab if we selected the add tab, which is always last
		addNewActionGroup();
}

void MainWidget::onAddTab()
{
	show();
	addNewActionGroup();
}

void MainWidget::onRemoveTab()
{
	openGroupConfigureMenu();
}

void MainWidget::onConfigure()
{
	show();
}

void MainWidget::onConfigureActionGroups()
{
	openGroupConfigureMenu();
}

void MainWidget::onSyncSelected()
{
	if (!AuthManager::instance()->isAuth())
	{
		LoginDialog* loginDialog = new LoginDialog(this);
		if (loginDialog->exec() != QDialog::DialogCode::Accepted)
			return;
	}
	else
	{
		AuthManager::instance()->syncActions();
	}

	removeTabs();
	setupTabs();

	loadFromFile();
}

void MainWidget::onRemoteControlSelected()
{
	m_RemoteControlDialog->exec();
}

void MainWidget::onAuthorizationsSelected()
{
	RemoteAuthorizations* remoteAuth = new RemoteAuthorizations(this);
	remoteAuth->exec();
}

void MainWidget::onSettingsSelected()
{
	SettingsDialog* settingsDialog = new SettingsDialog(this);
	settingsDialog->exec();
}

void MainWidget::onQuit()
{
	if (m_ShouldQuit)	// The user has already selected "Quit". We are now waiting for the cloud sync to finish.
		return;

	QMessageBox dialog(QMessageBox::Icon::Question, tr("Exit"), tr("Are you sure you want to exit?"), QMessageBox::Yes | QMessageBox::No);
	if (dialog.exec() == QMessageBox::No)
		return;

	saveToFile();

	if (AuthManager::instance()->isAuth())
	{
		m_TrayIcon->showMessage(tr("Closing down..."), tr("ExeQt is saving your actions to the cloud an will close in a few moments..."));
		m_RequestManager->uploadFile(QUrl(Constants::getUploadPath()), Common::getSaveFilePath());	// Overwrite old cloud file with new one
	}
	else
	{
		qApp->quit();
	}

	quitLater();
}

void MainWidget::onRequestFinished(QNetworkReply* reply, bool timedOut)
{
	if (timedOut)
	{
		QMessageBox::critical(this, tr("Timed Out"), tr("Request timed out."));
		return;
	}

	if (reply->error() != QNetworkReply::NetworkError::NoError)
	{
		QMessageBox::critical(this, tr("Error"), reply->errorString());
	}

	parseResponse(reply->readAll());

	if (m_ShouldQuit)
		qApp->quit();
}

void MainWidget::onActionsCanUpdate()
{
	if (!NetworkManager::instance()->hasConnectedClients())
		return;

	qDebug() << "Updating actions";

	Bundle bundle(getTagName());
	writeProperties(bundle);

	NetworkManager::instance()->updateActions(bundle);
}
