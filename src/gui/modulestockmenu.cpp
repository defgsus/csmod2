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

#include "modulestockmenu.h"

#include "log.h"
#include "mod/module.h"
#include "mod/modulestock.h"

ModuleStockMenu::ModuleStockMenu(QWidget *parent) :
    QMenu(parent)
{
    CSMOD_DEBUGF("ModuleStockMenu::ModuleStockMenu(" << parent << ")");

    updateMenu();
}

void ModuleStockMenu::updateMenu()
{
    CSMOD_DEBUGF("ModuleStockMenu::updateMenu()");

    clear();

    // get all module names
    std::vector<std::string> ids;
    CSMOD::ModuleStock::instance().getIdNames(ids);

    for (auto id : ids)
    {
        // get the module class
        auto mod = CSMOD::ModuleStock::instance().inspectModule(id);

        // create an action for each
        auto act = new QAction(this);
        act->setText(QString::fromStdString(mod->name()));
        connect(act, &QAction::triggered, [=](bool)
        {
            std::cout << mod->idName() << std::endl;
            moduleSelected(mod->idName());
        });
        addAction(act);
    }
}

