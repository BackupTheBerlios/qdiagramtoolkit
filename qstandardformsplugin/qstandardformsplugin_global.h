#ifndef QSTANDARDFORMSPLUGIN_GLOBAL_H
#define QSTANDARDFORMSPLUGIN_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(QSTANDARDFORMSPLUGIN_LIBRARY)
#  define QSTANDARDFORMSPLUGINSHARED_EXPORT Q_DECL_EXPORT
#else
#  define QSTANDARDFORMSPLUGINSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // QSTANDARDFORMSPLUGIN_GLOBAL_H
