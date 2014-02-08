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

#include "patchview.h"
#include "mod/patch.h"
#include "mod/module.h"
#include "mod/model.h"
#include "mod/modulestock.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    model_ =     new CSMOD::Model();
    patchview_ = new PatchView();

    setCentralWidget(patchview_);

    auto p = new CSMOD::Patch();

    for (int i=0; i<40; ++i)
    {
        CSMOD::Module * m = CSMOD::ModuleStock::instance().getModule("Math");
        if (!m) exit(-1);
        p->addModule( m );
    }

    model_->setPatch(p);
    model_->addPatchView(patchview_);
    patchview_->setModel(model_);

    patchview_->setPatch(p);
}

MainWindow::~MainWindow()
{

}
