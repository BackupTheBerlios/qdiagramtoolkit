#include "qdiagramshapetoolboxplugin.h"

#include <qdiagramshapetoolbox.h>

QDiagramShapeToolBoxPlugin::QDiagramShapeToolBoxPlugin(QObject *parent)
	: QObject(parent)
{

}

void QDiagramShapeToolBoxPlugin::initialize(QDesignerFormEditorInterface * /* core */)
{
    if (m_initialized)
        return;
    
    // Add extension registrations, etc. here
    
    m_initialized = true;
}

bool QDiagramShapeToolBoxPlugin::isInitialized() const
{
    return m_initialized;
}

QWidget *QDiagramShapeToolBoxPlugin::createWidget(QWidget *parent)
{
    return new QDiagramShapeToolBox(parent);
}

QString QDiagramShapeToolBoxPlugin::name() const
{
    return QLatin1String("QDiagramShapeToolBox");
}

QString QDiagramShapeToolBoxPlugin::group() const
{
    return QLatin1String("QDiagram");
}

QIcon QDiagramShapeToolBoxPlugin::icon() const
{
    return QIcon(":/qdiagramdesignerplugin/qdiagramshapetoolboxplugin");
}

QString QDiagramShapeToolBoxPlugin::toolTip() const
{
    return QLatin1String("");
}

QString QDiagramShapeToolBoxPlugin::whatsThis() const
{
    return QLatin1String("");
}

bool QDiagramShapeToolBoxPlugin::isContainer() const
{
    return false;
}

QString QDiagramShapeToolBoxPlugin::domXml() const
{
    return QLatin1String("<widget class=\"QDiagramShapeToolBox\" name=\"toolBox\">\n</widget>\n");
}

QString QDiagramShapeToolBoxPlugin::includeFile() const
{
    return QLatin1String("qdiagramshapetoolbox.h");
}
