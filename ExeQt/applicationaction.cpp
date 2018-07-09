/**************************************************************************
 *
 * Copyright (c) 2018 Alexandru Istrate
 *
 * This file is subject to the terms and conditions defined in the
 * file 'LICENSE', which is part of this source code package.
 *
**************************************************************************/

#include "applicationaction.h"
#include "ui_applicationaction.h"

#include <QDebug>
#include <QProcess>
#include <QFileDialog>
#include <QMessageBox>

#define APP_PATH_PROPERTY "appPath"

ApplicationAction::ApplicationAction(const QString& appPath, const QString& name, QWidget* parent) :
	Action(name, Action::Type::APPLICATION, parent),
	ui(new Ui::ApplicationAction),
	m_AppPath { appPath }
{
	ui->setupUi(this);
	setupSignalsAndSlots();
}

ApplicationAction::ApplicationAction(const QString& name, QWidget* parent) :
	Action(name, Action::Type::APPLICATION, parent),
	ui(new Ui::ApplicationAction),
	m_AppPath { "" }
{
	ui->setupUi(this);
	setupSignalsAndSlots();
}

ApplicationAction::~ApplicationAction()
{
	delete ui;
}

QString ApplicationAction::getTagName() const
{
	return QString("applicationAction");
}

bool ApplicationAction::checkBundle(const Bundle& bundle) const
{
	if (!Action::checkBundle(bundle))
		return false;

	if (!checkProperty(bundle, APP_PATH_PROPERTY))
		return false;

	return true;
}

void ApplicationAction::readProperties(Bundle& bundle)
{
	if (!checkBundle(bundle))
		return;

	m_AppPath = bundle.get(APP_PATH_PROPERTY);
	Action::readProperties(bundle);

	initBundle();
}

void ApplicationAction::writeProperties(Bundle& bundle)
{
	bundle.add(APP_PATH_PROPERTY, m_AppPath);
	Action::writeProperties(bundle);
}

QString ApplicationAction::getDescription() const
{
	return "An Application Action represents an executable file, or shell script, that can be run.";
}

void ApplicationAction::setupSignalsAndSlots()
{
	connect(ui->btnBrowse, SIGNAL(clicked()), this, SLOT(onBrowse()));
	connect(ui->edtPath, SIGNAL(textChanged(QString)), this, SLOT(onTextChanged(QString)));
}

void ApplicationAction::initBundle()
{
	ui->edtPath->setText(m_AppPath);
}

void ApplicationAction::execute()
{
	QProcess* process = new QProcess(this);
//	connect(process, &QProcess::finished, this, &ApplicationAction::onProcessFinished);
	connect(process, SIGNAL(errorOccurred(QProcess::ProcessError)), this, SLOT(onProcessError(QProcess::ProcessError)));
	process->startDetached(m_AppPath);
}

bool ApplicationAction::validate()
{
	if (QUrl(m_AppPath).isValid())
		return true;

	QMessageBox dialog(QMessageBox::Icon::Question, tr("Invalid Path"), tr("The path you supplied is invalid and it might not work. Are you sure you want to use it?"), QMessageBox::Yes | QMessageBox::No);
	if (dialog.exec() == QMessageBox::Yes)
		return true;

	return false;
}

void ApplicationAction::onTextChanged(QString text)
{
	m_AppPath = text;
}

void ApplicationAction::onBrowse()
{
	QFileDialog dialog;
	dialog.setAcceptMode(QFileDialog::AcceptMode::AcceptOpen);
	dialog.setFileMode(QFileDialog::FileMode::ExistingFile);

#if defined(Q_OS_WIN)
	dialog.setNameFilter(tr("Executable Files (*.exe *.bat)"));
#elif defined(Q_OS_UNIX)
	dialog.setNameFilter(tr("Executable Files (*)"));
#endif

	if (dialog.exec() == QDialog::DialogCode::Rejected)
		return;

	ui->edtPath->setText(dialog.selectedFiles()[0]);
}

void ApplicationAction::onProcessError(QProcess::ProcessError)
{
	qDebug() << "Application Launch Error";
}
