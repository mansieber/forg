#include "analysisyeardialog.h"
#include "ui_analysisyeardialog.h"
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

AnalysisYearDialog::AnalysisYearDialog(QSqlDatabase db, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AnalysisYearDialog)
{
    this->db = db;

    ui->setupUi(this);

    ui->outputTable->setColumnCount(5);
    ui->outputTable->verticalHeader()->setHidden(true);
    QStringList header;
    header << tr("species") << tr("count") << tr("tot. weight (kg)") << tr("av. weight (g)") << tr("av. length (cm)");
    ui->outputTable->setHorizontalHeaderLabels(header);
    ui->outputTable->setRowCount(0);

    QSqlQuery query;
    QList<int> years;
    query.exec("SELECT * FROM Session");
    while ( query.next() ) {
        QDateTime timeStart = query.value(SessionProxyModel::SessionStartTime).toDateTime();
        int year = timeStart.date().year();
        if ( ! years.contains(year) ) {
            years.append(year);
        }
    }
    if ( ! years.isEmpty() ) {
        std::sort(years.begin(), years.end());
        ui->comboSelect->addItem(tr(""));
        for ( int i = years.count() - 1; i >= 0; i-- ) {
            QString strYear;
            strYear.setNum(years[i]);
            ui->comboSelect->addItem(strYear);
        }
    }

    connect(ui->comboSelect, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(analysis(const QString &)));
}

AnalysisYearDialog::~AnalysisYearDialog()
{
    delete ui;
}

/*
 * Method is called after a year was selected by the user. Method
 * searches for catches in this year and evaluates data like the number
 * of fishing days etc.
 * @param sYear is the string containing the year
 */
void AnalysisYearDialog::analysis(const QString &sYear)
{
    QSqlQuery query;
    QMap<int, CatchResult *> catches;
    int year = sYear.toInt();
    if ( year != 0 ) {
        qDebug() << "Analysis of year" << year;

        // Extract all catches of the year from the database and store them in a map
        query.exec("SELECT catchtime, species_id, specieslength, speciesweight FROM Catch");
        while ( query.next() ) {
            int catchYear = query.value(0).toDate().year();
            if ( catchYear == year ) {
                int speciesId = query.value(1).toInt();
                if ( ! catches.contains(speciesId) ) {
                    catches.insert(speciesId, new CatchResult());
                }
                catches[speciesId]->increaseCount();
                catches[speciesId]->increaseLength(query.value(2).toDouble());
                catches[speciesId]->increaseWeight(query.value(3).toDouble());
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
        ui->outputTable->setItem(line, 3, twi = new QTableWidgetItem(QString::number(allAverageWeight / catches.size())));
        twi->setFont(font);
        ui->outputTable->setItem(line, 4, twi = new QTableWidgetItem(QString::number(allAverageLength / catches.size())));
        twi->setFont(font);

        // Extract all fishing days of the year from the database
        query.exec("SELECT starttime, endtime, rating FROM Session");
        int sumSeconds = 0, sumDays = 0, sumRating = 0, countSessions = 0;
        QDate lastDate = QDate(1, 1, 1);
        while ( query.next() ) {
            int catchYear = query.value(0).toDate().year();
            if ( catchYear == year ) {
                countSessions++;
                QDateTime startDateTime(query.value(0).toDateTime());
                QDateTime endDateTime(query.value(1).toDateTime());
                if ( startDateTime.date() > lastDate ) {
                    sumDays++;
                    lastDate = startDateTime.date();
                }
                sumSeconds += startDateTime.msecsTo(endDateTime) / 1000;
                sumRating += query.value(2).toInt();
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
    } else {
        qDebug() << "No analysis of year: year invalid!";
    }
}

