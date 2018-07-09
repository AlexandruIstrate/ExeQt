/**************************************************************************
 *
 * Copyright (c) 2018 Alexandru Istrate
 *
 * This file is subject to the terms and conditions defined in the
 * file 'LICENSE', which is part of this source code package.
 *
**************************************************************************/

#ifndef SETACTION_H
#define SETACTION_H

#include <QWidget>
#include <QAction>
#include <QList>

#include "action.h"
#include "actionsettree.h"
#include "mainwidget.h"

namespace Ui {
class SetAction;
}

class SetAction : public Action
{
	Q_OBJECT

private:
	Ui::SetAction* ui;
	ActionSetTree* m_ActionTree;

	QAction* m_ActionUp;
	QAction* m_ActionDown;

	QAction* m_ActionAdd;
	QAction* m_ActionRemove;

	QList<Action*> m_ActionList;
	Bundle m_Bundle;

public:
	SetAction(const QString& name = QString(), QWidget* parent = nullptr);
	~SetAction();

	QString getTagName() const override;
	QString getDescription() const override;

	bool checkBundle(const Bundle&) const override;

	void readProperties(Bundle&) override;
	void writeProperties(Bundle&) override;

	void execute() override;
	bool validate() override;

private:
	void setupActions();
	void setupSignalsAndSlots();
	void setupUI();

	ActionTabList getActionGroups();

	Action* resolveAction(const ActionReference& ref);
	ActionReference resolveAction(Action* action);

	void addAction(Action* action);
	void removeAction(int index);

private slots:
	void onUp();
	void onDown();

	void onAdd();
	void onRemove();

	void onRowsUpdated(ActionReferenceList rowRefList);

	void onRunTask();
};

#endif // SETACTION_H
