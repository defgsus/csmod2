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

#ifndef CSMOD_MODULE_MATH_H
#define CSMOD_MODULE_MATH_H

#include "mod/module.h"
#include "mod/modulestock.h"

namespace CSMOD {
namespace MODULE {

/* NOTE: Don't take this serious.
 * Currently i'm playing around with Modules to see
 * what patterns work best. */

class MathOperator : public Module
{
public:
    enum Operation
    {
        O_ADD,
        O_SUB,
        O_MUL,
        O_DIV
    };

    MathOperator(Operation op = O_ADD);

    virtual MathOperator * cloneClass() const { return new MathOperator(op_); }

    virtual void step();

protected:
    Operation op_;

    std::vector<ValueConnector*> inputs_;
    ValueConnector * output_;
};


class MathUnary : public Module
{
public:
    enum Operation
    {
        O_SIN,
        O_COS,
        O_ABS,
        O_NEG
    };

    MathUnary(Operation op = O_SIN);

    virtual MathUnary * cloneClass() const { return new MathUnary(op_); }

    virtual void step();

protected:
    Operation op_;

    std::vector<ValueConnector*>
        inputs_, outputs_;
};


} // namespace MODULE
} // namespace CSMOD



int numModules() { return 23; }

#endif // CSMOD_MODULE_MATH_H
