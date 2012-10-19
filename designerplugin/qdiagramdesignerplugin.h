#ifndef QDIAGRAMDESIGNERPLUGIN_H
#define QDIAGRAMDESIGNERPLUGIN_H

#include <QtDesigner/QtDesigner>
#include <QtCore/qplugin.h>

class QDiagramDesignerPlugin : public QObject, public QDesignerCustomWidgetCollectionInterface
{
	Q_OBJECT
    Q_INTERFACES(QDesignerCustomWidgetCollectionInterface)
public:
	QDiagramDesignerPlugin(QObject *parent = 0);
    virtual QList<QDesignerCustomWidgetInterface*> customWidgets() const;
private:
     QList<QDesignerCustomWidgetInterface*> m_widgets;
};

#endif // QDIAGRAMDESIGNERPLUGIN_H
