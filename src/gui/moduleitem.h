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

namespace CSMOD { class Module; class Connector; class Io; }

class PatchGraphicsView;

class ModuleItem : public QGraphicsRectItem
{
    enum Action
    {
        A_NOTHING,
        A_DRAGPOS
    };

public:
    explicit ModuleItem(CSMOD::Module * module,
                        PatchGraphicsView * view, QGraphicsItem *parent = 0);

    // ------------------- IO --------------------------

    bool store(CSMOD::Io * io);
    bool restore(CSMOD::Io * io);

    CSMOD::Module * module() const { return module_; }
    PatchGraphicsView * view() const { return view_; }

    void setInfo(const std::string& info);

    // -------- selection / highlighting ---------------

    void markConnectorsThatMatch(CSMOD::Connector * con);


signals:

public slots:

protected:

    // ------------------- paint -------------------------

    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    // ------------------- events ------------------------

    virtual void mousePressEvent(QGraphicsSceneMouseEvent *);
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *);

    /* TODO virtual void hoverEnterEvent(QGraphicsSceneHoverEvent *);
    virtual void hoverMoveEvent(QGraphicsSceneHoverEvent *);
    virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent *);
    */
    //

protected:

    virtual void updateFromModule_(CSMOD::Module * module);

    void deleteChildItems_();

    // _____________ PROTECTED MEMBER _________

    CSMOD::Module * module_;
    PatchGraphicsView * view_;

    Action action_;

    // --- gui stuff ---

    QGraphicsSimpleTextItem * tlabel_;
};

#endif // MODULEITEM_H
