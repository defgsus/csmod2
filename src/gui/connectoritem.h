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

#include "base.h"

class ValueEditItem;

namespace CSMOD { class Connector; }

class ModuleItem;
class PatchGraphicsView;
class QGraphicsTextItem;

class ConnectorItem : public QGraphicsRectItem
{
public:
    explicit ConnectorItem(ModuleItem * parent, CSMOD::Connector * con);

    /** Sets PatchView info field. */
    void setInfo(const String& info);

    /** Returns assigned CSMOD::Connector instance. */
    CSMOD::Connector * connector() const { return con_; }

    /** Returns point for connection in scene coords. */
    QPointF sceneConnectPoint() const;

    /** Checks if connectors match. */
    void matchConnector(CSMOD::Connector * con);

    /** Does Connector have a value input textfield */
    bool hasValueInput() const;

    /** Does Connector display it's value. */
    bool hasValueDisplay() const;

    /** refresh the textual value display */
    void updateValueDisplay();

protected:

    // ------------- events --------------

    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

    ModuleItem * moduleItem_;
    CSMOD::Connector * con_;
    PatchGraphicsView * view_;

    QGraphicsSimpleTextItem * tname_;
    ValueEditItem * tvalue_;

    bool is_con_,
        matches_;

    QString last_value_;
};

#endif // CONNECTORITEM_H
