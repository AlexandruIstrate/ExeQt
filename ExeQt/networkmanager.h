/**************************************************************************
 *
 * Copyright (c) 2018 Alexandru Istrate
 *
 * This file is subject to the terms and conditions defined in the
 * file 'LICENSE', which is part of this source code package.
 *
**************************************************************************/

#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <QObject>
#include <QString>
#include <QList>
#include <QTimer>

#include <QUdpSocket>
#include <QTcpSocket>
#include <QTcpServer>

#include "saveable.h"

class ActionReference;
class SettingsRegistry;

class Client : public QObject
{
	Q_OBJECT

private:
	QString m_Name;
	QString m_ID;
	QString m_Address;	// TODO: Remove this field and use the socket for getting the IP address

	QTcpSocket* m_ConnectSocket;

public:
	Client();
	Client(const QString& name, const QString& id, const QString& address, QTcpSocket* socket = nullptr);
	Client(const Client&);

	~Client();

	inline const QString& getName() const { return m_Name; }
	inline void setName(const QString& name) { m_Name = name; }

	inline const QString& getID() const { return m_ID; }
	inline const QString& getAddress() const { return m_Address; }

	inline QTcpSocket* getSocket() const { return m_ConnectSocket; }
	void setSocket(QTcpSocket* socket);

	QString getLocalizedName() const;
	QString getUnlocalizedName() const;

	void disconnectFromHost() const;

	void updateActions(const Bundle& actionBundle) const;
	void callAction(const ActionReference& reference);

	bool isValid() const;
	bool isClient(const Client&) const;

	bool operator==(const Client&) const;
	bool operator!=(const Client&) const;

	Client& operator=(const Client&);

	static QNetworkInterface* getCurrentInterface();
	static QString getCurrentIPAddress();

	static QString getHardwareID();

private:
	QString generateID();

signals:
	void actionsUpdated(Bundle bundle);

private slots:
	void onReadyRead();
};

class WritableSocket : public QObject
{
	Q_OBJECT

private:
	QTcpSocket* m_Socket;
	QByteArray m_Data;

public:
	WritableSocket(const QString& address, int port);
	WritableSocket();

	inline QTcpSocket* getSocket() { return m_Socket; }

	void connectToHost(const QString& address, int port);
	void write(const QByteArray& data);

private slots:
	void onConnect();
	void onError(QAbstractSocket::SocketError);
};

class NetworkManager : public QObject
{
	Q_OBJECT

private:
	friend class ActionServer;
	friend class RemoteAuthTab;

public:
	enum class State
	{
		IDLE, BROADCASTING, RECEIVING
	};

private:
	static NetworkManager* s_Instance;

	Client* m_ThisClient;
	QList<Client> m_ConnectedTo;
	QList<Client> m_ConnectedFrom;

	QUdpSocket m_UdpSocket;
	WritableSocket m_WritableSocket;

	QTimer m_BroadcastTimer;

	State m_State;
	bool m_ActionsUpToDate;

private:
	NetworkManager();
	~NetworkManager();

public:
	inline static NetworkManager* instance() { return s_Instance; }

	inline Client* getThisClient() { return m_ThisClient; }

	inline QList<Client>& getConnectedToClients() { return m_ConnectedTo; }
	inline const QList<Client>& getConnectedToClients() const { return m_ConnectedTo; }

	inline QList<Client>& getConnectedFromClients() { return m_ConnectedFrom; }
	inline const QList<Client>& getConnectedFromClients() const { return m_ConnectedFrom; }

	inline State getState() const { return m_State; }

	static void init();
	static void terminate();

	void requestActionUpdate();
	void updateActions(const Bundle&);

	void startBroadcast();
	void stopBroadcast();

	bool connectTo(Client);
	bool disconnectFrom(const Client&);

	bool hasConnectedClients() const;
	bool isConnectedToOthers() const;

	Client* getClientFromID(const QString&);

private:
	void initNetwork();
	void initTimer();

	void setState(State);

	bool connectToInternal(Client&);
	bool disconnectFromInternal(const Client&);

	QByteArray buildBroadcastDatagram();
	QByteArray buildConnectMessage(const Client& client);

	QString getIDFromDatagram(const QByteArray&);

	void acceptClientConnection(const Client&);
	void closeClientConnection(const Client&);

signals:
	void clientAvailable(Client);

	void toThisConnectionsUpdated();
	void fromThisConnectionsUpdated();

	void actionsCanUpdate();

private slots:
	void onBroadcastDatagram();
	void onPendingDatagram();

	void onSettingsUpdate(SettingsRegistry* settingsRegistry);
};

class ActionServer : public QObject
{
	Q_OBJECT

private:
	static ActionServer* s_Instance;

	QTcpServer m_TcpServer;

private:
	ActionServer();

public:
	inline static ActionServer* instance() { return s_Instance; }

	static void start();
	static void shutdown();

	void setListening(bool listening);

private:
	void setupSignalsAndSlots();

	void parseRequest(QTcpSocket* socket);
	void disconnectSocket(QTcpSocket* socket);

signals:
	void connectedToClient();
	void disconnectedFromClient();

private slots:
	void onNewConnection();
	void onDisconnect();
	void onReadyRead();
};

#endif // NETWORKMANAGER_H
