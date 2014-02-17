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

#include "propertiesview.h"

#include <QLayout>

#include "mod/property.h"
#include "propertywidget.h"

namespace CSMOD {
namespace GUI {


PropertiesView::PropertiesView(Properties *prop, QWidget *parent)
    :   QWidget(parent),
      props_   (prop)
{
    CSMOD_DEBUGF("PropertiesView::PropertiesView(" << prop << ", " << parent << ")");

    auto l0 = new QVBoxLayout(this);

    for (size_t i=0; i<props_->size(); ++i)
    {
        // create a widget for each property
        auto p = (*props_)[i]->createWidget(this);
        l0->addWidget(p);

        // set value
        p->updateWidget();

        // on user input change
        connect(p, &PropertyWidget::userInput, [this](PropertyWidget * w)
        {
            w->updateProperty();
            propertyChanged(w->property());
        });
    }

    l0->addStretch(2);
}


} // namespace GUI
} // namespace CSMOD
