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

#include "mod/module.h"
#include "mod/patch.h"

PatchView::PatchView(QWidget *parent) :
    QFrame(parent),
    patch_  (0)
{
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

    pview_ = new PatchGraphicsView(this);
    l0->addWidget(pview_);
    /*
    for (int i=0; i<300; ++i)
    {
        auto mod = new CSMOD::TestModule();
        auto m = new ModuleItem(mod);
        scene->addItem( m );
        m->setPos(i*10, rand()%100);
    }

    for (int i=0; i<100; ++i)
    {
        auto c = new CableItem();
        scene->addItem(c);
        c->setLine(rand()%400, rand()%400, rand()%400, rand()%400);
    }
    */
    //scene->addRect(0,0,50,50);
    //scene->addRect(width()-10,30,50,50);
    /*
    auto l = new QVBoxLayout(this);
    l->setContentsMargins(0,0,0,0);

    auto sb = new QScrollArea(this);
    l->addWidget(sb);
    sb->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    sb->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    sb->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    auto w = new QWidget(sb);
    w->setBackgroundRole(QPalette::Dark);

    for (int i=0; i<300; ++i)
    {
        auto b = new Module(w);
        b->setGeometry(i*10, rand()%1000, 80,80);
    }
    w->setGeometry(QRect(0,0,1000,1000));

    sb->setWidget(w);
    */
}

QSize PatchView::sizeHint() const
{
    return QSize(800,600);
}


void PatchView::setPatch(CSMOD::Patch * patch)
{
    patch_ = patch;
    pview_->setPatch(patch_);

    int k=0;
    for (auto m : patch_->modules())
    {
        auto mitem = new ModuleItem(m);
        pview_->scene()->addItem(mitem);
        mitem->setPos(10 + k*120, 10);
        ++k;
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
