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

#include "moduleitem.h"

#include <QPainter>
#include <QGraphicsSceneMouseEvent>

#include <iostream>

ModuleItem::ModuleItem(QGraphicsItem *parent) :
    QGraphicsRectItem(parent),
    focus_  (false),
    sel_    (false)
{
    setFlags(
        QGraphicsItem::ItemIsMovable
        | QGraphicsItem::ItemIsSelectable
        | QGraphicsItem::ItemIsFocusable
        | QGraphicsItem::ItemClipsToShape
        | QGraphicsItem::ItemClipsChildrenToShape
        //| QGraphicsItem::ItemSendsGeometryChanges
        //| QGraphicsItem::ItemSendsScenePositionChanges
                );
    //setBackgroundRole(QPalette::Light);
    setRect(0,0,50,50);
}


void ModuleItem::paint(QPainter * p, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    p->setBrush(QColor(10 + hasFocus() * 180,
                       50 + hasCursor() * 50,
                       10 + isSelected() * 100
                       ));
    p->drawRect(rect());
}

/*
void ModuleItem::mousePressEvent(QGraphicsSceneMouseEvent * e)
{
    sel_ = true;
    update();
}

void ModuleItem::mouseMoveEvent(QGraphicsSceneMouseEvent * e)
{
    moveBy(e->scenePos().x() - e->lastScenePos().x(),
           e->scenePos().y() - e->lastScenePos().y());
}


void ModuleItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *)
{
    sel_ = false;
    update();
}



void ModuleItem::hoverEnterEvent(QGraphicsSceneHoverEvent *)
{
    focus_ = true;
    update();

}


void ModuleItem::hoverMoveEvent(QGraphicsSceneHoverEvent *)
{
    update();
}


void ModuleItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *)
{
    focus_ = false;
    update();
}
*/
