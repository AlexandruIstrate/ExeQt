/**************************************************************************
 *
 * Copyright (c) 2018 Alexandru Istrate
 *
 * This file is subject to the terms and conditions defined in the
 * file 'LICENSE', which is part of this source code package.
 *
**************************************************************************/

#include "mergedialog.h"
#include "ui_mergedialog.h"

#include <QPushButton>
#include <QCheckBox>
#include <QVBoxLayout>

MergeDialog::MergeDialog(const Bundle& local, const Bundle& cloud, QWidget* parent) :
	QDialog(parent), ui(new Ui::MergeDialog),
	m_LocalBundle { local }, m_CloudBundle { cloud }
{
	ui->setupUi(this);

	mergeBundles();

	setupUI();
	setupSignalsAndSlots();
}

MergeDialog::~MergeDialog()
{
	delete ui;
}

void MergeDialog::setupUI()
{
	QVBoxLayout* localLayout = new QVBoxLayout(ui->grpLocal);
	localLayout->addWidget(&m_LocalTree);
	m_LocalTree.setActionBundle(m_LocalBundle);

	QVBoxLayout* cloudLayout = new QVBoxLayout(ui->grpCloud);
	cloudLayout->addWidget(&m_CloudTree);
	m_CloudTree.setActionBundle(m_CloudBundle);

	QVBoxLayout* mergedLayout = new QVBoxLayout(ui->grpMerged);
	mergedLayout->addWidget(&m_MergedTree);
	m_MergedTree.setActionBundle(m_MergedBundle);

	showLocalAndCloud();
}

void MergeDialog::setupSignalsAndSlots()
{
	connect(ui->btnConfirm, &QPushButton::clicked, this, &MergeDialog::onConfirm);
	connect(ui->btnDeny, &QPushButton::clicked, this, &MergeDialog::onDeny);
	connect(ui->chkShowResult, &QCheckBox::toggled, this, &MergeDialog::onShowMergeChanged);
}

void MergeDialog::mergeBundles()
{
	m_MergedBundle = Bundle::mergeBundles(m_CloudBundle, m_LocalBundle);
}

void MergeDialog::showMerged()
{
	ui->grpMerged->show();

	ui->grpCloud->hide();
	ui->grpLocal->hide();
}

void MergeDialog::showLocalAndCloud()
{
	ui->grpCloud->show();
	ui->grpLocal->show();

	ui->grpMerged->hide();
}

void MergeDialog::onConfirm()
{
	accept();
}

void MergeDialog::onDeny()
{
	reject();
}

void MergeDialog::onShowMergeChanged(bool checked)
{
	if (checked)
		showMerged();
	else
		showLocalAndCloud();
}
