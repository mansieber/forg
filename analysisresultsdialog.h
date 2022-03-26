#ifndef ANALYSISRESULTSDIALOG_H
#define ANALYSISRESULTSDIALOG_H

#include <QString>
#include <QDialog>
#include <QSqlDatabase>

namespace Ui {
    class AnalysisResultsDialog;
}

class AnalysisResultsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AnalysisResultsDialog(QSqlDatabase db, int year, QList<int> waterIds, QWidget *parent = 0);
    ~AnalysisResultsDialog();

private:
    Ui::AnalysisResultsDialog *ui;
    QSqlDatabase db;
    int year;
    QList<int> waterIds;
    void analysis();
    bool isInSelectedWaters(int sessionId);
};

#endif // ANALYSISRESULTSDIALOG_H
