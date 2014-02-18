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

#include "io.h"

#include <QDebug>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <QFile>

#include "log.h"

namespace CSMOD {


Io::Io()
    :   xmlw_   (0),
        xmlr_   (0),
        data_   (new QString)
{
    CSMOD_DEBUGF("Io::Io()");
}

Io::~Io()
{
    CSMOD_DEBUGF("Io::~Io()");
    if (xmlw_) delete xmlw_;
    if (xmlr_) delete xmlr_;
    if (data_) delete data_;
}


// -------------- file io -----------------------

bool Io::save(const String& filename)
{
    QFile f(QString::fromStdString(filename));
    if (!f.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        CSMOD_IO_ERROR("could not save '" << filename << "'");
        return false;
    }

    QTextStream out(&f);
    out << *data_;

    f.close();
    return true;
}

bool Io::load(const String& filename)
{
    QFile f(QString::fromStdString(filename));
    if (!f.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        CSMOD_IO_ERROR("could not load '" << filename << "'");
        return false;
    }

    QTextStream in(&f);
    *data_ = in.readAll();

    f.close();
    return true;
}


// -------------------- io ----------------------

bool Io::startWriting()
{
    CSMOD_DEBUGF("Io::startWriting()");

    data_->clear();
    if (xmlw_) delete xmlw_;
    xmlw_ = new QXmlStreamWriter(data_);
    xmlw_->setAutoFormatting(true);
    xmlw_->writeStartDocument();
    return newSection("csmod-io");
}

bool Io::stopWriting()
{
    CSMOD_DEBUGF("Io::stopWriting()");

    if (!xmlw_) return false;

    xmlw_->writeEndElement();
    xmlw_->writeEndDocument();

    delete xmlw_;
    xmlw_ = 0;

    cur_section_ = "";

    qDebug() << *data_ << "\n";
    return true;
}

bool Io::writeable() { return xmlw_; }

bool Io::startReading()
{
    CSMOD_DEBUGF("Io::startReading()");

    if (xmlr_) delete xmlr_;
    xmlr_ = new QXmlStreamReader(*data_);
    if (!(xmlr_->readNextStartElement() &&
          xmlr_->name() == "csmod-io"))
        return false;
    cur_section_ = "csmod-io";
    section_stack_.clear();
    return true;
}

bool Io::stopReading()
{
    CSMOD_DEBUGF("Io::stopReading()");

    if (!xmlr_) return false;
    delete xmlr_;
    xmlr_ = 0;
    return true;
}

bool Io::readable() { return xmlr_; }

// ------------------ sections ------------------

bool Io::newSection(const String& name)
{
    if (!xmlw_) return false;
    xmlw_->writeStartElement(QString::fromStdString(name));
    section_stack_.push_back(cur_section_);
    cur_section_ = name;
    return true;
}

bool Io::endSection()
{
    if (!xmlw_) return false;
    xmlw_->writeEndElement();
    // get previous section
    if (!section_stack_.empty())
    {
        cur_section_ = section_stack_.back();
        section_stack_.pop_back();
    }
    else cur_section_ = "";
    return true;
}

bool Io::isSection(const String& name) const
{
    return (name == cur_section_);
}

const String& Io::section() const
{
    return cur_section_;
}

bool Io::nextSubSection()
{
    if (!xmlr_) return false;
    cur_section_ = "";
    //qDebug() << ":: " << xmlr_->name().toString() << "\n";
    if (!xmlr_->readNextStartElement()) return false;
    cur_section_ = xmlr_->name().toString().toStdString();
    return true;
}

bool Io::leaveSection()
{
    if (!xmlr_->isEndElement())
        xmlr_->skipCurrentElement();
    cur_section_ = xmlr_->name().toString().toStdString();
    return true;
}

// ----------------- data write -----------------

bool Io::write(const String& key, const String& v)
{
    if (!xmlw_) return false;
    xmlw_->writeAttribute(QString::fromStdString(key), QString::fromStdString(v));
    return true;
}

bool Io::write(const String& key, int v)
{
    if (!xmlw_) return false;
    xmlw_->writeAttribute(QString::fromStdString(key), QString::number(v));
    return true;
}

bool Io::write(const String& key, unsigned int v)
{
    if (!xmlw_) return false;
    xmlw_->writeAttribute(QString::fromStdString(key), QString::number(v));
    return true;
}

bool Io::write(const String& key, long int v)
{
    if (!xmlw_) return false;
    xmlw_->writeAttribute(QString::fromStdString(key), QString::number(v));
    return true;
}

bool Io::write(const String& key, long unsigned int v)
{
    if (!xmlw_) return false;
    xmlw_->writeAttribute(QString::fromStdString(key), QString::number(v));
    return true;
}

bool Io::write(const String& key, float v)
{
    if (!xmlw_) return false;
    xmlw_->writeAttribute(QString::fromStdString(key), QString::number(v));
    return true;
}

bool Io::write(const String& key, double v)
{
    if (!xmlw_) return false;
    xmlw_->writeAttribute(QString::fromStdString(key), QString::number(v));
    return true;
}

// ----------------- data read ------------------

#define CSMOD_IO_CHECK_ATTRIBUTE(key__, value__, default__) \
    QString k__ = QString::fromStdString(key__); \
    if ((!xmlr_) || \
        (!xmlr_->attributes().hasAttribute(k__))) \
    { \
        value__ = default__; \
        return false; \
    } \
    const auto a = xmlr_->attributes().value(k__);

bool Io::read(const String& key, String& v, const String& def) const
{
    CSMOD_IO_CHECK_ATTRIBUTE(key, v, def);
    v = a.toString().toStdString();
    return true;
}

bool Io::read(const String& key, int& v, int def) const
{
    CSMOD_IO_CHECK_ATTRIBUTE(key, v, def);
    v = a.toInt();
    return true;
}

bool Io::read(const String& key, unsigned int& v, unsigned int def) const
{
    CSMOD_IO_CHECK_ATTRIBUTE(key, v, def);
    v = a.toUInt();
    return true;
}

bool Io::read(const String& key, long int& v, long int def) const
{
    CSMOD_IO_CHECK_ATTRIBUTE(key, v, def);
    v = a.toInt();
    return true;
}

bool Io::read(const String& key, long unsigned int& v, long unsigned int def) const
{
    CSMOD_IO_CHECK_ATTRIBUTE(key, v, def);
    v = a.toUInt();
    return true;
}

bool Io::read(const String& key, float& v, float def) const
{
    CSMOD_IO_CHECK_ATTRIBUTE(key, v, def);
    v = a.toFloat();
    return true;
}

bool Io::read(const String& key, double& v, double def) const
{
    CSMOD_IO_CHECK_ATTRIBUTE(key, v, def);
    v = a.toDouble();
    return true;
}




// --------------------------- test -------------------------------

void testIo()
{
    Io io;

    io.startWriting();

    io.newSection("patch");
        io.write("version", "1");
        io.newSection("module");
            io.write("class", "Math");
            io.write("id", "Math1");
            io.write("name", "Mathematik");
            io.newSection("param");
                io.write("id", "num_in");
                io.write("v", 2);
            io.endSection();
        io.endSection();
        io.newSection("connections");
            io.newSection("c");
            io.write("fm", "Math1");
            io.write("fc", "out1");
            io.write("tm", "Math1");
            io.write("tc", "in1");
            io.endSection();
        io.endSection();
    io.endSection();

    io.stopWriting();

    qDebug("-------------");

    io.startReading();

    io.nextSubSection();
    if (io.section() != "patch") { std::cout << "expected patch\n"; exit(-1); }

    std::cout << "version " << io.readInt("version") << std::endl;



    io.stopReading();
}





} // namespace CSMOD
