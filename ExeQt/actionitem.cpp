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

#define ACTION_TYPE_PROPERTY "actionType"

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

bool ActionItem::checkBundle(const Bundle& bundle) const
{
	if (!checkProperty(bundle, ACTION_TYPE_PROPERTY))
		return false;

	if (bundle.getChildrenCount() != CHILDREN_COUNT)
	{
		QMessageBox::critical(nullptr, tr("XML Parse Error"), tr("The action item \"%1\" doesn't have the right number of children (%2)!").arg(getName(), QString::number(CHILDREN_COUNT)));
		return false;
	}

	return true;
}

void ActionItem::readProperties(Bundle& bundle)
{
	if (!checkBundle(bundle))
		return;

	Action::Type type = (Action::Type) bundle.get(ACTION_TYPE_PROPERTY).toInt();

	Action* action = Action::create(type, this);
	action->readProperties(bundle.childAt(0));

	ui->cmbActionType->setCurrentIndex((int) action->getType());    // Here, we make sure we have the right type set
	setAction(action);

	initBundle();
}

void ActionItem::writeProperties(Bundle& bundle)
{
	bundle.add(ACTION_TYPE_PROPERTY, QString::number((int) m_Action->getType()));

	Bundle child(m_Action->getTagName());
	m_Action->writeProperties(child);
	bundle.addChild(child);
}

void ActionItem::setupSignalsAndSlots()
{
	connect(ui->cmbActionType, SIGNAL(currentIndexChanged(int)), this, SLOT(onActionTypeChanged(int)));
	connect(ui->edtName, &QLineEdit::textChanged, this, &ActionItem::onNameChanged);
}

void ActionItem::setupUI()
{
	setupActions();
	setAction(Action::Type::COMMAND);
}

void ActionItem::setupActions()
{
	ui->cmbActionType->addItem(Action::getTypeName(Action::Type::COMMAND));
	ui->cmbActionType->addItem(Action::getTypeName(Action::Type::APPLICATION));
	ui->cmbActionType->addItem(Action::getTypeName(Action::Type::LINK));
	ui->cmbActionType->addItem(Action::getTypeName(Action::Type::SET));
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
