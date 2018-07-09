/**************************************************************************
 *
 * Copyright (c) 2018 Alexandru Istrate
 *
 * This file is subject to the terms and conditions defined in the
 * file 'LICENSE', which is part of this source code package.
 *
**************************************************************************/

#ifndef NETWORKMESSAGEBUILDER_H
#define NETWORKMESSAGEBUILDER_H

#include <QString>
#include <QMap>

class NetworkMessage
{
private:
	QString m_AppIdentifier;
	QMap<QString, QString> m_Properties;

	bool m_HasError;
	QString m_ErrorMessage;

public:
	NetworkMessage(const QString& appIdentifier);
	NetworkMessage(const QString& appIdentifier, const QString& json);

	inline bool hasError() const { return m_HasError; }
	inline const QString& getErrorMessage() const { return m_ErrorMessage; }

	QList<QString> getPropertyList() const;

	void setProperty(const QString& key, const QString& value);
	QString getProperty(const QString& key);

	bool hasProperty(const QString& key);

	QString buildMessage() const;

private:
	void parseJson(const QString& json);
	void error(const QString& message);
};

#endif // NETWORKMESSAGEBUILDER_H
