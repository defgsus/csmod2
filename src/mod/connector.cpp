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

Connector::~Connector()
{
    CSMOD_DEBUGF("Connector::~Connector() this=" << this);
}

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

    // keep link to other connector
    cons_.push_back(con);
    // keep track of associated modules
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
    bool another = false;
    for (auto &c : cons_)
        if (c->module() == con->module())
            { another = true; break; }

    if (!another)
    {
        // delete the module from list
        for (auto m = modules_.begin(); m!=modules_.end(); ++m)
        {
            if (*m == con->module())
            {
                modules_.erase(m);
                break;
            }
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

// ------------- debug ---------------

void Connector::debug_dump()
{
    std::cout << "connector " << this << " " << idName() << "\n";
}





// ------------------------ DspConnector ---------------------------

DspConnector::DspConnector(Module * module, Direction dir,
                           const std::string& idname, const std::string& name)
    : Connector         (module, dir, idname, name),
      blockSize_        (0),
      dsp_block_ptr_    (0),
      do_sum_dsp_inputs_(false)
{
    updateDspStorage_();
}


void DspConnector::setBlockSize(size_t size)
{
    blockSize_ = size;
    updateDspStorage_();
}


// ----------------------- value ------------------------

csfloat DspConnector::blockAverage() const
{
    if (!dsp_block_ptr_ || !blockSize_) return 0.f;

    csfloat sum = 0.f;
    for (size_t i=0; i<blockSize_; ++i)
        sum += dsp_block_ptr_[i];

    return sum / blockSize_;
}

// ---------------------- runtime -----------------------

void DspConnector::updateDspStorage_()
{
    CSMOD_DEBUGF("DspConnector::updateDspStorage_() this=" << this);

    // outputs, multiple inputs or unconnected inputs
    // need their own storage
    if (dir() == OUT || numConnections() != 1)
    {
        dsp_block_.resize(blockSize_);
        dsp_block_ptr_ = &dsp_block_[0];
        // multiple INs need to be summed
        do_sum_dsp_inputs_ = (dir() == IN && numConnections() > 1);
    }
    // single input connectors can reuse the
    // storage of another output
    else
    {
        do_sum_dsp_inputs_ = false;

        // clear storage memory
        csfloats tmp; tmp.swap(dsp_block_);

        // point to the incomming output Connector
        auto dspcon = dynamic_cast<DspConnector*>(cons_[0]);
        if (dspcon)
            dsp_block_ptr_ = &dspcon->dsp_block_[0];
        else
        {
            // unlikely unless isConnectable() is screwed
            CSMOD_RT_ERROR("DspConnector not fed by DspConnector "
                           << cons_[0]->longIdName() << " -> " << longIdName());
            dsp_block_ptr_ = 0;
            exit(-1);
        }
    }
}

void DspConnector::updateInputs()
{
    // NOTE: We don't use dynamic_cast
    // because we assume that all connected Connectors are
    // also DspConnectors. There have been checks already
    // and this is a high-performance function.

    //if (dir() == IN && numConnections() > 1)
    if (do_sum_dsp_inputs_)
    {
        // fill the dsp_block_ with contents from first DspConnector
        csfloats::iterator b;
        const csfloat * bf = static_cast<DspConnector*>(cons_[0])->block();
        for (b = dsp_block_.begin(); b != dsp_block_.end(); ++b, ++bf)
            *b = *bf;

        // add the other DspConnectors
        for (size_t i=1; i<numConnections(); ++i)
        {
            bf = static_cast<DspConnector*>(cons_[i])->block();
            for (b = dsp_block_.begin(); b != dsp_block_.end(); ++b, ++bf)
                *b += *bf;
        }
    }
}


// ------------- debug ---------------

void DspConnector::debug_dump()
{
    std::cout << "dspconnector " << this << " " << idName()
              << " blocks=" << blockSize_;
    if (dsp_block_.size()) std::cout
            << " block.size=" << dsp_block_.size() << " &block[0]=" << &dsp_block_[0];
    std::cout << " blockptr=" << dsp_block_ptr_
              << "\n";
}

} // namespace CSMOD
