/*
 * Class realizes a dialog which allows to add and edit fish catch event. If the
 * catch date and time fits to an already added fishing session the catch will
 * automatically related to this fishing session.
 */

#include "editcatchdialog.h"
#include "ui_editcatchdialog.h"
#include "catchmodel.h"
#include "sessionmodel.h"
#include "baitmodel.h"
#include "sessionproxymodel.h"
#include "fishmodel.h"

#include <QMessageBox>
#include <QtDebug>
#include <QSqlRecord>
#include <QSqlQuery>
#include <QSqlRelationalDelegate>
#include <QtMath>

EditCatchDialog::EditCatchDialog(QSqlDatabase db, int id, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditCatchDialog)
{
    ui->setupUi(this);

    catchModel = new CatchModel(this, db);
    catchModel->setJoinMode(QSqlRelationalTableModel::LeftJoin);
    catchModel->setTable("Catch");

    catchModel->setRelation(CatchModel::CatchSessionId , QSqlRelation(SESSION_TABLE, SESSION_COLUMN_ID, SESSION_COLUMN_STARTTIME));

    catchModel->setRelation(CatchModel::CatchBaitId, QSqlRelation(BAIT_TABLE, BAIT_COLUMN_ID, BAIT_COLUMN_NAME));

    baitModel = catchModel->relationModel(CatchModel::CatchBaitId);
    baitModel->setSort(BaitModel::BaitName, Qt::AscendingOrder); // Baits shall be sorted alphabetically
    baitModel->select();
    ui->comboBait->setModel(baitModel);
    ui->comboBait->setModelColumn(BaitModel::BaitName);

    catchModel->setRelation(CatchModel::CatchSpeciesId , QSqlRelation(SPECIES_TABLE, SPECIES_COLUMN_ID, SPECIES_COLUMN_NAME));
    speciesModel = catchModel->relationModel(CatchModel::CatchSpeciesId);
    speciesModel->setSort(FishModel::FishName, Qt::AscendingOrder); // Species shall be sorted alphabetically
    speciesModel->select();
    ui->comboSpecies->setModel(speciesModel);
    ui->comboSpecies->setModelColumn(FishModel::FishName);

    if ( ! catchModel->select() ) {
        qCritical() << "No connection between CatchModel and database!";
    }
    mapper = new QDataWidgetMapper(this);
    mapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);
    mapper->setModel(catchModel);
    mapper->setItemDelegate(new QSqlRelationalDelegate(this));
    mapper->addMapping(ui->editCatchTime, CatchModel::CatchTime);
    mapper->addMapping(ui->comboSpecies, CatchModel::CatchSpeciesId);
    mapper->addMapping(ui->comboBait, CatchModel::CatchBaitId);
    mapper->addMapping(ui->editLength, CatchModel::CatchLength);
    mapper->addMapping(ui->editWeight, CatchModel::CatchWeight);
    mapper->addMapping(ui->editLatitude, CatchModel::CatchLatitude);
    mapper->addMapping(ui->editLongitude, CatchModel::CatchLongitude);
    mapper->addMapping(ui->editPath, CatchModel::CatchImage);
    mapper->addMapping(ui->editDescription, CatchModel::CatchDescription);

    if ( id != -1 ) {           // Edit an exisiting catch
        for ( int row = 0; row < catchModel->rowCount(); ++row ) {
            QSqlRecord record = catchModel->record(row);
            if ( record.value(CatchModel::CatchId).toInt() == id ) {
                mapper->setCurrentIndex(row);
                break;
            }
        }
    } else {                    // Add a new catch
        int row = catchModel->rowCount();
        qDebug() << "EditCatchDialog: add new catch, row count =" << row;
        bool result = catchModel->insertRow(row);
        qDebug() << "EditCatchDialog: new row added" << ( result ? "successful" : "not successful");
        mapper->setCurrentIndex(row);
    }

    ui->displaySession->setText(
        sessionToString(catchModel->index(mapper->currentIndex(), CatchModel::CatchSessionId)));

    connect(ui->dialogButtonBox, SIGNAL(accepted()), this, SLOT(onButtonBoxAccepted()));
    connect(ui->editLength, SIGNAL(editingFinished()), this, SLOT(onEditLengthFinished()));
    connect(ui->editWeight, SIGNAL(editingFinished()), this, SLOT(onEditWeightFinished()));
    connect(ui->editCatchTime, SIGNAL(editingFinished()), this, SLOT(onEditTimeFinished()));

    qDebug() << "EditCatchDialog: new catch dialog created; id = " << id;
}

/*
 * Destructor deletes the edit catch dialog user interface and all heap storage.
 */
EditCatchDialog::~EditCatchDialog()
{
    delete ui;
    delete catchModel;
    delete mapper;
}

/*
 * Method handles the acceptance Button. The database shall be updated.
 */
void EditCatchDialog::onButtonBoxAccepted()
{
    int session = findSessionId();
    if ( session <= 0 ) {
        qDebug() << "EditCatchDialog: no session found! session id = " << session;
        catchModel->setData(catchModel->index(mapper->currentIndex(), CatchModel::CatchSessionId), 0);
    } else {
        qDebug() << "EditCatchDialog: session found: id = " << session;
        catchModel->setData(catchModel->index(mapper->currentIndex(), CatchModel::CatchSessionId), session);
    }

    qDebug() << "EditCatchDialog: catch model session id: " << catchModel->data(catchModel->index(mapper->currentIndex(), CatchModel::CatchSessionId), Qt::DisplayRole);

    bool result = mapper->submit();
    catchModel->updateRow(mapper->currentIndex());         // All views must be updated

    qDebug() << "EditCatchDialog: catch edited/added. Submit" << ( result ? "successful" : "not successful");

    this->close();
}

/*
 * Method (slot) is called when edit of the length is finished. If the weight is not
 * yet edited, it will be calculated and set.
 */
void EditCatchDialog::onEditLengthFinished()
{
    QString sLength = ui->editLength->text();
    bool conversionOk;
    double length = sLength.toDouble(&conversionOk);
    int speciesComboId = ui->comboSpecies->currentIndex();
    if ( conversionOk && ( length >= 0 ) &&
         ( ui->editWeight->text().length() == 0 ) && ( speciesComboId >= 0 ) ) {
        int speciesId = speciesComboId + 1;
        QSqlQuery query;
        QString sQuery = "SELECT * FROM Species WHERE id = %1";
        query.exec(sQuery.arg(speciesId));
        qDebug() << "EditCatchDialog:editLengthFinished: species id = " << speciesId;
        if ( query.next() ) {
            double cf = query.value(FishModel::FishCFactor).toDouble();
            double weight = qPow(length, 3.0) * cf / 100.0;
            qDebug() << "EditCatchDialog:editLengthFinished: species id =" << speciesId << ", cf =" << cf
                     << ", w =" << weight;
            ui->editWeight->setText(QString::number(weight, 'f', 0));
        } else {
            qWarning() << "EditCatchDialog: no corp. factor!";
        }
    }
}

/*
 * Method (slot) is called when edit of the weight is finished. If the length is not
 * yet edited, it will be calculated and set.
 */
void EditCatchDialog::onEditWeightFinished()
{
    QString sWeight = ui->editWeight->text();
    bool conversionOk;
    double weight = sWeight.toDouble(&conversionOk);
    int speciesComboId = ui->comboSpecies->currentIndex();
    if ( conversionOk && ( weight > 0 ) &&
         ( ui->editLength->text().length() == 0 ) && ( speciesComboId >= 0 ) ) {
        int speciesId = speciesComboId + 1;
        QSqlQuery query;
        QString sQuery = "SELECT * FROM Species WHERE id = %1";
        query.exec(sQuery.arg(speciesId));
        if ( query.next() ) {
            double cf = query.value(FishModel::FishCFactor).toDouble();
            if ( cf != 0.0 ) {
                double length = qPow((weight * 100.0 / cf), 0.33333333333);
                qDebug() << "EditCatchDialog:editWeightFinished: species id =" << speciesId << ", cf =" << cf
                         << ", l =" << length;
                ui->editLength->setText(QString::number(length, 'f', 0));
            } else {
                qWarning() << "EditCatchDialog: corp. factor = 0.0!";
            }
        } else {
            qWarning() << "EditCatchDialog: no corp. factor!";
        }
    }
}

/*
 * Method (slot) is called when edit of the date/time is finished (also of dialog is closed).
 * Method will search a session which fits to the catch date/time. If successful, the session
 * id will be set.
 */
void EditCatchDialog::onEditTimeFinished()
{
    int session = findSessionId();
    if ( session <= 0 ) {
        qDebug() << "EditCatchDialog: no session found! session id = " << session;
        catchModel->setData(catchModel->index(mapper->currentIndex(), CatchModel::CatchSessionId), 0);
    } else {
        qDebug() << "EditCatchDialog: session found: id = " << session;
        catchModel->setData(catchModel->index(mapper->currentIndex(), CatchModel::CatchSessionId), session);
    }

    ui->displaySession->setText(
        sessionToString(catchModel->index(mapper->currentIndex(), CatchModel::CatchSessionId)));

    qDebug() << "EditCatchDialog: catch model session id: " << catchModel->data(catchModel->index(mapper->currentIndex(), CatchModel::CatchSessionId), Qt::DisplayRole);

}

/*
 * Private method returns the id of the session which fits to the catch date & time.
 * If no session is found -1 is returned.
 */
int EditCatchDialog::findSessionId()
{
    int sessionId = 0;
    QSqlQuery query;
    QDateTime timeCatch = ui->editCatchTime->dateTime();
    query.exec("SELECT * FROM Session");
    while ( query.next() ) {
        QDateTime timeStart = query.value(SessionProxyModel::SessionStartTime).toDateTime();
        QDateTime timeEnd = query.value(SessionProxyModel::SessionEndTime).toDateTime();
        if ( timeStart <= timeCatch && timeEnd >= timeCatch ) {
            sessionId = query.value(SessionProxyModel::SessionId).toInt();
            break;
        }
    }
    return sessionId;
}

/*
 * Private method returns a string representing the session.
 * The session is reflected by the start date/time. If no session exists an appropriate
 * message is returned.
 */
QString EditCatchDialog::sessionToString(const QModelIndex cIndex)
{
    qDebug() << "EditCatchDialog::sessionToString(): data = " << (catchModel->data(cIndex)).toString();
    if ( (catchModel->data(cIndex)).toString().isEmpty() ) {
        return tr("No session");
    } else {
        QDateTime dateTime = QDateTime::fromString((catchModel->data(cIndex)).toString(),
                                  "yyyy-MM-ddThh:mm:ss.zzz");
        QString sDateTime = dateTime.date().toString("dd.MM.yy") + " " + dateTime.time().toString("hh.mm");
        return sDateTime;
    }
}

