#ifndef AUTOCOMPLETEMODEL_H
#define AUTOCOMPLETEMODEL_H

#include <QAbstractTableModel>

class AutoCompleteModel : public QAbstractTableModel
{
	Q_OBJECT
public:
	enum ObjectType {
		Object,
		Member,
		Function,
		Property
	};
	AutoCompleteModel(QObject* parent);
	~AutoCompleteModel();

	void addObject(const QString & name, const QString & toolTip = QString::null);

	void addObjectMethod(const QString & object, const QString & method, const QString & toolTip = QString::null);
	void addObjectProperty(const QString & object, const QString & property, const QString & toolTip = QString::null);

	int columnCount(const QModelIndex & parent = QModelIndex()) const;

	bool contains(const QString & name) const;

	QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;

	int rowCount(const QModelIndex & parent = QModelIndex()) const;

	void setObjectType(const QString & object, ObjectType type);

	QString toolTip(const QString & object, const QString & member = QString::null) const;
private:
	void addObjectMember(ObjectType type, const QString & object, const QString & name, const QString & toolTip);
	struct MemberData {
		QString toolTip;
		ObjectType type;
	};		
	struct ObjectData {
		QMap<QString,MemberData> members;
		ObjectType type;
		QString toolTip;
	};
	QMap<QString,ObjectData> m_objects;
	QString m_object;
	ObjectType m_type;
};

#endif // AUTOCOMPLETEMODEL_H
