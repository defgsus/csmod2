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

#ifndef CSMOD_CABLEITEM_H
#define CSMOD_CABLEITEM_H

#include <QGraphicsLineItem>
#include <QColor>

#include "base.h"

namespace CSMOD {
namespace GUI {


class CableItem : public QGraphicsLineItem
{
public:
    explicit CableItem(CSMOD::Connection * con,
                       ConnectorItem * ci1, ConnectorItem * ci2,
                       PatchGraphicsView * view,
                       QGraphicsItem * parent = 0);

    void setInfo(const String& info);

    CSMOD::Connection * connection() const { return con_; }

    /** update position to module's connectors */
    void updatePos();

    /** base color */
    const QColor& color() const { return color_; }
    void color(const QColor& color) { color_ = color; }

protected:

    // ------------------- paint -------------------------

    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    // ------------------- events ------------------------

    PatchGraphicsView * view_;
    CSMOD::Connection * con_;
    ConnectorItem * citem1_, * citem2_;
    QColor color_;
};



} // namespace GUI
} // namespace CSMOD

#endif // CSMOD_CABLEITEM_H
