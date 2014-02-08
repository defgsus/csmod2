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

#ifndef PATCHVIEW_H
#define PATCHVIEW_H

#include <set>
#include <sstream>

#include <QFrame>
#include <QLabel>

// --- forwards ---

namespace CSMOD {
class Io;
class Module;
class Connection;
class Connector;
class Patch;
class Model;
}

class PatchGraphicsView;
class ModuleItem;
class ConnectorItem;
class CableItem;
class ModuleStockMenu;

#ifndef CSMOD_PATCH_INFO
#define CSMOD_PATCH_INFO(arg__) \
{   \
    std::stringstream s__; s__ << arg__; \
    setInfo(s__.str()); \
}
#endif


/** @brief direct view into a patch */
class PatchView : public QFrame
{
    Q_OBJECT
public:
    explicit PatchView(QWidget *parent = 0);

    QSize sizeHint() const;

    void setModel(CSMOD::Model * model);

    /** Assigns a patch for editing/viewing.
        Set to NULL to deconnect. */
    void setPatch(CSMOD::Patch * patch);

    /** update changes in patch */
    void updateFromPatch();

    /** update all cable positions */
    void updateCables();
    /** update all cables connected to this module */
    void updateCables(CSMOD::Module * mod);

    /** reset the info string in the toolbar */
    void setInfo(const std::string& info);

    // ------------------ IO -------------------

    bool store(CSMOD::Io * io);
    bool restore(CSMOD::Io * io);

signals:

public slots:

protected:
    // ---------------- module items -------------------

    /** return the ModuleItem for the Module, or NULL */
    ModuleItem * findModuleItem_(CSMOD::Module * mod);

    /** return the ModuleItem for the Module with the idName, or NULL */
    ModuleItem * findModuleItem_(const std::string& idName);

    /** create and install a ModuleItem for the module */
    ModuleItem * createModuleItem_(CSMOD::Module * mod);

    // ------------- connector items -------------------

    /** return the ConnectorItem for the Connector, or NULL */
    ConnectorItem * findConnectorItem_(CSMOD::Connector * con);

    // -------------- connection items -----------------

    /** Returns the installed CableItem, or NULL */
    CableItem * findCableItem_(CSMOD::Connection * con);

    /** create and install a CableItem for the Connection.
        @note The referred ModuleItem must exist already,
        if not, NULL will be returned. */
    CableItem * createCableItem_(CSMOD::Connection * con);

    // ----------------- event -------------------------
    /*
    virtual void mousePressEvent(QMouseEvent *);
    virtual void mouseMoveEvent(QMouseEvent *);
    virtual void mouseReleaseEvent(QMouseEvent *);
    */
    //virtual void paintEvent(QPaintEvent *);

    // __________ PROTECTED MEMBER __________________

    CSMOD::Model * model_;
    CSMOD::Patch * patch_;
    PatchGraphicsView * pview_;

    std::set<ModuleItem*> moduleitems_;
    std::set<CableItem*> cableitems_;

    std::stringstream infostream_;

    ModuleStockMenu * stockmenu_;
    QLabel * infoLabel_;
};

#endif // PATCHVIEW_H
