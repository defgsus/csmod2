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

#ifndef MODULEITEM_H
#define MODULEITEM_H

#include <QGraphicsItem>

namespace CSMOD { class Module; }


class ModuleItem : public QGraphicsRectItem
{
public:
    explicit ModuleItem(CSMOD::Module * module, QGraphicsItem *parent = 0);

signals:

public slots:

protected:

    virtual void updateFromModule_(CSMOD::Module * module);

    void deleteChildItems_();

    //virtual void paintEvent(QPaintEvent *);
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    /*
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *);
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *);
    */
    virtual void hoverEnterEvent(QGraphicsSceneHoverEvent *);
    /*virtual void hoverMoveEvent(QGraphicsSceneHoverEvent *);
    virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent *);
    */
    bool focus_, sel_;

    CSMOD::Module * module_;
};

#endif // MODULEITEM_H
