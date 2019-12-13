#ifndef DCONLIB_GLOBAL_H
#define DCONLIB_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(DCONLIB_LIBRARY)
#  define DCONLIBSHARED_EXPORT Q_DECL_EXPORT
#else
#  define DCONLIBSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // DCONLIB_GLOBAL_H
