#ifndef QDIAGRAMGRAPHICSITEMPROPERTIESVIEW_P_H
#define QDIAGRAMGRAPHICSITEMPROPERTIESVIEW_P_H

#include "qabstractdiagramgraphicsitem.h"
#include "qdiagrammetaproperty.h"
#include "qdiagramproperty.h"

class QAbstractDiagramGraphicsItem;
class QDiagramGraphicsItemDelegate;
class QDiagramEndOfLineStyle;

class QPropertiesModelItem
{
public:
	enum ItemType {
		Flag,
		MaskedFlag,
		Property
	};
    QPropertiesModelItem(QAbstractDiagramGraphicsItem* graphicsItem);
    QPropertiesModelItem(QAbstractDiagramGraphicsItem* graphicsItem, int index, QPropertiesModelItem* parent = 0);
    QPropertiesModelItem(const QString & name, QPropertiesModelItem* parent);
    QPropertiesModelItem(const QString & name, int flag, QPropertiesModelItem* parent);
	QPropertiesModelItem(const QString & name, QDiagramToolkit::PropertyType type, QPropertiesModelItem* parent);

	QPropertiesModelItem(const QString & name, const QString & mask, const QPair<QString,int> & maskedFlag, QPropertiesModelItem* parent);

    ~QPropertiesModelItem();

    QPropertiesModelItem* child(int index) const;

    int childCount() const;

    QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem & option, const QModelIndex & index, const QStyledItemDelegate* receiver) const;

    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;

	QDiagramMetaEnum metaEnumeration() const;

	QDiagramMetaFlag metaFlag() const;

	QDiagramMetaProperty metaProperty() const;

    Qt::ItemFlags flags() const;

    int index() const;

	bool isChild() const;

    QAbstractDiagramGraphicsItem* graphicsItem() const;

    QString name() const;

    bool paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index, const QStyleOptionViewItemV4 & opt4);

    QPropertiesModelItem* parent() const;

	QDiagramProperty property() const;

    QPropertiesModelItem* propertyItem() const;

    int row() const;

    bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);

    bool setEditorData(QWidget* editor, const QModelIndex & index) const;

    bool setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;

    void setValue(const QVariant & value);

    QDiagramToolkit::PropertyType type() const;

    QVariant value(bool parent = false) const;
private:
    QList<QPropertiesModelItem*> m_children;
    QMap<QString,QColor> m_colorNameMap;
	int m_flag;
    int m_index;
	QPair<QString,int> m_maskedFlag;
    QString m_name;
    QAbstractDiagramGraphicsItem* m_item;
    QPropertiesModelItem* m_parent;
	QDiagramToolkit::PropertyType m_type;
	ItemType m_itemType;
};

class QPropertiesModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    enum ItemRole {
        MetaTypeRole = Qt::UserRole + 1
    };

    QPropertiesModel(QObject* parent = 0);
    QPropertiesModel(QAbstractDiagramGraphicsItem* item, QObject* parent);
    ~QPropertiesModel();

    int columnCount(const QModelIndex & parent) const;

    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;

    Qt::ItemFlags flags(const QModelIndex & index) const;

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;

    QModelIndex index(int row, int column, const QModelIndex &parent) const;

    QModelIndex parent(const QModelIndex &index) const;

    int rowCount( const QModelIndex & parent = QModelIndex()) const;

    bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);

    void buildModel(QAbstractDiagramGraphicsItem* item);

    void reset();
private:
    QPropertiesModelItem* r;
};

class QDiagramGraphicsItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    QDiagramGraphicsItemDelegate(QObject* parent);

    QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem &option, const QModelIndex & index) const;

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

    void setEditorData(QWidget *editor, const QModelIndex &index) const;

    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;

    QSize sizeHint( const QStyleOptionViewItem & option, const QModelIndex & index ) const;
public slots:
	void commitAndClose();
    void editingFinished();
private:
    QMap<QString,QString> m_colorNameMap;
};

class QDiagramColorComboBox : public QComboBox
{
    Q_OBJECT
public:
    QDiagramColorComboBox(QWidget* parent);

    static QString colorName(const QColor & color);

    static QStringList colorNames();

    QColor currentColor() const;
private:
    static void initColorMap();

    static QMap<QString,QColor> sColorNameMap;
};

class QDiagramEndOfLineStyleComboBox : public QComboBox
{
    Q_OBJECT
public:
    QDiagramEndOfLineStyleComboBox(const QList<QDiagramEndOfLineStyle> & styles, QWidget* parent);
};

class QDiagramColorEditor : public QWidget
{
	Q_OBJECT
public:
	QDiagramColorEditor(QWidget* parent = 0);
	~QDiagramColorEditor();

	QColor color() const;

	void setColor(const QColor & color);
signals:
	void colorEdited();
private slots:
	void comboBoxActivated(int index);
	void toolButtonClicked();
private:
	QColor m_color;
	QComboBox* m_comboBox;
	QPushButton* m_toolButton;
};

#endif // QDIAGRAMGRAPHICSITEMPROPERTIESVIEW_P_H
