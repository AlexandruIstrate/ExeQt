#ifndef ADDGROUPDIALOG_H
#define ADDGROUPDIALOG_H

#include <QDialog>
#include <QString>
#include <QIcon>
#include <QVector>

#include "actiontab.h"

namespace Ui {
    class AddGroupDialog;
}

class AddGroupDialog : public QDialog
{
    Q_OBJECT

private:
    Ui::AddGroupDialog *ui;

    static QVector<QString> s_IconNames;

    QString m_Name;
    QIcon m_Icon;

public:
    explicit AddGroupDialog(QWidget* parent = nullptr);
    AddGroupDialog(ActionTab* tab);
    ~AddGroupDialog();

    inline const QString& getName() const { return m_Name; }
    inline const QIcon& getIcon() const { return m_Icon; }

private:
    void initUI();

    void addIcon(const QIcon& icon, const QString& name);
    void setupIcons();

public slots:
    void accept() override;
};

#endif // ADDGROUPDIALOG_H
