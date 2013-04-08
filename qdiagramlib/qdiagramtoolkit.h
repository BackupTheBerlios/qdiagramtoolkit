#ifndef QDIAGRAMTOOLKIT_H
#define QDIAGRAMTOOLKIT_H

#include <QObject>
#include <QMetaType>

#include "qdiagramlib_global.h"

class QAbstractDiagram;
class QAbstractDiagramShape;
class QAbstractDiagramShapeConnector;

#define QDIAGRAM_DECLARE_CONNECTOR(__c__) static QAbstractDiagramShapeConnector* Creator(const QVariantMap & properties)\
{\
	return new __c__(properties);\
}

#define QDIAGRAM_DECLARE_DIAGRAM(__c__,__t__) static QAbstractDiagram* Creator(const QString & pluginName, QObject* parent)\
{\
	return new __c__(pluginName, parent);\
}\
QString __c__::type() const\
{\
	return __c__::staticType();\
}\
static QString __c__::staticType()\
{\
	return QString(__t__);\
}

#define QDIAGRAM_DECLARE_PLUGIN(__c__,__n__) static QString staticName()\
{\
	return QString(__n__);\
};\
QString __c__::name() const\
{\
	return __c__::staticName();\
}

#define QDIAGRAM_DECLARE_SHAPE(__c__,__n__) static QAbstractDiagramShape* Creator(const QVariantMap & properties)\
{\
	return new __c__(properties);\
};\
static QVariantMap staticMetaData()\
{\
	QVariantMap m;\
	m["itemType"] = "Shape";\
	m["itemClass"] = #__n__;\
	return m;\
};\
static QString staticItemClass()\
{\
	return __c__::staticMetaData().value("itemClass").toString();\
};\
QString itemClass() const\
{\
	return __c__::staticItemClass();\
};

/**
 * @defgroup plugins The QDiagram Standard Plugins
 * \tableofcontents
 * @defgroup widgets The QDiagram Widgets
 * @mainpage The QDiagram Toolkit
 * \tableofcontents
 * @section intro Introduction
 * @page plugins Plugins
 * @section ifplugins Plugin Interface
 * @subsection customplugins Custom Plugins
 * @page scripting Scripting
 * @page qdesigner Qt Designer Plugin
 * @page widgets QDiagram Widgets
 */
class QDIAGRAMLIBSHARED_EXPORT QDiagramToolkit : public QObject
{
	Q_OBJECT
	Q_ENUMS(ConnectionPointOrientation)
	Q_ENUMS(PaperOrientation)
	Q_ENUMS(PaperSize)
	Q_ENUMS(PointerMode)
	Q_ENUMS(PropertyType)
public:
	QDiagramToolkit(QObject *parent);
	~QDiagramToolkit();	
    //! This enum describes the orientation of a connection point.
    enum ConnectionPointOrientation {
        North,
        East,
        South,
        West,
        ConnectionPointOrientationInvalid
    };
	//! This enum type is used to specify each page's orientation.
	enum PaperOrientation {
		Portrait
		,Landscape
	};
	//! This enum type specifies what paper size is used for a diagram.
	enum PaperSize {
		A0 /*!< 841 x 1189 mm */,
		A1 /*!< 594 x 841 mm */,
		A2 /*!< 420 x 594 mm */,
		A3 /*!< 297 x 420 mm */,
		A4 /*!< 210 x 297 mm, 8.26 x 11.69 inches */,
		Custom /*! Unknown, or a user defined size. */
	};
	enum PointerMode {
		SelectItemsPointer,
		ConnectItemsPointer
	};
    /**
      * This enum type defines the types of variable that a QDiagramMetaProperty can contain.
      */
    enum PropertyType {
        PropertyTypeInvalid, /*!< no type. */
		Alignment,
        Angle, /*!< an angle value */
        Bool, /*!< a boolean value */
        Brush, /*!< a QBrush */
		BrushStyle,
        Color, /*!< a QColor */
		ConnectionPoint,
        Double, /*!< a Double */
        EndOfLineStyle,
        Enumeration,
        Flag,
		Font, /*<! a QFont */
		FontFamily,
        Int, /*!< an Int. */
        LineStyle, /*!< a line style */
		Orientation,
        Pen,
		PenJoinStyle,
		PenStyle,
        Percent, /*!< a percentage value */
        Point, /*!< a point */
        Rect, /*!< a QRectF */
        Shadow, /*<! a shadow */
		Size, /*<! a QSize */
        String, /*!< a QString. */
        Text,
        TextStyle, /*!< a text style */
        UUID, /*!< an UUID. */
		Dynamic /*!< a user-defined type */
    };
	enum Unit {
		Millimeter
	};
private:
};

Q_DECLARE_METATYPE(QDiagramToolkit::PropertyType)
Q_DECLARE_METATYPE(QDiagramToolkit::PointerMode)

#endif // QDIAGRAMTOOLKIT_H
