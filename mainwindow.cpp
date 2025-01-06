#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "sessionmodel.h"
#include "sessionproxymodel.h"
#include "catchesdelegate.h"
#include "configuration.h"
#include "analysisresultsdialog.h"
#include "analysiszonaldistributiondialog.h"
#include "generalconstants.h"

#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlRecord>
#include <QModelIndex>
#include <QSettings>
#include <QFileDialog>
#include <QtDebug>

/*
 * Constructor.
 */
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    Configuration::Instance();

    readSettings();

    connect(ui->addButton, SIGNAL(clicked()), this, SLOT(addSession()));
    connect(ui->editButton, SIGNAL(clicked()), this, SLOT(editSession()));
    connect(ui->deleteButton, SIGNAL(clicked()), this, SLOT(deleteSession()));
    connect(ui->sessionTable, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(editSession(QModelIndex)));

    connect(ui->actionConnectDatabase, SIGNAL(triggered()), this, SLOT(onConnectDatabaseTriggered()));
    connect(ui->actionConfiguration, SIGNAL(triggered()), this, SLOT(onConfigurationTriggered()));
    connect(ui->actionQuit, SIGNAL(triggered()), this, SLOT(onQuitTriggered()));
    connect(ui->actionAddBait, SIGNAL(triggered()), this, SLOT(onAddBaitTriggered()));
    connect(ui->actionViewBait, SIGNAL(triggered()), this, SLOT(onViewBaitTriggered()));
    connect(ui->actionAddFish, SIGNAL(triggered()), this, SLOT(onAddFishTriggered()));
    connect(ui->actionViewFishes, SIGNAL(triggered()), this, SLOT(onViewFishesTriggered()));
    connect(ui->actionAddLocation, SIGNAL(triggered()), this, SLOT(onAddLocationTriggered()));
    connect(ui->actionViewLocations, SIGNAL(triggered()), this, SLOT(onViewLocationsTriggered()));
    connect(ui->actionAddCatch, SIGNAL(triggered()), this, SLOT(onAddCatchTriggered()));
    connect(ui->actionViewCatches, SIGNAL(triggered()), this, SLOT(onViewCatchesTriggered()));
    connect(ui->actionAnalysisYear, SIGNAL(triggered()), this, SLOT(onAnalysisYearTriggered()));
    connect(ui->actionAnalysisWaters, SIGNAL(triggered()), this, SLOT(onAnalysisWatersTriggered()));
    connect(ui->actionZonalDistribution, SIGNAL(triggered()), this, SLOT(onSelectionWatersTriggered()));
    connect(ui->actionConsistencyCheck, SIGNAL(triggered()), this, SLOT(onConsistencyCheckTriggered()));
    connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(onAboutTriggered()));
    connect(ui->actionAddSession, SIGNAL(triggered()), this, SLOT(addSession()));

    openDatabase();         // What happens if data base is not accessible??????????

    sessionModel = new SessionModel(this, db);
    sessionModel->setTable("Session");
    sessionModel->setRelation(SessionProxyModel::SessionLocationId, QSqlRelation("Location", "id", "name"));
    sessionModel->setEditStrategy(QSqlRelationalTableModel::OnRowChange);
    ui->sessionTable->sortByColumn(SessionProxyModel::SessionStartTime, Qt::AscendingOrder);
    sessionProxyModel = new SessionProxyModel(this);
    sessionProxyModel->setSourceModel(sessionModel);

    updateView();
    sessionProxyModel->setHeaderData(SessionProxyModel::SessionCatches,
                                     Qt::Horizontal, tr("Catches"), Qt::DisplayRole);
    sessionProxyModel->setHeaderData(SessionProxyModel::SessionStartTime,
                                     Qt::Horizontal, tr("Start"), Qt::DisplayRole);
    sessionProxyModel->setHeaderData(SessionProxyModel::SessionEndTime,
                                     Qt::Horizontal, tr("End"), Qt::DisplayRole);
    sessionProxyModel->setHeaderData(SessionProxyModel::SessionLocationId,
                                     Qt::Horizontal, tr("Location"), Qt::DisplayRole);
    sessionProxyModel->setHeaderData(SessionProxyModel::SessionRating,
                                     Qt::Horizontal, tr("Rating"), Qt::DisplayRole);
    ui->sessionTable->setModel(sessionProxyModel);
    ui->sessionTable->setItemDelegateForColumn(SessionProxyModel::SessionStartTime, new DateTimeDelegate);
    ui->sessionTable->setItemDelegateForColumn(SessionProxyModel::SessionEndTime, new DateTimeDelegate);
    ui->sessionTable->setItemDelegateForColumn(SessionProxyModel::SessionCatches, new CatchesDelegate);
    ui->sessionTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->sessionTable->setColumnHidden(SessionProxyModel::SessionId, true);
    ui->sessionTable->setColumnHidden(SessionProxyModel::SessionWeather, true);
    ui->sessionTable->setColumnHidden(SessionProxyModel::SessionHighTime, true);
    ui->sessionTable->setColumnHidden(SessionProxyModel::SessionAirPressure, true);
    ui->sessionTable->setColumnHidden(SessionProxyModel::SessionPressureChange, true);
    ui->sessionTable->setColumnHidden(SessionProxyModel::SessionAirTemperature, true);
    ui->sessionTable->setColumnHidden(SessionProxyModel::SessionWaterTemperature, true);
    ui->sessionTable->setColumnHidden(SessionProxyModel::SessionWindStrength, true);
    ui->sessionTable->setColumnHidden(SessionProxyModel::SessionWindDirection, true);
    ui->sessionTable->setColumnHidden(SessionProxyModel::SessionDescription, true);
    ui->sessionTable->setColumnHidden(SessionProxyModel::SessionCatches, false);
    ui->sessionTable->sortByColumn(SessionProxyModel::SessionStartTime, Qt::AscendingOrder);
    ui->sessionTable->resizeColumnsToContents();
    ui->sessionTable->resizeRowsToContents();
    updateView();
}

/*
 * Destruct application window and save settings.
 */
MainWindow::~MainWindow()
{
    writeSettings();

    delete ui;
}

/*
 * Close application.
 */
void MainWindow::onQuitTriggered()
{
    delete sessionModel;
    delete sessionProxyModel;
    MainWindow::close();
}

/*
 * Create new edit bait dialog and show it.
 */
void MainWindow::onAddBaitTriggered()
{
    editBaitDialog = new EditBaitDialog(db, -1, this);
    editBaitDialog->show();
}

/*
 * Create new view bait dialog and show it.
 */
void MainWindow::onViewBaitTriggered()
{
    viewBaitDialog = new ViewBaitDialog(db, this);
    viewBaitDialog->show();
}

/*
 * Create new edit fish dialog and show it.
 */
void MainWindow::onAddFishTriggered()
{
    editFishDialog = new EditFishDialog(db, -1, this);
    editFishDialog->show();
}

/*
 * Create new view fishes dialog and show it.
 */
void MainWindow::onViewFishesTriggered()
{
    viewFishDialog = new ViewFishDialog(db, this);
    viewFishDialog->show();
}

/*
 * Create new edit location dialog and show it.
 */
void MainWindow::onAddLocationTriggered()
{
    editLocationDialog = new EditLocationDialog(db, -1, this);
    editLocationDialog->show();
}

/*
 * Create new view locations dialog and show it.
 */
void MainWindow::onViewLocationsTriggered()
{
    viewLocationDialog = new ViewLocationDialog(db, this);
    viewLocationDialog->show();
}

/*
 * Create new edit catch dialog and show it.
 */
void MainWindow::onAddCatchTriggered()
{
    editCatchDialog = new EditCatchDialog(db, -1, this);
    editCatchDialog->show();
}

/*
 * Create new view catch dialog and show it.
 */
void MainWindow::onViewCatchesTriggered()
{
    viewCatchDialog = new ViewCatchDialog(db, this);
    viewCatchDialog->show();
}

/*
 * Connect to an sqlite database.
 */
void MainWindow::onConnectDatabaseTriggered()
{
    pathDatabase = QFileDialog::getOpenFileName(this, tr("Connect to sqlite Database"));
    openDatabase();
}

/*
 * Create new configuration dialog and show it.
 */
void MainWindow::onConfigurationTriggered()
{
    configurationDialog = new ConfigurationDialog(this);
    configurationDialog->show();
}

/*
 * Create new analysis year dialog and show it.
 */
void MainWindow::onAnalysisYearTriggered()
{
    analysisYearDialog = new AnalysisYearDialog(db, this);
    analysisYearDialog->show();
}

/*
 * Create first a waters selection dialog and then an analysis waters dialog and show it.
 */
void MainWindow::onAnalysisWatersTriggered()
{
    int year = -1;
    QList<int> waterIds = QList<int>();
    SelectionWatersDialog *selectionWatersDialog = new SelectionWatersDialog(db, &year, &waterIds, false, this);
    selectionWatersDialog->exec();
    qDebug() << "MainWindow: Water specific analysis of year" << year << ", waters" << waterIds;
    if ( year != -1 ) {           // year == -1 if dialog is canceled.
        AnalysisResultsDialog *analysisResultsDialog = new AnalysisResultsDialog(db, year, waterIds, this);
        analysisResultsDialog->exec();
    }
}

/*
 * Create first a selection waters dialog and then a zonal distribution dialog and show it.
 */
void MainWindow::onSelectionWatersTriggered()
{
    int year = -1;
    QList<int> waterIds = QList<int>();
    SelectionWatersDialog *selectionWatersDialog = new SelectionWatersDialog(db, &year, &waterIds, true, this);
    selectionWatersDialog->exec();
    qDebug() << "MainWindow: Zonal distribution of catches" << year << ", waters" << waterIds;
    if ( year != -1 ) {           // year == -1 if dialog is canceled.
        AnalysisZonalDistributionDialog *resultsDialog = new AnalysisZonalDistributionDialog(db, year, waterIds, this);
        resultsDialog->exec();
    }
}

/*
 * Create new consistency check dialog and show it.
 */
void MainWindow::onConsistencyCheckTriggered()
{
    QMessageBox::information(this, tr("Consistency Check"), tr("Consistency check triggerd"));
}

/*
 * Create message box showing information about the application.
 */
void MainWindow::onAboutTriggered()
{
    QMessageBox::information(this, tr("About"), tr("%1 by %2\nVersion %3").arg(APPLICATION_NAME).arg(AUTHOR).arg(VERSION));
}

/*
 * Create the connection to the database and try to open it.
 */
void MainWindow::openDatabase() {

    // Start with clean up
    if ( db.isValid() ) {
        qDebug() << "MainWindow: Valid database found";
        QSqlDatabase::removeDatabase(TYPE_DATABASE);
        qDebug() << "MainWindow: Database removed";
    }

    ui->addButton->setEnabled(false);
    ui->editButton->setEnabled(false);
    ui->deleteButton->setEnabled(false);
    ui->actionAddBait->setEnabled(false);
    ui->actionViewBait->setEnabled(false);
    ui->actionAddFish->setEnabled(false);
    ui->actionViewFishes->setEnabled(false);
    ui->actionAddLocation->setEnabled(false);
    ui->actionViewLocations->setEnabled(false);
    ui->actionAddCatch->setEnabled(false);
    ui->actionViewCatches->setEnabled(false);
    ui->actionAddSession->setEnabled(false);

    db = QSqlDatabase::addDatabase(TYPE_DATABASE);
    db.setDatabaseName(pathDatabase);
    if ( ! db.open() ) {
        qDebug("MainWindow: Failed to open sqlite database!");
    } else {
        qDebug() << "MainWindow: Database opened at" << pathDatabase;
        QStringList l = db.tables(QSql::Tables);
        if ( l.isEmpty() ) {
            qDebug() << "MainWindow: Database is empty!";
        } else {
            qDebug() << "MainWindow: Database has tables: " << l.size();
            qDebug() << l;
            ui->addButton->setEnabled(true);
            ui->editButton->setEnabled(true);
            ui->deleteButton->setEnabled(true);
            ui->actionAddBait->setEnabled(true);
            ui->actionViewBait->setEnabled(true);
            ui->actionAddFish->setEnabled(true);
            ui->actionViewFishes->setEnabled(true);
            ui->actionAddLocation->setEnabled(true);
            ui->actionViewLocations->setEnabled(true);
            ui->actionAddCatch->setEnabled(true);
            ui->actionViewCatches->setEnabled(true);
            ui->actionAddSession->setEnabled(true);
        }
    }
}

/*
 * Read persistent settings from the configuration database of the operating system.
 */
void MainWindow::readSettings() {
    QSettings settings(NAME_COMPANY, NAME_PRODUCT);
    settings.beginGroup("general");
    pathDatabase = settings.value("database", "").toString();
    Configuration::setPathBaitImageFiles(settings.value("baitimagefiles", "").toString());
    Configuration::setPathFishImageFiles(settings.value("fishimagefiles", "").toString());
    Configuration::setPathLocationImageFiles(settings.value("locationimagefiles", "").toString());
    Configuration::setPathCatchImageFiles(settings.value("catchimagefiles", "").toString());
    qDebug() << "MainWindow: setting baitimagefiles set to " << Configuration::getPathBaitImageFiles();
    qDebug() << "MainWindow: setting fishimagefiles set to " << Configuration::getPathFishImageFiles();
    qDebug() << "MainWindow: setting locationimagefiles set to " << Configuration::getPathLocationImageFiles();
    qDebug() << "MainWindow: setting catchimagefiles set to " << Configuration::getPathCatchImageFiles();
}

/*
 * Write settings to the configuration database of the operating system to make them persistent.
 */
void MainWindow::writeSettings() {
    QSettings settings(NAME_COMPANY, NAME_PRODUCT);
    settings.beginGroup("general");
    settings.setValue("database", pathDatabase);
    settings.setValue("baitimagefiles", Configuration::getPathBaitImageFiles());
    settings.setValue("fishimagefiles", Configuration::getPathFishImageFiles());
    settings.setValue("locationimagefiles", Configuration::getPathLocationImageFiles());
    settings.setValue("catchimagefiles", Configuration::getPathCatchImageFiles());
    settings.endGroup();
    qDebug("MainWindow: settings written");
}

/*
 * Update the table view. This is usually necessary if rows were added/deleted/edited.
 */
void MainWindow::updateView()
{
    if ( ! sessionModel->select() ) {
        qCritical() << "MainWindow: Connection to database failed!";
    } else {
        qDebug() << "MainWindow: Update of view. Columns in model: " << sessionModel->columnCount();
        if ( ! sessionProxyModel->insertColumns(sessionProxyModel->columnCount(), 1) )
            qCritical() << "MainWindow: Column insertion into SessionProxyModel failed!";
        qDebug() << "MainWindow: Update of view. Columns in proxy model: " << sessionProxyModel->columnCount();
    }
}

/*
 * Create new edit session dialog for adding a session to the session table and show it.
 */
void MainWindow::addSession()
{
    qDebug() << "MainWindow: add session called.";
    EditSessionDialog editSessionDialog(db, -1, this);
    editSessionDialog.exec();
    updateView();
}

/*
 * Create new edit session dialog for editing session information and show it.
 * @param index references the selected index of the session model
 */
void MainWindow::editSession(QModelIndex index)
{
    qDebug() << "MainWindow: edit session called.";
    if ( index.isValid() ) {
        QSqlRecord record = sessionModel->record(index.row());
        int sessionId = record.value(SessionProxyModel::SessionId).toInt();
        EditSessionDialog editSessionDialog(db, sessionId, this);
        editSessionDialog.exec();
        updateView();
    } else
        qDebug() << "MainWindow: invalid index!";
}

/*
 * Create new edit session dialog for editing session information and show it.
 */
void MainWindow::editSession()
{
    qDebug() << "MainWindow: edit session called.";
    QModelIndex index = ui->sessionTable->currentIndex();      // get the selected index
    if ( index.isValid() )
        editSession(index);
    else
        qDebug() << "MainWindow: no item selected!";
}

/*
 * Delete session.
 */
void MainWindow::deleteSession()
{
    qDebug() << "MainWindow: delete session called.";
    QModelIndex index = ui->sessionTable->currentIndex();      // get the selected index
    if ( index.isValid() ) {
        sessionModel->removeRow(index.row());
        if ( ! sessionModel->submit() ) {
            qCritical() << "MainWindow: submit to database failed!";
        } else {
            if ( ! sessionModel->select() ) {
                qCritical() << "MainWindow: select failed!";
            }
        }
        updateView();
    } else
        qDebug() << "MainWindow: no item selected!";
}

