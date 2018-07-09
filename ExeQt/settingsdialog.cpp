/**************************************************************************
 *
 * Copyright (c) 2018 Alexandru Istrate
 *
 * This file is subject to the terms and conditions defined in the
 * file 'LICENSE', which is part of this source code package.
 *
**************************************************************************/

#include "settingsdialog.h"
#include "ui_settingsdialog.h"

#include "constants.h"
#include "networkmanager.h"
#include "mainwidget.h"

#include <QSettings>

SettingsDialog::SettingsDialog(QWidget* parent) :
	QDialog(parent),
	ui(new Ui::SettingsDialog)
{
	ui->setupUi(this);
	setupUI();
}

SettingsDialog::~SettingsDialog()
{
	delete ui;
}

void SettingsDialog::setupUI()
{
	ui->edtAddress->setText(Constants::s_ServerAddress);
	ui->edtClientName->setText(NetworkManager::instance()->getThisClient()->getName());
}

void SettingsDialog::writeSettings()
{
	QSettings& settings = MainWidget::instance()->getSettings();
	settings.setValue(Constants::SETTING_KEY_IP, ui->edtAddress->text());
	settings.setValue(Constants::SETTING_KEY_CLIENT_NAME, ui->edtClientName->text());
	settings.sync();
}

void SettingsDialog::updateInternalSettings()
{
	Constants::s_ServerAddress = ui->edtAddress->text();
	NetworkManager::instance()->getThisClient()->setName(ui->edtClientName->text());
}

void SettingsDialog::accept()
{
	writeSettings();
	updateInternalSettings();

	QDialog::accept();
}
