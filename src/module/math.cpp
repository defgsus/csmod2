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

#include "math.h"

#include <math.h>

namespace CSMOD {
namespace MODULE {

CSMOD_REGISTER_MODULE(MathOperator)
CSMOD_REGISTER_MODULE(MathUnary)

MathOperator::MathOperator(Operation op)
    : Module("Math Operator", "Math Operator"),
      op_   (op)
{

   for (int i=0; i<2; ++i)
   {
       auto in = new ValueConnector(this, Connector::IN,  "in", "in");
       inputs_.push_back( in );
       add_( in );
   }

   add_( output_ = new ValueConnector(this, Connector::OUT, "out", "out") );
}

void MathOperator::step()
{
    csfloat o = 0;
    for (auto i : inputs_)
        o += i->value();
    output_->value(o);
}

MathUnary::MathUnary(Operation op)
    : Module("Math Unary", "Math Unary"),
      op_   (op)
{

   for (int i=0; i<4; ++i)
   {
       auto in = new ValueConnector(this, Connector::IN,  "in", "in");
       inputs_.push_back( in );
       add_( in );
   }

   for (int i=0; i<4; ++i)
   {
       auto out = new ValueConnector(this, Connector::OUT,  "out", "out");
       inputs_.push_back( out );
       add_( out );
   }
}

void MathUnary::step()
{
    for (size_t i=0; i<inputs_.size(); ++i)
        outputs_[i]->value( sinf(inputs_[i]->value()) );
}

} // namespace MODULE
} // namespace CSMOD
