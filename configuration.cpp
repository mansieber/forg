#include "configuration.h"

#include <QString>
#include <QtDebug>

Configuration* Configuration::_instance = 0;

/*
 * Constructor is protected. Never used from external code.
 */
Configuration::Configuration()
{
}

/*
 * Method creates exactly one instance of the class.
 */
Configuration* Configuration::Instance() {
    if ( _instance == 0 ) {
        _instance = new Configuration();
    }
    return _instance;
}

/*
 * Get the path to bait image files.
 */
QString Configuration::getPathBaitImageFiles() {
    return _instance->pathBaitImageFiles;
}

/*
 * Set the path to bait image files.
 */
void Configuration::setPathBaitImageFiles(QString path) {
    _instance->pathBaitImageFiles = path;
}

/*
 * Get the path to fish image files.
 */
QString Configuration::getPathFishImageFiles() {
    return _instance->pathFishImageFiles;
}

/*
 * Set the path to fish image files.
 */
void Configuration::setPathFishImageFiles(QString path) {
    _instance->pathFishImageFiles = path;
}

/*
 * Get the path to catch image files.
 */
QString Configuration::getPathCatchImageFiles() {
    return _instance->pathCatchImageFiles;
}

/*
 * Set the path to catch image files.
 */
void Configuration::setPathCatchImageFiles(QString path) {
    _instance->pathCatchImageFiles = path;
}

/*
 * Get the path to location image files.
 */
QString Configuration::getPathLocationImageFiles() {
    return _instance->pathLocationImageFiles;
}

/*
 * Set the path to location image files.
 */
void Configuration::setPathLocationImageFiles(QString path) {
    _instance->pathLocationImageFiles = path;
}

