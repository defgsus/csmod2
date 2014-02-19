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

#include "valueedititem.h"

#include <QKeyEvent>

namespace CSMOD {
namespace GUI {

ValueEditItem::ValueEditItem(QGraphicsItem *parent) :
    QGraphicsTextItem(parent),
    last_value_ (0),
    change_callback_    (0),
    col_                (200,200,200),
    editcol_            (255,255,255)
{
    setEditable(false);
    setPlainText("0");

}

void ValueEditItem::setColors(const QColor& color, const QColor& edit_color)
{
    col_ = color;
    editcol_ = edit_color;
}

void ValueEditItem::setEditable(bool editable)
{
    if (editable)
    {
        setTextInteractionFlags(Qt::TextEditorInteraction);
        setTabChangesFocus(true);
        setDefaultTextColor(editcol_);
    }
    else
    {
        setTextInteractionFlags(0);
        setDefaultTextColor(col_);
    }
}

void ValueEditItem::setValue(double value)
{
    if (value != last_value_)
    {
        last_value_ = value;
        setPlainText(QString::number(value));
    }
}


void ValueEditItem::keyPressEvent(QKeyEvent *event)
{
    static const Qt::Key accepted_keys[] =
    {
        Qt::Key_Left, Qt::Key_Right,
        Qt::Key_Home, Qt::Key_End,
        Qt::Key_Delete, Qt::Key_Backspace,
        Qt::Key_0, Qt::Key_1, Qt::Key_2, Qt::Key_3, Qt::Key_4,
        Qt::Key_5, Qt::Key_6, Qt::Key_7, Qt::Key_8, Qt::Key_9,
        Qt::Key_Comma, Qt::Key_Period, Qt::Key_Minus

    };
    static const size_t num_accepted_keys = sizeof(accepted_keys) / sizeof(Qt::Key);

    // navigation keys

    for (size_t i=0; i<num_accepted_keys; ++i)
    if (event->key() == accepted_keys[i])
    {
        QGraphicsTextItem::keyPressEvent(event);

        // notify value of text change
        double v = toPlainText().toDouble();
        if (v != last_value_)
        {
            if (change_callback_) change_callback_(v);
            last_value_ = v;
        }

        return;
    }

}

} // namespace GUI
} // namespace CSMOD
