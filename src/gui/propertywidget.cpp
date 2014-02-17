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

#include "propertywidget.h"

#include <QLabel>
#include <QLayout>

#include "log.h"
#include "mod/property.h"

namespace CSMOD {
namespace GUI {



PropertyWidget::PropertyWidget(Property * prop, QWidget *parent) :
    QWidget(parent),
    prop_   (prop)
{
    CSMOD_DEBUGF("PropertyWidget::PropertyWidget(" << prop << ", " << parent << ")");

    layout_ = new QVBoxLayout(this);

    auto label = new QLabel(QString::fromStdString(prop->name()), this);
    layout_->addWidget(label);
}

/*
void PropertyWidget::leaveEvent(QEvent * event)
{
    CSMOD_DEBUGF("########" << prop_->idName());
    QWidget::leaveEvent(event);
}*/





} // namespace GUI
} // namespace CSMOD
