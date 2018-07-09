/**************************************************************************
 *
 * Copyright (c) 2018 Alexandru Istrate
 *
 * This file is subject to the terms and conditions defined in the
 * file 'LICENSE', which is part of this source code package.
 *
**************************************************************************/

#ifndef CLIENTINFODIALOG_H
#define CLIENTINFODIALOG_H

#include <QDialog>

namespace Ui {
class ClientInfoDialog;
}

class ClientInfoDialog : public QDialog
{
	Q_OBJECT

private:
	Ui::ClientInfoDialog* ui;

public:
	explicit ClientInfoDialog(const QString& name, const QString& id, const QString& address, QWidget* parent = nullptr);
	~ClientInfoDialog();

	void setName(const QString& name);
	void setID(const QString& id);
	void setAddress(const QString& address);
};

#endif // CLIENTINFODIALOG_H
