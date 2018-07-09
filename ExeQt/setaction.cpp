/**************************************************************************
 *
 * Copyright (c) 2018 Alexandru Istrate
 *
 * This file is subject to the terms and conditions defined in the
 * file 'LICENSE', which is part of this source code package.
 *
**************************************************************************/

#include "setaction.h"
#include "ui_setaction.h"

#include <QDebug>
#include <QPushButton>
#include <QListWidgetItem>
#include <QMessageBox>

#include "mainwidget.h"
#include "task.h"

#define ACTION_NODE_NAME		"actionReference"
#define ACTION_GROUP_PROPERTY	"actionGroup"
#define ACTION_NAME_PROPERTY	"actionName"

SetAction::SetAction(const QString& name, QWidget* parent) :
	Action(name, Action::Type::SET, parent), ui(new Ui::SetAction)
{
	ui->setupUi(this);

	setupActions();

	setupUI();
	setupSignalsAndSlots();
}

SetAction::~SetAction()
{
	delete ui;
}

QString SetAction::getTagName() const
{
	return "setAction";
}

QString SetAction::getDescription() const
{
	return tr("A Set Action represents a set of actions that can be called together.");
}

bool SetAction::checkBundle(const Bundle&) const
{
	return true;
}

void SetAction::readProperties(Bundle& bundle)
{
	if (!checkBundle(bundle))
		return;

	m_Bundle = bundle;

	Task* task = new Task();
	connect(task, &Task::run, this, &SetAction::onRunTask);

	TaskManager::instance()->submitTask(task);

	Action::readProperties(bundle);
}

void SetAction::writeProperties(Bundle& bundle)
{
	for (Action* action : m_ActionList)
	{
		ActionReference ref = resolveAction(action);

		Bundle child(ACTION_NODE_NAME);
		child.add(ACTION_GROUP_PROPERTY, ref.getActionGroup());
		child.add(ACTION_NAME_PROPERTY, ref.getActionName());

		bundle.addChild(child);
	}

	Action::writeProperties(bundle);
}

void SetAction::execute()
{
	for (Action* action : m_ActionList)
		action->execute();
}

bool SetAction::validate()
{
	return true;
}

void SetAction::setupActions()
{
	m_ActionUp = new QAction(tr("Up"), this);
	m_ActionDown = new QAction(tr("Down"), this);

	m_ActionAdd = new QAction(tr("Add"), this);
	m_ActionRemove = new QAction(tr("Remove"), this);
}

void SetAction::setupSignalsAndSlots()
{
	connect(m_ActionUp, &QAction::triggered, this, &SetAction::onUp);
	connect(m_ActionDown, &QAction::triggered, this, &SetAction::onDown);
	connect(m_ActionAdd, &QAction::triggered, this, &SetAction::onAdd);
	connect(m_ActionRemove, &QAction::triggered, this, &SetAction::onRemove);

	connect(m_ActionTree, &ActionSetTree::rowsUpdated, this, &SetAction::onRowsUpdated);
}

void SetAction::setupUI()
{
	connect(ui->btnUp, &QPushButton::clicked, m_ActionUp, &QAction::trigger);
	connect(ui->btnDown, &QPushButton::clicked, m_ActionDown, &QAction::trigger);
	connect(ui->btnAdd, &QPushButton::clicked, m_ActionAdd, &QAction::trigger);
	connect(ui->btnRemove, &QPushButton::clicked, m_ActionRemove, &QAction::trigger);

	m_ActionTree = new ActionSetTree(this);
	ui->horizontalLayout->insertWidget(0, m_ActionTree);
}

ActionTabList SetAction::getActionGroups()
{
	return MainWidget::instance()->getActionTabs();
}

ActionReference SetAction::resolveAction(Action* action)
{
	for (ActionTab* tab : getActionGroups())
	{
		for(ActionItem* actionItem : tab->getActionItems())
		{
			if (actionItem->getAction() == action)
				return ActionReference(tab->getName(), actionItem->getName());
		}
	}

	return ActionReference(QString(), QString());
}

void SetAction::addAction(Action* action)
{
	m_ActionList.append(action);
}

void SetAction::removeAction(int index)
{
	m_ActionList.removeAt(index);
}

void SetAction::onUp()
{
	m_ActionTree->moveRowUp();
}

void SetAction::onDown()
{
	m_ActionTree->moveRowDown();
}

void SetAction::onAdd()
{
	m_ActionTree->addRow();
}

void SetAction::onRemove()
{
	m_ActionTree->removeRow();
}

void SetAction::onRowsUpdated(ActionReferenceList rowReffList)
{
	m_ActionList.clear();

	for (const ActionReference& ref : rowReffList)
	{
		Action* action = ref.getAction();
		if (action)
			addAction(action);
	}
}

void SetAction::onRunTask()
{
	ActionReferenceList actionRefs;

	for (const Bundle& child : m_Bundle.getChildren())
	{
		const QString& actionGroup = child.get(ACTION_GROUP_PROPERTY);
		const QString& actionName = child.get(ACTION_NAME_PROPERTY);

		ActionReference ref = ActionReference(actionGroup, actionName);
		Action* action = ref.getAction();

		if (!ref.isOk())
		{
			QMessageBox::critical(nullptr, tr("Invalid Action Reference"), tr(QString("The reference %1::%2 doesn't exist!").
																		arg(actionGroup, actionName).toStdString().c_str()));
			continue;
		}

		if (action == this)
		{
			QMessageBox::critical(nullptr, tr("Circular Reference"), tr(QString("The reference %1::%2 is refering to itself!").
																		arg(actionGroup, actionName).toStdString().c_str()));
			continue;
		}

		actionRefs.append(ref);
		m_ActionList.append(action);
	}

	m_ActionTree->useList(actionRefs);
}
