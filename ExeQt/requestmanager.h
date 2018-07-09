/**************************************************************************
 *
 * Copyright (c) 2018 Alexandru Istrate
 *
 * This file is subject to the terms and conditions defined in the
 * file 'LICENSE', which is part of this source code package.
 *
**************************************************************************/

#ifndef REQUESTMANAGER_H
#define REQUESTMANAGER_H

#include <QObject>
#include <QTimer>
#include <QFile>

#include <QUrl>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUrlQuery>

#define DEFAULT_TIMEOUT 5000

class RequestManager : public QObject
{
	Q_OBJECT

public:
	enum class RequestType
	{
		GET, POST
	};

	enum class Status
	{
		NOT_STARTED, RUNNING, DONE
	};

private:
	QNetworkAccessManager* m_RequestManager;

	QTimer* m_TimeoutTimer;
	bool m_RequestTimedOut;

	Status m_Status;

private:
	const QString BOUNDARY = "---------------------------723690991551375881941828858";

public:
	RequestManager(int timeout = DEFAULT_TIMEOUT);

	inline void setTimeout(int timeout) { m_TimeoutTimer->setInterval(timeout); }
	inline Status getStatus() const { return m_Status; }

	QNetworkReply* sendRequest(const QUrl& url, const QUrlQuery& query, RequestType type);
	QNetworkReply* sendRequest(const QString& url, const QUrlQuery& query, RequestType type);

	QNetworkReply* uploadFile(const QUrl& url, const QString &file);
	QNetworkReply* downloadFile(const QUrl& url);

	static QString buildUrl(const QString& url, const QString& token);

private:
	void setupNetwork();
	void startRequest();

	QByteArray prepareFile(const QString &file);

signals:
	void requestFinished(QNetworkReply* reply, bool timedOut);

private slots:
	void onRequestFinished(QNetworkReply* reply);
	void onRequestTimeout();
};

#endif // REQUESTMANAGER_H
