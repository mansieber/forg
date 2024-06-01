#ifndef EDITCATCHDIALOG_H
#define EDITCATCHDIALOG_H

#include "catchmodel.h"

#include <QDialog>
#include <QDataWidgetMapper>
#include <QSortFilterProxyModel>
#include <QSqlDatabase>

namespace Ui {
    class EditCatchDialog;
}

class EditCatchDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EditCatchDialog(QSqlDatabase db, int id, QWidget *parent = 0);
    ~EditCatchDialog();

private slots:
    void onButtonBoxAccepted();
    void onEditLengthFinished();
    void onEditWeightFinished();
    void onEditTimeFinished();
    void onComboBaitIndexChanged(int index);

private:
    Ui::EditCatchDialog *ui;
    CatchModel *catchModel;
    QSqlTableModel *baitModel;
    QSortFilterProxyModel *sortedBaitModel;
    QDataWidgetMapper *mapper;
    int findSessionId();
    QString sessionToString(const QModelIndex cIndex);
};

#endif // EDITCATCHDIALOG_H
