#include "editcatchdialog.h"
#include "ui_editcatchdialog.h"
#include "catchmodel.h"
#include "sessionproxymodel.h"
#include "fishmodel.h"
#include "sortedcomboboxdelegate.h"

#include <QMessageBox>
#include <QtDebug>
#include <QSqlRecord>
#include <QSqlQuery>
#include <QtMath>

EditCatchDialog::EditCatchDialog(QSqlDatabase db, int id, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditCatchDialog)
{
    ui->setupUi(this);

    catchModel = new CatchModel(this, db);
    catchModel->setTable("Catch");

    catchModel->setRelation(CatchModel::CatchSessionId , QSqlRelation("Session", "id", "starttime"));
    QSqlTableModel *sessionModel = catchModel->relationModel(CatchModel::CatchSessionId);
    ui->comboSession->setModel(sessionModel);
    ui->comboSession->setModelColumn(sessionModel->fieldIndex("starttime"));

    catchModel->setRelation(CatchModel::CatchBaitId, QSqlRelation("Bait", "id", "name"));
    QSqlTableModel *baitModel = catchModel->relationModel(CatchModel::CatchBaitId);
    ui->comboBait->setModel(baitModel);
    ui->comboBait->setModelColumn(baitModel->fieldIndex("name"));

    catchModel->setRelation(CatchModel::CatchSpeciesId , QSqlRelation("Species", "id", "name"));
    QSqlTableModel *speciesModel = catchModel->relationModel(CatchModel::CatchSpeciesId);
    ui->comboSpecies->setModel(speciesModel);
    ui->comboSpecies->setModelColumn(speciesModel->fieldIndex("name"));

    if ( ! catchModel->select() ) {
        qCritical() << "No connection between CatchModel and database!";
    }
    mapper = new QDataWidgetMapper(this);
    mapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);
    mapper->setModel(catchModel);
    mapper->setItemDelegate(new SortedComboBoxDelegate(this));
    mapper->addMapping(ui->editCatchTime, CatchModel::CatchTime);
    mapper->addMapping(ui->comboSession, CatchModel::CatchSessionId);
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

    connect(ui->dialogButtonBox, SIGNAL(accepted()), this, SLOT(onButtonBoxAccepted()));
    connect(ui->editLength, SIGNAL(editingFinished()), this, SLOT(onEditLengthFinished()));
    connect(ui->editWeight, SIGNAL(editingFinished()), this, SLOT(onEditWeightFinished()));

    qDebug() << "EditCatchDialog: new catch dialog created; id = " << id;
}

/*
 * Destructor deletes the edit catch dialog user interface and all heap storage.
 */
EditCatchDialog::~EditCatchDialog()
{
    delete ui;
    delete catchModel;
}

/*
 * Method handles the acceptance Button. The database shall be updated.
 */
void EditCatchDialog::onButtonBoxAccepted()
{
    int sessionId = -1;
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
    if ( sessionId <= 0 ) {
        qDebug() << "EditCatchDialog: no session found (session id <= 0!";
    } else {
        qDebug() << "EditCatchDialog: session found: id = " << sessionId;
        ui->comboSession->setCurrentIndex(sessionId - 1);  // The session id starts with 1, the combo box index with 0
    }
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
