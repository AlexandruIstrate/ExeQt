/**************************************************************************
 *
 * Copyright (c) 2018 Alexandru Istrate
 *
 * This file is subject to the terms and conditions defined in the
 * file 'LICENSE', which is part of this source code package.
 *
**************************************************************************/

#include "authconnecttab.h"
#include "ui_authconnecttab.h"

#include <QListWidget>

#include "clientinfodialog.h"

#define MSG_BROADCASTING			tr("Check your other devices. This device should show up in the \"Connect\" tab.")
#define MSG_IDLE					tr("Press the button to become discoverable.")

#define BTN_BROADCASTING			tr("Stop Broadcasting")
#define BTN_IDLE					tr("Broadcast")

#define AUTO_REFRESH_INTERVAL		3000

AuthConnectTab::AuthConnectTab(QWidget* parent) :
	QWidget(parent),
	ui(new Ui::AuthConnectTab)
{
	ui->setupUi(this);

	initUI();
	setupSignalsAndSlots();

	initTimer();
}

AuthConnectTab::~AuthConnectTab()
{
	delete ui;
}

void AuthConnectTab::setupSignalsAndSlots()
{
	connect(ui->btnBroadcast, &QPushButton::clicked, this, &AuthConnectTab::onBroadcast);
	connect(NetworkManager::instance(), &NetworkManager::clientAvailable, this, &AuthConnectTab::onClientAvailable);

	connect(ui->btnConnect, &QPushButton::clicked, this, &AuthConnectTab::onConnect);
	connect(ui->btnInfo, &QPushButton::clicked, this, &AuthConnectTab::onGetInfo);

	connect(ui->lstDevices, &QListWidget::doubleClicked, this, &AuthConnectTab::onDoubleClick);

	connect(&m_RefreshTimer, &QTimer::timeout, this, &AuthConnectTab::onAutoRefresh);
}

void AuthConnectTab::initUI()
{
	setUIState(NetworkManager::instance()->getState());
}

void AuthConnectTab::initTimer()
{
	m_RefreshTimer.setInterval(AUTO_REFRESH_INTERVAL);
	m_RefreshTimer.setSingleShot(false);
}

void AuthConnectTab::setButtonText(const QString& text)
{
	ui->btnBroadcast->setText(text);
}

void AuthConnectTab::setInfo(const QString& info)
{
	ui->lblInfo->setText(info);
}

void AuthConnectTab::setUIState(NetworkManager::State state)
{
	if (state == NetworkManager::State::IDLE)
	{
		setButtonText(BTN_IDLE);
		setInfo(MSG_IDLE);
	}
	else
	{
		setButtonText(BTN_BROADCASTING);
		setInfo(MSG_BROADCASTING);
	}
}

int AuthConnectTab::getSelectedIndex()
{
	return ui->lstDevices->currentRow();
}

int AuthConnectTab::getClientIndex(const Client& client)
{
	for (int i = 0; i < m_Clients.size(); ++i)
	{
		if (m_Clients[i] == client)
			return i;
	}

	return -1;
}

void AuthConnectTab::addClient(const Client& client)
{
	m_Clients.append(client);
	ui->lstDevices->addItem(client.getUnlocalizedName());
}

void AuthConnectTab::removeClient(const Client& client)
{
	m_Clients.removeOne(client);

	int index = getClientIndex(client);
	if (index != -1)
		delete ui->lstDevices->takeItem(index);
}

void AuthConnectTab::clearClients()
{
	m_Clients.clear();
	ui->lstDevices->clear();
}

void AuthConnectTab::startBroadcast()
{
	clearClients();
	NetworkManager::instance()->startBroadcast();

	setUIState(NetworkManager::State::BROADCASTING);
}

void AuthConnectTab::stopBroadcast()
{
	NetworkManager::instance()->stopBroadcast();
	clearClients();

	setUIState(NetworkManager::State::IDLE);
}

void AuthConnectTab::onBroadcast()
{
	if (NetworkManager::instance()->getState() == NetworkManager::State::IDLE)
		startBroadcast();
	else
		stopBroadcast();
}

void AuthConnectTab::onClientAvailable(Client client)
{
	m_RefreshTimer.start();	// Start or restart the timer

	// If the client is not the current device
	if (NetworkManager::instance()->getThisClient()->getLocalizedName() == client.getLocalizedName())
		return;

	// If already connected, skip it
	if (NetworkManager::instance()->getConnectedToClients().contains(client))
		return;

	// If the client is in the list, skip it
	if (getClientIndex(client) != -1)
		return;

	addClient(client);
}

void AuthConnectTab::onConnect()
{
	int index = getSelectedIndex();
	if (index == -1)
		return;

	NetworkManager::instance()->connectTo(m_Clients[index]);

	clearClients();
}

void AuthConnectTab::onGetInfo()
{
	int index = getSelectedIndex();
	if (index == -1)
		return;

	const Client& client = m_Clients.at(index);

	ClientInfoDialog infoDialog(client.getName(), client.getID(), client.getAddress(), this);
	infoDialog.exec();
}

void AuthConnectTab::onAutoRefresh()
{
	clearClients();
}

void AuthConnectTab::onDoubleClick(const QModelIndex&)
{
	onConnect();
}
