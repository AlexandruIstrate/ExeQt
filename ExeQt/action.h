/**************************************************************************
 *
 * Copyright (c) 2018 Alexandru Istrate
 *
 * This file is subject to the terms and conditions defined in the
 * file 'LICENSE', which is part of this source code package.
 *
**************************************************************************/

#ifndef ACTION_H
#define ACTION_H

#include <QWidget>
#include <QAction>
#include <QString>
#include <QIcon>

#include "saveable.h"

class Action : public QWidget, public Saveable
{
public:
	enum class Type
	{
		COMMAND, APPLICATION, LINK, SET, UNKNOWN
	};

protected:
	Type m_Type;
	QString m_Name;

public:
	Action(const QString& name, Type type, QWidget* parent = nullptr);
	virtual ~Action();

	inline void setName(const QString& name) { m_Name = name; }
	inline const QString& getName() const { return m_Name; }

	inline Type getType() const { return m_Type; }

	QIcon getIcon() const;

	virtual QString getTagName() const override;

	virtual bool checkBundle(const Bundle&) const override;

	virtual void readProperties(Bundle&) override;
	virtual void writeProperties(Bundle&) override;

public:
	virtual QString getDescription() const = 0;

	virtual void execute() = 0;
	virtual bool validate() = 0;

public:
	static Action* create(Type type, QWidget* parent = nullptr);
	static Action* createFromTagName(const QString& tagName, QWidget* parent = nullptr);

	static QIcon getActionIcon(Type type);
	static QIcon getActionIcon(const QString& tag);

	static QString getTypeName(Type type);

	static QString getActionTag(Type type);
	static Type getActionTypeFromTag(const QString& tag);
};

#endif // ACTION_H
