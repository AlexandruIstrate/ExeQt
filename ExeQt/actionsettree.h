/**************************************************************************
 *
 * Copyright (c) 2018 Alexandru Istrate
 *
 * This file is subject to the terms and conditions defined in the
 * file 'LICENSE', which is part of this source code package.
 *
**************************************************************************/

#ifndef ACTIONSETTREE_H
#define ACTIONSETTREE_H

#include <QObject>
#include <QWidget>
#include <QString>
#include <QList>
#include <QPair>

#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QComboBox>

#include "mainwidget.h"
#include "actionreference.h"

typedef QList<ActionReference> ActionReferenceList;
typedef QList<Action*> ActionList;

class ActionSetTree;
class ActionReference;

class ActionRow : public QObject, public QTreeWidgetItem
{
	Q_OBJECT

private:
	QComboBox* m_CmbGroup;
	QComboBox* m_CmbAction;

	QString m_ActionGroup;
	QString m_ActionName;

public:
	ActionRow(ActionSetTree* parent);
	ActionRow(ActionSetTree* parent, const ActionReference& reference);

	inline const QString& getGroup() const { return m_ActionGroup; }
	inline const QString& getAction() const { return m_ActionName; }

	ActionReference getActionRefference();

	void reload();

private:
	void init();
	void initCombos();
	void setupSignalsAndSlots();

	void loadGroups();
	void loadActions();

	void loadChoices();

	ActionTabList getActionGroups();
	ActionSetTree* getParent();

	void selectComboItem(QComboBox* combo, const QString& txt);

signals:
	void selectionUpdate();

private slots:
	void onGroupChange(const QString&);
	void onActionChange(const QString&);
};

class ActionSetTree : public QTreeWidget
{
	Q_OBJECT

private:
	Action* m_ParentAction;
	QList<ActionRow*> m_Rows;

public:
	ActionSetTree(Action* parent);
	~ActionSetTree();

	inline Action* getParentAction() { return m_ParentAction; }

	ActionReferenceList getActionRefferenceList();

	void addRow();
	void addRow(const ActionReference& ref);

	void removeRow();

	void moveRowUp();
	void moveRowDown();

	void useList(const ActionReferenceList&);

private:
	void setup();
	void setupHeaders();

	void resizeColumns();
	void updateRows();

signals:
	void rowsUpdated(ActionReferenceList rowReffList);

private slots:
	void onRowUpdate();
};

#endif // ACTIONSETTREE_H
