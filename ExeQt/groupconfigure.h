/**************************************************************************
 *
 * Copyright (c) 2018 Alexandru Istrate
 *
 * This file is subject to the terms and conditions defined in the
 * file 'LICENSE', which is part of this source code package.
 *
**************************************************************************/

#ifndef GROUPCONFIGURE_H
#define GROUPCONFIGURE_H

#include <QDialog>
#include <QAction>
#include <QList>

#include "actiontab.h"
#include "mainwidget.h"

namespace Ui {
class GroupConfigure;
}

class GroupConfigure : public QDialog
{
	Q_OBJECT

private:
	Ui::GroupConfigure* ui;

	QAction* m_ActionAdd;
	QAction* m_ActionRemove;
	QAction* m_ActionEdit;

	ActionTabList& m_ActionTabs;

public:
	GroupConfigure(MainWidget* parent = nullptr);
	~GroupConfigure();

private:
	void setupActions();
	void setupSignalsAndSlots();
	void setupUI();
	void setupDialogButtons();

	int getSelectedItem();
	void showActionGroups();

	bool checkDelete();

private slots:
	void onAdd();
	void onRemove();
	void onEdit();

	void onDoubleClick();
};

#endif // GROUPCONFIGURE_H
