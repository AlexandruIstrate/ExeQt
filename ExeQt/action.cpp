/**************************************************************************
 *
 * Copyright (c) 2018 Alexandru Istrate
 *
 * This file is subject to the terms and conditions defined in the
 * file 'LICENSE', which is part of this source code package.
 *
**************************************************************************/

#include "action.h"

#include <QDebug>

#include "commandaction.h"
#include "applicationaction.h"
#include "linkaction.h"
#include "setaction.h"

#include <QMessageBox>

#define NAME_PROPERTY "name"
#define TYPE_PROPERTY "type"

Action::Action(const QString& name, Type type, QWidget* parent)
	: QWidget { parent }, m_Type { type }, m_Name { name }
{

}

Action::~Action()
{

}

QIcon Action::getIcon() const
{
	return getActionIcon(m_Type);
}

QString Action::getTagName() const
{
	return QString("action");
}

bool Action::checkBundle(const Bundle& bundle) const
{
	if (!Saveable::checkBundle(bundle))
		return false;

	if (!checkProperty(bundle, NAME_PROPERTY) || !checkProperty(bundle, TYPE_PROPERTY))
		return false;

	return true;
}

void Action::readProperties(Bundle& bundle)
{
	if (!checkBundle(bundle))
		return;

	m_Name = bundle.get(NAME_PROPERTY);
	m_Type = (Type) bundle.get(TYPE_PROPERTY).toInt();
}

void Action::writeProperties(Bundle& bundle)
{
	bundle.add(NAME_PROPERTY, m_Name);
	bundle.add(TYPE_PROPERTY, QString::number((int) m_Type));
}

Action* Action::create(Type type, QWidget* parent)
{
	switch (type)
	{
	case Type::COMMAND:
		return new CommandAction("", parent);

	case Type::APPLICATION:
		return new ApplicationAction("", parent);

	case Type::LINK:
		return new LinkAction("", parent);

	case Type::SET:
		return new SetAction("", parent);

	default:
		return nullptr;
	}
}

QIcon Action::getActionIcon(Action::Type type)
{
	switch (type)
	{
	case Type::COMMAND:
		return QIcon(":/assets/images/action-types/terminal.png");

	case Type::APPLICATION:
		return QIcon(":/assets/images/action-types/applications.png");

	case Type::LINK:
		return QIcon(":/assets/images/action-types/link.png");

	case Type::SET:
		return QIcon(":/assets/images/action-types/set.png");

	default:
		return QIcon(":/assets/images/action-types/unknown.png");
	}
}

QIcon Action::getActionIcon(const QString& tag)
{
	return getActionIcon(getActionTypeFromTag(tag));
}

QString Action::getTypeName(Action::Type type)
{
	switch (type)
	{
	case Type::COMMAND:
		return tr("Command Action");

	case Type::APPLICATION:
		return tr("Application Action");

	case Type::LINK:
		return tr("Link Action");

	case Type::SET:
		return tr("Set Action");

	default:
		return tr("Unknown Action");
	}
}

QString Action::getActionTag(Action::Type type)
{
	switch (type)
	{
	case Type::COMMAND:
		return "commandAction";

	case Type::APPLICATION:
		return "applicationAction";

	case Type::LINK:
		return "linkAction";

	case Type::SET:
		return "setAction";

	default:
		return "unknown";
	}
}

Action::Type Action::getActionTypeFromTag(const QString& tag)
{
	if (tag == "commandAction")
		return Type::COMMAND;

	if (tag == "applicationAction")
		return Type::APPLICATION;

	if (tag == "linkAction")
		return Type::LINK;

	if (tag == "setAction")
		return Type::SET;

	return Type::UNKNOWN;
}
