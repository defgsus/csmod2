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

#include "patchview.h"

#include <iostream>

#include <QPainter>
#include <QPalette>
#include <QScrollArea>
#include <QLayout>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QToolBar>
#include <QPushButton>
#include <QAction>

#include "log.h"
#include "tool/io.h"
#include "mod/module.h"
#include "mod/connection.h"
#include "mod/patch.h"
#include "mod/model.h"
#include "connectoritem.h"
#include "moduleitem.h"
#include "cableitem.h"
#include "patchgraphicsview.h"
#include "modulestockmenu.h"


PatchView::PatchView(QWidget *parent) :
    QFrame(parent),
    model_  (0),
    patch_  (0),
    pview_  (0)
{
    CSMOD_DEBUGF("PatchView::PatchView(" << parent << ")");

    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    setFrameStyle(QFrame::Panel | QFrame::Sunken);
    setLineWidth(2);
/*
    // colors
    QPalette pal = palette();
    pal.setColor(QPalette::Base, QColor(40,40,40));
    setPalette(pal);
*/
    // setup stockmenu
    stockmenu_ = new ModuleStockMenu(this);
    connect(stockmenu_, &ModuleStockMenu::moduleSelected,
            [=](const std::string& idName)
    {
        model_->createModule(patch_, idName);
    });

    // layout
    auto l0 = new QVBoxLayout(this);
    l0->setContentsMargins(lineWidth(), lineWidth(),
                           lineWidth(), lineWidth());

        // --- toolbar ---
        auto tb = new QToolBar(this);
        l0->addWidget(tb);

            // stockmenu button
            auto b = new QPushButton();
            tb->addWidget( b );
            b->setText("+");
            b->setFixedWidth(tb->height()-4);
            connect(b, &QPushButton::clicked, [=]()
            {
                stockmenu_->popup(b->mapToGlobal(b->pos()));
            });

            tb->addSeparator();

            // info label
            tb->addWidget(infoLabel_ = new QLabel("info", tb));

        // actual patch view
        pview_ = new PatchGraphicsView(this, this);
        l0->addWidget(pview_);

}

QSize PatchView::sizeHint() const
{
    return QSize(800,600);
}

void PatchView::setInfo(const std::string& str)
{
    infoLabel_->setText(QString::fromStdString(str));
}


// ------------------ IO -------------------

bool PatchView::store(CSMOD::Io * io)
{
    if (!io->newSection("patchview")) return false;
    io->write("version", 1);

    // stream all moduleitems
    io->write("moditems", moduleitems_.size());
    for (auto i=moduleitems_.begin(); i!=moduleitems_.end(); ++i)
    {
        io->newSection("moditem");
        (*i)->store(io);
        io->endSection();
    }

    return io->endSection();
}

bool PatchView::restore(CSMOD::Io * io)
{
    int ver = io->readInt("version", 0);
    if (ver > 1)
    {
        CSMOD_RT_ERROR("unknown patchview version " << ver << ")");
        return false;
    }

    size_t num;
    if (!io->read("moditems",num)) return false;
    for (size_t i=0; i<num; ++i)
    {
        if (!(io->nextSection() &&
              io->section() == "moditem"))
        {
            CSMOD_RT_ERROR("expected moditem");
            return false;
        }
        // get the ModuleItem meant by current section
        auto mi = findModuleItem_(io->readString("id"));
        if (!mi) continue;
        mi->restore(io);
    }
    return true;
}





void PatchView::setPatch(CSMOD::Patch * patch)
{
    CSMOD_DEBUGF("PatchView::setPatch(" << patch << ")");

    // set patch
    patch_ = patch;
    pview_->setPatch(patch_);

    // clear scene
    pview_->scene()->clear();

    // clear quicklists
    moduleitems_.clear();
    cableitems_.clear();

    // create ModuleItem for each Module
    for (auto m : patch_->modules())
    {
        createModuleItem_(m);
    }

    // create CableItems for each Connection
    for (auto c : patch_->connections())
    {
        createCableItem_(c);
    }

    updateCables();
}

void PatchView::setModel(CSMOD::Model * model)
{
    CSMOD_DEBUGF("PatchView::setModel(" << model << ")");

    model_ = model;
    pview_->setModel(model);
}

void PatchView::updateFromPatch()
{
    CSMOD_DEBUGF("PatchView::updateFromPatch()");

    // update ModuleItem for each Module
    for (auto m : patch_->modules())
    {
        // find ModuleItem
        auto mitem = findModuleItem_(m);

        if (!mitem)
        {   // not there? create one
            mitem = createModuleItem_(m);
        }

        // update TODO
    }

    // remove all ModuleItems that are not needed
    for (auto mi = moduleitems_.begin(); mi!=moduleitems_.end(); ++mi)
    {
        bool present = false;

        for (auto m : patch_->modules())
            if (m == (*mi)->module())
                { present = true; break; }

        if (!present)
        {
            // remove from everywhere
            pview_->scene()->removeItem(*mi);
            moduleitems_.erase(*mi);
            delete *mi;
        }
    }

    // update CableItem for each Connection
    for (auto c : patch_->connections())
    {
        // find ModuleItem
        auto citem = findCableItem_(c);

        if (!citem)
        {   // not there? create one
            citem = createCableItem_(c);
        }
    }

    // remove all CableItems that are not needed
    for (auto ci = cableitems_.begin(); ci!=cableitems_.end(); ++ci)
    {
        bool present = false;

        for (auto c : patch_->connections())
            if (c == (*ci)->connection())
                { present = true; break; }

        if (!present)
        {
            // remove from everywhere
            pview_->scene()->removeItem(*ci);
            cableitems_.erase(*ci);
            delete *ci;
        }
    }

    updateCables();
}



void PatchView::updateCables()
{
    CSMOD_DEBUGF("PatchView::updateCables()");

    for (auto ci : cableitems_)
    {
        ci->updatePos();
    }
}

void PatchView::updateCables(CSMOD::Module * mod)
{
    CSMOD_DEBUGF("PatchView::updateCables(" << mod << ")");

    for (auto ci : cableitems_)
    if (ci->connection()->connectorFrom()->module() == mod
     || ci->connection()->connectorTo()->module() == mod)
    {
        ci->updatePos();
    }
}

// ---------------------- modules -------------------------------



// ---------------------- module items --------------------------

ModuleItem * PatchView::createModuleItem_(CSMOD::Module * mod)
{
    CSMOD_DEBUGF("PatchView::createModuleItem(" << mod << ")");

    // create
    auto mitem = new ModuleItem(mod, pview_);

    // add to graphic scene
    pview_->scene()->addItem(mitem);
    // position
    mitem->setPos(10 + moduleitems_.size()*120, 10);
    // add to quick access list
    moduleitems_.insert(mitem);

    return mitem;
}

ModuleItem * PatchView::findModuleItem_(CSMOD::Module * mod)
{
    for (auto m : moduleitems_)
    {
        if (m->module() == mod) return m;
    }
    return 0;
}

ModuleItem * PatchView::findModuleItem_(const std::string& idName)
{
    for (auto m : moduleitems_)
    {
        if (m->module()->idName() == idName) return m;
    }
    return 0;
}

// ---------------------- connector items --------------------------

ConnectorItem * PatchView::findConnectorItem_(CSMOD::Connector * con)
{
    // find the module for this Connector
    ModuleItem * mi = findModuleItem_(con->module());
    if (!mi) return 0;

    // find the ConnectorItem
    for (auto i : mi->childItems())
    if (auto ci = dynamic_cast<ConnectorItem*>(i))
    {
        if (ci->connector() == con) return ci;
    }
    return 0;
}


// -------------- connection items -----------------

CableItem * PatchView::findCableItem_(CSMOD::Connection * con)
{
    for (auto c : cableitems_)
    {
        if (c->connection() == con) return c;
    }
    return 0;

}

/** create and install a CableItem for the Connection */
CableItem * PatchView::createCableItem_(CSMOD::Connection * con)
{
    CSMOD_DEBUGF("PatchView::createConnectionItem(" << con << ")");

    auto c1 = findConnectorItem_(con->connectorFrom()),
         c2 = findConnectorItem_(con->connectorTo());
    if (!c1 || !c2)
    {
        CSMOD_RT_ERROR("PatchView::createConnectionItem(" << con << ") "
                       "called, but ConnectorItems are missing.");
        return 0;
    }

    // create
    auto citem = new CableItem(con, c1, c2, pview_);

    // add to graphic scene
    pview_->scene()->addItem(citem);
    // set position
    citem->updatePos();
    // add to quick access list
    cableitems_.insert(citem);

    return citem;
}


/*
void PatchView::paintEvent(QPaintEvent * event)
{
    QFrame::paintEvent(event);

    QPainter p(this);
    p.setBrush(QBrush(QColor(200,200,200)));
    p.drawLine(QLineF(0,0,width(),height()));
}


void PatchView::mousePressEvent(QMouseEvent * e)
{
    QFrame::mousePressEvent(e);
    //update();
}

void PatchView::mouseMoveEvent(QMouseEvent * e)
{
    QFrame::mouseMoveEvent(e);
}

void PatchView::mouseReleaseEvent(QMouseEvent * e)
{
    QFrame::mouseReleaseEvent(e);
}
*/
