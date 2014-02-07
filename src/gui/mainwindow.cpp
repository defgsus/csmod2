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

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    //auto sb = new QStatusBar();
    //setStatusBar(sb);

    //auto l0 = new QVBoxLayout(centralWidget());

    //auto b = new QPushButton(centralWidget());
    patchview_ = new PatchView();
    setCentralWidget(patchview_);
    //l0->addWidget(patchview_);

    auto p = new CSMOD::Patch();
    for (int i=0; i<40; ++i)
        p->addModule(new CSMOD::TestModule);
    patchview_->setPatch(p);
}

MainWindow::~MainWindow()
{

}
