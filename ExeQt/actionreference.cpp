/**************************************************************************
 *
 * Copyright (c) 2018 Alexandru Istrate
 *
 * This file is subject to the terms and conditions defined in the
 * file 'LICENSE', which is part of this source code package.
 *
**************************************************************************/

#include "actionreference.h"

#include "actiontab.h"
#include "mainwidget.h"

#define SEPARATOR "::"

ActionReference::ActionReference(const QString& group, const QString& name)
	: m_ActionGroup { group }, m_ActionName { name }
{
}

ActionReference::ActionReference(const QString& actionString)
{
	parseString(actionString);
}

ActionReference::~ActionReference()
{
}

bool ActionReference::isValid() const
{
	return !m_ActionGroup.isEmpty() && !m_ActionName.isEmpty();
}

bool ActionReference::exists() const
{
	for (ActionTab* tab : MainWidget::instance()->getActionTabs())
	{
		for (ActionItem* actionItem : tab->getActionItems())
		{
			if (tab->getName() == m_ActionGroup && actionItem->getName() == m_ActionName)
			{
				return true;
			}
		}
	}

	return false;
}

bool ActionReference::isOk() const
{
	return isValid() && exists();
}

QString ActionReference::toString() const
{
	return m_ActionGroup + QString(SEPARATOR) + m_ActionName;
}

Action::Type ActionReference::getActionType() const
{
	for (ActionTab* tab : MainWidget::instance()->getActionTabs())
	{
		for (ActionItem* actionItem : tab->getActionItems())
		{
			if (tab->getName() == m_ActionGroup && actionItem->getName() == m_ActionName)
			{
				return actionItem->getAction()->getType();
			}
		}
	}

	return Action::Type::UNKNOWN;
}

bool ActionReference::operator==(const ActionReference& other)
{
	return (this->m_ActionGroup == other.m_ActionGroup &&
			this->m_ActionName == other.m_ActionName);
}

bool ActionReference::operator!=(const ActionReference& other)
{
	return !(*this == other);
}

Action* ActionReference::getActionInternal() const
{
	for (ActionTab* tab : MainWidget::instance()->getActionTabs())
	{
		for (ActionItem* actionItem : tab->getActionItems())
		{
			if (tab->getName() == m_ActionGroup && actionItem->getName() == m_ActionName)
			{
				return actionItem->getAction();
			}
		}
	}

	return nullptr;
}

void ActionReference::parseString(const QString& str)
{
	if (!str.contains(SEPARATOR))
	{
		return;
	}

	QStringList parts = str.split(SEPARATOR);
	m_ActionGroup = parts.at(0);
	m_ActionName = parts.at(1);
}
