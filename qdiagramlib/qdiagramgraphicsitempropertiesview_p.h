#ifndef QDIAGRAMGRAPHICSITEMPROPERTIESVIEW_P_H
#define QDIAGRAMGRAPHICSITEMPROPERTIESVIEW_P_H

#include "qabstractdiagramgraphicsitem.h"

class QAbstractDiagramGraphicsItem;
class QDiagramGraphicsItemDelegate;
class QDiagramEndOfLineStyle;

class QPropertiesModelItem
{
public:
    QPropertiesModelItem(QAbstractDiagramGraphicsItem* graphicsItem);
    QPropertiesModelItem(QAbstractDiagramGraphicsItem* graphicsItem, int index, QPropertiesModelItem* parent = 0);
    QPropertiesModelItem(const QString & name, QPropertiesModelItem* parent);
    ~QPropertiesModelItem();

    QPropertiesModelItem* child(int index) const;

    int childCount() const;

    QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem & option, const QModelIndex & index, const QStyledItemDelegate* receiver) const;

    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;

    Qt::ItemFlags flags() const;

    int index() const;

    QAbstractDiagramGraphicsItem* graphicsItem() const;

    QString name() const;

    bool paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index, const QStyleOptionViewItemV4 & opt4);

    QPropertiesModelItem* parent() const;

    int row() const;

    bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);

    bool setEditorData(QWidget* editor, const QModelIndex & index) const;

    bool setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;

    void setValue(const QVariant & value);

    QDiagramGraphicsItemMetaProperty::Type type() const;

    QVariant value() const;
private:
    QList<QPropertiesModelItem*> m_children;
    QMap<QString,QColor> m_colorNameMap;
    int m_index;
    QString m_name;
    QAbstractDiagramGraphicsItem* m_item;
    QPropertiesModelItem* m_parent;
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

#endif // QDIAGRAMGRAPHICSITEMPROPERTIESVIEW_P_H
