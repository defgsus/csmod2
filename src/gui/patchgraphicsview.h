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

#ifndef PATCHGRAPHICSVIEW_H
#define PATCHGRAPHICSVIEW_H

#include <QGraphicsView>

namespace CSMOD {
class Patch;
class Connector;
class Model;
}

class PatchView;
class ConnectorItem;


class PatchGraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit PatchGraphicsView(PatchView * view, QWidget *parent = 0);
    virtual ~PatchGraphicsView();

    /** Assigns a patch for editing/viewing.
        Set to NULL to disconnect. */
    void setPatch(CSMOD::Patch * patch);
    void setModel(CSMOD::Model * model);

    PatchView * patchView() const { return view_; }

    // -------- for dragging cables -----------

    void startConnect(ConnectorItem * con);
    void moveConnect(const QPointF & scene_pos);
    void endConnect(const QPointF & scene_pos);

    void markConnectorsThatMatch(CSMOD::Connector * con);


signals:

public slots:

protected:

    enum Action
    {
        A_NOTHING,
        A_ZOOM
    };

    virtual void paintEvent(QPaintEvent *event);

    virtual void keyPressEvent(QKeyEvent *event);
    virtual void keyReleaseEvent(QKeyEvent *event);

    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);

    // ___________ PROTECTED MEMBER ____________

    CSMOD::Model * model_;
    CSMOD::Patch * patch_;
    PatchView * view_;

    Action action_;
    QPoint lastm_;
    QTransform lastt_;

    ConnectorItem * con_from_, * con_to_;
    QGraphicsLineItem * con_line_;
};

#endif // PATCHGRAPHICSVIEW_H
