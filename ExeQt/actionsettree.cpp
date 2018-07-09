/**************************************************************************
 *
 * Copyright (c) 2018 Alexandru Istrate
 *
 * This file is subject to the terms and conditions defined in the
 * file 'LICENSE', which is part of this source code package.
 *
**************************************************************************/

#include "actionsettree.h"

#include <QDebug>
#include <QStringList>

#include "actionreference.h"

#define COL_GROUP	0
#define COL_ACTION	1

// ActionRow

ActionRow::ActionRow(ActionSetTree* parent) : QTreeWidgetItem { parent }
{
	init();
	setupSignalsAndSlots();
}

ActionRow::ActionRow(ActionSetTree* parent, const ActionReference& reference) : QTreeWidgetItem { parent }
{
	init();
	setupSignalsAndSlots();

	selectComboItem(m_CmbGroup, reference.getActionGroup());
	selectComboItem(m_CmbAction, reference.getActionName());
}

ActionReference ActionRow::getActionRefference()
{
	return ActionReference(getGroup(), getAction());
}

void ActionRow::reload()
{
	QString group = getGroup();
	QString action = getAction();

	delete m_CmbGroup;
	delete m_CmbAction;

	init();

	selectComboItem(m_CmbGroup, group);
	selectComboItem(m_CmbAction, action);

	m_ActionGroup = group;
	m_ActionName = action;
}

void ActionRow::init()
{
	m_CmbGroup = new QComboBox();
	m_CmbAction = new QComboBox();

	initCombos();
	loadChoices();
}

void ActionRow::initCombos()
{
	QTreeWidget* tree = treeWidget();
	tree->setItemWidget(this, COL_GROUP, m_CmbGroup);
	tree->setItemWidget(this, COL_ACTION, m_CmbAction);
}

void ActionRow::setupSignalsAndSlots()
{
	connect(m_CmbGroup, SIGNAL(currentIndexChanged(QString)), this, SLOT(onGroupChange(QString)));
	connect(m_CmbAction, SIGNAL(currentIndexChanged(QString)), this, SLOT(onActionChange(QString)));
}

void ActionRow::loadGroups()
{
	m_CmbGroup->clear();

	for (ActionTab* tab : getActionGroups())
		m_CmbGroup->addItem(tab->getIcon().icon, tab->getName());

	m_ActionGroup = m_CmbGroup->currentText();
}

void ActionRow::loadActions()
{
	m_CmbAction->clear();

	for (ActionTab* tab : getActionGroups())
	{
		if (tab->getName() != m_CmbGroup->currentText())
			continue;

		for (ActionItem* actionItem : tab->getActionItems())
		{
			Action* action = actionItem->getAction();

			if (action != getParent()->getParentAction())
				m_CmbAction->addItem(action->getIcon(), actionItem->getName());
		}
	}

	m_ActionName = m_CmbAction->currentText();
}

void ActionRow::loadChoices()
{
	loadGroups();
	loadActions();
}

ActionTabList ActionRow::getActionGroups()
{
	return MainWidget::instance()->getActionTabs();
}

ActionSetTree* ActionRow::getParent()
{
	return (ActionSetTree*) treeWidget();
}

void ActionRow::selectComboItem(QComboBox* combo, const QString& txt)
{
	for (int i = 0; i < combo->count(); ++i)
	{
		if (combo->itemText(i) == txt)
		{
			combo->setCurrentIndex(i);
			break;
		}
	}
}

void ActionRow::onGroupChange(const QString& group)
{
	m_ActionGroup = group;

	loadActions();
	emit selectionUpdate();
}

void ActionRow::onActionChange(const QString& name)
{
	m_ActionName = name;
	emit selectionUpdate();
}

// ActionSetTree

ActionSetTree::ActionSetTree(Action* parent) : m_ParentAction { parent }
{
	setup();
}

ActionSetTree::~ActionSetTree()
{
	for (int i = 0; i < m_Rows.size(); ++i)
		delete m_Rows[i];
}

ActionReferenceList ActionSetTree::getActionRefferenceList()
{
	ActionReferenceList ret;
	for (ActionRow* row : m_Rows)
		ret.append(row->getActionRefference());

	return ret;
}

void ActionSetTree::addRow()
{
	ActionRow* row = new ActionRow(this);
	connect(row, &ActionRow::selectionUpdate, this, &ActionSetTree::onRowUpdate);

	m_Rows.append(row);
	updateRows();
}

void ActionSetTree::addRow(const ActionReference& ref)
{
	ActionRow* row = new ActionRow(this, ref);
//	row->setText(0, QString::number(m_Rows.size()));
	connect(row, &ActionRow::selectionUpdate, this, &ActionSetTree::onRowUpdate);

	m_Rows.append(row);
	updateRows();
}

void ActionSetTree::removeRow()
{
	int index = currentIndex().row();
	if (index == -1)
		return;

	delete m_Rows.takeAt(index);
	updateRows();
}

void ActionSetTree::moveRowUp()
{
	int index = currentIndex().row();
	if (index == -1 || index == 0)
		return;

	ActionRow* row = (ActionRow*) takeTopLevelItem(index);
	insertTopLevelItem(index - 1, row);
	row->reload();

	m_Rows.swap(index, index - 1);

	clearSelection();
	row->setSelected(true);

	updateRows();
}

void ActionSetTree::moveRowDown()
{
	int index = currentIndex().row();
	if (index == -1 || index == topLevelItemCount() - 1)
		return;

	ActionRow* row = (ActionRow*) takeTopLevelItem(index);
	insertTopLevelItem(index + 1, row);
	row->reload();

	m_Rows.swap(index, index + 1);

	clearSelection();
	row->setSelected(true);

	updateRows();
}

void ActionSetTree::useList(const ActionReferenceList& list)
{
	for (const ActionReference& ref : list)
		addRow(ref);
}

void ActionSetTree::setup()
{
	setupHeaders();
	setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);
}

void ActionSetTree::setupHeaders()
{
	QStringList headers;
	headers.append(tr("Group"));
	headers.append(tr("Action"));

	setColumnCount(headers.size());
	setHeaderLabels(headers);
}

void ActionSetTree::resizeColumns()
{
	for (int i = 0; i < columnCount(); ++i)
		resizeColumnToContents(i);
}

void ActionSetTree::updateRows()
{
//	for (int i = 0; i < topLevelItemCount(); ++i)
//		qDebug() << ((ActionRow*) topLevelItem(i))->getActionRefference().toString();

	emit rowsUpdated(getActionRefferenceList());
}

void ActionSetTree::onRowUpdate()
{
	updateRows();
}
