#ifndef QLOGICCIRCUITSIMENGINE_H
#define QLOGICCIRCUITSIMENGINE_H

#include <QThread>
#include <QScriptEngine>

class QAbstractDiagramShape;
class QDiagram;

class QLogicCircuitSimEngine : public QThread
{
    Q_OBJECT
public:
    explicit QLogicCircuitSimEngine(QObject *parent = 0);

    QString script() const;

    void setDiagram(QDiagram* diagram);

	QScriptEngine* scriptEngine() const;
signals:
    
public slots:
    void pause();
    void reset();
    void resume();
    void stop();
private:
    int level(QAbstractDiagramShape* shape, int previous);

    QScriptEngine* m_engine;
    QString m_script;
};

#endif // QLOGICCIRCUITSIMENGINE_H
