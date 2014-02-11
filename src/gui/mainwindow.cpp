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

#include "mainwindow.h"

#include <QLayout>
#include <QStatusBar>
#include <QPushButton>
#include <QMenu>
#include <QMenuBar>

#include "patchview.h"
#include "audiodeviceframe.h"
#include "mod/patch.h"
#include "mod/module.h"
#include "mod/model.h"
#include "mod/modulestock.h"
#include "mod/dspgraph.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    model_ =     new CSMOD::Model();
    patchview_ = new PatchView();

    setCentralWidget(patchview_);

    // XXX this is all a quick hack
    // The GUI will be it's own project...
    // and handling of Patches should go to Model someday

    auto patch_ = new CSMOD::Patch();

    model_->setPatch(patch_);
    model_->addPatchView(patchview_);
    patchview_->setModel(model_);

    patchview_->setPatch(patch_);

    // ------- main menu ---------

    auto a = menuBar()->addAction("save patch");
    connect(a, &QAction::triggered, [=]() { model_->savePatch("test.csmod"); });
    a = menuBar()->addAction("load patch");
    connect(a, &QAction::triggered, [=]() { model_->loadPatch("test.csmod"); });
    a = menuBar()->addAction("dsp graph");
    connect(a, &QAction::triggered, [=]() { patch_->updateDspGraph(); });

    auto devwin = new AudioDeviceFrame();
    devwin->show();
    /*
    auto menu = new QMenu(0);

    auto a = menu->addAction("save patch");

    menu->show();
    */
}

MainWindow::~MainWindow()
{

}
