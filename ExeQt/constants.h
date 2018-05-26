#ifndef ADDRESSES_H
#define ADDRESSES_H

#include <QString>

class Constants {
public:
	static QString SERVER_ADDRESS;
//	static const QString UPLOAD_PATH;
//	static const QString DOWNLOAD_PATH;
//	static const QString AUTH_PATH;
	static QString getUploadPath();
	static QString getDownloadPath();
	static QString getAuthPath();

	static const QString SAVE_FILE_NAME;

	static const QString JSON_PROPERTY_FLAG;
	static const QString JSON_PROPERTY_MESSSAGE;

	static const QString COMPANY_NAME;
	static const QString PRODUCT_NAME;

	static const QString SETTING_KEY_IP;

	enum
	{
		FLAG_OK,
		FLAG_ERROR
	};

};

#endif // ADDRESSES_H
