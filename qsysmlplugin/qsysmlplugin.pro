#-------------------------------------------------
#
# Project created by QtCreator 2012-05-06T08:57:07
#
#-------------------------------------------------

QT       += script svg xml

TARGET = qsysmlplugin
TEMPLATE = lib

DEFINES += QSYSMLPLUGIN_LIBRARY

win32:CONFIG(release, debug|release): DLLDESTDIR = ../qdiagramdraw/debug/plugins/diagram
else:win32:CONFIG(debug, debug|release): DLLDESTDIR = ../qdiagramdraw/debug/plugins/diagram

SOURCES += qsysmlplugin.cpp \
    stdafx.cpp \
    qsysmlnode.cpp \
    qsysmlblock.cpp \
    qsysmlaction.cpp

HEADERS += qsysmlplugin.h\
        qsysmlplugin_global.h \
    stdafx.h \
    qsysmlnode.h \
    qsysmlblock.h \
    qsysmlaction.h

symbian {
    MMP_RULES += EXPORTUNFROZEN
    TARGET.UID3 = 0xED740FDD
    TARGET.CAPABILITY = 
    TARGET.EPOCALLOWDLLDATA = 1
    addFiles.sources = qsysmlplugin.dll
    addFiles.path = !:/sys/bin
    DEPLOYMENT += addFiles
}

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../qdiagramlib/release/ -lqdiagramlib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../qdiagramlib/debug/ -lqdiagramlib
else:symbian: LIBS += -lqdiagramlib
else:unix: LIBS += -L$$OUT_PWD/../qdiagramlib/ -lqdiagramlib

INCLUDEPATH += $$PWD/../qdiagramlib
DEPENDPATH += $$PWD/../qdiagramlib

RESOURCES += \
    qsysmlplugin.qrc
