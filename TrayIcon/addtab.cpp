#include "addtab.h"
#include "ui_addtab.h"
#include "mainwidget.h"

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
}
