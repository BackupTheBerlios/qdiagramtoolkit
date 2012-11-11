#ifndef QDIAGRAMLAYERSVIEW_H
#define QDIAGRAMLAYERSVIEW_H

#include <QWidget>
#include <QPointer>

#include "qdiagramlib_global.h"

class QAbstractDiagram;

namespace Ui {class QDiagramLayersView;};

class QDiagramLayersModel : public QAbstractTableModel
{
	Q_OBJECT
public:
	QDiagramLayersModel(QObject* parent = 0);
	~QDiagramLayersModel();

	int columnCount(const QModelIndex & parent = QModelIndex()) const;

	QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;

	Qt::ItemFlags flags(const QModelIndex & index) const;

	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

	int rowCount(const QModelIndex & parent = QModelIndex()) const;

	bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);

	void setDiagram(QAbstractDiagram* diagram);
private:
	QPointer<QAbstractDiagram> m_diagram;
};

class QDIAGRAMLIBSHARED_EXPORT QDiagramLayersView : public QWidget
{
	Q_OBJECT

public:
	QDiagramLayersView(QWidget *parent = 0);
	~QDiagramLayersView();

	void setDiagram(QAbstractDiagram* diagram);
private:
	QDiagramLayersModel* m_model;
	Ui::QDiagramLayersView *ui;
};


#endif // QDIAGRAMLAYERSVIEW_H
