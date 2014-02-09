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
    assert(connectorFrom_->dir() != connectorTo_->dir() && "invalid connection of same direction");
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



} // namespace CSMOD
