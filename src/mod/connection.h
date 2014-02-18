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

#ifndef CSMOD_MOD_CONNECTION_H
#define CSMOD_MOD_CONNECTION_H

#include <string>
#include "base.h"

namespace CSMOD
{


/** general connection type */
class Connection
{
    public:

    /** This constructor will add the Modules belonging to the Connectors
        to the list of connected Modules of each Connector.
        <p>E.g. connectorFrom->connectModule(connectorTo->module()) and
        connectorTo->connectModule(connectorFrom->module())</p>
        <p>Also it will flip From and To, if the direction does not match.</p> */
    Connection(Connector * connectorFrom, Connector * connectorTo);
    ~Connection();

    // ------------------ IO -------------------

    bool store(CSMOD::Io * io);
    bool restore(CSMOD::Io * io);

    // ------ getter ---------

    Module * moduleFrom() const { return moduleFrom_; }
    Module * moduleTo() const { return moduleTo_; }

    Connector * connectorFrom() const { return connectorFrom_; }
    Connector * connectorTo() const { return connectorTo_; }

    // ------- handling ------

    /** This will remove references from the Connectors to each other.
        Note! This has to be done before deleting any of the assigned Connectors. */
    void disconnect();

    /** Removes references to the Connector.
        The Connector is left unchanged. This only makes sense when the Connector
        is already destroyed. If the other end of the Connection is not detached from,
        then the reference to @p fromOrTo will be deleted in the other Connector. */
    void detachFrom(Connector * fromOrTo);

    // ------------- debug ---------------

    void debug_dump();

    // __________ PRIVATE ______________

    private:

    Module	* moduleFrom_,
            * moduleTo_;

    Connector
            * connectorFrom_,
            * connectorTo_;
};

} // namespace CSMOD

#endif // CSMOD_MOD_CONNECTION_H
