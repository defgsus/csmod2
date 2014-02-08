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

#ifndef CSMOD_TOOL_IO_H
#define CSMOD_TOOL_IO_H

#include <cinttypes>
#include <string>
#include <QString>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>

namespace CSMOD {


/** @brief serializer and deserializer - abstract */
class Io
{
public:

    Io();
    ~Io();

    // --------------- io ---------------------------

    bool startWriting();
    bool stopWriting();
    bool writeable();

    bool startReading();
    bool stopReading();
    bool readable();

    // ------------------ sections ------------------

    /** Create a new (sub-)section. WRITE */
    bool newSection(const std::string& name);

    /** Ends the current sub-section and goes to containing section. WRITE */
    bool endSection();

    /** Returns if the current section matches @p name. READ/WRITE */
    bool isSection(const std::string& name) const;

    /** Returns name of the current section. READ/WRITE */
    const std::string& section() const;

    /** skip current section. READ */
    bool nextSection();

    // ----------------- data write -----------------

    bool write(const std::string& key, const std::string& v);
    bool write(const std::string& key, int v);
    bool write(const std::string& key, unsigned int v);
    bool write(const std::string& key, long int v);
    bool write(const std::string& key, long unsigned int v);
    bool write(const std::string& key, float);
    bool write(const std::string& key, double);

    // ----------------- data read ------------------

    bool read(const std::string& key, std::string& v, const std::string& def = "") const;
    bool read(const std::string& key, int& v, int def = 0) const;
    bool read(const std::string& key, unsigned int& v, unsigned int def = 0) const;
    bool read(const std::string& key, long int& v, long int def = 0) const;
    bool read(const std::string& key, long unsigned int& v, long unsigned int def = 0) const;
    bool read(const std::string& key, float& v, float def = 0) const;
    bool read(const std::string& key, double& v, double def = 0) const;

    int readInt(const std::string& key, int def = 0) const { int v; read(key, v, def); return v; }
    int readUInt(const std::string& key, unsigned int def = 0) const { unsigned int v; read(key, v, def); return v; }
    int readLInt(const std::string& key, long int def = 0) const { long int v; read(key, v, def); return v; }
    int readLUInt(const std::string& key, long unsigned int def = 0) const { long unsigned int v; read(key, v, def); return v; }
    int readFloat(const std::string& key, float def = 0) const { float v; read(key, v, def); return v; }
    int readDouble(const std::string& key, double def = 0) const { double v; read(key, v, def); return v; }

    // _______________ PRIVATE AREA _________________

private:
    std::string cur_section_;

    QXmlStreamWriter * xmlw_;
    QXmlStreamReader * xmlr_;
    QString data_;
};


void testIo();

} // namespace CSMOD

#endif // CSMOD_TOOL_IO_H
