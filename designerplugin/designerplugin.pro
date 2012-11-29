CONFIG      += designer plugin debug_and_release
TARGET      = $$qtLibraryTarget(qdiagramplugin)
TEMPLATE    = lib

HEADERS     = qdiagramviewplugin.h
SOURCES     = qdiagramviewplugin.cpp
RESOURCES   = designerplugin.qrc
LIBS        += -L. 

target.path = $$[QT_INSTALL_PLUGINS]/designer
INSTALLS    += target

include(qdiagramview.pri)
