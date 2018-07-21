/**************************************************************************
 *
 * Copyright (c) 2018 Alexandru Istrate
 *
 * This file is subject to the terms and conditions defined in the
 * file 'LICENSE', which is part of this source code package.
 *
**************************************************************************/
#include "settingsregistry.h"

#include <QDebug>
#include <QSettings>
#include <QApplication>

#include "constants.h"
#include "networkmanager.h"

SettingsRegistry* SettingsRegistry::s_Instance = nullptr;

SettingsRegistry::SettingsRegistry()
	: m_Settings { Constants::COMPANY_NAME, Constants::PRODUCT_NAME }
{
	checkDefaultsNeeded();
}

SettingsRegistry::~SettingsRegistry()
{
	sync();
}

void SettingsRegistry::set(const QString& key, const QVariant& value)
{
	m_Settings.setValue(key, value);
	emit settingsUpdated(this);
}

QVariant SettingsRegistry::get(const QString& key) const
{
	return m_Settings.value(key);
}

void SettingsRegistry::sync()
{
	m_Settings.sync();
}

void SettingsRegistry::init()
{
	s_Instance = new SettingsRegistry();
}

void SettingsRegistry::terminate()
{
	delete s_Instance;
}

bool SettingsRegistry::isSettingDefault(const QString& key)
{
	// Use "" as the default value since if we had this as a setting we would want to change it
	return m_Settings.value(key, "").toString().isEmpty();
}

void SettingsRegistry::checkDefaultsNeeded()
{
	if (isSettingDefault(Settings::SAVE_PATH))
		set(Settings::SAVE_PATH, !QApplication::applicationDirPath().isEmpty() ? QApplication::applicationDirPath() : QDir::homePath());

	if (isSettingDefault(Settings::CLOUD_SYNC_ADDRESS))
		set(Settings::USERNAME, Constants::DEFAULT_SERVER_ADDRESS);

	if (isSettingDefault(Settings::USERNAME))
		set(Settings::USERNAME, Client::getHardwareID());

	if (isSettingDefault(Settings::SHOW_MAIN_PAGE))
		set(Settings::SHOW_MAIN_PAGE, true);
}
