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

#ifndef CSMOD_GUI_PROPERTYWIDGET_IMPL_H
#define CSMOD_GUI_PROPERTYWIDGET_IMPL_H

#include "propertywidget.h"

#include <QLineEdit>
#include <QLabel>

#include "tool/stringmanip.h"

namespace CSMOD {
namespace GUI {
/*
class LineEdit : public QLineEdit
{
public:
    LineEdit(QWidget * parent) : QLineEdit(parent) { }
public signals:
    void userInput();
protected:
    virtual void leaveEvent(QEvent * event)
    {
        QLineEdit::leaveEvent(event);
    }
};
*/
template <class T>
class ValuePropertyWidget : public PropertyWidget
{
public:
    ValuePropertyWidget(ValueProperty<T> * prop, QWidget *parent = 0)
        :   PropertyWidget(prop, parent),
            vprop_  (prop)
    {
        text_ = new QLineEdit(this);
        layout_->addWidget(text_);

        // signal user input
        connect(text_, &QLineEdit::editingFinished, [this]()
        {
            if (oldtext_ != text_->text())
                issueEdited();
            oldtext_ = text_->text();
        });
    }

    virtual void updateWidget()
    {
        oldtext_ = QString::fromStdString(to_string(vprop_->value()));
        text_->setText(oldtext_);
    }

    virtual void updateProperty()
    {
        vprop_->value( from_string<T>(text_->text().toStdString()) );
    }

protected:

    ValueProperty<T> * vprop_;
    QLineEdit * text_;
    QString oldtext_;
};








template <class T>
class ListPropertyWidget : public PropertyWidget
{
public:
    ListPropertyWidget(ListProperty<T> * prop, QWidget *parent = 0)
        :   PropertyWidget(prop, parent),
            lprop_  (prop)
    {
        //layout_->addWidget(text_);
    }

    virtual void updateWidget()
    {
        //text_->setPlainText(QString::fromStdString(to_string(vprop_->value())));
    }

    virtual void updateProperty()
    {
        //text_->setPlainText(QString::fromStdString(to_string(vprop_->value())));
    }

protected:

    ListProperty<T> * lprop_;
    //QTextEdit * text_;
};

} // namespace GUI
} // namespace CSMOD



#endif // CSMOD_GUI_PROPERTYWIDGET_IMPL_H
