#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#define NAME_COMPANY "arkton.de"
#define NAME_PRODUCT "forg"
#define TYPE_DATABASE "QSQLITE"

#include <QMainWindow>
#include <QSqlDatabase>

#include "sessionmodel.h"
#include "sessionproxymodel.h"
#include "editbaitdialog.h"
#include "viewbaitdialog.h"
#include "editfishdialog.h"
#include "viewfishdialog.h"
#include "viewlocationdialog.h"
#include "editcatchdialog.h"
#include "viewcatchdialog.h"
#include "editsessiondialog.h"
#include "editlocationdialog.h"
#include "configurationdialog.h"
#include "analysisyeardialog.h"
#include "selectionwatersdialog.h"
#include "datetimedelegate.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void openDatabase();

private slots:
    void onConnectDatabaseTriggered();
    void onQuitTriggered();
    void onAddBaitTriggered();
    void onViewBaitTriggered();
    void onAddFishTriggered();
    void onViewFishesTriggered();
    void onAddLocationTriggered();
    void onViewLocationsTriggered();
    void onAddCatchTriggered();
    void onViewCatchesTriggered();
    void onConfigurationTriggered();
    void onAnalysisYearTriggered();
    void onAnalysisWatersTriggered();
    void onSelectionWatersTriggered();
    void onConsistencyCheckTriggered();
    void onAboutTriggered();
    void addSession();
    void editSession();
    void editSession(QModelIndex index);
    void deleteSession();

private:
    Ui::MainWindow *ui;
    EditBaitDialog *editBaitDialog;
    ViewBaitDialog *viewBaitDialog;
    EditFishDialog *editFishDialog;
    ViewFishDialog *viewFishDialog;
    EditLocationDialog *editLocationDialog;
    ViewLocationDialog *viewLocationDialog;
    EditCatchDialog *editCatchDialog;
    ViewCatchDialog *viewCatchDialog;
    EditSessionDialog *editSessionDialog;
    ConfigurationDialog *configurationDialog;
    AnalysisYearDialog *analysisYearDialog;
    SelectionWatersDialog *selectionWatersDialog;
    QString pathDatabase;
    DateTimeDelegate *delegateStartTime;
    QSqlDatabase db;
    SessionModel *sessionModel;
    SessionProxyModel *sessionProxyModel;

    void writeSettings();
    void readSettings();
    void updateView();
};

#endif // MAINWINDOW_H
