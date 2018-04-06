#include "commandaction.h"
#include "ui_commandaction.h"

#include <QDebug>

#include <stdio.h>
#include <stdlib.h>

#define COMMAND_PROPERTY "command"

CommandAction::CommandAction(const QString& command, const QString& name, QWidget* parent) :
    Action(name, Action::Type::COMMAND, parent),
    ui(new Ui::CommandAction),
    m_Command { command }
{
    ui->setupUi(this);
    setupSignalsAndSlots();
}

CommandAction::CommandAction(const QString& name, QWidget* parent) :
    Action(name, Action::Type::COMMAND, parent),
    ui(new Ui::CommandAction),
    m_Command { "" }
{
    ui->setupUi(this);
    setupSignalsAndSlots();
}

CommandAction::~CommandAction()
{
    delete ui;
}

QString CommandAction::getTagName() const
{
    return QString("commandAction");
}

void CommandAction::readProperties(Bundle& bundle)
{
    m_Command = bundle.get(COMMAND_PROPERTY);
    Action::readProperties(bundle);
}

void CommandAction::writeProperties(Bundle& bundle)
{
    bundle.add(COMMAND_PROPERTY, m_Command);
    Action::writeProperties(bundle);
}

void CommandAction::execute()
{
    system(m_Command.toStdString().c_str());
}

void CommandAction::setupSignalsAndSlots()
{
    connect(ui->edtCommand, SIGNAL(textChanged(QString)), this, SLOT(onEdit(QString)));
}

void CommandAction::onEdit(QString text)
{
    m_Command = text;
}
