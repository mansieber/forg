#ifndef ANALYSISYEAR_H
#define ANALYSISYEAR_H

#include <QString>
#include <QDialog>
#include <QSqlDatabase>

namespace Ui {
    class AnalysisYearDialog;
}

class AnalysisYearDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AnalysisYearDialog(QSqlDatabase db, QWidget *parent = 0);
    ~AnalysisYearDialog();

private slots:
    void analysis(const QString &sYear);

private:
    Ui::AnalysisYearDialog *ui;
    QSqlDatabase db;

};

#endif // ANALYSISYEAR_H
