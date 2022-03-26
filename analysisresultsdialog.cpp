#include "analysisresultsdialog.h"
#include "ui_analysisresultsdialog.h"
#include "sessionproxymodel.h"
#include "catchmodel.h"
#include "catchresult.h"

#include <QtDebug>
#include <QSqlRecord>
#include <QSqlQuery>
#include <QDateTime>
#include <QDate>
#include <QTime>
#include <QMap>
#include <QTableWidgetItem>
#include <algorithm>

AnalysisResultsDialog::AnalysisResultsDialog(QSqlDatabase db, int year, QList<int> waterIds, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AnalysisResultsDialog)
{
    this->year = year;
    this->waterIds = waterIds;
    this->db = db;

    ui->setupUi(this);

    ui->outputTable->setColumnCount(5);
    QStringList header;
    header << tr("species") << tr("count") << tr("tot. weight (kg)") << tr("av. weight (g)") << tr("av. length (cm)");
    ui->outputTable->setHorizontalHeaderLabels(header);
    ui->outputTable->setRowCount(0);

    analysis();
}

AnalysisResultsDialog::~AnalysisResultsDialog()
{
    delete ui;
}

/*
 * Method searches for catches in a specific (or all) year and specific waters
 * and evaluates data like the number of fishing days etc.
 */
void AnalysisResultsDialog::analysis()
{
    QSqlQuery query;
    QMap<int, CatchResult *> catches;
    QMap<int, int> baits;

    qDebug() << "Analysis of waters" << year;

    /* Extract all catches of a specific year (or all years if year = 0) from the database
       and store them in a map. Store the baits used to catch the fish in another map. */
    query.exec("SELECT catchtime, session_id, species_id, specieslength, speciesweight, bait_id FROM Catch");
    while ( query.next() ) {
        int catchYear = query.value(0).toDate().year();
        if ( year == 0 || catchYear == year ) {
            if ( isInSelectedWaters(query.value(1).toInt()) ) {
                int speciesId = query.value(2).toInt();
                int baitId = query.value(5).toInt();
                /* Add species to catches map */
                if ( ! catches.contains(speciesId) ) {  // New species: insert species into map
                    catches.insert(speciesId, new CatchResult());
                }
                catches[speciesId]->increaseCount();
                catches[speciesId]->increaseLength(query.value(3).toDouble());
                catches[speciesId]->increaseWeight(query.value(4).toDouble());
                /* Add bait to baits map */
                if ( ! baits.contains(baitId) ) {       // New bait: insert bait into map
                    baits.insert(baitId, 0);
                }
                baits[baitId] = baits[baitId] + 1;
            }
        }
    }

    // Extract the names of the species catched in the year and store them in the map
    query.exec("SELECT id, name FROM Species");
    while ( query.next() ) {
        int id;
        if ( catches.contains(id = query.value(0).toInt()) ) {
            catches[id]->setName(query.value(1).toString());
        }
    }

    // Fill the table widget with the catches of the year
    ui->outputTable->setRowCount(catches.size() + 1);
    ui->outputTable->verticalHeader()->setHidden(true);
    QMap<int, CatchResult *>::iterator i;
    int line = 0, allCount = 0;
    double allTotalWeight = 0.0, allAverageWeight = 0.0, allAverageLength = 0.0;
    for ( i = catches.begin(); i != catches.end(); ++i ) {
        ui->outputTable->setItem(line, 0, new QTableWidgetItem(i.value()->getName()));
        ui->outputTable->setItem(line, 1, new QTableWidgetItem(QString::number(i.value()->getCount())));
        allCount += i.value()->getCount();
        ui->outputTable->setItem(line, 2, new QTableWidgetItem(QString::number(i.value()->getTotalWeight() / 1000.0)));
        allTotalWeight += i.value()->getTotalWeight();
        ui->outputTable->setItem(line, 3, new QTableWidgetItem(QString::number(i.value()->getAverageWeight())));
        allAverageWeight += i.value()->getAverageWeight();
        ui->outputTable->setItem(line, 4, new QTableWidgetItem(QString::number(i.value()->getAverageLength())));
        allAverageLength += i.value()->getAverageLength();
        line++;
        delete i.value();
    }
    QTableWidgetItem *twi;
    ui->outputTable->setItem(line, 0, twi = new QTableWidgetItem("Sum"));
    QFont font(twi->font());
    font.setBold(true);
    twi->setFont(font);
    ui->outputTable->setItem(line, 1, twi = new QTableWidgetItem(QString::number(allCount)));
    twi->setFont(font);
    ui->outputTable->setItem(line, 2, twi = new QTableWidgetItem(QString::number(allTotalWeight / 1000.0)));
    twi->setFont(font);
    if ( catches.size() == 0 ) {
        ui->outputTable->setItem(line, 3, twi = new QTableWidgetItem("---"));
        twi->setFont(font);
        ui->outputTable->setItem(line, 4, twi = new QTableWidgetItem("---"));
        twi->setFont(font);
    } else {
        ui->outputTable->setItem(line, 3, twi = new QTableWidgetItem(QString::number(allAverageWeight / catches.size())));
        twi->setFont(font);
        ui->outputTable->setItem(line, 4, twi = new QTableWidgetItem(QString::number(allAverageLength / catches.size())));
        twi->setFont(font);
    }
    // Extract all fishing days of the year from the database
    query.exec("SELECT starttime, endtime, location_id, rating FROM Session");
    int sumSeconds = 0, sumDays = 0, sumRating = 0, countSessions = 0;
    QDate lastDate = QDate(1, 1, 1);        // some very, very early date
    while ( query.next() ) {
        int catchYear = query.value(0).toDate().year();
        if ( ( year == 0 || catchYear == year ) && waterIds.contains(query.value(2).toInt()) ) {
            countSessions++;
            QDateTime startDateTime(query.value(0).toDateTime());
            QDateTime endDateTime(query.value(1).toDateTime());
            if ( startDateTime.date() > lastDate ) {
                sumDays++;
                lastDate = startDateTime.date();
            }
            sumSeconds += startDateTime.msecsTo(endDateTime) / 1000;
            sumRating += query.value(3).toInt();
            qDebug() << "Start, Time:" << startDateTime.toString() << "," << endDateTime.toString();
        }
    }
    ui->editDays->setText(QString::number(sumDays));
    int sumHours = sumSeconds / 3600;
    ui->editHours->setText(QString::number(sumHours));
    double hoursPerFish = static_cast<double> (sumHours) /
                          static_cast<double> (allCount);
    ui->editHoursPerFish->setText(QString::number(hoursPerFish));
    if ( countSessions > 0 ) {
        double averageRating = static_cast<double> ( sumRating ) /
                               static_cast<double> ( countSessions );
        ui->editRating->setText(QString::number(averageRating));
    } else {
        ui->editRating->setText("---");
    }

    /* Evaluate best bait */
    int bestBaitId = -1;
    for ( int i = 0, max = 0; i < baits.count(); i++) {
        if ( baits[i] > max ) {
            max = baits[i];
            bestBaitId = i;
        }
    }
    if ( bestBaitId > -1 ) {
        QString sQuery;
        sQuery = QString("SELECT name FROM Bait WHERE id=%1").arg(bestBaitId);
        query.exec(sQuery);     // Extract the name of the best bait
        if ( query.next() ) {
            ui->editBestBait->setText(query.value(0).toString());
        } else {
            qWarning() << "AnalysisResultsDialog: Bait name not found!";
        }

    } else {
        ui->editBestBait->setText("---");
    }
}

bool AnalysisResultsDialog::isInSelectedWaters(int sessionId)
{
    QSqlQuery query;
    QString sQuery;
    sQuery = QString("SELECT location_id FROM Session WHERE id=%1").arg(sessionId);
    query.exec(sQuery);
    if ( query.next() ) {
        int waterId = query.value(0).toInt(0);
        return ( waterIds.contains(waterId) );
    } else {
        qWarning() << "AnalysisResultsDialog: Session not found!";
        return false;
    }
}
