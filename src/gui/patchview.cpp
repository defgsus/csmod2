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

#include "moduleitem.h"
#include "patchgraphicsview.h"

#include <QPainter>
#include <QPalette>
#include <QScrollArea>
#include <QPushButton>
#include <QLayout>
#include <QGroupBox>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QToolBar>

PatchView::PatchView(QWidget *parent) :
    QFrame(parent)
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

    auto scene = new QGraphicsScene(this);
    auto view = new PatchGraphicsView(scene, this);
    l0->addWidget(view);
    //view->translate(200,2000);
    //view->setSceneRect(0,0,1000,1000);
    //view->scale(2,2);

    for (int i=0; i<300; ++i)
    {
        auto m = new ModuleItem();
        scene->addItem( m );
        m->setPos(i*10, rand()%100);
    }
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
