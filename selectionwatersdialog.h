#ifndef SELECTIONWATERSDIALOG_H
#define SELECTIONWATERSDIALOG_H

#include <QString>
#include <QDialog>
#include <QSqlDatabase>

namespace Ui {
    class SelectionWatersDialog;
}

class SelectionWatersDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SelectionWatersDialog(QSqlDatabase db, int *year, QList<int> *ids, bool singleSelection, QWidget *parent = 0);
    ~SelectionWatersDialog();

private:
    Ui::SelectionWatersDialog *ui;
    QSqlDatabase db;
    int *pYear;
    QList<int> *pLocationIds;

private slots:
    void onButtonBoxAccepted();
    void onButtonBoxRejected();
};

#endif // SELECTIONWATERSDIALOG_H
