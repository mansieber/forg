#ifndef CONFIGURATIONDIALOG_H
#define CONFIGURATIONDIALOG_H

#include <QDialog>

#include "ui_configurationdialog.h"

/*
 * Class represents a dialog which allows to edit settings of the application.
 */
class ConfigurationDialog : public QDialog, public Ui::configurationDialog
{
    Q_OBJECT

public:
    ConfigurationDialog(QWidget *parent = 0);

private slots:
    void onButtonBoxAccepted();

};

#endif // CONFIGURATIONDIALOG_H

