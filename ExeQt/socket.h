#ifndef SOCKET_H
#define SOCKET_H

#include <QObject>
#include <QByteArray>
#include <QTcpSocket>

class WritableSocket : public QObject
{
	Q_OBJECT

private:
	QTcpSocket* m_Socket;
	QByteArray m_Data;

public:
	WritableSocket(const QString& address, unsigned short port);
	WritableSocket();

	inline QTcpSocket* getSocket() { return m_Socket; }

	void connectToHost(const QString& address, unsigned short port);
	void write(const QByteArray& data);

private slots:
	void onConnect();
	void onError(QAbstractSocket::SocketError);
};

class BufferedSocket : public QObject
{
	Q_OBJECT

private:
	QByteArray m_BufferData;
	QTcpSocket* m_BufferSocket;

public:
	BufferedSocket(QTcpSocket* socket = nullptr);

	void setBufferSocket(QTcpSocket* socket);

signals:
	void transferFinished(QByteArray data);

private slots:
	void onReadyRead();
};

#endif // SOCKET_H
