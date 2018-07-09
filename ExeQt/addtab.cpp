/**************************************************************************
 *
 * Copyright (c) 2018 Alexandru Istrate
 *
 * This file is subject to the terms and conditions defined in the
 * file 'LICENSE', which is part of this source code package.
 *
**************************************************************************/

#include "addtab.h"
#include "ui_addtab.h"
#include "mainwidget.h"

#include "networkmanager.h"

AddTab::AddTab(QWidget* parent) :
	QWidget(parent),
	ui(new Ui::AddTab)
{
	ui->setupUi(this);
	setupSignalsAndSlots();
}

AddTab::~AddTab()
{
	delete ui;
}

void AddTab::setupSignalsAndSlots()
{
	connect(ui->btnAdd, SIGNAL(clicked()), this, SLOT(onAdd()));
}

void AddTab::onAdd()
{
	emit createActionGroup();
	NetworkManager::instance()->requestActionUpdate();
}
