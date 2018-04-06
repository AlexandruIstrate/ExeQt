#include "mainwidget.h"
#include "ui_mainwidget.h"
#include "addtab.h"
#include "addgroupdialog.h"
#include "groupconfigure.h"

#include <QDebug>
#include <QMenu>
#include <QImage>
#include <QCloseEvent>
#include <QDesktopWidget>
#include <QMessageBox>
#include <QStringLiteral>
#include <QDomDocument>

#include "addgroupdialog.h"

#define ACTION_TAB_COUNT_PROPERTY   "actionTabCount"

MainWidget::MainWidget(QWidget* parent) :
    QWidget(parent), ui(new Ui::MainWidget)
{
    ui->setupUi(this);

    setupActions();
    setupUI();
    setupSignalsAndSlots();

    loadFromFile();
}

MainWidget::~MainWidget()
{
    saveToFile();
    delete ui;
}

QIcon MainWidget::getTabIcon(int index)
{
    return m_ActionTabs.at(index)->getIcon();
}

void MainWidget::addNewActionGroup()
{
    AddGroupDialog* addDialog = new AddGroupDialog(this);
    if (!addDialog->exec())
        return;

    ActionTab* newTab = new ActionTab(addDialog->getName(), addDialog->getIcon(), this);
    addTab(newTab);

    ui->tabGroup->setCurrentWidget(newTab);
    ui->tabGroup->setTabEnabled(getTabCount() - 1, false);
}

void MainWidget::removeActionGroup(int index)
{
    ui->tabGroup->widget(index)->deleteLater();
    ui->tabGroup->removeTab(index);
    m_ActionTabs.erase(m_ActionTabs.begin() + index);

    ui->tabGroup->setCurrentIndex(getSelectedTabIndex() - 1);

    if (getTabCount() == 1)
        ui->tabGroup->setTabEnabled(getTabCount() - 1, true);
}

QString MainWidget::getTagName() const
{
    return QString("mainWidget");
}

void MainWidget::readProperties(Bundle& bundle)
{
    int actionTabCount = bundle.get(ACTION_TAB_COUNT_PROPERTY).toInt();

    for (int i = 0; i < bundle.getChildrenCount(); ++i)
        bundle.childAt(i).getName();

    for (int i = 0; i < actionTabCount; ++i)
    {
        ActionTab* tab = new ActionTab("", QIcon(), this);
        tab->readProperties(bundle.childAt(i));
        addTab(tab);
    }
}

void MainWidget::writeProperties(Bundle& bundle)
{
    bundle.add(ACTION_TAB_COUNT_PROPERTY, QString::number(m_ActionTabs.size()));
    for (ActionTab* tab : m_ActionTabs)
    {
        Bundle child(tab->getTagName());
        tab->writeProperties(child);

        bundle.addChild(child);
    }
}

void MainWidget::closeEvent(QCloseEvent* event)
{
    hide();
    event->ignore();
}

void MainWidget::setupActions()
{
    m_AddTabAction = new QAction(QIcon(":/assets/images/app-actions/add.png"), tr("Add Group"), this);
    connect(m_AddTabAction, SIGNAL(triggered()), this, SLOT(onAddTab()));

    m_RemoveTabAction = new QAction(QIcon(":/assets/images/app-actions/delete.png"), tr("Remove Group"), this);
    connect(m_RemoveTabAction, SIGNAL(triggered()), this, SLOT(onRemoveTab()));

    m_ConfigureAction = new QAction(QIcon(":/assets/images/app-actions/configure.png"), tr("Configure..."), this);
    connect(m_ConfigureAction, SIGNAL(triggered()), SLOT(onConfigure()));

    m_ConfigureGroupsAction = new QAction(QIcon(":/assets/images/app-actions/configure-action-groups.png"), tr("Configure Groups..."), this);
    connect(m_ConfigureGroupsAction, SIGNAL(triggered()), this, SLOT(onConfigureActionGroups()));

    m_QuitAction = new QAction(QIcon(":/assets/images/app-actions/quit.png"), tr("Quit"), this);
    connect(m_QuitAction, SIGNAL(triggered()), this, SLOT(onQuit()));
}

void MainWidget::setupSignalsAndSlots()
{
    connect(m_TrayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(onIconActivated(QSystemTrayIcon::ActivationReason)));
    connect(ui->tabGroup, SIGNAL(tabBarClicked(int)), this, SLOT(onTabSelected(int)));

    connect(ui->btnGroupConfigure, SIGNAL(clicked()), m_ConfigureGroupsAction, SLOT(trigger()));
}

void MainWidget::setupUI()
{
    setupTrayIcon();
    setupTabs();

    setGeometry(QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter, size(), qApp->desktop()->availableGeometry()));
}

void MainWidget::setupTrayIcon()
{
    QMenu* trayMenu = new QMenu(this);
    trayMenu->addAction(m_AddTabAction);
    trayMenu->addAction(m_RemoveTabAction);
    trayMenu->addAction(m_ConfigureAction);
    trayMenu->addAction(m_ConfigureGroupsAction);
    trayMenu->addSeparator();
    trayMenu->addAction(m_QuitAction);

    m_TrayIcon = new QSystemTrayIcon(this);
    m_TrayIcon->setContextMenu(trayMenu);
    m_TrayIcon->setIcon(QIcon(":/assets/images/app-icon.png"));
    m_TrayIcon->show();
}

void MainWidget::setupTabs()
{
    AddTab* addTab = new AddTab(this);
    ui->tabGroup->setTabEnabled(ui->tabGroup->indexOf(addTab), false);
    ui->tabGroup->addTab(addTab, "+");
    connect(addTab, SIGNAL(createActionGroup()), m_AddTabAction, SLOT(trigger()));
}

void MainWidget::insertTab(ActionTab* tab)
{
    ui->tabGroup->insertTab(getTabCount() - 1, tab, tab->getIcon(), tab->getName());
    ui->tabGroup->setTabEnabled(getTabCount() - 1, false);
}

void MainWidget::addTab(ActionTab* tab)
{
    insertTab(tab);
    m_ActionTabs.push_back(tab);
}

void MainWidget::openGroupConfigureMenu()
{
    GroupConfigure* conf = new GroupConfigure(this);
    conf->exec();

    reloadTabs();
}

void MainWidget::reloadTabs()
{
    int selectedTab = getSelectedTabIndex();

    for (int i = 0; getTabCount() > 1; ++i)
        ui->tabGroup->removeTab(i);

    for (ActionTab* tab : m_ActionTabs)
        insertTab(tab);

    ui->tabGroup->setCurrentIndex(selectedTab < getTabCount() ? selectedTab : getTabCount() - 1);
}

void MainWidget::saveToFile()
{
    QString str;

    QXmlStreamWriter writer(&str);
    writer.setAutoFormatting(true);
    writer.writeStartDocument();
    writer.writeStartElement(getTagName());

    Bundle bundle(getTagName());
    writeProperties(bundle);
    writeBundle(bundle, writer);

    writer.writeEndDocument();

    Saveable::writeToFile(m_SaveFile, str);
}

void MainWidget::loadFromFile()
{
    QString str;
    Saveable::readFromFile(m_SaveFile, str);

    Bundle bundle(getTagName());
    readBundle(bundle, str);
    readProperties(bundle);
}

int MainWidget::getTabCount()
{
    return ui->tabGroup->count();
}

int MainWidget::getSelectedTabIndex()
{
    return ui->tabGroup->currentIndex();
}

void MainWidget::writeBundle(Bundle bundle, QXmlStreamWriter& writer)
{
    auto map = bundle.getValues();
    for (QMap<QString, QString>::const_iterator i = map.constBegin(); i != map.constEnd(); ++i)
        writer.writeAttribute(i.key(), i.value());

    for (int i = 0; i < bundle.getChildrenCount(); ++i)
    {
        Bundle& child = bundle.childAt(i);

        writer.writeStartElement(child.getName());
        writeBundle(child, writer);
        writer.writeEndElement();
    }
}

void processBundle(Bundle& bundle, QDomElement& node)
{
    QDomNamedNodeMap map = node.attributes();
    for (int i = 0; i < map.length(); ++i)
    {
        QDomNode childNode = map.item(i);
        bundle.add(childNode.nodeName(), childNode.nodeValue());
    }

    QDomNodeList nodes = node.childNodes();
    for (int i = 0; i < nodes.size(); ++i)
    {
        QDomElement elem = nodes.at(i).toElement();
        Bundle child(elem.tagName());

        processBundle(child, elem);
        bundle.addChild(child);
    }
}

void MainWidget::readBundle(Bundle& bundle, const QString& str)
{
    QDomDocument doc("doc");
    doc.setContent(str);

    QDomElement docRoot = doc.documentElement();
    processBundle(bundle, docRoot);
}

void MainWidget::onIconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason)
    {
    case QSystemTrayIcon::ActivationReason::Trigger:
        break;

    case QSystemTrayIcon::ActivationReason::DoubleClick:
        show();
        break;

    case QSystemTrayIcon::ActivationReason::MiddleClick:
        break;

    default:
        break;
    }
}

void MainWidget::onTabSelected(int index)
{
    if (index == ui->tabGroup->count() - 1 || index == -1) // Add a new tab if we selected the add tab, which is always last
        addNewActionGroup();
}

void MainWidget::onAddTab()
{
    show();
    addNewActionGroup();
}

void MainWidget::onRemoveTab()
{
    openGroupConfigureMenu();
}

void MainWidget::onConfigure()
{
    show();
}

void MainWidget::onConfigureActionGroups()
{
    openGroupConfigureMenu();
}

void MainWidget::onQuit()
{
    QMessageBox dialog(QMessageBox::Icon::Question, tr("Exit"), tr("Are you sure you want to exit?"), QMessageBox::Yes | QMessageBox::No);
    if (dialog.exec() == QMessageBox::No)
        return;

    qApp->quit();
}
