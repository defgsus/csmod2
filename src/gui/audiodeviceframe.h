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

#ifndef CSMOD_AUDIODEVICEFRAME_H
#define CSMOD_AUDIODEVICEFRAME_H

#include <QWidget>

class QListWidget;
class QLineEdit;

/** Displaying and selection of Audio Devices */
class AudioDeviceFrame : public QWidget
{
    Q_OBJECT
public:
    struct Properties
    {
        size_t device_index,
               sampleRate,
               bufferLength;
    };


    explicit AudioDeviceFrame(QWidget *parent = 0);

    QSize sizeHint() const { return QSize(500, 350); }
signals:

    void deviceSelected(Properties);

public slots:

private:
    void updateWidgets_();

    int device_index_;

    QListWidget * device_list_;
    QLineEdit * text_rate_,
              * text_buffer_;
};

#endif // CSMOD_AUDIODEVICEFRAME_H
