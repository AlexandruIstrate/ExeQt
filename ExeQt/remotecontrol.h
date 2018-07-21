/**************************************************************************
 *
 * Copyright (c) 2018 Alexandru Istrate
 *
 * This file is subject to the terms and conditions defined in the
 * file 'LICENSE', which is part of this source code package.
 *
**************************************************************************/

#ifndef REMOTECONTROL_H
#define REMOTECONTROL_H

#include <QDialog>

#include "remotecontroltab.h"

namespace Ui {
class RemoteControl;
}

class Client;

class RemoteControl : public QDialog
{
	Q_OBJECT

private:
	Ui::RemoteControl* ui;

public:
	explicit RemoteControl(QWidget* parent = nullptr);
	~RemoteControl();

private:
	void setupUI();
	void setupDialogButtons();
	void setupSignalsAndSlots();

	void addTab(Client&);

	void addTabs();
	void clearTabs();
	void showTabs();

private slots:
	void onClientsUpdated();

//	void onRefresh();
};

#endif // REMOTECONTROL_H
