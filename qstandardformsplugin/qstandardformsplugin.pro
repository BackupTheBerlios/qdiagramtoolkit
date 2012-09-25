#-------------------------------------------------
#
# Project created by QtCreator 2012-03-25T12:04:57
#
#-------------------------------------------------

QT       += svg

TARGET = qstandardformsplugin
TEMPLATE = lib

DEFINES += QSTANDARDFORMSPLUGIN_LIBRARY

DLLDESTDIR = ../qdiagramdraw/debug/plugins/diagram

SOURCES += qstandardformsplugin.cpp \
    stdafx.cpp \
    qstandardblockshape.cpp \
    qstandardline.cpp \
    qstandardshapeconnector.cpp

HEADERS += qstandardformsplugin.h\
        qstandardformsplugin_global.h \
    stdafx.h \
    qstandardblockshape.h \
    qstandardblockshape_p.h \
    qstandardline.h \
    qstandardshapeconnector.h

symbian {
    MMP_RULES += EXPORTUNFROZEN
    TARGET.UID3 = 0xEDA66056
    TARGET.CAPABILITY = 
    TARGET.EPOCALLOWDLLDATA = 1
    addFiles.sources = qstandardformsplugin.dll
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
    qstandardformsplugin.qrc
