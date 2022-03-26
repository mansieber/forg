#include "configurationdialog.h"
#include "ui_configurationdialog.h"
#include "configuration.h"

#include <QtDebug>

/*
 * Constructor
 */
ConfigurationDialog::ConfigurationDialog(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);
    editBaitImagePath->setText(Configuration::getPathBaitImageFiles());
    editFishImagePath->setText(Configuration::getPathFishImageFiles());
    editCatchImagePath->setText(Configuration::getPathCatchImageFiles());
    editLocationImagePath->setText(Configuration::getPathLocationImageFiles());

    connect(dialogButtonBox, SIGNAL(accepted()), this, SLOT(onButtonBoxAccepted()));

    qDebug() << "ConfigurationDialog: new configuration dialog created";
}

/*
 * Method handles the acceptance Button. The database shall be updated.
 */
void ConfigurationDialog::onButtonBoxAccepted()
{
    Configuration::setPathBaitImageFiles(editBaitImagePath->text());
    Configuration::setPathFishImageFiles(editFishImagePath->text());
    Configuration::setPathCatchImageFiles(editCatchImagePath->text());
    Configuration::setPathLocationImageFiles(editLocationImagePath->text());
    qDebug() << "ConfigurationDialog: pathBaitImageFiles set to: " << Configuration::getPathBaitImageFiles();

    this->close();
}


