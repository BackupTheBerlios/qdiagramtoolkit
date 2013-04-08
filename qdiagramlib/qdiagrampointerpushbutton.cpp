#include "stdafx.h"
#include "qdiagrampointerpushbutton.h"

#include <qabstractdiagram.h>
#include <qabstractdiagramplugin.h>
#include <qdiagrampluginloader.h>

QDiagramPointerPushButton::QDiagramPointerPushButton(QWidget *parent)
	: QPushButton(parent)
{
    setIcon(QIcon(":/qdiagram/cursor.default"));
	m_mode = QDiagramToolkit::SelectItemsPointer;
}

QDiagramPointerPushButton::~QDiagramPointerPushButton()
{

}

void QDiagramPointerPushButton::addConnector(QAbstractDiagram* diagram)
{
	if (diagram == 0){
		return;
	}
    QAbstractDiagramPlugin* p = 0;
    Q_FOREACH(QString n, diagram->plugins()){
        p = QDiagramPluginLoader::plugin(n, true);
        if (p){
            Q_FOREACH(QDiagramConnectorStyle s, p->connectors()){
                addConnector(s);
            }
        }
    }
}

void QDiagramPointerPushButton::addConnector(const QDiagramConnectorStyle & style)
{
    QAction* a = 0;
    if (menu() == 0){
        QMenu* m = new QMenu(this);
        setMenu(m);
        a = m->addAction(QIcon(":/qdiagram/cursor.default"), tr("Selection"), this, SLOT(pointerModeActionTriggered()));
		//a->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_1));
        a->setData("invalid");
	    connect(a, SIGNAL(triggered()), SLOT(menuActionTriggered()));
    }
    a = menu()->addAction(style.icon(), style.name());
    a->setData(qVariantFromValue(style));
	//if (menu()->findChildren<QAction*>().size() < 9){
	//	a->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_1 + (menu()->findChildren<QAction*>().size())));
	//}
    connect(a, SIGNAL(triggered()), SLOT(menuActionTriggered()));
}

void QDiagramPointerPushButton::addConnector(const QIcon & icon, const QString & text, const QString & shape)
{
    QAction* a = 0;
    if (menu() == 0){
        QMenu* m = new QMenu(this);
        setMenu(m);
        a = m->addAction(QIcon(":/qdiagram/cursor.default"), tr("Selection"), this, SLOT(pointerModeActionTriggered()));
        a->setData("invalid");
    }
    a = menu()->addAction(icon, text);
    a->setData(shape);
    connect(a, SIGNAL(triggered()), SLOT(menuActionTriggered()));
}

void QDiagramPointerPushButton::clear()
{
	delete menu();
	QMenu* m = new QMenu(this);
	setMenu(m);
}

void QDiagramPointerPushButton::menuActionTriggered()
{
    QAction* a = qobject_cast<QAction*>(sender());
    if (a){
        setIcon(a->icon());
        QDiagramConnectorStyle s = qvariant_cast<QDiagramConnectorStyle>(a->data());
        if (!s.isNull()){
			if (m_mode == QDiagramToolkit::SelectItemsPointer){
				m_mode = QDiagramToolkit::ConnectItemsPointer;
				emit modeChanged(QDiagramToolkit::ConnectItemsPointer);
			}
			//if (m_style != s){
				m_style = s;
				emit connectorStyleChanged(m_style);
			//}
        }
    }
}

QDiagramConnectorStyle QDiagramPointerPushButton::currentConnectorStyle() const
{
	return m_style;
}

QDiagramToolkit::PointerMode QDiagramPointerPushButton::pointerMode() const
{
	return m_mode;
}

void QDiagramPointerPushButton::pointerModeActionTriggered()
{
    QAction* a = qobject_cast<QAction*>(sender());
    if (a){
        setIcon(a->icon());
	}
	m_mode = QDiagramToolkit::SelectItemsPointer;
	m_style = QDiagramConnectorStyle();
	emit modeChanged(m_mode);
}

void QDiagramPointerPushButton::setPointerMode(QDiagramToolkit::PointerMode mode)
{
	m_mode = mode;
}
