/**************************************************************************
 *
 * Copyright (c) 2018 Alexandru Istrate
 *
 * This file is subject to the terms and conditions defined in the
 * file 'LICENSE', which is part of this source code package.
 *
**************************************************************************/

#include "remoteauthtab.h"
#include "ui_remoteauthtab.h"

#include <QPushButton>

#include "networkmanager.h"

RemoteAuthTab::RemoteAuthTab(AccessType accesType, QWidget* parent) :
	QWidget(parent), ui(new Ui::RemoteAuthTab),
	m_AccesType { accesType }
{
	ui->setupUi(this);

	setupUI();
	setupSignalsAndSlots();
}

RemoteAuthTab::~RemoteAuthTab()
{
	delete ui;
}

void RemoteAuthTab::setupUI()
{
	if (m_AccesType == AccessType::ACCESSES_THIS)
		ui->btnMainAction->setText("Revoke");
	else
		ui->btnMainAction->setText("Disconnect");

	showClients();
}

void RemoteAuthTab::setupSignalsAndSlots()
{
	connect(ui->btnMainAction, &QPushButton::clicked, this, &RemoteAuthTab::onMainAction);
	connect(NetworkManager::instance(), m_AccesType == AccessType::ACCESSES_THIS ? &NetworkManager::toThisConnectionsUpdated : &NetworkManager::fromThisConnectionsUpdated,
			this, &RemoteAuthTab::onConnectionsUpdated);
}

int RemoteAuthTab::getSelectedRow()
{
	return ui->lstClients->currentRow();
}

bool RemoteAuthTab::hasSelection()
{
	return (getSelectedRow() != -1);
}

void RemoteAuthTab::addClient(const Client& client)
{
	m_Clients.append(client);
	ui->lstClients->addItem(client.getUnlocalizedName());
}

void RemoteAuthTab::clearClients()
{
	m_Clients.clear();
	ui->lstClients->clear();
}

void RemoteAuthTab::showClients()
{
	NetworkManager* instance = NetworkManager::instance();
	const QList<Client>& clients = (m_AccesType == AccessType::ACCESSES_THIS ? instance->getConnectedFromClients() : instance->getConnectedToClients());

	clearClients();

	for (const Client& client : clients)
		addClient(client);
}

void RemoteAuthTab::disconnectFromClient()
{
	if (!hasSelection())
		return;

	NetworkManager::instance()->disconnectFrom(m_Clients.at(getSelectedRow()));
}

void RemoteAuthTab::revokeRight()
{
	if (!hasSelection())
		return;

	NetworkManager::instance()->closeClientConnection(m_Clients.at(getSelectedRow()));
}

void RemoteAuthTab::onMainAction()
{
	if (m_AccesType == AccessType::ACCESSES_THIS)
		revokeRight();
	else
		disconnectFromClient();
}

void RemoteAuthTab::onConnectionsUpdated()
{
	showClients();
}
