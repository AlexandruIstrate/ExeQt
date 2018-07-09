/**************************************************************************
 *
 * Copyright (c) 2018 Alexandru Istrate
 *
 * This file is subject to the terms and conditions defined in the
 * file 'LICENSE', which is part of this source code package.
 *
**************************************************************************/

#ifndef STYLEMANAGER_H
#define STYLEMANAGER_H

#include <QString>

class StyleManager	// TODO: Support for multiple, named styles
{
private:
	static StyleManager* s_Instance;

private:
	QString m_StyleFile;
	QString m_Style;

private:
	StyleManager();

public:
	inline const QString& getStyleFile() const { return m_StyleFile; }
	bool setStyleFile(const QString& styleFilePath);

	inline const QString& getStyle() const { return m_Style; }

public:
	inline static StyleManager* instance() { return s_Instance; }

	static void init();
	static void terminate();
};

#endif // STYLEMANAGER_H
