/**************************************************************************
 *
 * Copyright (c) 2018 Alexandru Istrate
 *
 * This file is subject to the terms and conditions defined in the
 * file 'LICENSE', which is part of this source code package.
 *
**************************************************************************/

#include "constants.h"

QString Constants::s_ServerAddress = "https://andu.internals.ro/endpoint";

QString Constants::getUploadPath()
{
	return s_ServerAddress + QString("/upload.php");
}

QString Constants::getDownloadPath()
{
	return s_ServerAddress + QString("/download.php");
}

QString Constants::getAuthPath()
{
	return s_ServerAddress + QString("/auth.php");
}

const QString Constants::SAVE_FILE_NAME = "exeqt-save.xml";

const QString Constants::JSON_PROPERTY_FLAG = "flag";
const QString Constants::JSON_PROPERTY_MESSSAGE = "msg";

const QString Constants::COMPANY_NAME = "ExeQt Company";
const QString Constants::PRODUCT_NAME = "ExeQt";

const QString Constants::SETTING_KEY_IP = "serverAddress";
const QString Constants::SETTING_KEY_CLIENT_NAME = "clientName";
