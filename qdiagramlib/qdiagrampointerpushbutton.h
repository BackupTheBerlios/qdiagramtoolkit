#ifndef QDIAGRAMPOINTERPUSHBUTTON_H
#define QDIAGRAMPOINTERPUSHBUTTON_H

#include <QPushButton>
#include "qdiagramlib_global.h"

#include <qdiagramtoolkit.h>
#include <qdiagramconnectorstyle.h>

class QAbstractDiagram;

//! The QDiagramPointerPushButton widget provides a command button for changing the diagram view pointer.
class QDIAGRAMLIBSHARED_EXPORT QDiagramPointerPushButton : public QPushButton
{
	Q_OBJECT
	Q_PROPERTY(QDiagramToolkit::PointerMode pointerMode READ pointerMode WRITE setPointerMode)
public:
	/**
	 * Constructs a pointer push button with the given @p parent.
	 */
	QDiagramPointerPushButton(QWidget *parent);
	/**
	 * Destroys the diagram pointer push button.
	 */
	~QDiagramPointerPushButton();

	void addConnector(QAbstractDiagram* diagram);

	void addConnector(const QDiagramConnectorStyle & style);

	void addConnector(const QIcon & icon, const QString & text, const QString & shape);

	void clear();

	QDiagramConnectorStyle currentConnectorStyle() const;
	/**
	 * Returns the current pointer mode.
	 */
	QDiagramToolkit::PointerMode pointerMode() const;
	/**
	 * Sets the pointer mode to @p mode.
	 */
	void setPointerMode(QDiagramToolkit::PointerMode mode);
signals:
	/**
	 * This signal is emitted when the user has changed the connector style.
	 */
	void connectorStyleChanged(const QDiagramConnectorStyle & style);
	/**
	 * This signal is emitted when the user has changed the pointer mode.
	 */
	void modeChanged(QDiagramToolkit::PointerMode mode);
private slots:
	void menuActionTriggered();
	void pointerModeActionTriggered();
private:
	QDiagramToolkit::PointerMode m_mode;
	QDiagramConnectorStyle m_style;
};

#endif // QDIAGRAMPOINTERPUSHBUTTON_H
