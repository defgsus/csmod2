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

#ifndef VALUEEDITITEM_H
#define VALUEEDITITEM_H

#include <functional>

#include <QGraphicsTextItem>

class ValueEditItem : public QGraphicsTextItem
{

public:
    explicit ValueEditItem(QGraphicsItem *parent = 0);

    void setEditable(bool editable);

    /** update the text display */
    void setValue(double value);

    /** Installs a callback for whenever the textedit was changed
        by the user and a new value was declared. */
    void setValueChanged(std::function<void(double)> callback)
        { change_callback_ = callback; }


protected:

    virtual void keyPressEvent(QKeyEvent *event);

    // ___________ MEMBER _________

    double last_value_;

    std::function<void (double)> change_callback_;
};

#endif // VALUEEDITITEM_H
