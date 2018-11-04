/**************************************************************************
 *
 * Copyright (c) 2018 Alexandru Istrate
 *
 * This file is subject to the terms and conditions defined in the
 * file 'LICENSE', which is part of this source code package.
 *
**************************************************************************/
#ifndef ICONMANAGER_H
#define ICONMANAGER_H

#include <QList>
#include <QString>
#include <QIcon>

struct ImageResource
{
	QString name;
	QIcon image;

	ImageResource(QString n, QIcon img) : name { n }, image { img } {}
	ImageResource() {}

	inline bool operator==(const ImageResource& other) const { return name == other.name; }
	inline bool operator!=(const ImageResource& other) const { return !(*this == other); }
};

typedef QList<ImageResource> IconList;

class IconManager
{
private:
	static IconManager* s_Instance;

	 IconList m_Icons;

private:
	IconManager();

public:
	static inline IconManager* instance() { return s_Instance; }
	inline const IconList& getIconList() const { return m_Icons; }

	void registerIcon(ImageResource image);
	void registerIcon(const QString& name, const QString& imagePath);

	ImageResource getIcon(const QString& name) const;

public:
	static void init();
	static void terminate();
};

#endif // ICONMANAGER_H
