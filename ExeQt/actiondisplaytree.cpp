/**************************************************************************
 *
 * Copyright (c) 2018 Alexandru Istrate
 *
 * This file is subject to the terms and conditions defined in the
 * file 'LICENSE', which is part of this source code package.
 *
**************************************************************************/

#include "actiondisplaytree.h"

#include "addgroupdialog.h"

// TreeItem

TreeItem::TreeItem(TreeItem::Type type) : m_Type { type }
{
	setItemName(type);
}

TreeItem::TreeItem(TreeItem::Type type, const QString& name) : m_Type { type }, m_Name { name }
{
	setText(0, name);
}

TreeItem::TreeItem(ActionReference* reference, const QString& name)
	: m_Type { Type::ACTION_REFERENCE }, m_Name { name }, m_ActionReference { reference }
{
	setText(0, name);
}

TreeItem::~TreeItem()
{
//	if (m_ActionReference)
//		delete m_ActionReference;
}

void TreeItem::setItemName(TreeItem::Type type)
{
	QString name;

	switch (type)
	{
		case Type::TAB:
			name = QObject::tr("Action Tab");
			break;

		case Type::ACTION:
			name = QObject::tr("Action");
			break;

		case Type::ACTION_REFERENCE:
			name = QObject::tr("Action Reference");
			break;

		case Type::UNKNOWN:

		default:
			name = QObject::tr("Unknown");
			break;
	}

	setText(0, name);
}

// ActionDisplayTree

ActionDisplayTree::ActionDisplayTree(const Bundle& actionBundle)
{
	setActionBundle(actionBundle);
}

ActionDisplayTree::ActionDisplayTree()
{

}

void ActionDisplayTree::setActionBundle(const Bundle& bundle)
{
	TreeItem* parentItem = new TreeItem(TreeItem::Type::UNKNOWN, "Actions");
	createTreeItem(parentItem, bundle);
	addTopLevelItem(parentItem);

	expandAll();
}

QString getBundleActionTag(const Bundle& bundle)
{
	if (bundle.hasKey("command"))
	{
		return bundle.get("command");
	}

	if (bundle.hasKey("appPath"))
	{
		return bundle.get("appPath");
	}

	if (bundle.hasKey("link"))
	{
		return bundle.get("link");
	}

	return QString();
}

void ActionDisplayTree::createTreeItem(TreeItem* parent, const Bundle& actionBundle)
{
	TreeItem::Type type = TreeItem::Type::UNKNOWN;
	QString name = actionBundle.getName();

	if (name == "actionTab")
	{
		type = TreeItem::Type::TAB;
	}

	if (QStringList { "commandAction", "applicationAction", "linkAction", "setAction" }.contains(name))
	{
		type = TreeItem::Type::ACTION;
	}

	if (name == "actionReference")
	{
		type = TreeItem::Type::ACTION_REFERENCE;
	}

	TreeItem* childWidget;

	if (type == TreeItem::Type::UNKNOWN)
	{
		childWidget = parent;
	}
	else
	{
		childWidget = new TreeItem(type);

		if (type == TreeItem::Type::TAB)
		{
			childWidget->setName(actionBundle.get("name"));
			childWidget->setText(0, actionBundle.get("name"));
			childWidget->setIcon(0, AddGroupDialog::getIconByName(actionBundle.get("icon")).icon);
		}
		else if (type == TreeItem::Type::ACTION)
		{
			QString group = dynamic_cast<TreeItem*>(parent)->getName();
			QString action = actionBundle.get("name");

			ActionReference* ref = new ActionReference(group, action);

			childWidget->setActionReference(ref);
			childWidget->setText(0, (actionBundle.getName() == "setAction") ? action : QString("%1 : %2").arg(action, getBundleActionTag(actionBundle)));
			childWidget->setIcon(0, IconManager::instance()->getIcon(actionBundle.get("icon")).image);
		}
		else if (type == TreeItem::Type::ACTION_REFERENCE)
		{
			QString group = actionBundle.get("actionGroup");
			QString action = actionBundle.get("actionName");

			ActionReference* ref = new ActionReference(group, action);

			childWidget->setActionReference(ref);
			childWidget->setText(0, QString("Reference to ") + ref->toString());
			childWidget->setIcon(0, QIcon(":/assets/images/reference.png"));
		}

		parent->addChild(childWidget);
	}

	for (const Bundle& child : actionBundle.getChildren())
	{
		createTreeItem(childWidget, child);
	}
}
