#include "stdafx.h"
#include "qdiagramlayers.h"

QDiagramLayers::QDiagramLayers(QObject* parent) :
	QObject(parent)
{
	m_activeLayer = 0;
	m_layers << new QDiagramLayer("default", this);
}

QDiagramLayers::~QDiagramLayers()
{
}

QDiagramLayer* QDiagramLayers::activeLayer() const
{
	return m_layers.at(m_activeLayer);
}

QDiagramLayer* QDiagramLayers::add(const QString & name)
{
	Q_FOREACH(QDiagramLayer* l, m_layers){
		if (l->name() == name){
			return 0;
		}
	};
	QDiagramLayer* l = new QDiagramLayer(name, this);
	m_layers << l;

	emit layerAdded(name);

	return l;
}

int QDiagramLayers::count() const
{
	return m_layers.size();
}

int QDiagramLayers::indexOf(const QString & name) const
{
	for (int i = 0; i < m_layers.size(); i++){
		if (m_layers.at(i)->name() == name){
			return i;
		}
	}
	return -1;
}

bool QDiagramLayers::isLocked(QAbstractDiagramGraphicsItem* item) const
{
	Q_FOREACH(QDiagramLayer* l, m_layers){
		if (l->isLocked()){
			return true;
		}
	}
	return false;
}

QDiagramLayer* QDiagramLayers::layer(int index) const
{
	if (index < m_layers.size()){
		return m_layers.at(index);
	}
	return 0;
}

QDiagramLayer* QDiagramLayers::layer(const QString & name) const
{
	Q_FOREACH(QDiagramLayer* l, m_layers){
		if (l->name() == name){
			return l;
		}
	};
	return 0;
}

QList<QDiagramLayer*> QDiagramLayers::layers(QAbstractDiagramGraphicsItem* item) const
{
	QList<QDiagramLayer*> l;
	for (int i = 0; i < m_layers.size(); i++){
		if (m_layers.at(i)->contains(item)){
			l.append(m_layers.at(i));
		}
	}
	return l;
}

void QDiagramLayers::remove(QAbstractDiagramGraphicsItem* item)
{
	for (int i = 0; i < m_layers.size(); i++){
		m_layers.at(i)->remove(item);
	}
}

void QDiagramLayers::setActiveLayer(const QString & name)
{
	int i = indexOf(name);
	if (i != -1 && i != m_activeLayer){
		m_activeLayer = i;
		emit activeLayerChanged(name);
	}
}

void QDiagramLayers::setLocked(const QString & name, bool locked)
{
	QDiagramLayer* l = layer(name);
	if (l){
		if (l->isLocked() != locked){
			l->setLocked(locked);
			emit layerStateChanged(name, l->isVisible(), l->isLocked());
		}
	}
}

void QDiagramLayers::setVisible(const QString & name, bool visible)
{
	QDiagramLayer* l = layer(name);
	if (l){
		if (l->isVisible() != visible){
			l->setVisible(visible);
			emit layerStateChanged(name, l->isVisible(), l->isLocked());
		}
	}
}
