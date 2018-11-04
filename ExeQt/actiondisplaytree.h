/**************************************************************************
 *
 * Copyright (c) 2018 Alexandru Istrate
 *
 * This file is subject to the terms and conditions defined in the
 * file 'LICENSE', which is part of this source code package.
 *
**************************************************************************/

#ifndef ACTIONDISPLAYTREE_H
#define ACTIONDISPLAYTREE_H

#include <QObject>
#include <QTreeWidget>
#include <QTreeWidgetItem>

#include "bundle.h"
#include "actionreference.h"

class TreeItem : public QTreeWidgetItem
{
public:
	enum class Type
	{
		UNKNOWN, TAB, ACTION, ACTION_REFERENCE
	};

protected:
	Type m_Type;
	QString m_Name;

	ActionReference* m_ActionReference;

public:
	TreeItem(Type type);
	TreeItem(Type type, const QString& name);
	TreeItem(ActionReference* reference, const QString& name = QString());

	~TreeItem();

	inline Type getType() const { return m_Type; }

	inline const QString& getName() const { return m_Name; }
	inline void setName(const QString& name) { m_Name = name; }

	inline ActionReference* getActionReference() { return m_ActionReference; }
	inline void setActionReference(ActionReference* reference) { m_ActionReference = reference; }

private:
	void setItemName(Type);
};

class ActionDisplayTree : public QTreeWidget
{
	Q_OBJECT

public:
	ActionDisplayTree(const Bundle& actionBundle);
	ActionDisplayTree();

	void setActionBundle(const Bundle& bundle);

private:
	void createTreeItem(TreeItem* parent, const Bundle& actionBundle);
};

#endif // ACTIONDISPLAYTREE_H
