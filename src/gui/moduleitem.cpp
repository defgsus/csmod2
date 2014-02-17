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

#include "log.h"
#include "tool/io.h"
#include "mod/module.h"
#include "mod/connector.h"
#include "connectoritem.h"
#include "patchview.h"
#include "patchgraphicsview.h"

ModuleItem::ModuleItem(CSMOD::Module * module,
                       PatchGraphicsView * view,
                       QGraphicsItem *parent)
    :   QGraphicsRectItem(parent),
        module_ (0),
        view_   (view),
        action_ (A_NOTHING)
{
    //setAcceptHoverEvents(true);

    setFlags(
//        QGraphicsItem::ItemIsMovable
           QGraphicsItem::ItemIsSelectable
         | QGraphicsItem::ItemIsFocusable
        //| QGraphicsItem::ItemClipsToShape
        //| QGraphicsItem::ItemClipsChildrenToShape
        //| QGraphicsItem::ItemSendsGeometryChanges
        //| QGraphicsItem::ItemSendsScenePositionChanges
                );

    // --- default child items ---

    updateFromModule_(module);

/*    auto it = new QGraphicsSimpleTextItem(this);
    it->setText("hello");
    */
}

// ------------------- IO --------------------------

bool ModuleItem::store(CSMOD::Io * io)
{
    if (!io->write("ver", 1)) return false;
    io->write("id", module_->idName());
    io->write("x", pos().x());
    io->write("y", pos().y());
    io->write("w", rect().width());
    io->write("h", rect().height());
    return true;
}

bool ModuleItem::restore(CSMOD::Io * io)
{
    int ver = io->readInt("ver", 0);
    if (ver > 1)
    {
        CSMOD_IO_ERROR("unknown moduleitem version " << ver);
        return false;
    }

    qreal x,y,w,h;
    if (io->read("x", x) && io->read("y", y)
     && io->read("w", w) && io->read("h", h))
    {
        setPos(x,y);
        // XXX setRect does not update the child items!!
        //setRect(x,y,w,h);
    }
    return true;
}


void ModuleItem::setInfo(const std::string& info)
{
    view_->setInfo(info);
}


void ModuleItem::updateFromModule_(CSMOD::Module * module)
{
    CSMOD_DEBUGF("ModuleItem::updateFromModule_(" << module << ")");

    deleteChildItems_();
    module_ = module;

    if (!module_) return;

    int width = 100;
    int ystart = 23;
    int conspace = 23;

    // -- label --
    tlabel_ = new QGraphicsSimpleTextItem(this);
    tlabel_->setText(QString::fromStdString(module_->idName()));
    tlabel_->setPos((width - tlabel_->boundingRect().width())/2, 4);
    tlabel_->setBrush(QBrush(QColor(255,255,255)));

    // -- connectors --
    int num_in = 0, num_out = 0;
    for (size_t i=0; i<module_->connectors().size(); ++i)
    {
        auto c = module_->connectors()[i];
        auto ci = new ConnectorItem(this, c);

        if (c->dir() == CSMOD::Connector::IN)
        {
            ci->setPos(0, ystart + num_in * conspace);
            ++num_in;
        }
        else
        {
            ci->setPos(width - 10, ystart + num_out * conspace);
            ++num_out;
        }

        conitems_.push_back(ci);
    }

    setRect(0,0,width, std::max(num_in, num_out) * conspace + ystart + 2);

}

void ModuleItem::deleteChildItems_()
{
    for (auto c : childItems())
        delete c;

    childItems().clear();
    conitems_.clear();
}



// -------------------- selection ------------------------------

void ModuleItem::markConnectorsThatMatch(CSMOD::Connector * con)
{
    for (auto i : childItems())
    if (auto c = dynamic_cast<ConnectorItem*>(i))
    {
        c->matchConnector(con);
    }
    update();
}




// ----------------------- paint -------------------------------

void ModuleItem::updateValueDisplays()
{
    for (auto c : conitems_)
        c->updateValueDisplay();
}

void ModuleItem::paint(QPainter * p, const
                       QStyleOptionGraphicsItem * /*option*/,
                       QWidget * /*widget*/)
{
    int f = hasFocus() || isSelected();
    p->setBrush(QColor(30,50,30).lighter(100+f * 50));

    p->drawRect(rect());
}




void ModuleItem::mousePressEvent(QGraphicsSceneMouseEvent * e)
{
    CSMOD_DEBUGE("ModuleItem::mousePressEvent("
                 << e->pos().x() << ", " << e->pos().y() << ", " << e->button() << ")");

    if (e->button() == Qt::LeftButton)
    {
        CSMOD_PATCH_INFO(
            "Module: " << module()->name() << " [" << module()->idName() << "]");

        action_ = A_DRAGPOS;
        e->accept();
        return;
    }
    QGraphicsRectItem::mousePressEvent(e);
}


void ModuleItem::mouseMoveEvent(QGraphicsSceneMouseEvent * e)
{
    CSMOD_DEBUGE("ModuleItem::mouseMoveEvent("
                 << e->pos().x() << ", " << e->pos().y() << ", " << e->button() << ")");

    if (action_ == A_DRAGPOS)
    {
        moveBy(e->scenePos().x() - e->lastScenePos().x(),
               e->scenePos().y() - e->lastScenePos().y());
        // update cables connected to this module
        view_->patchView()->updateCables(module_);
    }

    QGraphicsRectItem::mouseMoveEvent(e);
}


void ModuleItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *e)
{
    CSMOD_DEBUGE("ModuleItem::mouseReleaseEvent("
                 << e->pos().x() << ", " << e->pos().y() << ", " << e->button() << ")");

    action_ = A_NOTHING;

    QGraphicsRectItem::mouseReleaseEvent(e);
}

/*
void ModuleItem::hoverEnterEvent(QGraphicsSceneHoverEvent *)
{
    focus_ = true;
    update();
    std::cout << "hello\n";
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
