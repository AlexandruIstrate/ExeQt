#ifndef LINKACTION_H
#define LINKACTION_H

#include <QString>

#include "action.h"

namespace Ui {
class LinkAction;
}

class LinkAction : public Action
{
    Q_OBJECT

private:
    Ui::LinkAction* ui;

    QString m_Link;

public:
    LinkAction(const QString& link, const QString& name = QString(), QWidget* parent = nullptr);
    LinkAction(const QString& name = QString(), QWidget* parent = nullptr);
    ~LinkAction();

    QString getTagName() const override;

    void readProperties(Bundle&) override;
    void writeProperties(Bundle&) override;

    void execute() override;

private:
    void setupSignalsAndSlots();

private slots:
    void onTextChanged(QString);
};

#endif // LINKACTION_H
