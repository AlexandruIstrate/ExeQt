/**************************************************************************
 *
 * Copyright (c) 2018 Alexandru Istrate
 *
 * This file is subject to the terms and conditions defined in the
 * file 'LICENSE', which is part of this source code package.
 *
**************************************************************************/

#include "remotecontrol.h"
#include "ui_remotecontrol.h"

#include <QPushButton>

#include "networkmanager.h"

RemoteControl::RemoteControl(QWidget* parent) :
	QDialog(parent),
	ui(new Ui::RemoteControl)
{
	ui->setupUi(this);
	setupSignalsAndSlots();
}

RemoteControl::~RemoteControl()
{
	delete ui;
}

void RemoteControl::setupSignalsAndSlots()
{
	connect(NetworkManager::instance(), &NetworkManager::fromThisConnectionsUpdated, this, &RemoteControl::onClientsUpdated);

//	connect(ui->btnRefresh, &QPushButton::clicked, this, &RemoteControl::onRefresh);
}

void RemoteControl::addTab(Client& client)
{
	ui->tabGrpClients->addTab(new RemoteControlTab(client, this), QIcon(":/assets/images/remote-control.png"), client.getUnlocalizedName());
}

void RemoteControl::addTabs()
{
	for (Client& client : NetworkManager::instance()->getConnectedToClients())
		addTab(client);
}

void RemoteControl::clearTabs()
{
	ui->tabGrpClients->clear();
}

void RemoteControl::showTabs()
{
	clearTabs();
	addTabs();
}

void RemoteControl::onClientsUpdated()
{
	showTabs();
}

//void RemoteControl::onRefresh()
//{
//	showTabs();
//}
