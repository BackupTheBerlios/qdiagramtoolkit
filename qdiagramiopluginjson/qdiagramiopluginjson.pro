#-------------------------------------------------
#
# Project created by QtCreator 2012-05-26T16:43:51
#
#-------------------------------------------------

TARGET = qdiagramiojson
TEMPLATE = lib

DLLDESTDIR = ../qdiagramdraw/debug/plugins/diagram/formats

SOURCES += \
    stdafx.cpp \
    json.cpp \
    jsonformatiohandler.cpp \
    jsonformatioplugin.cpp

HEADERS +=\
    stdafx.h \
    json.h \
    jsonformatiohandler.h \
    jsonformatioplugin.h

symbian {
    MMP_RULES += EXPORTUNFROZEN
    TARGET.UID3 = 0xED0A025A
    TARGET.CAPABILITY = 
    TARGET.EPOCALLOWDLLDATA = 1
    addFiles.sources = qjsonformat.dll
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
