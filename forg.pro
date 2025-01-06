#-------------------------------------------------
#
# Project created by QtCreator 2015-03-30T19:19:00
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = forg
TEMPLATE = app

RESOURCES = forg.qrc

win32:RC_ICONS += forg.ico

SOURCES += main.cpp\
    catchesdelegate.cpp \
        mainwindow.cpp \
    editbaitdialog.cpp \
    viewbaitdialog.cpp \
    baitmodel.cpp \
    configurationdialog.cpp \
    configuration.cpp \
    viewfishdialog.cpp \
    fishmodel.cpp \
    editfishdialog.cpp \
    locationmodel.cpp \
    viewlocationdialog.cpp \
    imagedelegate.cpp \
    editlocationdialog.cpp \
    sessionmodel.cpp \
    sessionproxymodel.cpp \
    datetimedelegate.cpp \
    editsessiondialog.cpp \
    viewcatchdialog.cpp \
    catchmodel.cpp \
    editcatchdialog.cpp \
	analysisyeardialog.cpp \
    analysisresultsdialog.cpp \
	catchresult.cpp \
    listwidgetiditem.cpp \
    selectionwatersdialog.cpp \
    analysiszonaldistributiondialog.cpp

HEADERS  += mainwindow.h \
    catchesdelegate.h \
    editbaitdialog.h \
    generalconstants.h \
    viewbaitdialog.h \
    baitmodel.h \
    configurationdialog.h \
    configuration.h \
    viewfishdialog.h \
    fishmodel.h \
    editfishdialog.h \
    locationmodel.h \
    viewlocationdialog.h \
    imagedelegate.h \
    editlocationdialog.h \
    sessionmodel.h \
    sessionproxymodel.h \
    datetimedelegate.h \
    editsessiondialog.h \
    viewcatchdialog.h \
    catchmodel.h \
    editcatchdialog.h \
	analysisyeardialog.h \
    analysisresultsdialog.h \
	catchresult.h \
    listwidgetiditem.h \
    selectionwatersdialog.h \
    analysiszonaldistributiondialog.h

FORMS    += mainwindow.ui \
    editbaitdialog.ui \
    viewbaitdialog.ui \
    configurationdialog.ui \
    viewfishdialog.ui \
    editfishdialog.ui \
    viewlocationdialog.ui \
    editlocationdialog.ui \
    editsessiondialog.ui \
    viewcatchdialog.ui \
    editcatchdialog.ui \
    analysisyeardialog.ui \
    analysisresultsdialog.ui \
    selectionwatersdialog.ui \
    analysiszonaldistributiondialog.ui
