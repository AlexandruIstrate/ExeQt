/**************************************************************************
 *
 * Copyright (c) 2018 Alexandru Istrate
 *
 * This file is subject to the terms and conditions defined in the
 * file 'LICENSE', which is part of this source code package.
 *
**************************************************************************/

#ifndef AUTHMANAGER_H
#define AUTHMANAGER_H

#include <QObject>
#include <QString>

#include "requestmanager.h"

class AuthManager : public QObject
{
	Q_OBJECT

private:
	static AuthManager* s_Instance;

private:
	RequestManager* m_RequestManager;

	QString m_Token;

	bool m_IsAuth;
	bool m_SaveOnExit;

private:
	AuthManager();
	~AuthManager();

public:
	inline const QString& getToken() const { return m_Token; }
	inline bool isAuth() const { return m_IsAuth; }

	inline bool shouldSaveOnExit() const { return m_SaveOnExit; }
	inline void setShouldSaveOnExit(bool sync) { m_SaveOnExit = sync; }

	void authenticate(const QString& userName, const QString& password);
	void syncActions();

	inline static AuthManager* instance() { return s_Instance; }

	static void init();
	static void terminate();

private:
	void setupNetwork();

	void parseLoginResponse(const QString&);
	bool parseSyncActions(const QString&, QString* outMessage = nullptr);

	void reloadActions();

signals:
	void doneLogin();
	void doneSync();

private slots:
	void onLoginFinished(QNetworkReply* reply, bool timedOut);
	void onFileSyncFinished(QNetworkReply* reply, bool timedOut);
};

#endif // AUTHMANAGER_H
