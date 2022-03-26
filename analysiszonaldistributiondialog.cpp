#include "analysiszonaldistributiondialog.h"
#include "ui_analysiszonaldistributiondialog.h"

#include "configuration.h"

#include <QSqlQuery>
#include <QDebug>
#include <QDir>
#include <QPixmap>
#include <QPainter>
#include <QPen>
#include <QList>
#include <QPointF>
#include <QDate>

AnalysisZonalDistributionDialog::AnalysisZonalDistributionDialog(QSqlDatabase db, int year, QList<int> waterIds, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AnalysisZonalDistributionDialog)
{
    this->db = db;
    QSqlQuery query(db);

    ui->setupUi(this);

    createDistribution(year, waterIds[0]);
}

AnalysisZonalDistributionDialog::~AnalysisZonalDistributionDialog()
{
    delete ui;
}

/*
 * Method extracts all catch locations corresponding to the passed year(s) and the
 * corresonding water location.
 */
void AnalysisZonalDistributionDialog::createDistribution(int year, int waterId)
{
    QString sQuery;
    QList<QPointF> coordinateList;

    qDebug() << "AnalysisZonalDistributionDialog: Zonal distribution of catches of year" << year;

    /* Extract the image path of the water from the database */
    sQuery = QString("SELECT name, image, latitudeNW, longitudeNW, latitudeSE, longitudeSE FROM Location WHERE id=%1").arg(waterId);
    query.exec(sQuery);
    qDebug() << "Query on Location active?" << query.isActive();
    if ( query.next() ) {
        QString locName = query.value(0).toString();
        QString locPath = Configuration::getPathLocationImageFiles() + QDir::separator() +
                          query.value(1).toString();
        double latiNW = query.value(2).toDouble();
        double longNW = query.value(3).toDouble();
        double latiSE = query.value(4).toDouble();
        double longSE = query.value(5).toDouble();
        double dx = IMAGE_SIZE_X / ( longSE - longNW );
        double dy = IMAGE_SIZE_Y / ( latiNW - latiSE );
        qDebug() << "AnalysisZonalDistributionDialog: Location = >" << locName << "<, Path = >"
                 << locPath << "<, latitude NW = " << latiNW << ", longitude NW = " << longNW
                 << ", latitude SE = " << latiSE << ", longitude SE = " << longSE;
        QPixmap pixmap(locPath);
        if ( ! pixmap.isNull()) {
            evaluateCatchesCoordinates(&coordinateList, waterId, year);
            qDebug() << "AnalysisZonalDistributionDialog:" << coordinateList.size() << "Catches found.";
            QPainter painter(&pixmap);
            QPen pen(QColor("red"));
            pen.setWidth(2);
            painter.setPen(pen);
            foreach ( QPointF pF, coordinateList ) {
                QPoint p = QPoint((int) (dx * (pF.rx() - longNW)), (int) (dy * (latiNW - pF.ry())));
                qDebug() << "Catch at" << pF.ry() << "N," << pF.rx() << "E";
                painter.drawEllipse(p, 5, 5);
            }
            painter.end();
            ui->picture->setPixmap(pixmap);
            ui->picture->setScaledContents(true);
        } else {
            qWarning() << "AnalysisZonalDistributionDialog: Image of water location not found!";
            ui->picture->setText("Error: image of water location not found!");
        }
    } else {
        qWarning() << "AnalysisZonalDistributionDialog: Location not found!";
        ui->picture->setText("Error: location not found!");
    }
}

/*
 * Method evaluates the coordinates of a catch of the water and the year from the database.
 * If the sessions lasts longer than 23:59:59, all catches of the next day(s) are also counted!
 */
void AnalysisZonalDistributionDialog::evaluateCatchesCoordinates(QList<QPointF> *coordList, int waterId, int year)
{
    QString sQuery;
    QList<int> sessionList;

    /* Extract all sessions which took place at the specified water */
    sQuery = QString("SELECT id, starttime FROM Session WHERE location_id=%1").arg(waterId);
    query.exec(sQuery);
    qDebug() << "Query on session active?: water id =" << waterId << ", year =" << year << " => " << query.isActive();
    while ( query.next() ) {
        int sessionYear = query.value(1).toDate().year();
        int sessionId = query.value(0).toInt();
        if ( year == 0 || sessionYear == year ) {
            sessionList.append(sessionId);
        }
    }

    /* Extract all catches which took place during the sessions */
    foreach ( int id, sessionList ) {
        sQuery = QString("SELECT latitude, longitude FROM Catch WHERE session_id=%1").arg(id);
        query.exec(sQuery);
        while ( query.next() ) {
            coordList->append(QPointF(query.value(1).toDouble(), query.value(0).toDouble()));
        }
    }
}
