/**************************************************************************
 *
 * Copyright (c) 2018 Alexandru Istrate
 *
 * This file is subject to the terms and conditions defined in the
 * file 'LICENSE', which is part of this source code package.
 *
**************************************************************************/

#include "actionitem.h"
#include "ui_actionitem.h"

#include <QDebug>

#include <QMessageBox>
#include <QPushButton>

#include "mainwidget.h"
#include "commandaction.h"

#define ACTION_TYPE_PROPERTY "type"

#define CHILDREN_COUNT 1

static const QList<QString> FORBIDDEN_ENTRIES { ":" };

ActionItem::ActionItem(QWidget* parent) :
	QDialog(parent), ui(new Ui::ActionItem), m_Action { nullptr }, m_Valid { false }
{
	ui->setupUi(this);

	setupUI();
	setupSignalsAndSlots();
}

ActionItem::~ActionItem()
{
	delete ui;
}

QString ActionItem::getTagName() const
{
	return QString("actionItem");
}

bool ActionItem::checkBundle(const Bundle&) const
{
	return true;
}

void ActionItem::readProperties(Bundle& bundle)
{
	if (!checkBundle(bundle))
		return;

	Action* action = Action::createFromTagName(bundle.getName(), this);

	if (action)
		action->readProperties(bundle);
	else
		action = new CommandAction("", "Unknown Action");

	ui->cmbActionType->setCurrentIndex((int) action->getType());    // Here, we make sure we have the right type set
	setAction(action);

	initBundle();
}

void ActionItem::writeProperties(Bundle& bundle)
{
	m_Action->writeProperties(bundle);
}

void ActionItem::setupSignalsAndSlots()
{
	connect(ui->cmbActionType, SIGNAL(currentIndexChanged(int)), this, SLOT(onActionTypeChanged(int)));
	connect(ui->edtName, &QLineEdit::textChanged, this, &ActionItem::onNameChanged);
}

void ActionItem::setupUI()
{
	setupDialogButtons();

	setupActions();
	setAction(Action::Type::COMMAND);
}

void ActionItem::setupDialogButtons()
{
	ui->buttonBox->button(QDialogButtonBox::StandardButton::Ok)->setIcon(QIcon(":/assets/images/button-icons/ok.png"));
	ui->buttonBox->button(QDialogButtonBox::StandardButton::Cancel)->setIcon(QIcon(":/assets/images/button-icons/cancel.png"));
}

void ActionItem::setupActions()
{
	addComboAction(Action::Type::COMMAND);
	addComboAction(Action::Type::APPLICATION);
	addComboAction(Action::Type::LINK);
	addComboAction(Action::Type::SET);
}

void ActionItem::addComboAction(Action::Type type)
{
	ui->cmbActionType->addItem(Action::getActionIcon(type), Action::getTypeName(type));
}

void ActionItem::setAction(Action::Type type)
{
	setAction(Action::create(type));
}

void ActionItem::setAction(Action* action)
{
	delete m_Action;    // Delete old memory
	m_Action = action;

	setActionWidget();

	if (ui->widgetStack->count() > 1)
		ui->widgetStack->removeWidget(0);

	ui->lblDescription->setToolTip(action->getDescription());
}

void ActionItem::setActionWidget()
{
	ui->widgetStack->addWidget(m_Action);
}

ActionTab* ActionItem::getActionTab()
{
	return dynamic_cast<ActionTab*>(parent());
}

void ActionItem::setValid(bool state)
{
	m_Valid = state;
	ui->buttonBox->button(QDialogButtonBox::StandardButton::Ok)->setEnabled(state);
}

void ActionItem::setErrorMessage(const QString& message)
{
	ui->lblError->setText(message);
}

void ActionItem::clearErrorMessage()
{
	ui->lblError->clear();
}

bool ActionItem::checkName(const QString& name)
{
	for (const QString& item : FORBIDDEN_ENTRIES)
	{
		if (name.contains(item))
		{
			setErrorMessage(QString("The name contains a forbidden token \"%1\"").arg(item));
			return false;
		}
	}

	// Check to see if this action already exists
	for (const ActionTab* tab : MainWidget::instance()->getActionTabs())
	{
		if (tab != getActionTab())
			continue;

		for (const ActionItem* actionItem : tab->getActionItems())
		{
			if (actionItem == this)
				continue;

			if (actionItem->getName() == name)
			{
				setErrorMessage("There is already an action with this name in this group");
				return false;
			}
		}
	}

	if (name.isEmpty())
	{
		setErrorMessage("You must provide a name for the action");
		return false;
	}

	return true;
}

void ActionItem::initBundle()
{
	ui->edtName->setText(m_Action->getName());
}

void ActionItem::accept()
{
	if (m_Valid)
		m_Action->setName(ui->edtName->text());

	if (!m_Action->validate())
		return;

	QDialog::accept();
}

void ActionItem::onActionTypeChanged(int index)
{
	setAction((Action::Type) index);
}

void ActionItem::onNameChanged(const QString& name)
{
	if (checkName(name))
	{
		clearErrorMessage();
		setValid(true);
	}
	else
	{
		setValid(false);
	}
}
