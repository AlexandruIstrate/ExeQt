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

#include <QFileDialog>

#include "constants.h"
#include "networkmanager.h"
#include "mainwidget.h"
#include "settingsregistry.h"
#include "common.h"

SettingsDialog::SettingsDialog(QWidget* parent) :
	QDialog(parent), ui(new Ui::SettingsDialog)
{
	ui->setupUi(this);

	setupUI();
	setupSignalsAndSlots();
}

SettingsDialog::~SettingsDialog()
{
	delete ui;
}

void SettingsDialog::setupUI()
{
	SettingsRegistry* settings = SettingsRegistry::instance();
	ui->edtSavePath->setText(Common::getSaveFilePath());
	ui->edtAddress->setText(settings->get(Settings::CLOUD_SYNC_ADDRESS).toString());
	ui->edtClientName->setText(settings->get(Settings::USERNAME).toString());
}

void SettingsDialog::setupSignalsAndSlots()
{
	connect(ui->edtSavePath, &QLineEdit::textChanged, this, &SettingsDialog::onSavePathChanged);
	connect(ui->btnBrowse, &QPushButton::clicked, this, &SettingsDialog::onBrowse);
}

void SettingsDialog::writeSettings()
{
	SettingsRegistry::instance()->sync();
}

void SettingsDialog::updateInternalSettings()
{
	SettingsRegistry* settings = SettingsRegistry::instance();
	settings->set(Settings::SAVE_PATH, ui->edtSavePath->text());
	settings->set(Settings::CLOUD_SYNC_ADDRESS, ui->edtAddress->text());
	settings->set(Settings::USERNAME, ui->edtClientName->text());
}

void SettingsDialog::setUIPathState(bool ok)
{
	ui->lblPathError->setText(ok ? "" : tr("Invalid save path!"));
	ui->buttonBox->button(QDialogButtonBox::StandardButton::Ok)->setEnabled(ok);
}

void SettingsDialog::accept()
{
	updateInternalSettings();
	writeSettings();

	QDialog::accept();
}

void SettingsDialog::onBrowse()
{
	QFileDialog dialog;
	dialog.setAcceptMode(QFileDialog::AcceptMode::AcceptSave);
	dialog.setFileMode(QFileDialog::FileMode::AnyFile);

	if (dialog.exec() == QDialog::DialogCode::Rejected)
		return;

	ui->edtSavePath->setText(dialog.selectedFiles().at(0));
}

void SettingsDialog::onSavePathChanged(const QString& newPath)
{
	setUIPathState(QFileInfo(newPath).dir().exists());
}
