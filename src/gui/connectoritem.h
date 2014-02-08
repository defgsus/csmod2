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

#ifndef CONNECTORITEM_H
#define CONNECTORITEM_H

#include <QGraphicsRectItem>

namespace CSMOD { class Connector; }

class ModuleItem;
class PatchGraphicsView;

class ConnectorItem : public QGraphicsRectItem
{
public:
    explicit ConnectorItem(ModuleItem * parent, CSMOD::Connector * con);

    void setInfo(const std::string& info);

    CSMOD::Connector * connector() const { return con_; }

    /** return point for connection in scene coords */
    QPointF sceneConnectPoint() const;

    /** see if connectors match and set internal state. */
    void matchConnector(CSMOD::Connector * con);

protected:

    // ------------- events --------------

    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

    ModuleItem * moduleItem_;
    CSMOD::Connector * con_;
    PatchGraphicsView * view_;

    bool is_con_,
        matches_;
};

#endif // CONNECTORITEM_H
