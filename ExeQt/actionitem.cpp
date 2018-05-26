#include "actionitem.h"
#include "ui_actionitem.h"

#include <QDebug>
#include <QMessageBox>

#define ACTION_TYPE_PROPERTY "actionType"

#define CHILDREN_COUNT 1

ActionItem::ActionItem(QWidget* parent) :
	QDialog(parent), ui(new Ui::ActionItem), m_Action { nullptr }
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

	Action* action = Action::create(type);
	action->readProperties(bundle.childAt(0));

	ui->cmbActionType->setCurrentIndex((int) action->getType());    // Make sure we have the right type set
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
}

void ActionItem::setAction(Action::Type type)
{
	delete m_Action;    // Delete old memory
	m_Action = Action::create(type);

	setActionWidget();

	if (ui->widgetStack->count() > 1)
		ui->widgetStack->removeWidget(0);
}

void ActionItem::setAction(Action* action)
{
	delete m_Action;    // Delete old memory
	m_Action = action;

	setActionWidget();

	if (ui->widgetStack->count() > 1)
		ui->widgetStack->removeWidget(0);
}

void ActionItem::setActionWidget()
{
	ui->widgetStack->addWidget(m_Action);
}

void ActionItem::initBundle()
{
	ui->edtName->setText(m_Action->getName());
}

void ActionItem::accept()
{
	m_Action->setName(ui->edtName->text());

	if (!m_Action->validate())
		return;

	QDialog::accept();
}

void ActionItem::onActionTypeChanged(int index)
{
	setAction((Action::Type) index);
}
