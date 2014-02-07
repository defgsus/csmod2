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

#ifndef LOG_H
#define LOG_H

#include <iostream>

// ------------------------ debugging ---------------------------

#define CSMOD_DEBUG(arg__) { std::cerr << arg__ << std::endl; }

#if (1)
#define CSMOD_DEBUGF(arg__) { std::cerr << arg__ << std::endl; }
#else
#define CSMOD_DEBUGF(unused__) { }
#endif

#if (0)
#define CSMOD_DEBUGE(arg__) { std::cerr << arg__ << std::endl; }
#else
#define CSMOD_DEBUGE(unused__) { }
#endif

// ------------------- error reporting --------------------------

#define CSMOD_RT_ERROR(arg__) { std::cerr << "RT ERROR: " << arg__ << std::endl; }

#endif // LOG_H
