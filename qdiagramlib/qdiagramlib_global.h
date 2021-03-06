/******************************************************************************
** Copyright (C) 2011 Martin Hoppe martin@2x2hoppe.de
**
** This file is part of the QDiagram Toolkit (qdiagramlib)
**
** qdiagramlib is free software: you can redistribute it and/or modify
** it under the terms of the GNU Lesser General Public License as
** published by the Free Software Foundation, either version 3 of the
** License, or (at your option) any later version.
**
** qdiagramlib is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU Leser General Public License for more details.
**
** You should have received a copy of the GNU Lesser General Public License
** along with qdialgramlib.  If not, see <http://www.gnu.org/licenses/>.
******************************************************************************/
#ifndef QDIAGRAMLIB_GLOBAL_H
#define QDIAGRAMLIB_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(QDIAGRAMLIB_LIBRARY)
#  define QDIAGRAMLIBSHARED_EXPORT Q_DECL_EXPORT
#else
#  define QDIAGRAMLIBSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // QDIAGRAMLIB_GLOBAL_H
