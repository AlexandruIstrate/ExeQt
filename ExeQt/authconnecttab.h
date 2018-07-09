/**************************************************************************
 *
 * Copyright (c) 2018 Alexandru Istrate
 *
 * This file is subject to the terms and conditions defined in the
 * file 'LICENSE', which is part of this source code package.
 *
**************************************************************************/

#ifndef AUTHCONNECTTAB_H
#define AUTHCONNECTTAB_H

#include <QWidget>

#include "networkmanager.h"

namespace Ui {
class AuthConnectTab;
}

class AuthConnectTab : public QWidget
{
	Q_OBJECT

private:
	Ui::AuthConnectTab* ui;

	QList<Client> m_Clients;

public:
	explicit AuthConnectTab(QWidget* parent = nullptr);
	~AuthConnectTab();

private:
	void setupSignalsAndSlots();
	void initUI();

	void setButtonText(const QString& text);
	void setInfo(const QString& info);

	void setUIState(NetworkManager::State);

	int getSelectedIndex();
	int getClientIndex(const Client& client);

	void addClient(const Client&);
	void removeClient(const Client&);
	void clearClients();

	void startBroadcast();
	void stopBroadcast();

private slots:
	void onBroadcast();
	void onClientAvailable(Client);

	void onConnect();
	void onGetInfo();

	void onDoubleClick(const QModelIndex&);
};

#endif // AUTHCONNECTTAB_H
