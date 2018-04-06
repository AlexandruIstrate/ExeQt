#include "actionitem.h"
#include "ui_actionitem.h"

#include <QDebug>
#include <QVBoxLayout>

#define TAG_NAME                "actionItem"
#define ACTION_TYPE_PROPERTY    "actionType"

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
    delete m_Action;
}

QString ActionItem::getTagName() const
{
    return QString("actionItem");
}

void ActionItem::readProperties(Bundle& bundle)
{
    Action::Type type = (Action::Type) bundle.get(ACTION_TYPE_PROPERTY).toInt();
    m_Action = Action::create(type);
    m_Action->readProperties(bundle.childAt(0));
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
    // TODO: Refactor this
    delete m_Action;    // Delete old memory
    m_Action = Action::create(type);

    ui->widgetStack->addWidget(m_Action);
    ui->widgetStack->removeWidget(0);
}

void ActionItem::setActionWidget()
{
    ui->widgetStack->addWidget(m_Action);
}

void ActionItem::accept()
{
    m_Action->setName(ui->edtName->text());
    QDialog::accept();
}

void ActionItem::onActionTypeChanged(int index)
{
    setAction((Action::Type) index);
}
