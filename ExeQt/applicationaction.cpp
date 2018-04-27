#include "applicationaction.h"
#include "ui_applicationaction.h"

#include <QDebug>
#include <QProcess>
#include <QFileDialog>

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

void ApplicationAction::readProperties(Bundle& bundle)
{
    m_AppPath = bundle.get(APP_PATH_PROPERTY);
    Action::readProperties(bundle);

    initBundle();
}

void ApplicationAction::writeProperties(Bundle& bundle)
{
    bundle.add(APP_PATH_PROPERTY, m_AppPath);
    Action::writeProperties(bundle);
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
    process->startDetached(m_AppPath);
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

    if (!dialog.exec())
        return;

    ui->edtPath->setText(dialog.selectedFiles()[0]);
}
