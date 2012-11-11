#include "stdafx.h"
#include "qdiagramstylesheet.h"

#include "qdiagrampluginloader.h"

QDiagramStyleSheet::QDiagramStyleSheet(QDiagram* diagram)
{
	m_diagram = diagram;
}

QDiagramStyleSheet::~QDiagramStyleSheet()
{
}

QDiagramConnectorStyle QDiagramStyleSheet::connector(const QString & plugin, const QString & name) const
{
	QAbstractDiagramPlugin* p = QDiagramPluginLoader::plugin(plugin);
	Q_FOREACH(QDiagramConnectorStyle s, p->connectors()){
		if (s.name() == name){
			return s;
		}
	}
	return QDiagramConnectorStyle();
}

QList<QDiagramConnectorStyle> QDiagramStyleSheet::connectors() const
{
	QList<QDiagramConnectorStyle> l;
	if (m_diagram){
		Q_FOREACH(QString n, m_diagram->plugins()){
			QAbstractDiagramPlugin* p = QDiagramPluginLoader::plugin(n);
			if (p){
				l += p->connectors();
			}
		}
	}
	return l;
}
