#ifndef EDITCATCHDIALOG_H
#define EDITCATCHDIALOG_H

#include "catchmodel.h"

#include <QDialog>
#include <QDataWidgetMapper>
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

private:
    Ui::EditCatchDialog *ui;
    CatchModel *catchModel;
    QDataWidgetMapper *mapper;
};

#endif // EDITCATCHDIALOG_H
