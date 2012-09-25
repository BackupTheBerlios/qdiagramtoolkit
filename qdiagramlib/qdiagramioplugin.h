#ifndef QDIAGRAMIOPLUGIN_H
#define QDIAGRAMIOPLUGIN_H

#include <QObject>

#include <qdiagramlib_global.h>

class QDiagramIOHandler;
class QIODevice;

//! The QDiagramIOPlugin class defines an interface for writing an diagram format plugin.
/**
  * QDiagramIOPlugin is a factory for creating QDiagramIOHandler objects, which are used internally by QDiagramReader and QDiagramWriter to add support for different diagram formats to QDiagram.
  */
class QDIAGRAMLIBSHARED_EXPORT QDiagramIOPlugin : public QObject
{
    Q_OBJECT
public:
    //! This enum describes the capabilities of a QDiagramIOPlugin.
    enum Capability {
        CanRead = 0x1 /*!< The plugin can read diagram. */,
        CanWrite = 0x2 /*!< The plugin can write diagrams. */
    };
    Q_DECLARE_FLAGS(Capabilities, Capability)
    /**
      * Constructs a diagram, plugin with the given @p parent. This is invoked automatically by the Q_EXPORT_PLUGIN2() macro.
      */
    explicit QDiagramIOPlugin(QObject *parent = 0);
    /**
      * Returns the capabilities on the plugin, based on the data in @p device and the format @p format. For example, if the QDiagramIOHandler supports the BMP format, and the data in the device starts with the characters "BM", this function should return CanRead. If format is "bmp" and the handler supports both reading and writing, this function should return CanRead | CanWrite.
      */
    virtual Capabilities capabilities(QIODevice * device, const QByteArray & format) const = 0;
    /**
      * Creates and returns a QDiagramIOHandler subclass, with @p device and @p format set. The @p format must come from the list returned by keys(). Format names are case sensitive.
      */
    virtual QDiagramIOHandler* create(QIODevice* device, const QByteArray & format = QByteArray()) const  = 0;
    /**
      * Returns the list of diagram keys this plugin supports.
      *
      * These keys are usually the names of the diagram formats that are implemented in the plugin (e.g., "jsn" or "xml").
      */
    virtual QStringList keys() const = 0;
};

Q_DECLARE_INTERFACE(QDiagramIOPlugin, "qdiagramlib/diagramioplugin/1.0")

#endif // QDIAGRAMIOPLUGIN_H
