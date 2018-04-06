#include "linkaction.h"
#include "ui_linkaction.h"

#include <QDesktopServices>
#include <QUrl>

#define LINK_PROPERTY "link"

LinkAction::LinkAction(const QString& link, const QString& name, QWidget* parent) :
    Action(name, Action::Type::LINK, parent), ui(new Ui::LinkAction), m_Link { link }
{
    ui->setupUi(this);
    setupSignalsAndSlots();
}

LinkAction::LinkAction(const QString& name, QWidget* parent) :
    Action(name, Action::Type::LINK, parent), ui(new Ui::LinkAction), m_Link { "" }
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

void LinkAction::readProperties(Bundle& bundle)
{
    m_Link = bundle.get(LINK_PROPERTY);
    Action::readProperties(bundle);
}

void LinkAction::writeProperties(Bundle& bundle)
{
    bundle.add(LINK_PROPERTY, m_Link);
    Action::writeProperties(bundle);
}

void LinkAction::execute()
{
    QDesktopServices::openUrl(QUrl(m_Link));
}

void LinkAction::setupSignalsAndSlots()
{
    connect(ui->edtLink, SIGNAL(textChanged(QString)), this, SLOT(onTextChanged(QString)));
}

void LinkAction::onTextChanged(QString text)
{
    m_Link = text;
}
