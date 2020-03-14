#include "socket.h"

#include <QMessageBox>

#include "networkmessage.h"

WritableSocket::WritableSocket(const QString& address, unsigned short port) : WritableSocket()
{
	connectToHost(address, port);
}

WritableSocket::WritableSocket()
{
	m_Socket = new QTcpSocket();
	connect(m_Socket, &QTcpSocket::connected, this, &WritableSocket::onConnect);
	connect(m_Socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(onError(QAbstractSocket::SocketError)));
}

void WritableSocket::connectToHost(const QString& address, unsigned short port)
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
	{
		QMessageBox::critical(nullptr, tr("Network Error"), tr("A network communication error has ocurred!"));
	}
}

void WritableSocket::onError(QAbstractSocket::SocketError error)
{
	qDebug() << "Socket Error:" << error;
}

// BufferedSocket

BufferedSocket::BufferedSocket(QTcpSocket* socket) : m_BufferSocket { socket }
{
	if (m_BufferSocket)
	{
		connect(m_BufferSocket, &QTcpSocket::readyRead, this, &BufferedSocket::onReadyRead);
	}
}

void BufferedSocket::setBufferSocket(QTcpSocket* socket)
{
	m_BufferSocket = socket;
	connect(m_BufferSocket, &QTcpSocket::readyRead, this, &BufferedSocket::onReadyRead);
}

void BufferedSocket::onReadyRead()
{
	QByteArray readData = m_BufferSocket->readAll();
	m_BufferData.append(readData);

	if (QString(readData).endsWith(MESSAGE_END_CHARACTER))
	{
		emit transferFinished(m_BufferData);
		m_BufferData.clear();
	}
}
