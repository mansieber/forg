#ifndef EDITBAITDIALOG_H
#define EDITBAITDIALOG_H

#include "baitmodel.h"

#include <QDialog>
#include <QDataWidgetMapper>
#include <QSqlDatabase>

namespace Ui {
    class EditBaitDialog;
}

class EditBaitDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EditBaitDialog(QSqlDatabase db, int id, QWidget *parent = 0);
    ~EditBaitDialog();

private slots:
    void onButtonBoxAccepted();
    void onButtonSelect();

private:
    Ui::EditBaitDialog *ui;
    BaitModel *baitModel;
    QDataWidgetMapper *mapper;
};

#endif // EDITBAITDIALOG_H
