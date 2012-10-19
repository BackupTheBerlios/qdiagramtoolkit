#include "qdiagramdesignerplugin.h"

#include "qdiagramviewplugin.h"
#include "qdiagramshapetoolboxplugin.h"
#include "qdiagramgraphicsitempropertiesviewplugin.h"

QDiagramDesignerPlugin::QDiagramDesignerPlugin(QObject *parent)
	: QObject(parent)
{

	m_widgets.append(new QDiagramViewPlugin(this));
	m_widgets.append(new QDiagramShapeToolBoxPlugin(this));
	m_widgets.append(new QDiagramGraphicsItemPropertiesViewPlugin(this));
}

 QList<QDesignerCustomWidgetInterface*> QDiagramDesignerPlugin::customWidgets() const
 {
     return m_widgets;
 }

 Q_EXPORT_PLUGIN2(qdiagramdesignerplugin,QDiagramDesignerPlugin)
 
 