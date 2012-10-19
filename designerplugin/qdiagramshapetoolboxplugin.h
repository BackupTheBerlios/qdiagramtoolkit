#ifndef QDIAGRAMSHAPETOOLBOXPLUGIN_H
#define QDIAGRAMSHAPETOOLBOXPLUGIN_H

#include <QDesignerCustomWidgetInterface>

class QDiagramShapeToolBoxPlugin : public QObject, public QDesignerCustomWidgetInterface
{
	Q_OBJECT
	Q_INTERFACES(QDesignerCustomWidgetInterface)
public:
	QDiagramShapeToolBoxPlugin(QObject *parent);
	bool isContainer() const;
	bool isInitialized() const;
	QIcon icon() const;
	QString domXml() const;
	QString group() const;
	QString includeFile() const;
	QString name() const;
	QString toolTip() const;
	QString whatsThis() const;
	QWidget *createWidget(QWidget *parent);
	void initialize(QDesignerFormEditorInterface *core);
private:
	bool m_initialized;
};

#endif // QDIAGRAMSHAPETOOLBOXPLUGIN_H
