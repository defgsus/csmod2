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

#include <vector>

#include "mod/base.h"

class QString;
class QXmlStreamWriter;
class QXmlStreamReader;

namespace CSMOD {


/** @brief serializer and deserializer
    <p>The backend is QXmlStream, so this is actually
    a convenience wrapper around it.</p>
*/
class Io
{
public:

    Io();
    ~Io();

    // -------------- file io -----------------------

    bool save(const String& filename);
    bool load(const String& filename);

    // --------------- streaming --------------------

    bool startWriting();
    bool stopWriting();
    bool writeable();

    bool startReading();
    bool stopReading();
    bool readable();

    // ------------------ sections ------------------

    /** Create a new (sub-)section. WRITE */
    bool newSection(const String& name);

    /** Ends the current sub-section and goes to containing section. WRITE */
    bool endSection();

    /** Returns if the current section matches @p name. READ/WRITE */
    bool isSection(const String& name) const;

    /** Returns name of the current section. READ/WRITE */
    const String& section() const;

    /** open next sub-section. READ */
    bool nextSubSection();

    /** leave current (sub-)section. READ */
    bool leaveSection();

    // ----------------- data write -----------------

    bool write(const String& key, const String& v);
    bool write(const String& key, bool v);
    bool write(const String& key, int v);
    bool write(const String& key, unsigned int v);
    bool write(const String& key, long int v);
    bool write(const String& key, long unsigned int v);
    bool write(const String& key, float);
    bool write(const String& key, double);

    // ----------------- data read ------------------

    bool read(const String& key, String& v, const String& def = "") const;
    bool read(const String& key, bool& v, bool def = false) const;
    bool read(const String& key, int& v, int def = 0) const;
    bool read(const String& key, unsigned int& v, unsigned int def = 0) const;
    bool read(const String& key, long int& v, long int def = 0) const;
    bool read(const String& key, long unsigned int& v, long unsigned int def = 0) const;
    bool read(const String& key, float& v, float def = 0) const;
    bool read(const String& key, double& v, double def = 0) const;

    String readString(const String& key, const String& def = "") const { String v; read(key, v, def); return v; }
    bool readBool(const String& key, bool def = false) const { bool v; read(key, v, def); return v; }
    int readInt(const String& key, int def = 0) const { int v; read(key, v, def); return v; }
    unsigned int readUInt(const String& key, unsigned int def = 0) const { unsigned int v; read(key, v, def); return v; }
    long int readLInt(const String& key, long int def = 0) const { long int v; read(key, v, def); return v; }
    long unsigned int readLUInt(const String& key, long unsigned int def = 0) const { long unsigned int v; read(key, v, def); return v; }
    float readFloat(const String& key, float def = 0) const { float v; read(key, v, def); return v; }
    double readDouble(const String& key, double def = 0) const { double v; read(key, v, def); return v; }

    // _______________ PRIVATE AREA _________________

private:
    String cur_section_;
    std::vector<String> section_stack_;

    QXmlStreamWriter * xmlw_;
    QXmlStreamReader * xmlr_;
    QString * data_;
};


void testIo();

} // namespace CSMOD

#endif // CSMOD_TOOL_IO_H
