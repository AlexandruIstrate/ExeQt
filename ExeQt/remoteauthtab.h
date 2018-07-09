/**************************************************************************
 *
 * Copyright (c) 2018 Alexandru Istrate
 *
 * This file is subject to the terms and conditions defined in the
 * file 'LICENSE', which is part of this source code package.
 *
**************************************************************************/

#ifndef REMOTEAUTHTAB_H
#define REMOTEAUTHTAB_H

#include <QWidget>
#include <QPushButton>

#include <QList>

class Client;

namespace Ui {
class RemoteAuthTab;
}

class RemoteAuthTab : public QWidget
{
	Q_OBJECT

public:
	enum class AccessType
	{
		ACCESSES_THIS, ACCESSES_OTHER
	};

private:
	Ui::RemoteAuthTab* ui;

	AccessType m_AccesType;
	QList<Client> m_Clients;

public:
	RemoteAuthTab(AccessType accesType, QWidget* parent = nullptr);
	~RemoteAuthTab();

private:
	void setupUI();
	void setupSignalsAndSlots();

	int getSelectedRow();
	bool hasSelection();

	void addClient(const Client&);
	void clearClients();
	void showClients();

	void disconnectFromClient();
	void revokeRight();

private slots:
	void onMainAction();
	void onConnectionsUpdated();
};

#endif // REMOTEAUTHTAB_H
