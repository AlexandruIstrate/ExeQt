/**************************************************************************
 *
 * Copyright (c) 2018 Alexandru Istrate
 *
 * This file is subject to the terms and conditions defined in the
 * file 'LICENSE', which is part of this source code package.
 *
**************************************************************************/

#include "linkaction.h"
#include "ui_linkaction.h"

#include <QMessageBox>
#include <QDesktopServices>
#include <QUrl>

#define LINK_PROPERTY "link"

LinkAction::LinkAction(const QString& link, const QString& name, QWidget* parent) :
	Action(name, Action::Type::LINK, ImageResource(), parent), ui(new Ui::LinkAction), m_Link { link }
{
	ui->setupUi(this);
	setupSignalsAndSlots();
}

LinkAction::LinkAction(const QString& name, QWidget* parent) :
	Action(name, Action::Type::LINK, ImageResource(), parent), ui(new Ui::LinkAction), m_Link { "" }
{
	ui->setupUi(this);
	setupSignalsAndSlots();
}

LinkAction::~LinkAction()
{
	delete ui;
}

QString LinkAction::getTagName() const
{
	return QString("linkAction");
}

bool LinkAction::checkBundle(const Bundle& bundle) const
{
	if (!Action::checkBundle(bundle))
	{
		return false;
	}

//	if (!checkProperty(bundle, LINK_PROPERTY))
//		return false;

	return true;
}

void LinkAction::readProperties(Bundle& bundle)
{
	if (!checkBundle(bundle))
	{
		return;
	}

	m_Link = bundle.get(LINK_PROPERTY);
	Action::readProperties(bundle);

	initBundle();
}

void LinkAction::writeProperties(Bundle& bundle)
{
	bundle.add(LINK_PROPERTY, m_Link);
	Action::writeProperties(bundle);
}

QString LinkAction::getDescription() const
{
	return "A Link Action represents a link to a local resource (such as a file) or to a remote one (such as a web page).";
}

void LinkAction::execute()
{
	QDesktopServices::openUrl(QUrl(m_Link));
}

bool LinkAction::validate()
{
	if (QUrl(m_Link).isValid())
	{
		return true;
	}

	QMessageBox dialog(QMessageBox::Icon::Question, tr("Invalid Link"), tr("The link you supplied is invalid and it might not work. Are you sure you want to use it?"), QMessageBox::Yes | QMessageBox::No);
	if (dialog.exec() == QMessageBox::Yes)
	{
		return true;
	}

	return false;
}

void LinkAction::setupSignalsAndSlots()
{
	connect(ui->edtLink, SIGNAL(textChanged(QString)), this, SLOT(onTextChanged(QString)));
}

void LinkAction::initBundle()
{
	ui->edtLink->setText(m_Link);
}

void LinkAction::onTextChanged(QString text)
{
	m_Link = text;
}
