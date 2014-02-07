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

PatchGraphicsView::PatchGraphicsView(QGraphicsScene *scene, QWidget *parent) :
    QGraphicsView(scene, parent),
    action_ (A_NOTHING)
{
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
