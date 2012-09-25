#-------------------------------------------------
#
# Project created by QtCreator 2011-09-06T20:06:33
#
#-------------------------------------------------

QT       += core gui sql

TARGET = qdiagramdraw
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    diagramwindow.cpp

HEADERS  += mainwindow.h \
    diagramwindow.h

FORMS    += mainwindow.ui \
    sysmldiagramwindow.ui \
    sysmldiagramtoolbox.ui \
    diagramwindow.ui

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../qdiagramlib/release/ -lqdiagramlib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../qdiagramlib/debug/ -lqdiagramlib
else:symbian: LIBS += -qdiagramlib
else:unix: LIBS += -L$$OUT_PWD/../qdiagramlib/ -qdiagramlib

INCLUDEPATH += $$PWD/../qdiagramlib
DEPENDPATH += $$PWD/../qdiagramlib

RESOURCES += \
    qdiagramdraw.qrc








win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../qdiagramlib/release/ -lqdiagramlib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../qdiagramlib/debug/ -lqdiagramlib
else:symbian: LIBS += -lqdiagramlib
else:unix: LIBS += -L$$OUT_PWD/../qdiagramlib/ -lqdiagramlib

INCLUDEPATH += $$PWD/../qdiagramlib
DEPENDPATH += $$PWD/../qdiagramlib








win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../qlogiccircuitsim/release/ -lqlogiccircuitsim
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../qlogiccircuitsim/debug/ -lqlogiccircuitsim
else:symbian: LIBS += -lqlogiccircuitsim
else:unix: LIBS += -L$$OUT_PWD/../qlogiccircuitsim/ -lqlogiccircuitsim

INCLUDEPATH += $$PWD/../qlogiccircuitsim
DEPENDPATH += $$PWD/../qlogiccircuitsim
