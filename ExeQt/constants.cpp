#include "constants.h"

QString Constants::SERVER_ADDRESS = "https://andu.internals.ro/endpoint";
//const QString Constants::UPLOAD_PATH = SERVER_ADDRESS + QString("/upload.php");
//const QString Constants::DOWNLOAD_PATH = SERVER_ADDRESS + QString("/download.php");
//const QString Constants::AUTH_PATH = SERVER_ADDRESS + QString("/auth.php");

QString Constants::getUploadPath()
{
	return SERVER_ADDRESS + QString("/upload.php");
}

QString Constants::getDownloadPath()
{
	return SERVER_ADDRESS + QString("/download.php");
}

QString Constants::getAuthPath()
{
	return SERVER_ADDRESS + QString("/auth.php");
}

const QString Constants::SAVE_FILE_NAME = "exeqt-save.xml";

const QString Constants::JSON_PROPERTY_FLAG = "flag";
const QString Constants::JSON_PROPERTY_MESSSAGE = "msg";

const QString Constants::COMPANY_NAME = "ExeQt Company";
const QString Constants::PRODUCT_NAME = "ExeQt";

const QString Constants::SETTING_KEY_IP = "serverAddress";
