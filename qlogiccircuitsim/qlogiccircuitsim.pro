#-------------------------------------------------
#
# Project created by QtCreator 2012-05-09T20:27:57
#
#-------------------------------------------------

QT       += script xml

TARGET = qlogiccircuitsim
TEMPLATE = lib

DEFINES += QLOGICCIRCUITSIM_LIBRARY

SOURCES += qlogiccircuitsim.cpp \
    stdafx.cpp \
    qlogiccircuitsimview.cpp \
    qlogiccircuitsimchannelhistorygraphicsview.cpp \
    qlogiccircuitsimdigitalchanneledit.cpp \
    qlogiccircuitsimengine.cpp \
    qlogiccircuitsimulator.cpp

HEADERS += qlogiccircuitsim.h\
        qlogiccircuitsim_global.h \
    stdafx.h \
    qlogiccircuitsimview.h \
    qlogiccircuitsimchannelhistorygraphicsview.h \
    qlogiccircuitsimdigitalchanneledit.h \
    qlogiccircuitsimengine.h \
    qlogiccircuitsimulator.h

symbian {
    MMP_RULES += EXPORTUNFROZEN
    TARGET.UID3 = 0xED27B76F
    TARGET.CAPABILITY = 
    TARGET.EPOCALLOWDLLDATA = 1
    addFiles.sources = qlogiccircuitsim.dll
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

FORMS += \
    qlogiccircuitsimview.ui \
    qlogiccircuitsimdigitalchanneledit.ui \
    qlogiccircuitsimulator.ui

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../qdiagramlib/release/ -lqdiagramlib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../qdiagramlib/debug/ -lqdiagramlib
else:symbian: LIBS += -lqdiagramlib
else:unix: LIBS += -L$$OUT_PWD/../qdiagramlib/ -lqdiagramlib

INCLUDEPATH += $$PWD/../qdiagramlib
DEPENDPATH += $$PWD/../qdiagramlib

RESOURCES += \
    qlogiccircuitsim.qrc
