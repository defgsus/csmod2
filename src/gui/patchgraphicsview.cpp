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

#include "patchgraphicsview.h"

#include <iostream>

#include <QMouseEvent>
#include <QBrush>
#include <QPen>
#include <QPoint>

#include "log.h"
#include "mod/connector.h"
#include "mod/model.h"
#include "connectoritem.h"
#include "moduleitem.h"

PatchGraphicsView::PatchGraphicsView(PatchView * view, QWidget * parent) :
    QGraphicsView(new QGraphicsScene(parent), parent),
    patch_  (0),
    view_   (view),
    action_ (A_NOTHING),
    con_from_   (0),
    con_to_     (0),
    con_line_   (0)
{
    CSMOD_DEBUGF("PatchGraphicsView::PatchGraphicsView(" << view << ", " << parent << ")");

    setBackgroundBrush(Qt::NoBrush);
}

PatchGraphicsView::~PatchGraphicsView()
{
    CSMOD_DEBUGF("PatchGraphicsView::~PatchGraphicsView()");
}

void PatchGraphicsView::setPatch(CSMOD::Patch * patch)
{
    CSMOD_DEBUGF("PatchGraphicsView::setPatch(" << patch << ")");
    patch_ = patch;
}

void PatchGraphicsView::setModel(CSMOD::Model * model)
{
    CSMOD_DEBUGF("PatchGraphicsView::setModel(" << model << ")");
    model_ = model;
}







void PatchGraphicsView::startConnect(ConnectorItem * con)
{
    CSMOD_DEBUGF("PatchGraphicsView::startConnect(" << con << ")");

    con_from_ = con;
    con_to_ = 0;

    if (!con_from_) return;

    markConnectorsThatMatch(con->connector());

    if (!con_line_)
    {
        con_line_ = new QGraphicsLineItem;
        scene()->addItem(con_line_);
        con_line_->setPen( QPen(QColor(100,255,100)) );
        QPointF p = con_from_->sceneConnectPoint();
        con_line_->setLine(QLineF(p, p));
    }

}

void PatchGraphicsView::moveConnect(const QPointF &pos)
{
    CSMOD_DEBUGF("PatchGraphicsView::moveConnect(" << pos.x() << ", " << pos.y() << ")");

    if (!con_from_ || !con_line_) return;

    // -- check if end-point is on a connector --

    // we need to turn off the line for itemAt() to work
    con_line_->setVisible(false);
    QPoint wp = mapFromScene(pos);
    QGraphicsItem * it = itemAt(wp);
    // see if connector
    if (it && (con_to_ = dynamic_cast<ConnectorItem*>(it)))
    {
        // see if connectable
        if (!con_from_->connector()->isConnectable(con_to_->connector()))
            con_to_ = false;
        // highlight connectability
        else
        {
            con_line_->setPen(QPen(QColor(100,255,100), 3));
        }
    }
    else
        con_line_->setPen(QPen(QColor(100,255,100)));

    con_line_->setVisible(true);

    // adjust temporary line
    con_line_->setLine(QLineF(
                           con_from_->sceneConnectPoint(),
                           pos));

}

void PatchGraphicsView::endConnect(const QPointF &pos)
{
    CSMOD_DEBUGF("PatchGraphicsView::endConnect(" << pos.x() << ", " << pos.y() << ")");

    // check if on connector
    if (con_to_)
    {
        // create connection
        model_->connect(con_from_->connector(), con_to_->connector());
    }

    // clear highlights
    markConnectorsThatMatch(0);
    // remove helpers
    con_from_ = 0;
    if (con_line_)
    {
        scene()->removeItem(con_line_);
        delete con_line_;
        con_line_ = 0;
    }
}

void PatchGraphicsView::markConnectorsThatMatch(CSMOD::Connector * con)
{
    for (auto i : items())
    if (auto m = dynamic_cast<ModuleItem*>(i))
    {
        m->markConnectorsThatMatch(con);
    }
}


void PatchGraphicsView::paintEvent(QPaintEvent *event)
{
    CSMOD_DEBUGE("PatchGraphicsView::paintEvent(" << event << ")");

    QPainter p(this->viewport());

    // ---- background ----

    p.setBrush(QBrush(QColor(30,30,40)));
    p.drawRect(rect());

    // grid

    p.setPen(QPen(QColor(40,40,45)));

    // line distance with zoom
    int dist = mapFromScene(50,0).x() - mapFromScene(0,0).x();
    int ox = mapFromScene(0,0).x() % dist;
    int oy = mapFromScene(0,0).y() % dist;

    for (int i=0; i<100+width(); i += dist)
        p.drawLine(ox+i,0, ox+i,height());

    for (int i=0; i<height(); i += dist)
        p.drawLine(0, oy+i, width(), oy+i);

    // ---- children ----

    QGraphicsView::paintEvent(event);

}



void PatchGraphicsView::keyPressEvent(QKeyEvent * e)
{
    QGraphicsView::keyPressEvent(e);
}

void PatchGraphicsView::keyReleaseEvent(QKeyEvent * e)
{
    QGraphicsView::keyReleaseEvent(e);
}


void PatchGraphicsView::mousePressEvent(QMouseEvent * e)
{
    // zoom
    if (e->button() == Qt::RightButton
        && !itemAt(e->x(), e->y()))
    {
        action_ = A_ZOOM;
        lastm_ = e->pos();
        lastt_ = transform();
        e->accept();
        return;
    }

    if (e->button() == Qt::LeftButton)
    {
        // start selecting
        if (e->modifiers() && Qt::ShiftModifier)
        {
            setDragMode(QGraphicsView::RubberBandDrag);
        }
        else
        {
            setDragMode(QGraphicsView::ScrollHandDrag);
        }
    }
    QGraphicsView::mousePressEvent(e);
}

void PatchGraphicsView::mouseMoveEvent(QMouseEvent * e)
{
    if (action_ == A_ZOOM)
    {
        qreal zoom = std::max(0.1, std::min(5.0,
                       1.0 - 0.01 * (e->pos().y() - lastm_.y()) ));
        // reuse matrix from mouseDown so it doesn't get spoiled
        // during zooming
        QTransform t(lastt_);
        t.scale(zoom, zoom);

        setTransform(t);
        e->accept();
        return;
    }
    QGraphicsView::mouseMoveEvent(e);
}

void PatchGraphicsView::mouseReleaseEvent(QMouseEvent * e)
{
    action_ = A_NOTHING;
    QGraphicsView::mouseReleaseEvent(e);
    setDragMode(QGraphicsView::NoDrag);
}
