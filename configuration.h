#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <QString>

/*
 * Class implemented as a Singleton represents all configuration options
 * of the application.
 */
class Configuration
{

public:
    static Configuration* Instance();
    static QString getPathBaitImageFiles();
    static void setPathBaitImageFiles(QString path);
    static QString getPathFishImageFiles();
    static void setPathFishImageFiles(QString path);
    static QString getPathCatchImageFiles();
    static void setPathCatchImageFiles(QString path);
    static QString getPathLocationImageFiles();
    static void setPathLocationImageFiles(QString path);

protected:
    Configuration();

private:
    static Configuration* _instance;
    QString pathBaitImageFiles;
    QString pathFishImageFiles;
    QString pathCatchImageFiles;
    QString pathLocationImageFiles;

};

#endif // CONFIGURATION_H

