/**************************************************************************
 *
 * Copyright (c) 2018 Alexandru Istrate
 *
 * This file is subject to the terms and conditions defined in the
 * file 'LICENSE', which is part of this source code package.
 *
**************************************************************************/

#ifndef ADDRESSES_H
#define ADDRESSES_H

#include <QString>

class Constants
{
public:
	static QString s_ServerAddress;

	static QString getUploadPath();
	static QString getDownloadPath();
	static QString getAuthPath();

	static const QString SAVE_FILE_NAME;

	static const QString JSON_PROPERTY_FLAG;
	static const QString JSON_PROPERTY_MESSSAGE;

	static const QString COMPANY_NAME;
	static const QString PRODUCT_NAME;

	static const QString SETTING_KEY_IP;
	static const QString SETTING_KEY_CLIENT_NAME;

	enum
	{
		FLAG_OK,
		FLAG_ERROR
	};
};

#endif // ADDRESSES_H
