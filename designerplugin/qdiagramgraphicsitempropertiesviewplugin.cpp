#include "qdiagramgraphicsitempropertiesviewplugin.h"

#include <qdiagramgraphicsitempropertiesview.h>

QDiagramGraphicsItemPropertiesViewPlugin::QDiagramGraphicsItemPropertiesViewPlugin(QObject *parent)
	: QObject(parent)
{

}


void QDiagramGraphicsItemPropertiesViewPlugin::initialize(QDesignerFormEditorInterface * /* core */)
{
    if (m_initialized)
        return;
    
    // Add extension registrations, etc. here
    
    m_initialized = true;
}

bool QDiagramGraphicsItemPropertiesViewPlugin::isInitialized() const
{
    return m_initialized;
}

QWidget *QDiagramGraphicsItemPropertiesViewPlugin::createWidget(QWidget *parent)
{
    return new QDiagramGraphicsItemPropertiesView(parent);
}

QString QDiagramGraphicsItemPropertiesViewPlugin::name() const
{
    return QLatin1String("QDiagramGraphicsItemPropertiesView");
}

QString QDiagramGraphicsItemPropertiesViewPlugin::group() const
{
    return QLatin1String("QDiagramToolkit");
}

QIcon QDiagramGraphicsItemPropertiesViewPlugin::icon() const
{
    return QIcon(":/qdiagramdesignerplugin/qdiagramgraphicsitempropertiesview");
}

QString QDiagramGraphicsItemPropertiesViewPlugin::toolTip() const
{
    return QLatin1String("");
}

QString QDiagramGraphicsItemPropertiesViewPlugin::whatsThis() const
{
    return QLatin1String("");
}

bool QDiagramGraphicsItemPropertiesViewPlugin::isContainer() const
{
    return false;
}

QString QDiagramGraphicsItemPropertiesViewPlugin::domXml() const
{
    return QLatin1String("<widget class=\"QDiagramGraphicsItemPropertiesView\" name=\"propertiesView\">\n</widget>\n");
}

QString QDiagramGraphicsItemPropertiesViewPlugin::includeFile() const
{
    return QLatin1String("qdiagramgraphicsitempropertiesview.h");
}
