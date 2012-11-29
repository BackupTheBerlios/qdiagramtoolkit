#include "qdiagrampointerpushbuttonplugin.h"
#include <qdiagrampointerpushbutton.h>

#include <QtCore/QtPlugin>

QDiagramPointerPushButtonPlugin::QDiagramPointerPushButtonPlugin(QObject *parent)
    : QObject(parent)
{
    m_initialized = false;
}

void QDiagramPointerPushButtonPlugin::initialize(QDesignerFormEditorInterface * /* core */)
{
    if (m_initialized)
        return;
    
    // Add extension registrations, etc. here
    
    m_initialized = true;
}

bool QDiagramPointerPushButtonPlugin::isInitialized() const
{
    return m_initialized;
}

QWidget *QDiagramPointerPushButtonPlugin::createWidget(QWidget *parent)
{
    return new QDiagramPointerPushButton(parent);
}

QString QDiagramPointerPushButtonPlugin::name() const
{
    return QLatin1String("QDiagramPointerPushButton");
}

QString QDiagramPointerPushButtonPlugin::group() const
{
    return QLatin1String("QDiagramToolkit");
}

QIcon QDiagramPointerPushButtonPlugin::icon() const
{
    return QIcon(":/qdiagramdesignerplugin/qdiagrampointerpushbutton");
}

QString QDiagramPointerPushButtonPlugin::toolTip() const
{
    return QLatin1String("");
}

QString QDiagramPointerPushButtonPlugin::whatsThis() const
{
    return QLatin1String("");
}

bool QDiagramPointerPushButtonPlugin::isContainer() const
{
    return false;
}

QString QDiagramPointerPushButtonPlugin::domXml() const
{
    return QLatin1String("<widget class=\"QDiagramPointerPushButton\" name=\"pointerPushButton\">\n</widget>\n");
}

QString QDiagramPointerPushButtonPlugin::includeFile() const
{
    return QLatin1String("qdiagrampointerpushbutton.h");
}
