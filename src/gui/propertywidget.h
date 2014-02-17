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

#ifndef CSMOD_GUI_PROPERTYWIDGET_H
#define CSMOD_GUI_PROPERTYWIDGET_H

#include <QWidget>
#include <QVBoxLayout>

#include "base.h"

namespace CSMOD {
namespace GUI {

class PropertyWidget : public QWidget
{
    Q_OBJECT
public:
    PropertyWidget(Property * prop, QWidget *parent = 0);

    Property * property() { return prop_; }

signals:

    void userInput(PropertyWidget * prop);

public slots:

    virtual void updateWidget() = 0;
    virtual void updateProperty() = 0;

protected:

    void issueEdited() { userInput(this); }

    QVBoxLayout * layout_;
    Property * prop_;
};


template <class T>
class ValuePropertyWidget;

template <class T>
class ListPropertyWidget;


} // namespace GUI
} // namespace CSMOD

#endif // CSMOD_GUI_PROPERTYWIDGET_H
