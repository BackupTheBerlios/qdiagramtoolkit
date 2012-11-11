#ifndef QDIAGRAMLAYER_H
#define QDIAGRAMLAYER_H

#include "qdiagramlib_global.h"

#include <qabstractdiagramgraphicsitem.h>

class QDiagramLayers;

class QDIAGRAMLIBSHARED_EXPORT QDiagramLayer
{
public:
	QDiagramLayer();
	~QDiagramLayer();

	void add(QAbstractDiagramGraphicsItem* item);

	bool contains(QAbstractDiagramGraphicsItem* item) const;

	bool isLocked() const;

	bool isVisible() const;

	QList<QAbstractDiagramGraphicsItem*> items() const;

	QString name() const;

	void remove(QAbstractDiagramGraphicsItem* item);

	void setLocked(bool locked);

	void setName(const QString & name);

	void setVisible(bool visible);
private:
	friend class QDiagramLayers;
	Q_DISABLE_COPY(QDiagramLayer);
	QDiagramLayer(const QString & name, QDiagramLayers* parent);

	QList<QAbstractDiagramGraphicsItem*> m_items;
	QString m_name;
	QDiagramLayers* m_parent;
	bool m_locked;
	bool m_visible;
};

Q_DECLARE_METATYPE(QDiagramLayer*)
Q_DECLARE_METATYPE(QList<QDiagramLayer*>)
#endif // QDIAGRAMLAYER_H
