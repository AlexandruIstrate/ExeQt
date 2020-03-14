/**************************************************************************
 *
 * Copyright (c) 2018 Alexandru Istrate
 *
 * This file is subject to the terms and conditions defined in the
 * file 'LICENSE', which is part of this source code package.
 *
**************************************************************************/

#include "mainpage.h"
#include "ui_mainpage.h"

#include <QPushButton>

#include "logindialog.h"
#include "mainwidget.h"
#include "stylemanager.h"
#include "settingsdialog.h"

MainPage::MainPage(QWidget* parent) :
	QDialog(parent),
	ui(new Ui::MainPage)
{
	ui->setupUi(this);
	setStyleSheet(StyleManager::instance()->getStyle());

	setupActions();
	setupSignalsAndSlots();
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
	connect(m_ActionSync, &QAction::triggered, this, &MainPage::onSync);

	m_ActionSettings = new QAction(tr("Settings"), this);
	connect(m_ActionSettings, &QAction::triggered, this, &MainPage::onOpenSettings);

	m_ActionContinue = new QAction(tr("Continue"), this);
	connect(m_ActionContinue, &QAction::triggered, this, &MainPage::onContinue);
}

void MainPage::setupSignalsAndSlots()
{
	connect(ui->btnSync, &QPushButton::clicked, m_ActionSync, &QAction::trigger);
	connect(ui->btnSettings, &QPushButton::clicked, m_ActionSettings, &QAction::trigger);
	connect(ui->btnContinue, &QPushButton::clicked, m_ActionContinue, &QAction::trigger);
}

void MainPage::onSync()
{
	LoginDialog logDialog(this);

	if (logDialog.exec() != QDialog::DialogCode::Accepted)
	{
		return;
	}

	onContinue();
}

void MainPage::onOpenSettings()
{
	SettingsDialog settingsDialog(this);
	settingsDialog.exec();
}

void MainPage::onContinue()
{
	hide();

	MainWidget* mw = new MainWidget();
	mw->show();
}
