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

#include "connector.h"

#include "log.h"
#include "module.h"


namespace CSMOD {

// --------------- info -------------

std::string Connector::longIdName() const
{
    return module_->idName() + "." + idName_;
}

std::string Connector::longName() const
{
    return module_->name() + "." + name_;
}

// -------- connections -------------

bool Connector::connectTo(Connector * con)
{
    if (isConnectedTo(con))
    {
        CSMOD_RT_WARN("duplicate request of connection "
                      << longIdName() << " -> " << con->longIdName());
        return false;
    }
    // keep track
    cons_.push_back(con);
    if (!isConnectedTo(con->module()))
        modules_.push_back(con->module());
    return true;
}

bool Connector::disconnectFrom(Connector * con)
{
    // delete the connector from list
    bool found = false;
    for (auto c = cons_.begin(); c!=cons_.end(); ++c)
    {
        if (*c == con)
        {
            cons_.erase(c);
            found = true;
            break;
        }
    }

    if (!found) return false;

    // see if we stay connected to the Module
    // (another Connector on the same Module)
    for (auto &c : cons_)
        if (c->module() == con->module())
            return true;

    // delete the module from list
    for (auto m = modules_.begin(); m!=modules_.end(); ++m)
    {
        if (*m == con->module())
        {
            modules_.erase(m);
            return true;
        }
    }
    return true;
}

bool Connector::isConnectedTo(Connector * con) const
{
    for (auto &c : cons_)
        if (c == con) return true;
    return false;
}

bool Connector::isConnectedTo(Module * module) const
{
    for (auto &m : modules_)
        if (m == module) return true;
    return false;
}






// ------------------------ DspConnector ---------------------------

void DspConnector::setBlockSize(size_t size)
{
    blockSize_ = size;
    updateDspData_();
}

bool DspConnector::connectTo(Connector * con)
{
    if (!Connector::connectTo(con)) return false;
    updateDspData_();
    return true;
}

bool DspConnector::disconnectFrom(Connector * con)
{
    if (!Connector::disconnectFrom(con)) return false;
    updateDspData_();
    return true;
}

void DspConnector::updateDspData_()
{
    // outputs or multiple inputs need own storage
    if (dir() == OUT || numConnections() > 1)
    {
        dsp_block_.resize(blockSize_);
        dsp_block_ptr_ = &dsp_block_[0];
    }
    // single input connectors can reuse the
    // storage of another output
    else
    {
        // clear memory
        csfloats tmp; tmp.swap(dsp_block_);
        // point to the connected output Connector
        auto dspcon = dynamic_cast<DspConnector*>(cons_[0]);
        if (dspcon)
            dsp_block_ptr_ = &dspcon->dsp_block_[0];
        else
        {
            // unlikely unless isConnectable() is screwed
            CSMOD_RT_ERROR("DspConnector not fed by DspConnector");
            exit(-1);
        }
    }
}


} // namespace CSMOD
