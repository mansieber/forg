#ifndef VIEWBAITDIALOG_H
#define VIEWBAITDIALOG_H

#include "editbaitdialog.h"
#include "baitmodel.h"
#include "imagedelegate.h"

#include <QDialog>
#include <QSqlDatabase>

namespace Ui {
    class ViewBaitDialog;
}

class ViewBaitDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ViewBaitDialog(QSqlDatabase db, QWidget *parent = 0);
    ~ViewBaitDialog();

private slots:
    void addBait();
    void editBait();
    void editBait(QModelIndex index);

private:
    Ui::ViewBaitDialog *ui;
    ImageDelegate *imageDelegate;
    QSqlDatabase db;
    BaitModel *baitModel;
    EditBaitDialog *editBaitDialog;
};

#endif // VIEWBAITDIALOG_H
