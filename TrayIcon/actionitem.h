#ifndef ACTIONITEM_H
#define ACTIONITEM_H

#include <QDialog>
#include <QString>
#include <QAction>

#include "action.h"
#include "saveable.h"

namespace Ui {
class ActionItem;
}

class ActionItem : public QDialog, public Saveable
{
    Q_OBJECT

private:
    Ui::ActionItem *ui;
    Action* m_Action;

public:
    explicit ActionItem(QWidget* parent = 0);
    ~ActionItem();

    inline const QString& getName() const { return m_Action->getName(); }
    inline Action* getAction() { return m_Action; }

    QString getTagName() const override;

    void readProperties(Bundle&) override;
    void writeProperties(Bundle&) override;

private:
    void setupSignalsAndSlots();
    void setupUI();

    void setupActions();
    void setAction(Action::Type);
    void setActionWidget();

public slots:
    void accept() override;

private slots:
    void onActionTypeChanged(int);
};

#endif // ACTIONITEM_H
