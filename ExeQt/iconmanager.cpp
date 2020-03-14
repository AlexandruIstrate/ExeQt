/**************************************************************************
 *
 * Copyright (c) 2018 Alexandru Istrate
 *
 * This file is subject to the terms and conditions defined in the
 * file 'LICENSE', which is part of this source code package.
 *
**************************************************************************/

#include "iconmanager.h"

IconManager* IconManager::s_Instance = nullptr;

IconManager::IconManager()
{
}

void IconManager::registerIcon(ImageResource image)
{
	m_Icons.append(image);
}

void IconManager::registerIcon(const QString& name, const QString& imagePath)
{
	m_Icons.append(ImageResource(name, QIcon(imagePath)));
}

ImageResource IconManager::getIcon(const QString& name) const
{
	for (const ImageResource& img : m_Icons)
	{
		if (img.name == name)
		{
			return img;
		}
	}

	return ImageResource("Not Found", QIcon(":/assets/images/action-custom-icons/notfound.png"));
}

void IconManager::init()
{
	s_Instance = new IconManager();
}

void IconManager::terminate()
{
	delete s_Instance;
}
