#include "StdAfx.h"
#include "qdiagramlayersview.h"
#include "ui_qdiagramlayersview.h"

#include <qabstractdiagram.h>

QDiagramLayersModel::QDiagramLayersModel(QObject* parent) :
QAbstractTableModel(parent)
{
	m_diagram = 0;
}

QDiagramLayersModel::~QDiagramLayersModel()
{
}

int QDiagramLayersModel::columnCount(const QModelIndex & parent) const
{
	Q_UNUSED(parent);
	return 5;
}

QVariant QDiagramLayersModel::data(const QModelIndex & index, int role) const
{
	if (m_diagram == 0 || !index.isValid()){
		return QVariant();
	}
	QDiagramLayer* l = m_diagram->layers()->layer(index.row());
	if (role == Qt::CheckStateRole){
		if (index.column() == 2){
			return l->isVisible()?Qt::Checked:Qt::Unchecked;
		} else if (index.column() == 3){
			return l == m_diagram->layers()->activeLayer()?Qt::Checked:Qt::Unchecked;
		} else if (index.column() == 4){
			return l->isLocked()?Qt::Checked:Qt::Unchecked;
		}
	} else if (role == Qt::EditRole){
		if (index.column() == 0){
			return l->name();
		}
	} else if (role == Qt::DisplayRole){
		if (index.column() == 0){
			return l->name();
		} else if (index.column() == 1){
			return l->items().size();
		}
	}
	return QVariant();
}

Qt::ItemFlags QDiagramLayersModel::flags(const QModelIndex & index) const
{
	Qt::ItemFlags f;
	if (!index.isValid()){
		return 0;
	}
	f |= Qt::ItemIsEnabled;
	f |= Qt::ItemIsSelectable;

	if (index.column() == 0){
		f |= Qt::ItemIsEditable;
	} else if (index.column() == 2){
		f |= Qt::ItemIsEditable;
		f |= Qt::ItemIsUserCheckable;
	} else if (index.column() == 3){
		f |= Qt::ItemIsEditable;
		f |= Qt::ItemIsUserCheckable;
	} else if (index.column() == 4){
		f |= Qt::ItemIsEditable;
		f |= Qt::ItemIsUserCheckable;
	}
	return f;
}

QVariant QDiagramLayersModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (orientation == Qt::Horizontal && role == Qt::DisplayRole){
		if (section == 0){
			return tr("Name");
		}
		if (section == 1){
			return tr("#");
		}
		if (section == 2){
			return tr("Visible");
		}
		if (section == 3){
			return tr("Active");
		}
		if (section == 4){
			return tr("Locked");
		}
	}
	return QAbstractTableModel::headerData(section, orientation, role);
}

int QDiagramLayersModel::rowCount(const QModelIndex & parent) const
{
	Q_UNUSED(parent);
	if (m_diagram){
		return m_diagram->layers()->count();
	}
	return 0;
}

bool QDiagramLayersModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
	if (m_diagram == 0 || !index.isValid()){
		return false;
	}
	QDiagramLayer* l = m_diagram->layers()->layer(index.row());
	if (role == Qt::CheckStateRole){
		if (index.column() == 2){
			m_diagram->layers()->setVisible(l->name(), value.toInt() == Qt::Checked);
			return true;
		} else if (index.column() == 3){
			if (value.toInt() == Qt::Checked){
				m_diagram->layers()->setActiveLayer(l->name());
			}
		} else if (index.column() == 4){
			m_diagram->layers()->setLocked(l->name(), value.toInt() == Qt::Checked);
			return true;
		}
	} else if (role == Qt::EditRole){
		if (index.column() == 0){
			l->setName(value.toString());
			return true;
		}
	}
	return false;
}

void QDiagramLayersModel::setDiagram(QAbstractDiagram* diagram)
{
	beginResetModel();
	m_diagram = diagram;
	endResetModel();
}

QDiagramLayersView::QDiagramLayersView(QWidget *parent)
	: QWidget(parent)
{
	ui = new Ui::QDiagramLayersView();
	ui->setupUi(this);

	m_model = new QDiagramLayersModel(this);
	ui->tableView->setModel(m_model);
}

QDiagramLayersView::~QDiagramLayersView()
{
	delete ui;
}

void QDiagramLayersView::setDiagram(QAbstractDiagram* diagram)
{
	m_model->setDiagram(diagram);
	ui->tableView->resizeRowsToContents();
	for (int i = 0; i < m_model->columnCount() - 1; i++){
		ui->tableView->resizeColumnToContents(i);
	}
}

