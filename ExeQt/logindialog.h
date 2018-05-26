#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QString>
#include <QDialog>

#include "requestmanager.h"

namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog
{
	Q_OBJECT

private:
	Ui::LoginDialog* ui;

	RequestManager* m_RequestManager;

public:
	LoginDialog(QWidget* parent = nullptr);
	~LoginDialog();

private:
	void setupNetwork();
	void setupSignalsAndSlots();

	QString parseJsonMessage(const QString& jsonText);

private slots:
	void onLogIn();
	void onLoginDone();

	void onRequestFinished(QNetworkReply* reply, bool timedOut);
};

#endif // LOGINDIALOG_H
