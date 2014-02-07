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

/** direct view into a patch */
class PatchView : public QFrame
{
    Q_OBJECT
public:
    explicit PatchView(QWidget *parent = 0);

    virtual QSize sizeHint() const;
signals:

public slots:


    virtual void paintEvent(QPaintEvent *);
};

#endif // PATCHVIEW_H
