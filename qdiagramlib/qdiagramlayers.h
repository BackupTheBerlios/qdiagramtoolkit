#ifndef QDIAGRAMLAYERS_H
#define QDIAGRAMLAYERS_H

#include "qdiagramlib_global.h"

#include "qdiagramlayer.h"

class QDIAGRAMLIBSHARED_EXPORT QDiagramLayers : public QObject
{
	Q_OBJECT
public:
	QDiagramLayers(QObject* parent = 0);
	/**
	 * Destroys the layer.
	 */
	~QDiagramLayers();
	/**
	 * Returns the active layer.
	 */
	QDiagramLayer* activeLayer() const;
	/**
	 * Adds a new layer with the given @p name to the list of layers.
	 */
	QDiagramLayer* add(const QString & name);
	/**
	 * Returns the number of layers.
	 */
	int count() const;
	/**
	 * Searches the layer @p name. Returns the index of @p name or -1 if @p name is not found.
	 */
	int indexOf(const QString & name) const;
	/**
	 * Returns true if the diagram graphics item @p item belongs to a locked layer. Otherwise false.
	 */
	bool isLocked(QAbstractDiagramGraphicsItem* item) const;
	/**
	 * Returns the layer at the given @p index.
	 */
	QDiagramLayer* layer(int index) const;
	/**
	 * Returns the layer specified by the given @p name or 0 if @p name is not found.
	 */
	QDiagramLayer* layer(const QString & name) const;

	QList<QDiagramLayer*> layers(QAbstractDiagramGraphicsItem* item) const;

	void remove(QAbstractDiagramGraphicsItem* item);
	/**
	 * Sets the active layer to @p name.
	 * @see activeLayer()
	 */
	void setActiveLayer(const QString & name);
	/**
	 * Locks the layer specified by the given @p name.
	 */
	void setLocked(const QString & name, bool locked);
	/**
	 * Shows or hides the layer specified by the given @p name.
	 */
	void setVisible(const QString & name, bool visible);
signals:
	/**
	 * This signal is emitted when the active layer has changed. The name of the new active layer is passed in @p name.
	 */
	void activeLayerChanged(const QString & name);
	void layerAdded(const QString & name);
	/**
	 * This signal is emitted when the state (visible and/or locked) of a layer has changed. The layer's name is passed in @p name.
	 */
	void layerStateChanged(const QString & name, bool visible, bool locked);
	void layerRemoved(const QString & name);
private:
	int m_activeLayer;
	QList<QDiagramLayer*> m_layers;
};

Q_DECLARE_METATYPE(QDiagramLayers*)

#endif // QDIAGRAMLAYERS_H
