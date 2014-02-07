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

#include "connectoritem.h"

#include <QBrush>

#include "moduleitem.h"



ConnectorItem::ConnectorItem(ModuleItem * parent, CSMOD::Connector * con)
    :   QGraphicsRectItem(parent),
        moduleItem_  (parent),
        con_         (con)
{
    setFlags(
          QGraphicsItem::ItemIsSelectable
        | QGraphicsItem::ItemIsFocusable
        //| QGraphicsItem::ItemClipsToShape
        );

    setBrush(QBrush(QColor(50,50,50)));
    setRect(0,0, 10, 10);
}
