/**************************************************************************
 *
 * Copyright (c) 2018 Alexandru Istrate
 *
 * This file is subject to the terms and conditions defined in the
 * file 'LICENSE', which is part of this source code package.
 *
**************************************************************************/
#ifndef SETTINGSREGISTRY_H
#define SETTINGSREGISTRY_H

#include <QObject>
#include <QString>
#include <QSettings>

namespace Settings
{
	const QString SAVE_PATH = "savePath";
	const QString CLOUD_SYNC_ADDRESS = "cloudSyncAddress";
	const QString USERNAME = "username";
	const QString SHOW_MAIN_PAGE = "showMainPage";
	const QString CONFIRM_DELETE = "confirmDelete";
}

class SettingsRegistry : public QObject
{
	Q_OBJECT

private:
	QSettings m_Settings;

private:
	static SettingsRegistry* s_Instance;

	SettingsRegistry();
	~SettingsRegistry();

public:
	void set(const QString& key, const QVariant& value);
	QVariant get(const QString& key) const;

	void sync();

	static inline SettingsRegistry* instance() { return s_Instance; }

	static void init();
	static void terminate();

private:
	bool isSettingDefault(const QString& key);
	void checkDefaultsNeeded();

signals:
	void settingsUpdated(SettingsRegistry* settingsRegistry);
};

#endif // SETTINGSREGISTRY_H
