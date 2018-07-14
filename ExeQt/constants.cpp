/**************************************************************************
 *
 * Copyright (c) 2018 Alexandru Istrate
 *
 * This file is subject to the terms and conditions defined in the
 * file 'LICENSE', which is part of this source code package.
 *
**************************************************************************/

#include "constants.h"
#include "settingsregistry.h"

//QString Constants::s_ServerAddress = "https://andu.internals.ro/endpoint";

QString Constants::getUploadPath()
{
	return SettingsRegistry::instance()->get(Settings::CLOUD_SYNC_ADDRESS).toString() + QString("/upload.php");
}

QString Constants::getDownloadPath()
{
	return SettingsRegistry::instance()->get(Settings::CLOUD_SYNC_ADDRESS).toString() + QString("/download.php");
}

QString Constants::getAuthPath()
{
	return SettingsRegistry::instance()->get(Settings::CLOUD_SYNC_ADDRESS).toString() + QString("/auth.php");
}

const QString Constants::DEFAULT_SAVE_FILE_NAME = "exeqt-save.xml";
const QString Constants::DEFAULT_SERVER_ADDRESS = "https://andu.internals.ro/endpoint";

const QString Constants::JSON_PROPERTY_FLAG = "flag";
const QString Constants::JSON_PROPERTY_MESSSAGE = "msg";

const QString Constants::COMPANY_NAME = "ExeQt Company";
const QString Constants::PRODUCT_NAME = "ExeQt";
