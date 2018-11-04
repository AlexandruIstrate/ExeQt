/**************************************************************************
 *
 * Copyright (c) 2018 Alexandru Istrate
 *
 * This file is subject to the terms and conditions defined in the
 * file 'LICENSE', which is part of this source code package.
 *
**************************************************************************/

#include "commandaction.h"
#include "ui_commandaction.h"

#include <QDebug>

#include <stdio.h>
#include <stdlib.h>

#define COMMAND_PROPERTY "command"

CommandAction::CommandAction(const QString& command, const QString& name, QWidget* parent) :
	Action(name, Action::Type::COMMAND, ImageResource(), parent),
	ui(new Ui::CommandAction),
	m_Command { command }
{
	ui->setupUi(this);
	setupSignalsAndSlots();
}

CommandAction::CommandAction(const QString& name, QWidget* parent) :
	Action(name, Action::Type::COMMAND, ImageResource(), parent),
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

bool CommandAction::checkBundle(const Bundle& bundle) const
{
	if (!Action::checkBundle(bundle))
		return false;

//	if (!checkProperty(bundle, COMMAND_PROPERTY))
//		return false;

	return true;
}

void CommandAction::readProperties(Bundle& bundle)
{
	if (!checkBundle(bundle))
		return;

	m_Command = bundle.get(COMMAND_PROPERTY);
	Action::readProperties(bundle);

	initBundle();
}

void CommandAction::writeProperties(Bundle& bundle)
{
	bundle.add(COMMAND_PROPERTY, m_Command);
	Action::writeProperties(bundle);
}

QString CommandAction::getDescription() const
{
	return "A Command Action represents a command line command.";
}

void CommandAction::execute()
{
	int exitCode = system(m_Command.toStdString().c_str());
	emit exitCodeEmitted(exitCode);
}

bool CommandAction::validate()
{
	return true;
}

void CommandAction::setupSignalsAndSlots()
{
	connect(ui->edtCommand, SIGNAL(textChanged(QString)), this, SLOT(onEdit(QString)));
}

void CommandAction::initBundle()
{
	ui->edtCommand->setText(m_Command);
}

void CommandAction::onEdit(QString text)
{
	m_Command = text;
}
