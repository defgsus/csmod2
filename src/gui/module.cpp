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

#include "module.h"

#include <QPainter>
#include <QMouseEvent>

#include <iostream>

Module::Module(QWidget *parent) :
    QWidget(parent),
    focus_  (false),
    sel_    (false)
{
    setBackgroundRole(QPalette::Light);
}



void Module::paintEvent(QPaintEvent * )
{
    QPainter p(this);

    p.setBrush(QColor(10 + sel_ * 180,50 + focus_ * 50,10));
    p.drawRect(rect());
}


void Module::mousePressEvent(QMouseEvent * e)
{
    lastx_ = x();
    lasty_ = y();
    lastmx_ = e->globalX();
    lastmy_ = e->globalY();

    sel_ = true;
    update();
}

void Module::mouseMoveEvent(QMouseEvent * e)
{
    move(lastx_ + e->globalX() - lastmx_,
         lasty_ + e->globalY() - lastmy_);
}

void Module::mouseReleaseEvent(QMouseEvent * e)
{
    sel_ = false;
    update();
}

void Module::enterEvent(QEvent * e)
{
    focus_ = true;
    update();
}

void Module::leaveEvent(QEvent * e)
{
    focus_ = false;
    update();
}
