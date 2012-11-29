#ifndef QDIAGRAMTOOLKIT_H
#define QDIAGRAMTOOLKIT_H

#include <QObject>
#include <QMetaType>

#include "qdiagramlib_global.h"

/**
 * @defgroup plugins The QDiagram Standard Plugins
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
private:
};

Q_DECLARE_METATYPE(QDiagramToolkit::PropertyType)
Q_DECLARE_METATYPE(QDiagramToolkit::PointerMode)

#endif // QDIAGRAMTOOLKIT_H
