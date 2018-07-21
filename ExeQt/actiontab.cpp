/**************************************************************************
 *
 * Copyright (c) 2018 Alexandru Istrate
 *
 * This file is subject to the terms and conditions defined in the
 * file 'LICENSE', which is part of this source code package.
 *
**************************************************************************/

#include "actiontab.h"
#include "ui_actiontab.h"

#include <QDebug>
#include <QListWidgetItem>
#include <QMessageBox>

#include "commandaction.h"
#include "menuaction.h"
#include "appicon.h"
#include "addgroupdialog.h"
#include "networkmanager.h"
#include "settingsregistry.h"

#define NAME_PROPERTY           "name"
#define ICON_PROPERTY           "icon"

ActionTab::ActionTab(const QString& name, const AppIcon& icon, QWidget* parent) :
	QWidget(parent), ui(new Ui::ActionTab),
	m_Name { name }, m_Icon { icon }
{
	ui->setupUi(this);

	setupUI();
	setupActions();
	setupSignalsAndSlots();
}

ActionTab::~ActionTab()
{
	delete m_TrayIcon;
	delete ui;
}

void ActionTab::setName(const QString& name)
{
	m_Name = name;
}

void ActionTab::setIcon(const AppIcon& icon)
{
	m_Icon = icon;
	m_TrayIcon->setIcon(m_Icon.icon);
}

QString ActionTab::getTagName() const
{
	return QString("actionTab");
}

bool ActionTab::checkBundle(const Bundle& bundle) const
{
	if (!checkProperty(bundle, NAME_PROPERTY) || !checkProperty(bundle, ICON_PROPERTY))
		return false;

	return true;
}

void ActionTab::readProperties(Bundle& bundle)
{
//	if (!checkBundle(bundle))
//		return;

	m_Name = bundle.get(NAME_PROPERTY);
	m_Icon = AddGroupDialog::getIconByName(bundle.get(ICON_PROPERTY));
	m_TrayIcon->setIcon(m_Icon.icon);

	for (int i = 0; i < bundle.getChildrenCount(); ++i)
	{
		ActionItem* action = new ActionItem(this);
		action->readProperties(bundle.childAt(i));

		addAction(action);
	}
}

void ActionTab::writeProperties(Bundle& bundle)
{
	bundle.add(NAME_PROPERTY, m_Name);
	bundle.add(ICON_PROPERTY, m_Icon.name);

	for (ActionItem* item : m_ActionItems)
	{
		Bundle child(item->getAction()->getTagName());
		item->writeProperties(child);

		bundle.addChild(child);
	}
}

void ActionTab::removeTrayIcon()
{
	m_TrayIcon->setVisible(false);
}

bool ActionTab::checkDelete()
{
	if (!SettingsRegistry::instance()->get(Settings::CONFIRM_DELETE).toBool())
		return true;

	QMessageBox dialog(QMessageBox::Icon::Question, tr("Remove Action"),
					   tr("Are you sure you want to remove this action?"), QMessageBox::Yes | QMessageBox::No, this);
	return dialog.exec() == QMessageBox::Yes;
}

void ActionTab::setupActions()
{
	m_AddAction = new QAction(tr("Add"), this);
	connect(m_AddAction, SIGNAL(triggered()), this, SLOT(onActionAdd()));

	m_RemoveAction = new QAction(tr("Remove"), this);
	connect(m_RemoveAction, SIGNAL(triggered()), this, SLOT(onActionRemove()));

	m_EditAction = new QAction(tr("Edit"), this);
	connect(m_EditAction, SIGNAL(triggered()), this, SLOT(onActionEdit()));

	addGhostAction();
}

void ActionTab::setupSignalsAndSlots()
{
	connect(ui->btnAdd, SIGNAL(clicked()), m_AddAction, SLOT(trigger()));
	connect(ui->btnRemove, SIGNAL(clicked()), m_RemoveAction, SLOT(trigger()));
	connect(ui->btnEdit, SIGNAL(clicked()), m_EditAction, SLOT(trigger()));

	connect(ui->actionList, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(onDoubleClick()));
}

void ActionTab::setupUI()
{
	setupTrayIcon();
	ui->actionList->setSelectionMode(QAbstractItemView::SingleSelection);
}

void ActionTab::setupTrayIcon()
{
	m_TrayMenu = new QMenu();

	m_TrayIcon = new QSystemTrayIcon(this);
	m_TrayIcon->setContextMenu(m_TrayMenu);
	m_TrayIcon->setIcon(m_Icon.icon);
	m_TrayIcon->show();
}

void ActionTab::addAction(ActionItem* action)
{
	m_ActionItems.push_back(action);
	ui->actionList->addItem(action->getName());

	updateActions();
}

void ActionTab::removeAction(ActionItem* action)
{
	for (int i = 0; i < m_ActionItems.size(); ++i)
	{
		if (m_ActionItems[i] == action)
		{
			m_ActionItems.erase(m_ActionItems.begin() + i);
			break;
		}
	}

	updateActions();
}

void ActionTab::updateActions()
{
	ui->actionList->clear();
	m_TrayMenu->clear();

	for (ActionItem* actionItem : m_ActionItems)
	{
		Action* action = actionItem->getAction();
		ui->actionList->addItem(new QListWidgetItem(Action::getActionIcon(action->getType()), action->getName()));

		MenuAction* menuAct = new MenuAction(action, this);
		m_TrayMenu->addAction(menuAct);
	}
}

void ActionTab::addGhostAction()
{
	m_GhostAction = new QAction("No Actions");
	m_GhostAction->setEnabled(false);
	m_TrayMenu->addAction(m_GhostAction);
}

void ActionTab::removeGhostAction()
{
	if (m_GhostAction)
	{
		m_TrayMenu->removeAction(m_GhostAction);
		m_GhostAction->deleteLater();

		m_GhostAction = nullptr;
	}
}

void ActionTab::onActionAdd()
{
	ActionItem* action = new ActionItem(this);
	if (!action->exec())
		return;

	removeGhostAction();
	addAction(action);

	NetworkManager::instance()->requestActionUpdate();
}

void ActionTab::onActionRemove()
{
	int line = ui->actionList->currentRow();
	if (line == -1)
		return;

	if (!checkDelete())
		return;

	removeAction(m_ActionItems[line]);

	NetworkManager::instance()->requestActionUpdate();
}

void ActionTab::onActionEdit()
{
	int line = ui->actionList->currentRow();
	if (line == -1)
		return;

	m_ActionItems[line]->exec();
	updateActions();

	NetworkManager::instance()->requestActionUpdate();
}

void ActionTab::onDoubleClick()
{
	m_EditAction->trigger();
}
