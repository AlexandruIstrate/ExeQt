/**************************************************************************
 *
 * Copyright (c) 2018 Alexandru Istrate
 *
 * This file is subject to the terms and conditions defined in the
 * file 'LICENSE', which is part of this source code package.
 *
**************************************************************************/

#include "authmanager.h"

#include <QDebug>

#include <QMessageBox>

#include <QJsonDocument>
#include <QJsonObject>

#include "constants.h"
#include "common.h"
#include "bundle.h"
#include "mainwidget.h"
#include "mergedialog.h"
#include "stylemanager.h"

#define PROPERTY_USER			"user"
#define PROPERTY_PASSWORD		"pwd"

AuthManager* AuthManager::s_Instance = nullptr;

AuthManager::AuthManager() : m_IsAuth { false }, m_SaveOnExit { true }
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

void AuthManager::syncActions()
{
	connect(m_RequestManager, &RequestManager::requestFinished, this, &AuthManager::onFileSyncFinished);

	QString url = RequestManager::buildUrl(Constants::getDownloadPath(), AuthManager::instance()->getToken());
	m_RequestManager->downloadFile(url);
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
	connect(m_RequestManager, &RequestManager::requestFinished, this, &AuthManager::onLoginFinished);
}

void AuthManager::parseLoginResponse(const QString& response)
{
	QJsonDocument doc(QJsonDocument::fromJson(response.toUtf8()));

	QJsonObject jsonObj = doc.object();
	int flag = jsonObj.value(Constants::JSON_PROPERTY_FLAG).toInt();
	QString message = jsonObj.value(Constants::JSON_PROPERTY_MESSSAGE).toString();

	if (flag == Constants::FLAG_OK)
	{
		m_Token = message;
		m_IsAuth = true;
	}
	else
	{
		QMessageBox::critical(nullptr, tr("Login failed"), message);
	}
}

Bundle getActionBundle(const QString& saveFile)
{
	MainWidget* mw = MainWidget::instance();

	if (mw)	// If it's not null, then it means we are accessing this trough the main app and not from the start page
	{
		Bundle result(mw->getTagName());
		mw->writeProperties(result);

		return result;
	}

	return Bundle::fromFile(saveFile);
}

bool AuthManager::parseSyncActions(const QString& jsonText, QString* outMessage)
{
	QJsonDocument doc(QJsonDocument::fromJson(jsonText.toUtf8()));

	QJsonObject jsonObj = doc.object();
	int flag = jsonObj.value(Constants::JSON_PROPERTY_FLAG).toInt();
	QString message = jsonObj.value(Constants::JSON_PROPERTY_MESSSAGE).toString();

	if (flag == Constants::FLAG_OK)
	{
		const QString SAVE_FILE = Common::getSaveFilePath();

		Bundle webBundle = Bundle::fromXML(message);
		Bundle localBundle = getActionBundle(SAVE_FILE);	// Get the actions from the app if it's running or from the save file otherwise
		localBundle.saveToFile("/data/local.txt");

		MergeDialog md(localBundle, webBundle, MainWidget::instance());
		md.setStyleSheet(StyleManager::instance()->getStyle());

		if (md.exec() == QDialog::DialogCode::Rejected)
		{
			if (outMessage)
			{
				*outMessage = message;
			}

			return false;
		}

		const Bundle& merged = md.getMergeResult();
		merged.saveToFile(SAVE_FILE);

		reloadActions();
	}
	else
	{
		QMessageBox::critical(nullptr, tr("Download failed"), message);
	}

	if (outMessage)
	{
		*outMessage = message;
	}

	return true;
}

void AuthManager::reloadActions()
{
	MainWidget* mw = MainWidget::instance();

	mw->removeTabs();
	mw->setupTabs();

	mw->loadFromFile();
}

void AuthManager::onLoginFinished(QNetworkReply* reply, bool timedOut)
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

	disconnect(m_RequestManager, &RequestManager::requestFinished, this, &AuthManager::onLoginFinished);

	parseLoginResponse(reply->readAll());
	emit doneLogin();
}

void AuthManager::onFileSyncFinished(QNetworkReply* reply, bool timedOut)
{
	disconnect(m_RequestManager, &RequestManager::requestFinished, this, &AuthManager::onFileSyncFinished);

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

	bool ok = parseSyncActions(reply->readAll());
	AuthManager::instance()->setShouldSaveOnExit(ok);

	emit doneSync();
}
