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
		const QString SAVE_FILE = QApplication::applicationDirPath() + QString("/") + Constants::SAVE_FILE_NAME;

		QString str;
		if (!Saveable::readFromFile(SAVE_FILE, str))
		{
			QMessageBox::critical(this, tr("File Read Error"), tr("Could not read local XML save file!"));
			return message;
		}

		Bundle webBundle = Bundle::fromXml(message);
		Bundle localBundle = Bundle::fromXml(str);

		Bundle diff = Bundle::getBundleDiff(webBundle, localBundle);
		QString diffXml = diff.toXml();

//		Saveable::writeToFile(QApplication::applicationDirPath() + QString("/test.xml"), diffXml);

		Saveable::writeToFile(SAVE_FILE, diffXml);
	}
	else
	{
		QMessageBox::critical(this, tr("Download failed"), message);
	}

	return message;
}

void LoginDialog::onLogIn()
{
	AuthManager::instance()->authenticate(ui->edtUser->text(), ui->edtPassword->text());
}

void LoginDialog::onLoginDone()
{
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
	accept();   // TODO: Maybe let the user close the dialog?
}
