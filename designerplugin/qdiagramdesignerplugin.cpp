#include "qdiagramdesignerplugin.h"

#include "qdiagrampointerpushbuttonplugin.h"
#include "qdiagramviewplugin.h"
#include "qdiagramviewcontrolpanelplugin.h"
#include "qdiagramviewzoomsliderplugin.h"
#include "qdiagramshapetoolboxplugin.h"
#include "qdiagramgraphicsitempropertiesviewplugin.h"

QDiagramDesignerPlugin::QDiagramDesignerPlugin(QObject *parent)
	: QObject(parent)
{

	m_widgets.append(new QDiagramPointerPushButtonPlugin(this));
	m_widgets.append(new QDiagramViewPlugin(this));
	m_widgets.append(new QDiagramViewControlPanelPlugin(this));
	m_widgets.append(new QDiagramViewZoomSliderPlugin(this));
	m_widgets.append(new QDiagramShapeToolBoxPlugin(this));
	m_widgets.append(new QDiagramGraphicsItemPropertiesViewPlugin(this));
}

 QList<QDesignerCustomWidgetInterface*> QDiagramDesignerPlugin::customWidgets() const
 {
     return m_widgets;
 }

 Q_EXPORT_PLUGIN2(qdiagramdesignerplugin,QDiagramDesignerPlugin)
 
 