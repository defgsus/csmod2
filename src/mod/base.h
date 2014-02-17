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

#ifndef CSMOD_MOD_BASE_H
#define CSMOD_MOD_BASE_H

#include <vector>
#include <string>

namespace CSMOD {

#define CSMOD_MAX_INPUTS (1024)


/** default float type */
typedef float csfloat;
typedef std::vector<csfloat> csfloats;

/** default double type */
typedef double csdouble;
typedef std::vector<csdouble> csdoubles;

/** default string type
    XXX not sure to use QString everywhere... */
typedef std::string String;

// ------- some forwards --------

class Io;
class Model;
class Patch;
class Module;
class Connector;
class DspConnector;
class Connection;
class Property;

typedef std::vector<Module*> Modules;
typedef std::vector<Connector*> Connectors;
typedef std::vector<DspConnector*> DspConnectors;
typedef std::vector<Connection*> Connections;

template <class T> class ValueProperty;
typedef ValueProperty<int> IntProperty;
typedef ValueProperty<csfloat> FloatProperty;
typedef ValueProperty<csdouble> DoubleProperty;
typedef ValueProperty<String> StringProperty;

template <class T> class ListProperty;

} // namespace CSMOD


#endif // CSMOD_MOD_BASE_H
