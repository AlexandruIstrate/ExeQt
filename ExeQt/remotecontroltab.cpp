/**************************************************************************
 *
 * Copyright (c) 2018 Alexandru Istrate
 *
 * This file is subject to the terms and conditions defined in the
 * file 'LICENSE', which is part of this source code package.
 *
**************************************************************************/

#include "remotecontroltab.h"
#include "ui_remotecontroltab.h"

#include <QTreeWidgetItem>

#include "mainwidget.h"
#include "remotecontrol.h"
#include "addgroupdialog.h"

RemoteControlTab::RemoteControlTab(Client& client, QWidget* parent) :
	QWidget(parent), ui(new Ui::RemoteControlTab),
	m_Client { client }
{
	ui->setupUi(this);
	setupUI();

	setupTree();
	setupSignalsAndSlots();
}

RemoteControlTab::~RemoteControlTab()
{
	delete ui;
}

void RemoteControlTab::setupUI()
{
	ui->verticalLayout->insertWidget(0, &m_ActionTree);
}

void RemoteControlTab::setupSignalsAndSlots()
{
	connect(ui->btnRun, &QPushButton::clicked, this, &RemoteControlTab::onRun);
	connect(&m_ActionTree, &QTreeWidget::doubleClicked, this, &RemoteControlTab::onDoubleClick);

	connect(&m_Client, &Client::actionsUpdated, this, &RemoteControlTab::onActionsUpdated);
}

void RemoteControlTab::setupTree()
{
	m_ActionTree.setHeaderLabel(QString("Client ID: ") + m_Client.getID());
}

void RemoteControlTab::addToTree(const Bundle& bundle)
{
	m_ActionTree.setActionBundle(bundle);
}

void RemoteControlTab::clearTree()
{
	m_ActionTree.clear();
}

void RemoteControlTab::onRun()
{
	TreeItem* treeItem = dynamic_cast<TreeItem*>(m_ActionTree.currentItem());

	if (!treeItem)
	{
		return;
	}

	if (treeItem->getType() != TreeItem::Type::ACTION && treeItem->getType() != TreeItem::Type::ACTION_REFERENCE)
	{
		return;
	}

	ActionReference* ref = treeItem->getActionReference();

	if (!ref)
	{
		return;
	}

	m_Client.callAction(*ref);
	m_ActionTree.expandItem(treeItem);
}

void RemoteControlTab::onDoubleClick(const QModelIndex&)
{
	onRun();
}

void RemoteControlTab::onActionsUpdated(Bundle actions)
{
	clearTree();
	addToTree(actions);
}
