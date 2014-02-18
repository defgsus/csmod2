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

#include "cableitem.h"

#include <QPen>
#include <QPainter>

#include "log.h"
#include "mod/connection.h"
#include "connectoritem.h"
#include "patchgraphicsview.h"

namespace CSMOD {
namespace GUI {

CableItem::CableItem(CSMOD::Connection * con,
                     ConnectorItem * ci1, ConnectorItem * ci2,
                     PatchGraphicsView * view,
                     QGraphicsItem * parent)
    :   QGraphicsLineItem(parent),
        view_            (view),
        con_             (con),
        citem1_          (ci1),
        citem2_          (ci2),
        color_           (120,150,120)
{
    CSMOD_DEBUGF("CableItem::CableItem(" << con << ", " << ci1 << ", " << ci2
                                << ", " << view << ", " << parent << ") this=" << this);

    setFlags(
          QGraphicsItem::ItemIsSelectable
        | QGraphicsItem::ItemIsFocusable
        //| QGraphicsItem::ItemClipsToShape
        //| QGraphicsItem::ItemClipsChildrenToShape
        //| QGraphicsItem::ItemSendsGeometryChanges
        //| QGraphicsItem::ItemSendsScenePositionChanges
                );

}

void CableItem::setInfo(const String& info)
{
    view_->setInfo(info);
}

void CableItem::updatePos()
{
    CSMOD_DEBUGE("CableItem::updatePos()");

    if (!con_ || !citem1_ || !citem2_) return;

    setLine(QLineF(
                citem1_->sceneConnectPoint(),
                citem2_->sceneConnectPoint()
                ));
}


// ----------------------- paint -------------------------------

void CableItem::paint(QPainter * p, const
                       QStyleOptionGraphicsItem * /*option*/,
                       QWidget * /*widget*/)
{
    if (isSelected() || hasFocus())
        p->setPen(QPen(color_.lighter(), 3));
    else
        p->setPen(QPen(color_, 1));

    p->drawLine(line());
}



} // namespace GUI
} // namespace CSMOD
