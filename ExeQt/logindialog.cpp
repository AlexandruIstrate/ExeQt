/**************************************************************************
 *
 * Copyright (c) 2018 Alexandru Istrate
 *
 * This file is subject to the terms and conditions defined in the
 * file 'LICENSE', which is part of this source code package.
 *
**************************************************************************/

#include "logindialog.h"
#include "ui_logindialog.h"

#include <QMessageBox>
#include <QPushButton>

#include <QJsonDocument>
#include <QJsonObject>

#include "constants.h"
#include "saveable.h"
#include "mainwidget.h"
#include "authmanager.h"
#include "common.h"

LoginDialog::LoginDialog(QWidget* parent) :
	QDialog(parent), ui(new Ui::LoginDialog)
{
	ui->setupUi(this);

	setupNetwork();
	setupSignalsAndSlots();
}

LoginDialog::~LoginDialog()
{
	delete ui;
}

void LoginDialog::setupNetwork()
{
	m_RequestManager = new RequestManager();
	connect(m_RequestManager, SIGNAL(requestFinished(QNetworkReply*, bool)), this, SLOT(onRequestFinished(QNetworkReply*, bool)));
}

void LoginDialog::setupSignalsAndSlots()
{
	connect(ui->btnLogIn, &QPushButton::clicked, this, &LoginDialog::onLogIn);
	connect(AuthManager::instance(), &AuthManager::done, this, &LoginDialog::onLoginDone);
}

QString LoginDialog::parseJsonMessage(const QString& jsonText)
{
	QJsonDocument doc(QJsonDocument::fromJson(jsonText.toUtf8()));

	QJsonObject jsonObj = doc.object();
	int flag = jsonObj[Constants::JSON_PROPERTY_FLAG].toInt();
	QString message = jsonObj[Constants::JSON_PROPERTY_MESSSAGE].toString();

	if (flag == Constants::FLAG_OK)
	{
		const QString SAVE_FILE = Common::getSaveFilePath();

		Bundle webBundle = Bundle::fromXML(message);
		Bundle localBundle = Bundle::fromFile(SAVE_FILE);

		Bundle merged = Bundle::mergeBundles(webBundle, localBundle);
		merged.saveToFile(SAVE_FILE);
	}
	else
	{
		QMessageBox::critical(this, tr("Download failed"), message);
	}

	return message;
}

void LoginDialog::onLogIn()
{
	ui->lblState->setText("Logging in...");
	AuthManager::instance()->authenticate(ui->edtUser->text(), ui->edtPassword->text());
}

void LoginDialog::onLoginDone()
{
	emit doneLogin();
	ui->lblState->setText("Synchronizing actions...");

	QString url = RequestManager::buildUrl(Constants::getDownloadPath(), AuthManager::instance()->getToken());
	m_RequestManager->downloadFile(url);
}

void LoginDialog::onRequestFinished(QNetworkReply* reply, bool timedOut)
{
	if (timedOut)
	{
		QMessageBox::critical(this, tr("Timed Out"), tr("Request timed out."));
		return;
	}

	if (reply->error() != QNetworkReply::NetworkError::NoError)
	{
		QMessageBox::critical(this, tr("Error"), reply->errorString());
		return;
	}

	parseJsonMessage(reply->readAll());
	accept();

	ui->lblState->setText("Done");
	emit doneFileDownload();
}
