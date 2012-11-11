#-------------------------------------------------
#
# Project created by QtCreator 2011-10-23T10:13:59
#
#-------------------------------------------------

QT       += sql
CONFIG   += precompile_header
PRECOMPILED_HEADER = stdafx.h

TARGET = qdiagramlib
TEMPLATE = lib

DLLDESTDIR = ../qdiagramdraw/debug

DEFINES += QDIAGRAMLIB_LIBRARY

SOURCES += qdiagram.cpp \
    qdiagramgraphicsview.cpp \
    qdiagramgraphicsscene.cpp \
    qdiagramview.cpp \
    qdiagramundocommand.cpp \
    qdiagramgraphicsitempropertiesview.cpp \
    qabstractdiagramgraphicsitem.cpp \
    qabstractdiagramshape.cpp \
    qabstractdiagramshapeconnectionpoint.cpp \
    qdiagramshapeconnectionpoint.cpp \
    qabstractdiagramshapeconnector.cpp \
    qdiagramshapeconnector.cpp \
    qabstractdiagram.cpp \
    qabstractdiagramscene.cpp \
    qdiagramshape.cpp \
    stdafx.cpp \
    qdiagramshapetoolbox.cpp \
    qdiagramshapelistwidget.cpp \
    qdiagrampluginloader.cpp \
    qdiagrammoveshapecommand.cpp \
    qdiagraminsertshapecommand.cpp \
    qdiagramconnectorstyle.cpp \
    qabstractdiagramplugin.cpp \
    qdiagramshapestyle.cpp \
    qdiagramgraphicsitemmetadata.cpp \
    qdiagrammetaproperty.h.cpp \
    qdiagramshapesizegrip.cpp \
    qdiagramlinestyle.cpp \
    qdiagramlineitempositionhandle.cpp \
    qabstractdiagramgraphicsitemhandle.cpp \
    qdiagramtextstyle.cpp \
    qdiagramstyle.cpp \
    json.cpp \
    qdiagramendoflinestyle.cpp \
    qdiagramgraphicsitemmetaenum.cpp \
    qdiagramgraphicsitemmetaflag.cpp \
    qdiagramiohandler.cpp \
    qdiagramreader.cpp \
    qdiagramwriter.cpp \
    qdiagramioplugin.cpp

HEADERS += qdiagram.h\
        qdiagramlib_global.h \
    qdiagramgraphicsview.h \
    qdiagramgraphicsscene.h \
    qdiagramview.h \
    qdiagramundocommand.h \
    qdiagram_p.h \
    qdiagramgraphicsitempropertiesview.h \
    qdiagramgraphicsitem_p.h \
    qabstractdiagramgraphicsitem.h \
    qabstractdiagramshape.h \
    qabstractdiagramshapeconnectionpoint.h \
    qdiagramshapeconnectionpoint.h \
    qabstractdiagramshapeconnector.h \
    qdiagramshapeconnector.h \
    qabstractdiagram.h \
    qabstractdiagramscene.h \
    qdiagramshape.h \
    stdafx.h \
    qdiagramshapetoolbox.h \
    qdiagramshapelistwidget.h \
    qdiagrampluginloader.h \
    qdiagrammoveshapecommand.h \
    qdiagraminsertshapecommand.h \
    qdiagramconnectorstyle.h \
    qabstractdiagramplugin.h \
    qdiagramshapestyle.h \
    qdiagrammetadata.h \
    qdiagrammetaproperty.h.h \
    qdiagramgraphicsitempropertiesview_p.h \
    qdiagramshapesizegrip.h \
    qdiagramlinestyle.h \
    qdiagramlineitempositionhandle.h \
    qabstractdiagramgraphicsitemhandle.h \
    qdiagramtextstyle.h \
    qdiagramstyle.h \
    json.h \
    qdiagramendoflinestyle.h \
    qdiagramgraphicsitemmetaenum.h \
    qdiagramgraphicsitemmetaflag.h \
    qdiagramiohandler.h \
    qdiagramreader.h \
    qdiagramwriter.h \
    qdiagramioplugin.h

symbian {
    MMP_RULES += EXPORTUNFROZEN
    TARGET.UID3 = 0xEF839C72
    TARGET.CAPABILITY = 
    TARGET.EPOCALLOWDLLDATA = 1
    addFiles.sources = qdiagramlib.dll
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
    qdiagramview.ui \
    qdiagramgraphicsitempropertiesview.ui \
    qdiagramshapetoolbox.ui

RESOURCES += \
    qdiagramlib.qrc



























































































































