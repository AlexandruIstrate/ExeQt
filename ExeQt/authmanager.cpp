#include "authmanager.h"

#include <QDebug>

#include <QMessageBox>

#include <QJsonDocument>
#include <QJsonObject>

#include "constants.h"

AuthManager* AuthManager::s_Instance = nullptr;

#define PROPERTY_USER "user"
#define PROPERTY_PASSWORD "pwd"

AuthManager::AuthManager() : m_IsAuth { false }
{
	setupNetwork();
}

AuthManager::~AuthManager()
{
	delete m_RequestManager;
}

void AuthManager::authenticate(const QString& userName, const QString& password)
{
	QUrlQuery query;
	query.addQueryItem(PROPERTY_USER, userName);
	query.addQueryItem(PROPERTY_PASSWORD, password);

	m_RequestManager->sendRequest(Constants::getAuthPath(), query, RequestManager::RequestType::POST);
}

void AuthManager::init()
{
	s_Instance = new AuthManager();
}

void AuthManager::terminate()
{
	delete s_Instance;
}

void AuthManager::setupNetwork()
{
	m_RequestManager = new RequestManager();
	connect(m_RequestManager, SIGNAL(requestFinished(QNetworkReply*, bool)), this, SLOT(onRequestFinished(QNetworkReply*, bool)));
}

void AuthManager::parseResponse(const QString& response)
{
	QJsonDocument doc(QJsonDocument::fromJson(response.toUtf8()));

	QJsonObject jsonObj = doc.object();
	int flag = jsonObj["flag"].toInt();
	QString message = jsonObj["msg"].toString();

	if (flag == 0)
	{
		m_Token = message;
		m_IsAuth = true;
	}
	else
	{
		QMessageBox::critical(nullptr, tr("Login failed"), message);
	}
}

void AuthManager::onRequestFinished(QNetworkReply* reply, bool timedOut)
{
	if (timedOut)
	{
		QMessageBox::critical(nullptr, tr("Timed Out"), tr("Request timed out."));
		return;
	}

	if (reply->error() != QNetworkReply::NetworkError::NoError)
	{
		QMessageBox::critical(nullptr, tr("Error"), reply->errorString());
		return;
	}

//    QMessageBox::information(this, tr("Info"), reply->readAll());
	parseResponse(reply->readAll());
	emit done();
}
