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

#include "audiodeviceframe.h"

#include <QLayout>
#include <QListWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>

#include "audio/audiodevices.h"

namespace CSMOD {
namespace GUI {

AudioDeviceFrame::AudioDeviceFrame(QWidget *parent)
    :   QWidget(parent),
        device_index_   (-1)
{
    auto l0 = new QVBoxLayout(this);

        device_list_ = new QListWidget(this);
        l0->addWidget(device_list_, 2);
        device_list_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

        auto l = new QLabel("samplerate", this);
        l0->addWidget(l);
        text_rate_ = new QLineEdit(this);
        l0->addWidget(text_rate_);

        l = new QLabel("buffer length", this);
        l0->addWidget(l);
        text_buffer_ = new QLineEdit(this);
        l0->addWidget(text_buffer_);

        l = new QLabel("number of input channels", this);
        l0->addWidget(l);
        text_chan_in_ = new QLineEdit(this);
        l0->addWidget(text_chan_in_);

        l = new QLabel("number of output channels", this);
        l0->addWidget(l);
        text_chan_out_ = new QLineEdit(this);
        l0->addWidget(text_chan_out_);

        auto go_button = new QPushButton("use", this);
        l0->addWidget(go_button);

    // ------- hook-up -------

    // list click
    connect(device_list_, &QListWidget::itemClicked, [this](QListWidgetItem * item)
    {
        device_index_ = item->data(Qt::UserRole).toInt();
        text_rate_->setText(QString::number(item->data(Qt::UserRole+1).toInt()));
        text_buffer_->setText(QString::number(item->data(Qt::UserRole+2).toInt()));
        text_chan_in_->setText(QString::number(std::min(2, item->data(Qt::UserRole+3).toInt())));
        text_chan_out_->setText(QString::number(std::min(2, item->data(Qt::UserRole+4).toInt())));
    });

    // go button click
    connect(go_button, &QPushButton::clicked, [this]()
    {
        if (device_index_<0) return;

        AudioDevice::Properties p;
        p.deviceIndex = device_index_;
        p.sampleRate = text_rate_->text().toInt();
        p.bufferLength = text_buffer_->text().toInt();
        p.numChannelsIn = text_chan_in_->text().toInt();
        p.numChannelsOut = text_chan_out_->text().toInt();
        // issue signal
        deviceSelected(p);
        close();
    });
}


void AudioDeviceFrame::updateWidgets_()
{
    device_list_->clear();

    AudioDevices devs;
    devs.checkDevices();

    for (size_t i=0; i<devs.numDevices(); ++i)
    {
        // each device info
        auto info = devs.getDeviceInfo(i);

        // setup a list item
        auto item = new QListWidgetItem(device_list_);
        item->setData(Qt::UserRole, (int)i);
        item->setData(Qt::UserRole+1, (int)info->defaultSampleRate);
        item->setData(Qt::UserRole+2, (int)info->defaultBufferLength);
        item->setData(Qt::UserRole+3, (int)info->numInputChannels);
        item->setData(Qt::UserRole+4, (int)info->numOutputChannels);
        item->setData(Qt::DisplayRole,
                      QString::fromStdString(info->name)
                      + " " + QString::number(info->numInputChannels) + " ins / "
                      + QString::number(info->numOutputChannels) + " outs ");

        device_list_->addItem(item);
    }
}

void AudioDeviceFrame::showEvent(QShowEvent *)
{
    updateWidgets_();
}



} // namespace GUI
} // namespace CSMOD
