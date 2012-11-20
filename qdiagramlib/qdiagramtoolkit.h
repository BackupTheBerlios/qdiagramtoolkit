#ifndef QDIAGRAMTOOLKIT_H
#define QDIAGRAMTOOLKIT_H

#include <QObject>

#include "qdiagramlib_global.h"

class QDIAGRAMLIBSHARED_EXPORT QDiagramToolkit : public QObject
{
	Q_OBJECT
	Q_ENUMS(ConnectionPointOrientation)
	Q_ENUMS(PropertyType)
public:
    //! This enum describes the orientation of a connection point.
    enum ConnectionPointOrientation {
        North,
        East,
        South,
        West,
        ConnectionPointOrientationInvalid
    };
    /**
      * This enum type defines the types of variable that a qdiagrammetaproperty.h can contain.
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
		Dynamic
    };
	QDiagramToolkit(QObject *parent);
	~QDiagramToolkit();

private:
	
};

Q_DECLARE_METATYPE(QDiagramToolkit::PropertyType)

#endif // QDIAGRAMTOOLKIT_H
