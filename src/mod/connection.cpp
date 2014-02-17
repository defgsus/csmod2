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

#include "connection.h"

#include <assert.h>

#include "log.h"
#include "tool/io.h"
#include "module.h"


namespace CSMOD
{

Connection::Connection(Connector * connectorFrom, Connector * connectorTo)
        :	moduleFrom_		(connectorFrom->module()),
            moduleTo_		(connectorTo->module()),
            connectorFrom_	(connectorFrom),
            connectorTo_	(connectorTo)
{
    CSMOD_DEBUGF("Connection::Connection("
                 << connectorFrom << ", " << connectorTo << ") this="
                 << this);

    // correct order
    if (connectorFrom_->dir() == Connector::IN)
    {
        std::swap(connectorFrom_, connectorTo_);
        std::swap(moduleFrom_, moduleTo_);
    }

    // this error can not happen, unless Connector::isConnectable()
    // is bogus. However, at this point we can't resolve it, it's too late.
    // Only way out would be an exception
    if (connectorFrom_->dir() == connectorTo_->dir())
    {
        CSMOD_RT_ERROR("::Connection() invalid connection of same direction "
                       << moduleFrom_->idName() << "." << connectorFrom_->idName()
                       << " -> " << moduleTo_->idName() << "." << connectorTo_->idName());
    }

    // same goes for this error check
    if (!connectorFrom_->connectTo(connectorTo_) ||
        !connectorTo_->connectTo(connectorFrom_))
    {
        CSMOD_RT_ERROR("::Connection() can't connect "
                       << moduleFrom_->idName() << "." << connectorFrom_->idName()
                       << " -> " << moduleTo_->idName() << "." << connectorTo_->idName());
    }
}

Connection::~Connection()
{
    CSMOD_DEBUGF("Connection::~Connection() this=" << this);
}

// ------------------ IO -------------------

bool Connection::store(CSMOD::Io * io)
{
    // essential data
    if (!io->write("ver", 1)) return false;
    io->write("fm", moduleFrom_->idName());
    io->write("fc", connectorFrom_->idName());
    io->write("tm", moduleTo_->idName());
    io->write("tc", connectorTo_->idName());

    // additional data (who knows...)

    return true;
}

bool Connection::restore(CSMOD::Io * io)
{
    int ver;
    if (!io->read("ver", ver)) return false;
    if (ver > 1)
    {
        CSMOD_IO_ERROR("unknown cable version " << ver);
        return false;
    }

    // essential data has been read by Patch::restore() already

    return true;
}


// ------------- handling ----------------

void Connection::disconnect()
{
    connectorFrom_->disconnectFrom(connectorTo_);
    connectorTo_->disconnectFrom(connectorFrom_);
    connectorFrom_ = 0;
    connectorTo_ = 0;
    moduleFrom_ = 0;
    moduleTo_ = 0;
}

// ------------- debug ---------------

void Connection::debug_dump()
{
    std::cout << "connection " << this << " "
    << "from " << moduleFrom_ << "." << connectorFrom_
    << " to " << moduleTo_ << "." << connectorTo_
    //<< " (" << moduleFrom_->idName() << "." << connectorFrom_->idName() << " -> "
    //<< moduleTo_->idName() << connectorTo_->idName() << ")"
    << "\n";

}


} // namespace CSMOD
