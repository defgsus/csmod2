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

#include <QApplication>

#include "gui/mainwindow.h"

//#include "mod/modulestock.h"
//#include "tool/stringdigit.h"
//#include "tool/io.h"

#if (0)
#include "audio/audiodevices.h"
#include "audio/audiodevice.h"
#include <functional>
void some(const float * in, float * out, size_t chan, size_t length)
{
    static long int sam = 0;
    for (size_t i=0; i<length; ++i)
    {
        float t = (float)sam++ * 6.28 * 200 / 44100.f;

        for (size_t j=0; j<chan; ++j)
            *out++ = 0.3f * sin(t);
    }
}
void testAudio()
{
    using namespace CSMOD;
    using namespace std::placeholders;

    AudioDevices devs;
    devs.checkDevices();
    devs.dump_info();

    AudioDevice dev;
    dev.init(3, 2,2, 44100, 512);
    dev.setCallback(std::bind(some, _1, _2, dev.numOutputChannels(), dev.bufferLength()));
    dev.start();

    //usleep(1000 * 10);
    sleep(1);

    dev.stop();
    dev.close();
}
#endif



int main(int argc, char *argv[])
{
    //testAudio(); return 0;
    //CSMOD::testIo(); return 0;
    /*
    String s("bla"), s2(s);
    CSMOD::increase_number(s2, 0);
    std::cout << s << ": " << s2 << std::endl;
    return 0;
    */

    QApplication a(argc, argv);

    //CSMOD::ModuleStock::instance().addDll("/home/defgsus/prog/qt_project/build-testbullet-Desktop_Qt_5_1_1_GCC_64bit-Debug/math.o");

    MainWindow w;
    w.show();

    return a.exec();

}
