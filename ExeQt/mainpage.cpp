#include "mainpage.h"
#include "ui_mainpage.h"

#include <QSettings>

#include "logindialog.h"
#include "mainwidget.h"
#include "stylemanager.h"

MainPage::MainPage(QWidget* parent) :
	QDialog(parent),
	ui(new Ui::MainPage)
{
	ui->setupUi(this);
	setStyleSheet(StyleManager::instance()->getStyle());

	setupActions();
	setupSignalsAndSlots();
	loadSettings();
}

MainPage::~MainPage()
{
	delete ui;
}

void MainPage::closeEvent(QCloseEvent*)
{
	qApp->quit();
}

void MainPage::setupActions()
{
	m_ActionSync = new QAction(tr("Sync"), this);
	connect(m_ActionSync, SIGNAL(triggered()), this, SLOT(onSync()));

	m_ActionContinue = new QAction(tr("Continue"), this);
	connect(m_ActionContinue, SIGNAL(triggered()), this, SLOT(onContinue()));
}

void MainPage::setupSignalsAndSlots()
{
	connect(ui->btnSync, SIGNAL(clicked()), m_ActionSync, SLOT(trigger()));
	connect(ui->btnContinue, SIGNAL(clicked()), m_ActionContinue, SLOT(trigger()));
}

void MainPage::loadSettings()
{
	QSettings settings(Constants::COMPANY_NAME, Constants::PRODUCT_NAME);
	Constants::SERVER_ADDRESS = settings.value(Constants::SETTING_KEY_IP, Constants::SERVER_ADDRESS).toString();
}

void MainPage::onSync()
{
	LoginDialog* logDialog = new LoginDialog(this);

	if (logDialog->exec() != QDialog::DialogCode::Accepted)
		return;

	hide();

	MainWidget* mw = new MainWidget();
	mw->show();
}

void MainPage::onContinue()
{
	MainWidget* mw = new MainWidget();
	mw->show();

	hide();
}
