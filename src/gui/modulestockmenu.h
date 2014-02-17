/***************************************************************************

Copyright (C) 2014  stefan.berke @ modular-audio-graphics.com

This source is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either
version 3.0 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
General Public License for more details.

You should have received a copy of the GNU General Public License
along with this software; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA

****************************************************************************/

#ifndef MODULESTOCKMENU_H
#define MODULESTOCKMENU_H

#include <QMenu>

#include "base.h"

/** A menu containg all Modules. */
class ModuleStockMenu : public QMenu
{
    Q_OBJECT
public:
    explicit ModuleStockMenu(QWidget *parent = 0);

    /** Call this to recreat the list when Module dlls have been loaded
        after the creation of this class. */
    void updateMenu();

signals:
    /** signal when a module was selected. */
    void moduleSelected(const String& idName);

public slots:

};

#endif // MODULESTOCKMENU_H
