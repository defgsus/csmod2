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

namespace CSMOD
{

class Module;
class Connector;

/** abstract connection type */
class Connection
{
    public:

    Connection(Connector * connectorFrom, Connector * connectorTo);

    // ------ getter ---------

    Module * moduleFrom() const { return moduleFrom_; }
    Module * moduleTo() const { return moduleTo_; }

    Connector * connectorFrom() const { return connectorFrom_; }
    Connector * connectorTo() const { return connectorTo_; }

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
