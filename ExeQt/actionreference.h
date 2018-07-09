/**************************************************************************
 *
 * Copyright (c) 2018 Alexandru Istrate
 *
 * This file is subject to the terms and conditions defined in the
 * file 'LICENSE', which is part of this source code package.
 *
**************************************************************************/

#ifndef ACTIONREFERENCE_H
#define ACTIONREFERENCE_H

#include <QString>

#include "action.h"

class CommandAction;
class ApplicationAction;
class LinkAction;
class SetAction;

class ActionReference
{
private:
	QString m_ActionGroup;
	QString m_ActionName;

public:
	ActionReference(const QString& group, const QString& name);
	ActionReference(const QString& actionString);

	~ActionReference();

	inline const QString& getActionGroup() const { return m_ActionGroup; }
	inline void setActionGroup(const QString& actionGroup) { m_ActionGroup = actionGroup; }

	inline const QString& getActionName() const { return m_ActionName; }
	inline void setActionName(const QString& actionName) { m_ActionName = actionName; }

	bool isValid() const;
	bool exists() const;
	bool isOk() const;

	QString toString() const;

	Action::Type getActionType() const;

	template <typename T>
	T* getAction() const
	{
		return (T*) getActionInternal();
	}

	Action* getAction() const
	{
		return (Action*) getActionInternal();
	}

private:
	Action* getActionInternal() const;

	void parseString(const QString&);
};

#endif // ACTIONREFERENCE_H
