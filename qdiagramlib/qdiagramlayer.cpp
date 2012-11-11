#include "stdafx.h"
#include "qdiagramlayer.h"

QDiagramLayer::QDiagramLayer()
{
	m_parent = 0;
	m_locked = false;
	m_visible = true;
}

QDiagramLayer::QDiagramLayer(const QString & name, QDiagramLayers* parent)
{
	m_name = name;
	m_parent = parent;
	m_locked = false;
	m_visible = true;
}

QDiagramLayer::~QDiagramLayer()
{
}

void QDiagramLayer::add(QAbstractDiagramGraphicsItem* item)
{
	if (!m_items.contains(item)){
		m_items.append(item);
		item->setVisible(m_visible);
	}
}

bool QDiagramLayer::contains(QAbstractDiagramGraphicsItem* item) const
{
	return m_items.contains(item);
}

QString QDiagramLayer::name() const
{
	return m_name;
}

bool QDiagramLayer::isLocked() const
{
	return m_locked;
}

bool QDiagramLayer::isVisible() const
{
	return m_visible;
}

QList<QAbstractDiagramGraphicsItem*> QDiagramLayer::items() const
{
	return m_items;
}

void QDiagramLayer::remove(QAbstractDiagramGraphicsItem* item)
{
	m_items.removeAt(m_items.indexOf(item));
}

void QDiagramLayer::setLocked(bool locked)
{
	m_locked = locked;
}

void QDiagramLayer::setName(const QString & name)
{
	m_name = name;
}

void QDiagramLayer::setVisible(bool visible)
{
	if (visible != m_visible){
		m_visible = visible;
		Q_FOREACH(QAbstractDiagramGraphicsItem* i, m_items){
			i->setVisible(m_visible);
		}
	}
}
