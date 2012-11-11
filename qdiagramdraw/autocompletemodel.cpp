#include "autocompletemodel.h"

#include <QIcon>

AutoCompleteModel::AutoCompleteModel(QObject* parent) :
QAbstractTableModel(parent)
{
	m_type = Object;
}

AutoCompleteModel::~AutoCompleteModel()
{
}

void AutoCompleteModel::addObject(const QString & name, const QString & toolTip)
{
	if (!m_objects.contains(name)){
		ObjectData d;
		d.toolTip = toolTip;
		m_objects[name] = d;
	}
}

void AutoCompleteModel::addObjectMember(ObjectType type, const QString & object, const QString & name, const QString & toolTip)
{
	if (!m_objects.contains(object)){
		ObjectData o;
		MemberData m;
		m.type = type;
		m.toolTip = toolTip;
		o.members[name] = m;
		m_objects[object] = o;
	} else {
		if (!m_objects.value(object).members.contains(name)){
			MemberData m;
			m.toolTip = toolTip;
			m.type = type;
			m_objects[object].members[name] = m;
		}
	}
}

void AutoCompleteModel::addObjectMethod(const QString & object, const QString & method, const QString & toolTip)
{
	addObjectMember(Function, object, method, toolTip);
}

void AutoCompleteModel::addObjectProperty(const QString & object, const QString & property, const QString & toolTip)
{
	addObjectMember(Property, object, property, toolTip);
}

int AutoCompleteModel::columnCount(const QModelIndex & parent) const
{
	Q_UNUSED(parent)
	return 1;
}

bool AutoCompleteModel::contains(const QString & name) const
{
	if (m_type == Object){
		return m_objects.contains(name);
	}
	return false;
}

QVariant AutoCompleteModel::data(const QModelIndex & index, int role) const
{
	if (role == Qt::DecorationRole){
		if (m_type == Object){
			return QIcon(":/qdiagramdraw/autocomplete/object");
		} else if (m_type == Member){
			MemberData m = m_objects.value(m_object).members.values().at(index.row());
			if (m.type == Function){
				return QIcon(":/qdiagramdraw/autocomplete/object.method");
			} else if (m.type == Property){
				return QIcon(":/qdiagramdraw/autocomplete/object.property");
			}
		}
	} else if (role == Qt::DisplayRole){
		if (m_type == Object){
			return m_objects.keys().at(index.row());
		} else if (m_type == Member){
			return m_objects.value(m_object).members.keys().at(index.row());
		}
	} else if (role == Qt::EditRole){
		if (m_type == Object){
			return m_objects.keys().at(index.row());
		} else if (m_type == Member){
			return m_objects.value(m_object).members.keys().at(index.row());
		}
	} else if (role == Qt::ToolTipRole){
		if (m_type == Object){
			return m_objects.values().at(index.row()).toolTip;
		} else if (m_type == Member){
			return m_objects.value(m_object).members.values().at(index.row()).toolTip;
		}
	}
	return QVariant();
}

int AutoCompleteModel::rowCount(const QModelIndex & parent) const
{
	Q_UNUSED(parent)
	if (m_type == Object){
		return m_objects.keys().size();
	} else if (m_type == Member){
		if (m_objects.contains(m_object)){
			return m_objects.value(m_object).members.size();
		}
	}
	return 0;
}

void AutoCompleteModel::setObjectType(const QString & object, AutoCompleteModel::ObjectType type)
{
	m_object = object;
	m_type = type;
}

QString AutoCompleteModel::toolTip(const QString & object, const QString & member) const
{
	ObjectData o = m_objects.value(object);
	if (!member.isEmpty()){
		MemberData m = o.members.value(member);
		return m.toolTip;
	}
	return o.toolTip;
}
