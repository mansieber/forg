#ifndef ANALYSISZONALDISTRIBUTIONDIALOG_H
#define ANALYSISZONALDISTRIBUTIONDIALOG_H

#include <QDialog>
#include <QSqlDatabase>
#include <QSqlQuery>

#define IMAGE_SIZE_X 1024
#define IMAGE_SIZE_Y 512

namespace Ui {
    class AnalysisZonalDistributionDialog;
}

class AnalysisZonalDistributionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AnalysisZonalDistributionDialog(QSqlDatabase db, int year, QList<int> waterIds, QWidget *parent = 0);
    ~AnalysisZonalDistributionDialog();

private:
    void createDistribution(int, int);
    void evaluateCatchesCoordinates(QList<QPointF> *coordList, int waterId, int year);
    Ui::AnalysisZonalDistributionDialog *ui;
    QSqlDatabase db;
    QSqlQuery query;
};

#endif // ANALYSISZONALDISTRIBUTIONDIALOG_H
