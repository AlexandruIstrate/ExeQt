/**************************************************************************
 *
 * Copyright (c) 2018 Alexandru Istrate
 *
 * This file is subject to the terms and conditions defined in the
 * file 'LICENSE', which is part of this source code package.
 *
**************************************************************************/

#ifndef REMOTECONTROLTAB_H
#define REMOTECONTROLTAB_H

#include <QList>

#include <QWidget>
#include <QTreeWidgetItem>

#include "networkmanager.h"
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

namespace Ui {
class RemoteControlTab;
}

class RemoteControlTab : public QWidget
{
	Q_OBJECT

private:
	Ui::RemoteControlTab* ui;

	Client& m_Client;
	int m_TabIndex;

public:
	RemoteControlTab(Client& client, QWidget* parent = nullptr);
	~RemoteControlTab();

private:
	void setupSignalsAndSlots();
	void setupTree();

	void addToTree(const Bundle& bundle);
	void clearTree();

	void createTreeItem(TreeItem* parent, const Bundle& actionBundle);

private slots:
	void onRun();
	void onDoubleClick(const QModelIndex&);

	void onActionsUpdated(Bundle actions);
};

#endif // REMOTECONTROLTAB_H
