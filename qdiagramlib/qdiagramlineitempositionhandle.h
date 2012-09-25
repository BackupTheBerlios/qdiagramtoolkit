#ifndef QDIAGRAMLINEITEMPOSITIONHANDLE_H
#define QDIAGRAMLINEITEMPOSITIONHANDLE_H

#include <qabstractdiagramgraphicsitemhandle.h>

#include <qdiagramlib_global.h>

class QDIAGRAMLIBSHARED_EXPORT QDiagramLineItemPositionHandle : public QAbstractDiagramGraphicsItemHandle
{
public:
    enum PositionFlag {
        Start,
        End
    };

    QDiagramLineItemPositionHandle(QDiagramLineItemPositionHandle::PositionFlag flag, QGraphicsItem* parent = 0);

    QDiagramLineItemPositionHandle::PositionFlag positionFlag() const;

    void updatePosition();
protected:
    void mouseMoveEvent( QGraphicsSceneMouseEvent * event );
    void mousePressEvent( QGraphicsSceneMouseEvent * event );
private:
    QPointF m_scenePos;
    PositionFlag m_posFlag;
};

#endif // QDIAGRAMLINEITEMPOSITIONHANDLE_H
