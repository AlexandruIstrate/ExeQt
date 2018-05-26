#include "settingsdialog.h"
#include "ui_settingsdialog.h"

#include "constants.h"

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
	ui->edtAddress->setText(Constants::SERVER_ADDRESS);
}

void SettingsDialog::writeSettings()
{
	QSettings settings(Constants::COMPANY_NAME, Constants::PRODUCT_NAME);
	settings.setValue(Constants::SETTING_KEY_IP, ui->edtAddress->text());
	settings.sync();
}

void SettingsDialog::updateInternalSettings()
{
	Constants::SERVER_ADDRESS = ui->edtAddress->text();
}

void SettingsDialog::accept()
{
	writeSettings();
	updateInternalSettings();

	QDialog::accept();
}
