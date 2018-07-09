/**************************************************************************
 *
 * Copyright (c) 2018 Alexandru Istrate
 *
 * This file is subject to the terms and conditions defined in the
 * file 'LICENSE', which is part of this source code package.
 *
**************************************************************************/

#ifndef ACTIONTAB_H
#define ACTIONTAB_H

#include <QWidget>
#include <QAction>
#include <QString>
#include <QVector>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QIcon>
#include <QModelIndex>

#include "actionitem.h"
#include "saveable.h"
#include "appicon.h"

typedef QVector<ActionItem*> ActionItemList;

namespace Ui {
class ActionTab;
}

class ActionTab : public QWidget, public Saveable
{
	Q_OBJECT

private:
	Ui::ActionTab* ui;
	QSystemTrayIcon* m_TrayIcon;
	QMenu* m_TrayMenu;

	QAction* m_AddAction;
	QAction* m_RemoveAction;
	QAction* m_EditAction;

	QAction* m_GhostAction;

	QString m_Name;
	AppIcon m_Icon;

	ActionItemList m_ActionItems;

public:
	ActionTab(const QString& name, const AppIcon &icon, QWidget* parent = nullptr);
	~ActionTab();

	inline const QString& getName() const { return m_Name; }
	void setName(const QString&);

	inline const AppIcon& getIcon() const { return m_Icon; }
	void setIcon(const AppIcon&);

	inline const ActionItemList& getActionItems() const { return m_ActionItems; }

	QString getTagName() const override;

	bool checkBundle(const Bundle&) const override;

	void readProperties(Bundle&) override;
	void writeProperties(Bundle&) override;

	void removeTrayIcon();

private:
	void setupActions();
	void setupSignalsAndSlots();

	void setupUI();
	void setupTrayIcon();

	void addAction(ActionItem*);
	void removeAction(ActionItem*);

	void updateActions();
	void addGhostAction();
	void removeGhostAction();

private slots:
	void onActionAdd();
	void onActionRemove();
	void onActionEdit();

	void onDoubleClick();
};

#endif // ACTIONTAB_H
