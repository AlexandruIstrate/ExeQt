#include "requestmanager.h"

#include <QDebug>
#include <QFileInfo>

#include "authmanager.h"

#define UPLOAD_NAME "fileToUpload"

RequestManager::RequestManager(int timeout) : m_RequestTimedOut { false }, m_Status { Status::NOT_STARTED }
{
	setupNetwork();
	setTimeout(timeout);
}

QNetworkReply* RequestManager::sendRequest(const QUrl& url, const QUrlQuery& query, RequestType type)
{
	startRequest();

	QNetworkRequest request;
	request.setRawHeader(QString("Content-Type").toUtf8(), QString("application/x-www-form-urlencoded").toUtf8());
	request.setUrl(url);

	QSslConfiguration conf = request.sslConfiguration();
	conf.setPeerVerifyMode(QSslSocket::VerifyNone);
	request.setSslConfiguration(conf);

	QNetworkReply* reply = nullptr;

	switch (type)
	{
	case RequestType::GET:
		reply = m_RequestManager->get(request);
		break;

	case RequestType::POST:
//        query.addQueryItem("token", AuthManager::instance()->getToken());
		reply = m_RequestManager->post(request, query.toString(QUrl::FullyEncoded).toUtf8());
		break;

	default:
		qDebug() << "Unknown request type!";
		return nullptr;
	}

	connect(m_TimeoutTimer, SIGNAL(timeout()), reply, SLOT(abort()));
	m_TimeoutTimer->start();

	return reply;
}

QNetworkReply* RequestManager::sendRequest(const QString& url, const QUrlQuery& query, RequestManager::RequestType type)
{
	return sendRequest(QUrl(url), query, type);
}

QNetworkReply* RequestManager::uploadFile(const QUrl& url, const QString& file)
{
	startRequest();

	QByteArray data = prepareFile(file);

	QNetworkRequest request;
	request.setRawHeader(QString("Content-Type").toUtf8(), (QString("multipart/form-data; boundary=") + BOUNDARY).toUtf8());
	request.setRawHeader(QString("Content-Length").toUtf8(), QString::number(data.length()).toUtf8());

	QSslConfiguration conf = request.sslConfiguration();
	conf.setPeerVerifyMode(QSslSocket::VerifyNone);
	request.setSslConfiguration(conf);

	request.setUrl(url);

	QNetworkReply* reply = m_RequestManager->post(request, data);
	connect(m_TimeoutTimer, SIGNAL(timeout()), reply, SLOT(abort()));

	m_TimeoutTimer->start();

	return reply;
}

QNetworkReply* RequestManager::downloadFile(const QUrl& url)
{
	startRequest();

	QNetworkRequest request;
	request.setRawHeader(QString("Content-Type").toUtf8(), QString("application/json; charset=UTF-8").toUtf8());
	request.setUrl(url);

	QSslConfiguration conf = request.sslConfiguration();
	conf.setPeerVerifyMode(QSslSocket::VerifyNone);
	request.setSslConfiguration(conf);

	QNetworkReply* reply = m_RequestManager->get(request);
	connect(m_TimeoutTimer, SIGNAL(timeout()), reply, SLOT(abort()));

	m_TimeoutTimer->start();

	return reply;
}

QString RequestManager::buildUrl(const QString& url, const QString& token)
{
	return url + QString("?token=%1").arg(token);
}

void RequestManager::setupNetwork()
{
	m_RequestManager = new QNetworkAccessManager();
	connect(m_RequestManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(onRequestFinished(QNetworkReply*)));

	m_TimeoutTimer = new QTimer(this);
	m_TimeoutTimer->setSingleShot(true);
	connect(m_TimeoutTimer, SIGNAL(timeout()), this, SLOT(onRequestTimeout()));
}

void RequestManager::startRequest()
{
	m_RequestTimedOut = false;
	m_Status = Status::RUNNING;
}

QByteArray RequestManager::prepareFile(const QString& filePath)
{
	// TODO: Use Line-Feeds
	QByteArray data(QString("--" + BOUNDARY + "\r\n").toUtf8());
	data += "Content-Disposition: form-data; name=\"action\"\r\n\r\n";
	data += "file_upload\r\n";

	data += QString("--" + BOUNDARY + "\r\n").toUtf8();
	data += "Content-Disposition: form-data; name=\"token\"\r\n\r\n";
	data += AuthManager::instance()->getToken() + "\r\n";

	QFile file(filePath);

	QFileInfo fileInfo(file.fileName());
	QString filename(fileInfo.fileName());

	// file
	data += QString("--" + BOUNDARY + "\r\n").toUtf8();
//    data += "Content-Disposition: form-data; name=\"sfile\"; filename=\"test1.jpg\"\r\n";
//    data += "Content-Type: image/jpeg\r\n\r\n";
	data += QString("Content-Disposition: form-data; name=\"%1\"; filename=\"%2\"\r\n").arg(UPLOAD_NAME).arg(filename);
	data += "Content-Type: text/xml\r\n\r\n";

	if (!file.open(QIODevice::ReadOnly))
	{
		qDebug() << "Could not read file!";
		return data;
	}

	data += file.readAll();
	data += "\r\n";

	// password
	data += QString("--" + BOUNDARY + "\r\n").toUtf8();
	data += "Content-Disposition: form-data; name=\"password\"\r\n\r\n";
	//data += "password\r\n"; // put password if needed
	data += "\r\n";

	// description
	data += QString("--" + BOUNDARY + "\r\n").toUtf8();
	data += "Content-Disposition: form-data; name=\"description\"\r\n\r\n";
	//data += "description\r\n"; // put description if needed
	data += "\r\n";

	// agree
	data += QString("--" + BOUNDARY + "\r\n").toUtf8();
	data += "Content-Disposition: form-data; name=\"agree\"\r\n\r\n";
	data += "1\r\n";

	data += QString("--" + BOUNDARY + "--\r\n").toUtf8();

	return data;
}

void RequestManager::onRequestFinished(QNetworkReply* reply)
{
	m_Status = Status::DONE;
	emit requestFinished(reply, m_RequestTimedOut);
}

void RequestManager::onRequestTimeout()
{
	m_RequestTimedOut = true;
}
