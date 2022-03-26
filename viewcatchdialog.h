#ifndef VIEWCATCHDIALOG_H
#define VIEWCATCHDIALOG_H

#include <QDialog>
#include <QSqlDatabase>

#include "editcatchdialog.h"
#include "catchmodel.h"
#include "imagedelegate.h"

namespace Ui {
    class ViewCatchDialog;
}

class ViewCatchDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ViewCatchDialog(QSqlDatabase db, QWidget *parent = 0);
    ~ViewCatchDialog();

private slots:
    void addCatch();
    void editCatch();
    void deleteCatch();
    void editCatch(QModelIndex index);

private:
    Ui::ViewCatchDialog *ui;
    ImageDelegate *imageDelegate;
    QSqlDatabase db;
    CatchModel *catchModel;
    EditCatchDialog *editCatchDialog;
};

#endif // VIEWCATCHDIALOG_H
