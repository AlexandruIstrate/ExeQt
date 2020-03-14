/**************************************************************************
 *
 * Copyright (c) 2018 Alexandru Istrate
 *
 * This file is subject to the terms and conditions defined in the
 * file 'LICENSE', which is part of this source code package.
 *
**************************************************************************/

#include "groupconfigure.h"
#include "ui_groupconfigure.h"

#include <QDebug>
#include <QListWidgetItem>
#include <QMessageBox>

#include "addgroupdialog.h"
#include "networkmanager.h"
#include "settingsregistry.h"

GroupConfigure::GroupConfigure(MainWidget* parent) :
	QDialog(parent), ui(new Ui::GroupConfigure),
	m_ActionTabs { parent->m_ActionTabs }
{
	ui->setupUi(this);

	setupUI();
	setupActions();
	setupSignalsAndSlots();
}

GroupConfigure::~GroupConfigure()
{
	delete ui;
}

void GroupConfigure::setupActions()
{
	m_ActionAdd = new QAction(tr("Add"), this);
	connect(m_ActionAdd, SIGNAL(triggered()), this, SLOT(onAdd()));

	m_ActionRemove = new QAction(tr("Remove"), this);
	connect(m_ActionRemove, SIGNAL(triggered()), this, SLOT(onRemove()));

	m_ActionEdit = new QAction(tr("Edit"), this);
	connect(m_ActionEdit, SIGNAL(triggered()), this, SLOT(onEdit()));
}

void GroupConfigure::setupSignalsAndSlots()
{
	connect(ui->btnAdd, SIGNAL(clicked()), m_ActionAdd, SLOT(trigger()));
	connect(ui->btnRemove, SIGNAL(clicked()), m_ActionRemove, SLOT(trigger()));
	connect(ui->btnEdit, SIGNAL(clicked()), m_ActionEdit, SLOT(trigger()));

	connect(ui->lstGroups, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(onDoubleClick()));
}

void GroupConfigure::setupUI()
{
	setupDialogButtons();
	showActionGroups();
}

void GroupConfigure::setupDialogButtons()
{
	ui->buttonBox->button(QDialogButtonBox::StandardButton::Ok)->setIcon(QIcon(":/assets/images/button-icons/ok.png"));
	ui->buttonBox->button(QDialogButtonBox::StandardButton::Cancel)->setIcon(QIcon(":/assets/images/button-icons/cancel.png"));
}

int GroupConfigure::getSelectedItem()
{
	return ui->lstGroups->currentRow();
}

void GroupConfigure::showActionGroups()
{
	ui->lstGroups->clear();

	for (ActionTab* tab : m_ActionTabs)
	{
		new QListWidgetItem(tab->getIcon().icon, tab->getName(), ui->lstGroups);
	}
}

bool GroupConfigure::checkDelete()
{
	if (!SettingsRegistry::instance()->get(Settings::CONFIRM_DELETE).toBool())
	{
		return true;
	}

	QMessageBox dialog(QMessageBox::Icon::Question, tr("Remove Action Group"),
					   tr("Are you sure you want to remove this action group?"), QMessageBox::Yes | QMessageBox::No, this);
	return dialog.exec() == QMessageBox::Yes;
}

void GroupConfigure::onAdd()
{
	MainWidget* mw = (MainWidget*) parent();
	mw->addNewActionGroup();

	showActionGroups();
	NetworkManager::instance()->requestActionUpdate();
}

void GroupConfigure::onRemove()
{
	int selectedItem = getSelectedItem();

	if (selectedItem == -1)
	{
		return;
	}

	if (!checkDelete())
	{
		return;
	}

	MainWidget* mw = (MainWidget*) parent();
	mw->removeActionGroup(selectedItem);

	delete ui->lstGroups->takeItem(selectedItem);

	showActionGroups();
	NetworkManager::instance()->requestActionUpdate();
}

void GroupConfigure::onEdit()
{
	AddGroupDialog* addDialog = new AddGroupDialog(m_ActionTabs.at(getSelectedItem()));
	addDialog->deleteLater();

	showActionGroups();
	NetworkManager::instance()->requestActionUpdate();
}

void GroupConfigure::onDoubleClick()
{
	m_ActionEdit->trigger();
}
