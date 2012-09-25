#-------------------------------------------------
#
# Project created by QtCreator 2011-11-19T20:13:04
#
#-------------------------------------------------

TARGET = qlogiccircuitplugin
TEMPLATE = lib
CONFIG   += precompile_header
PRECOMPILED_HEADER = stdafx.h

DEFINES += QLOGICCIRCUITPLUGIN_LIBRARY

DLLDESTDIR = ../qdiagramdraw/debug/plugins/diagram

SOURCES += qlogiccircuit.cpp \
    stdafx.cpp \
    qlogiccircuitplugin.cpp \
    qlogiccircuitshapeconnector.cpp \
    qlogiccircuitinputshape.cpp \
    qlogiccircuitoutputshape.cpp \
    qlogiccircuitfunctionshape.cpp \
    qlogiccircuitgateshape.cpp \
    qlogiccircuitflipflopshape.cpp \
    qlogiccircuitdiagram.cpp

HEADERS += qlogiccircuit.h\
        qlogiccircuitplugin_global.h \
    stdafx.h \
    qlogiccircuitplugin.h \
    qlogiccircuitshapeconnector.h \
    qlogiccircuitinputshape.h \
    qlogiccircuitinputshape_p.h \
    qlogiccircuitoutputshape.h \
    qlogiccircuitoutputshape_p.h \
    qlogiccircuitfunctionshape.h \
    qlogiccircuitgateshape.h \
    qlogiccircuitgateshape_p.h \
    qlogiccircuitfunctionshape_p.h \
    qlogiccircuitflipflopshape.h \
    qlogiccircuitdiagram.h

symbian {
    MMP_RULES += EXPORTUNFROZEN
    TARGET.UID3 = 0xEF06FE48
    TARGET.CAPABILITY = 
    TARGET.EPOCALLOWDLLDATA = 1
    addFiles.sources = qlogiccircuitplugin.dll
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

RESOURCES += \
    qlogiccircuitplugin.qrc




win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../qdiagramlib/release/ -lqdiagramlib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../qdiagramlib/debug/ -lqdiagramlib
else:symbian: LIBS += -lqdiagramlib
else:unix: LIBS += -L$$OUT_PWD/../qdiagramlib/ -lqdiagramlib

INCLUDEPATH += $$PWD/../qdiagramlib
DEPENDPATH += $$PWD/../qdiagramlib

OTHER_FILES +=

FORMS +=















