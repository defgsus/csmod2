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

#include <iostream>

#include <QPainter>
#include <QGraphicsSceneMouseEvent>

#include "mod/module.h"
#include "mod/connector.h"
#include "connectoritem.h"

ModuleItem::ModuleItem(CSMOD::Module * module,
                       PatchGraphicsView * view,
                       QGraphicsItem *parent) :
    QGraphicsRectItem(parent),
    focus_  (false),
    sel_    (false),
    module_ (0),
    view_  (view)
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

    updateFromModule_(module);

/*    auto it = new QGraphicsSimpleTextItem(this);
    it->setText("hello");
    */
}


void ModuleItem::markConnectorsThatMatch(CSMOD::Connector * con)
{
    for (auto i : childItems())
    if (auto c = dynamic_cast<ConnectorItem*>(i))
    {
        c->matchConnector(con);
    }
    update();
}


void ModuleItem::updateFromModule_(CSMOD::Module * module)
{
    deleteChildItems_();
    module_ = module;

    if (!module_) return;

    int width = 100;

    int num_in = 0, num_out = 0;
    for (size_t i=0; i<module_->numConnectors(); ++i)
    {
        auto c = module_->connector(i);
        auto ci = new ConnectorItem(this, c);

        if (c->dir() == CSMOD::Connector::IN)
        {
            ci->setPos(0, 10 + num_in * 15);
            ++num_in;
        }
        else
        {
            ci->setPos(width - 10, 10 + num_out * 15);
            ++num_out;
        }
    }

    setRect(0,0,width, std::max(num_in, num_out) * 15 + 20);
}

void ModuleItem::deleteChildItems_()
{
    for (auto c : childItems())
        delete c;
    childItems().clear();
}






void ModuleItem::paint(QPainter * p, const
                       QStyleOptionGraphicsItem * /*option*/,
                       QWidget * /*widget*/)
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


*/
void ModuleItem::hoverEnterEvent(QGraphicsSceneHoverEvent *)
{
    focus_ = true;
    update();
    std::cout << "hello\n";
}

/*
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
