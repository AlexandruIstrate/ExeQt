/**************************************************************************
 *
 * Copyright (c) 2018 Alexandru Istrate
 *
 * This file is subject to the terms and conditions defined in the
 * file 'LICENSE', which is part of this source code package.
 *
**************************************************************************/

#include "networkmanager.h"

#include <QDebug>
#include <QByteArray>

#include <QMessageBox>

#include <QNetworkInterface>
#include <QCryptographicHash>
#include <QAbstractSocket>

#include "constants.h"
#include "networkmessage.h"
#include "actionreference.h"
#include "mainwidget.h"

#define PORT						45454
#define TIMER_INTERVAL				1000

#define JSON_KEY_NAME				"name"
#define JSON_KEY_ID					"id"
#define JSON_KEY_ACTIONS			"actions"
#define JSON_KEY_IDENTIFIER			"appIdentifier"
#define JSON_KEY_ACTION_TO_RUN		"actionToRun"
#define JSON_KEY_OPERATION			"operation"

#define JSON_APP_IDENTIFIER			"ExeQt"

#define JSON_CONNECT_OPERATION		"accept"
#define JSON_DISCONNECT_OPERATION	"reject"

// Client

Client::Client()
	: m_Name { MainWidget::instance()->getSettings().value(Constants::SETTING_KEY_CLIENT_NAME).toString() }, m_ID { generateID() }, m_Address { getCurrentIPAddress() }
{

}

Client::Client(const QString& name, const QString& id, const QString& address, QTcpSocket* socket) :
	m_Name { name }, m_ID { id }, m_Address { address }, m_ConnectSocket { socket }
{
	if (m_ConnectSocket)
		connect(m_ConnectSocket, &QTcpSocket::readyRead, this, &Client::onReadyRead);
}

Client::Client(const Client& other)
	: QObject {}, m_Name { other.m_Name }, m_ID { other.m_ID }, m_Address { other.m_Address }, m_ConnectSocket { other.m_ConnectSocket }
{
	if (m_ConnectSocket)
		connect(m_ConnectSocket, &QTcpSocket::readyRead, this, &Client::onReadyRead);
}

Client::~Client()
{

}

void Client::setSocket(QTcpSocket* socket)
{
	m_ConnectSocket = socket;
	connect(m_ConnectSocket, &QTcpSocket::readyRead, this, &Client::onReadyRead);
}

QString Client::getLocalizedName() const
{
	return QString("%1 @ %2").arg(m_ID, m_Address);
}

QString Client::getUnlocalizedName() const
{
	return QString("%1 @ %2").arg(m_Name, m_Address);
}

void Client::disconnectFromHost() const
{
	qDebug() << "Disconnected from host";

	NetworkMessage message(JSON_APP_IDENTIFIER);
	message.setProperty(JSON_KEY_OPERATION, JSON_DISCONNECT_OPERATION);

	m_ConnectSocket->write(message.buildMessage().toUtf8());
	m_ConnectSocket->disconnectFromHost();
}

void Client::updateActions(const Bundle& actionBundle) const
{
	if (!m_ConnectSocket)
		return;

	NetworkMessage message(JSON_APP_IDENTIFIER);
	message.setProperty(JSON_KEY_ACTIONS, actionBundle.toXML());

	m_ConnectSocket->write(message.buildMessage().toUtf8());
}

void Client::callAction(const ActionReference& reference)
{
	if (!m_ConnectSocket)
		return;

	NetworkMessage message(JSON_APP_IDENTIFIER);
	message.setProperty(JSON_KEY_ACTION_TO_RUN, reference.toString());

	m_ConnectSocket->write(message.buildMessage().toUtf8());
}

bool Client::isValid() const
{
	return !m_ID.isEmpty() && !m_Address.isEmpty();
}

bool Client::isClient(const Client& other) const
{
	return (m_Name == other.m_Name && m_ID == other.m_ID && m_Address == other.m_Address && m_ConnectSocket == other.m_ConnectSocket);
}

bool Client::operator==(const Client& other) const
{
	return (m_Name == other.m_Name && m_ID == other.m_ID && m_Address == other.m_Address);
}

bool Client::operator!=(const Client& other) const
{
	return !(other == *this);
}

Client& Client::operator=(const Client& other)
{
	if (this == &other)
		return *this;

	m_Name = other.m_Name;
	m_ID = other.m_ID;
	m_Address = other.m_Address;
	m_ConnectSocket = other.m_ConnectSocket;

	return *this;
}

QNetworkInterface* Client::getCurrentInterface()
{
	for (QNetworkInterface& interface : QNetworkInterface::allInterfaces())
	{
		if (!(interface.flags() & QNetworkInterface::IsLoopBack))
			return &interface;
	}

	return nullptr;
}

QString Client::getCurrentIPAddress()
{
	for (const QHostAddress& address : QNetworkInterface::allAddresses())
	{
		if (address.protocol() == QAbstractSocket::IPv4Protocol && address != QHostAddress(QHostAddress::LocalHost))
			 return address.toString();
	}

	return QString();
}

QString Client::generateID()
{
	// BUG: This causes the app to crash sometimes for some reason
	return QString(QCryptographicHash::hash(getHardwareID().toLatin1(), QCryptographicHash::Md5).toHex());
}

QString Client::getHardwareID()
{
	return getCurrentInterface() ? getCurrentInterface()->hardwareAddress() : QString();
}

void Client::onReadyRead()
{
	QTcpSocket* socket = static_cast<QTcpSocket*>(QObject::sender());

	NetworkMessage message(JSON_APP_IDENTIFIER, socket->readAll());

	if (message.hasError())
		return;

	if (message.hasProperty(JSON_KEY_ACTIONS))
	{
		QString bundle = message.getProperty(JSON_KEY_ACTIONS);

		qDebug() << "Actions recieved:" << bundle;

		if (bundle.isEmpty())
			return;

		emit actionsUpdated(Bundle::fromXML(bundle));
	}
	else if(message.hasProperty(JSON_KEY_ACTION_TO_RUN))
	{
		ActionReference ref(message.getProperty(JSON_KEY_ACTION_TO_RUN));
		if (!ref.isOk())
		{
			qDebug() << "Invalid remote action reference!";
			return;
		}

		ref.getAction()->execute();
	}
	else if (message.getProperty(JSON_KEY_OPERATION) == JSON_DISCONNECT_OPERATION)
	{
		NetworkManager::instance()->disconnectFrom(*this);
	}
	else if (message.getProperty(JSON_KEY_OPERATION) == JSON_CONNECT_OPERATION)
	{

	}
}

// WritableSocket

WritableSocket::WritableSocket(const QString& address, int port) : WritableSocket()
{
	connectToHost(address, port);
}

WritableSocket::WritableSocket()
{
	m_Socket = new QTcpSocket();
	connect(m_Socket, &QTcpSocket::connected, this, &WritableSocket::onConnect);
	connect(m_Socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(onError(QAbstractSocket::SocketError)));
}

void WritableSocket::connectToHost(const QString& address, int port)
{
	m_Socket->connectToHost(address, port);
}

void WritableSocket::write(const QByteArray& data)
{
	m_Data = data;
}

void WritableSocket::onConnect()
{
	qint64 size = m_Socket->write(m_Data);
	if (size != m_Data.size())
		QMessageBox::critical(nullptr, tr("Network Error"), tr("A network communication error has ocurred!"));
}

void WritableSocket::onError(QAbstractSocket::SocketError error)
{
	qDebug() << "Socket Error:" << error;
}

// NetworkManager

NetworkManager* NetworkManager::s_Instance = nullptr;

NetworkManager::NetworkManager() : m_WritableSocket {  }, m_State { State::IDLE }
{
	m_ThisClient = new Client();

	initNetwork();
	initTimer();
}

NetworkManager::~NetworkManager()
{
	delete m_ThisClient;
}

void NetworkManager::init()
{
	s_Instance = new NetworkManager();
}

void NetworkManager::terminate()
{
	delete s_Instance;
}

void NetworkManager::requestActionUpdate()
{
	m_ActionsUpToDate = false;
	emit actionsCanUpdate();
}

void NetworkManager::updateActions(const Bundle& actionBundle)
{
	for (const Client& client : m_ConnectedFrom)
		client.updateActions(actionBundle);

	m_ActionsUpToDate = true;
}

void NetworkManager::startBroadcast()
{
	m_BroadcastTimer.start();
	setState(State::BROADCASTING);
}

void NetworkManager::stopBroadcast()
{
	m_BroadcastTimer.stop();
	setState(State::IDLE);
}

bool NetworkManager::connectTo(Client client)
{
	if (!client.isValid())
		return false;

	if (m_ConnectedTo.contains(client))
		return false;

	connectToInternal(client);
	m_ConnectedTo.append(client);

	emit fromThisConnectionsUpdated();

	return true;
}

bool NetworkManager::disconnectFrom(const Client& client)
{
	if (!client.isValid())
		return false;

	if (!m_ConnectedTo.contains(client))
		return false;

	disconnectFromInternal(client);
	m_ConnectedTo.removeOne(client);

	emit fromThisConnectionsUpdated();

	return true;
}

bool NetworkManager::hasConnectedClients() const
{
	return !m_ConnectedFrom.empty();
}

bool NetworkManager::isConnectedToOthers() const
{
	return !m_ConnectedTo.empty();
}

Client* NetworkManager::getClientFromID(const QString& id)
{
	for (Client& client : m_ConnectedTo)
	{
		if (client.getID() == id)
			return &client;
	}

	for (Client& client : m_ConnectedFrom)
	{
		if (client.getID() == id)
			return &client;
	}

	return nullptr;
}

void NetworkManager::initNetwork()
{
	connect(&m_UdpSocket, &QUdpSocket::readyRead, this, &NetworkManager::onPendingDatagram);
	m_UdpSocket.bind(PORT, QUdpSocket::ShareAddress);
}

void NetworkManager::initTimer()
{
	m_BroadcastTimer.setInterval(TIMER_INTERVAL);
	connect(&m_BroadcastTimer, &QTimer::timeout, this, &NetworkManager::onBroadcastDatagram);
}

void NetworkManager::setState(NetworkManager::State state)
{
	m_State = state;
}

bool NetworkManager::connectToInternal(Client& client)
{
	m_WritableSocket.connectToHost(client.getAddress(), PORT);
	m_WritableSocket.write(buildConnectMessage(client));

	client.setSocket(m_WritableSocket.getSocket());

	qDebug() << "Client" << client.getID() << "from" << client.getAddress() << "connected.";
	return true;
}

bool NetworkManager::disconnectFromInternal(const Client& client)
{
	client.disconnectFromHost();

	qDebug() << "Client" << client.getID() << "from" << client.getAddress() << "disconnected.";
	return true;
}

QByteArray NetworkManager::buildBroadcastDatagram()
{
	NetworkMessage message(JSON_APP_IDENTIFIER);
	message.setProperty(JSON_KEY_NAME, m_ThisClient->getName());
	message.setProperty(JSON_KEY_ID, m_ThisClient->getID());

	return message.buildMessage().toUtf8();
}

QByteArray NetworkManager::buildConnectMessage(const Client& client)
{
	NetworkMessage message(JSON_APP_IDENTIFIER);
	message.setProperty(JSON_KEY_NAME, m_ThisClient->getName());
	message.setProperty(JSON_KEY_ID, client.getID());

	return message.buildMessage().toUtf8();
}

void NetworkManager::acceptClientConnection(const Client& client)
{
	if (m_ConnectedFrom.contains(client))
		return;

	m_ConnectedFrom.append(client);
	NetworkManager::instance()->requestActionUpdate();

	emit toThisConnectionsUpdated();
}

void NetworkManager::closeClientConnection(const Client& client)
{
//	if (m_ConnectedFrom.contains(client))
//		return;

	client.disconnectFromHost();
	m_ConnectedFrom.removeOne(client);

	emit toThisConnectionsUpdated();
}

void NetworkManager::onBroadcastDatagram()
{
	m_UdpSocket.writeDatagram(buildBroadcastDatagram(), QHostAddress::Broadcast, PORT);
}

QString stripAddress(const QString& address)
{
	const QChar separator = ':';

	if (!address.contains(separator))
		return address;

	int index = address.lastIndexOf(separator);
	return address.mid(address.lastIndexOf(separator) + 1, address.size() - index);
}

void NetworkManager::onPendingDatagram()
{
	QByteArray datagram;
	QHostAddress address;

	while (m_UdpSocket.hasPendingDatagrams())
	{
		datagram.resize(int(m_UdpSocket.pendingDatagramSize()));
		m_UdpSocket.readDatagram(datagram.data(), datagram.size(), &address);

		NetworkMessage message(JSON_APP_IDENTIFIER, datagram);

		if (message.hasError())
			continue;

		if (!(message.hasProperty(JSON_KEY_ID)))
			continue;

		Client client(message.getProperty(JSON_KEY_NAME), message.getProperty(JSON_KEY_ID), stripAddress(address.toString()));
		emit clientAvailable(client);
	}
}

// Server

ActionServer* ActionServer::s_Instance = nullptr;

ActionServer::ActionServer()
{
	setupSignalsAndSlots();
	setListening(true);
}

void ActionServer::start()
{
	s_Instance = new ActionServer();
}

void ActionServer::shutdown()
{
	delete s_Instance;
}

void ActionServer::setListening(bool listening)
{
	if (listening)
		m_TcpServer.listen(QHostAddress::Any, PORT);
	else
		m_TcpServer.close();
}

void ActionServer::setupSignalsAndSlots()
{
	connect(&m_TcpServer, &QTcpServer::newConnection, this, &ActionServer::onNewConnection);
}

void ActionServer::parseRequest(QTcpSocket* socket)
{
	QString response = socket->readAll();
	qDebug() << response;

	QJsonDocument doc = QJsonDocument::fromJson(response.toUtf8());
	QJsonObject object = doc.object();

	// Check that this request is a valid ExeQt request
	if (object.value(JSON_KEY_IDENTIFIER).toString() != JSON_APP_IDENTIFIER)
		return;

	Client client(object.value(JSON_KEY_NAME).toString(),
				  object.value(JSON_KEY_ID).toString(),
				  stripAddress(socket->localAddress().toString()),
				  socket);

	QMessageBox::StandardButton reply =
			QMessageBox::question(nullptr, tr("New Connection"), tr("Accept connection from client %1").arg(client.getUnlocalizedName()), QMessageBox::Yes | QMessageBox::No);

	if (reply == QMessageBox::Yes)
		NetworkManager::instance()->acceptClientConnection(client);
	else
		client.disconnectFromHost();
}

void ActionServer::disconnectSocket(QTcpSocket* socket)
{
	for (const Client& client : NetworkManager::instance()->getConnectedFromClients())
	{
		if (client.getSocket() == socket)
			NetworkManager::instance()->closeClientConnection(client);
	}
}

void ActionServer::onNewConnection()
{
	qDebug() << "New Connection";

	while (m_TcpServer.hasPendingConnections())
	{
		QTcpSocket* socket = m_TcpServer.nextPendingConnection();
		connect(socket, &QTcpSocket::readyRead, this, &ActionServer::onReadyRead);
		connect(socket, &QTcpSocket::disconnected, this, &ActionServer::onDisconnect);

		qDebug() << "Connection from" << socket->localAddress().toString();
	}

	emit connectedToClient();
}

void ActionServer::onDisconnect()
{
	QTcpSocket* socket = static_cast<QTcpSocket*>(QObject::sender());
	qDebug() << "Socket" << socket->localAddress().toString() << "disconnected";

	disconnectSocket(socket);

	emit disconnectedFromClient();
}

void ActionServer::onReadyRead()
{
	QTcpSocket* socket = static_cast<QTcpSocket*>(QObject::sender());
	parseRequest(socket);
	disconnect(socket, &QTcpSocket::readyRead, this, &ActionServer::onReadyRead);
}
