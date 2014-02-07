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

#ifndef PATCHVIEW_H
#define PATCHVIEW_H

#include <QFrame>

namespace CSMOD {
class Module;
class Connection;
class Connector;
class Patch;
class Model;
}

class PatchGraphicsView;
class ModuleItem;
class CableItem;


/** direct view into a patch */
class PatchView : public QFrame
{
    Q_OBJECT
public:
    explicit PatchView(QWidget *parent = 0);

    QSize sizeHint() const;

    /** Assigns a patch for editing/viewing.
        Set to NULL to deconnect. */
    void setPatch(CSMOD::Patch * patch);

    void setModel(CSMOD::Model * model);

    /** update changes in patch */
    void updateFromPatch();

    /** update all cable positions */
    void updateCables();

signals:

public slots:

protected:
    virtual void mousePressEvent(QMouseEvent *);
    virtual void mouseMoveEvent(QMouseEvent *);
    virtual void mouseReleaseEvent(QMouseEvent *);

    //virtual void paintEvent(QPaintEvent *);

    // __________ PROTECTED MEMBER __________________

    CSMOD::Model * model_;
    CSMOD::Patch * patch_;
    PatchGraphicsView * pview_;

    std::vector<ModuleItem*> moduleitems_;
    std::vector<CableItem*> cableitems_;
};

#endif // PATCHVIEW_H
