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

#include "patchview.h"

#include <iostream>

#include <QPainter>
#include <QPalette>
#include <QScrollArea>
#include <QPushButton>
#include <QLayout>
#include <QGroupBox>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QToolBar>

#include "moduleitem.h"
#include "cableitem.h"
#include "patchgraphicsview.h"

#include "log.h"
#include "mod/module.h"
#include "mod/patch.h"

PatchView::PatchView(QWidget *parent) :
    QFrame(parent),
    model_  (0),
    patch_  (0),
    pview_  (0)
{
    CSMOD_DEBUGF("PatchView::PatchView(" << parent << ")");

    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    setFrameStyle(QFrame::Panel | QFrame::Sunken);
    setLineWidth(2);
/*
    // colors
    QPalette pal = palette();
    pal.setColor(QPalette::Base, QColor(40,40,40));
    setPalette(pal);
*/
    auto l0 = new QVBoxLayout(this);
    l0->setContentsMargins(lineWidth(), lineWidth(),
                           lineWidth(), lineWidth());

    auto tb = new QToolBar(this);
    l0->addWidget(tb);

    pview_ = new PatchGraphicsView(this, this);
    l0->addWidget(pview_);

}

QSize PatchView::sizeHint() const
{
    return QSize(800,600);
}


void PatchView::setPatch(CSMOD::Patch * patch)
{
    CSMOD_DEBUGF("PatchView::setPatch(" << patch << ")");

    patch_ = patch;
    pview_->setPatch(patch_);

    moduleitems_.clear();
    cableitems_.clear();

    int k=0;
    for (auto m : patch_->modules())
    {
        // create a graphic representation of Module
        auto mitem = new ModuleItem(m, pview_);
        // add to graphic scene
        pview_->scene()->addItem(mitem);
        mitem->setPos(10 + k*120, 10);
        ++k;
        moduleitems_.push_back(mitem);
    }

    // add connections
    for (auto c : patch_->connections())
    {
        auto citem = new CableItem(c);
        pview_->scene()->addItem(citem);
        cableitems_.push_back(citem);
    }

}

void PatchView::setModel(CSMOD::Model * model)
{
    CSMOD_DEBUGF("PatchView::setModel(" << model << ")");

    model_ = model;
    pview_->setModel(model);
}

void PatchView::updateFromPatch()
{
    CSMOD_DEBUGF("PatchView::updateFromPatch()");

    setPatch(patch_);
}

void PatchView::updateCables()
{
    CSMOD_DEBUGF("PatchView::updateCables()");

    for (auto ci : cableitems_)
    {
        ci->updatePos();
    }
}




/*
void PatchView::paintEvent(QPaintEvent * event)
{
    QFrame::paintEvent(event);

    QPainter p(this);
    p.setBrush(QBrush(QColor(200,200,200)));
    p.drawLine(QLineF(0,0,width(),height()));
}
*/

void PatchView::mousePressEvent(QMouseEvent * e)
{
    QFrame::mousePressEvent(e);
    //update();
}

void PatchView::mouseMoveEvent(QMouseEvent * e)
{
    QFrame::mouseMoveEvent(e);
}

void PatchView::mouseReleaseEvent(QMouseEvent * e)
{
    QFrame::mouseReleaseEvent(e);
}
