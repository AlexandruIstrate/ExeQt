/**************************************************************************
 *
 * Copyright (c) 2018 Alexandru Istrate
 *
 * This file is subject to the terms and conditions defined in the
 * file 'LICENSE', which is part of this source code package.
 *
**************************************************************************/

#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>

namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public QDialog
{
	Q_OBJECT

private:
	Ui::SettingsDialog* ui;

public:
	explicit SettingsDialog(QWidget* parent = nullptr);
	~SettingsDialog();

private:
	void setupUI();

	void writeSettings();
	void updateInternalSettings();

public slots:
	void accept() override;
};

#endif // SETTINGSDIALOG_H
