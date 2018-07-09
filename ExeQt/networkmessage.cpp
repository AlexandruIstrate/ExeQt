/**************************************************************************
 *
 * Copyright (c) 2018 Alexandru Istrate
 *
 * This file is subject to the terms and conditions defined in the
 * file 'LICENSE', which is part of this source code package.
 *
**************************************************************************/

#include "networkmessage.h"

#include <QDebug>

#include <QJsonDocument>
#include <QJsonObject>

#define JSON_KEY_IDENTIFIER	"appIdentifier"

NetworkMessage::NetworkMessage(const QString& appIdentifier) : m_AppIdentifier { appIdentifier }, m_HasError { false }
{

}

NetworkMessage::NetworkMessage(const QString& appIdentifier, const QString& json) : m_AppIdentifier { appIdentifier }, m_HasError { false }
{
	parseJson(json);
}

QList<QString> NetworkMessage::getPropertyList() const
{
	return m_Properties.keys();
}

void NetworkMessage::setProperty(const QString& key, const QString& value)
{
	m_Properties.insert(key, value);
}

QString NetworkMessage::getProperty(const QString& key)
{
	return m_Properties.value(key);
}

bool NetworkMessage::hasProperty(const QString& key)
{
	return m_Properties.contains(key);
}

QString NetworkMessage::buildMessage() const
{
	QJsonObject obj;
	obj.insert(JSON_KEY_IDENTIFIER, m_AppIdentifier);

	for (QMap<QString, QString>::const_iterator iter = m_Properties.constBegin(); iter != m_Properties.constEnd(); ++iter)
		obj.insert(iter.key(), iter.value());

	return QJsonDocument(obj).toJson(QJsonDocument::Compact);
}

void NetworkMessage::parseJson(const QString& json)
{
	QJsonDocument doc = QJsonDocument::fromJson(json.toUtf8());
	QJsonObject object = doc.object();

	// Check that this request is a valid app request
	if (object.value(JSON_KEY_IDENTIFIER).toString() != m_AppIdentifier)
	{
		error("Message doesn't have the correct application key.");
		return;
	}

	for (QJsonObject::const_iterator iter = object.constBegin(); iter != object.constEnd(); ++iter)
	{
		if (iter.key() == JSON_KEY_IDENTIFIER)
			continue;

		m_Properties.insert(iter.key(), iter.value().toString());
	}
}

void NetworkMessage::error(const QString& message)
{
	m_HasError = true;
	m_ErrorMessage = message;
}
