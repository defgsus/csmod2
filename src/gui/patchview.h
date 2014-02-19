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
#include <QTimer>

#include "base.h"
#include "palette.h"

#ifndef CSMOD_PATCH_INFO
#define CSMOD_PATCH_INFO(arg__) \
{   \
    std::stringstream s__; s__ << arg__; \
    setInfo(s__.str()); \
}
#endif

class QHBoxLayout;

namespace CSMOD {
namespace GUI {

/** @brief direct view into a patch */
class PatchView : public QFrame
{
    Q_OBJECT
public:
    explicit PatchView(QWidget *parent = 0);

    QSize sizeHint() const;

    // ------------------ IO -------------------

    bool store(CSMOD::Io * io);
    bool restore(CSMOD::Io * io);

    // --------- association -------------------

    void setModel(CSMOD::Model * model);

    /** Assigns a patch for editing/viewing.
        Set to NULL to deconnect. */
    void setPatch(CSMOD::Patch * patch);

    // ------------ look and feel --------------

    const Palette& palette() const { return palette_; }

    // ----------- graphics update -------------

    /** Sets the contents of the info string in the toolbar */
    void setInfo(const String& info);

    /** immidiately updates changes in patch */
    void updateFromPatch();

    /** Sets update request which will be fulfilled on next draw */
    void updateFromPatchLater() { doUpdateFromPatchLater_ = true; update(); }

    /** update all cable positions */
    void updateCables();
    /** update all cables connected to this module */
    void updateCables(Module * mod);

    /** update all Connector values */
    void updateValueDisplays();

    /** Starts or stops automatic value update.
        A value of 0 will stop the automatic update. */
    void setValueUpdateInterval(int milliseconds);

    // ------------- functionallity --------------------

    void openProperties(Module * mod);
    void closeProperties();

signals:

public slots:

protected:

    // ----------------- events ------------------------

    virtual void paintEvent(QPaintEvent * event);
    virtual void keyPressEvent(QKeyEvent * event);

    // ---------------- module items -------------------

    /** return the ModuleItem for the Module, or NULL */
    ModuleItem * findModuleItem_(CSMOD::Module * mod);

    /** return the ModuleItem for the Module with the idName, or NULL */
    ModuleItem * findModuleItem_(const String& idName);

    /** create and install a ModuleItem for the module */
    ModuleItem * createModuleItem_(CSMOD::Module * mod);

    /** remove the ModuleItem from GUI */
    void deleteModuleItem_(ModuleItem * ci);

    // ------------- connector items -------------------

    /** return the ConnectorItem for the Connector, or NULL */
    ConnectorItem * findConnectorItem_(CSMOD::Connector * con);

    // -------------- connection items -----------------

    /** Returns the installed CableItem, or NULL */
    CableItem * findCableItem_(CSMOD::Connection * con);

    /** Returns the installed CableItem leading to or coming from @p ci, or NULL */
    CableItem * findCableItem_(ConnectorItem * ci);

    /** create and install a CableItem for the Connection.
        @note The referred ModuleItem must exist already,
        if not, NULL will be returned. */
    CableItem * createCableItem_(CSMOD::Connection * con);

    /** remove the CableItem from GUI */
    void deleteCableItem_(CableItem * ci);

    /** remove all CableItems from GUI */
    void deleteCableItems_();

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

    // ---- updates ---

    QTimer update_timer_;

    bool doUpdateFromPatchLater_;

    // --- widgets ---

    Palette palette_;

    QHBoxLayout * layout_;
    ModuleStockMenu * stockmenu_;
    PropertiesView * propview_;
    Module * propmodule_;

    QLabel * infoLabel_;

};


} // namespace GUI
} // namespace CSMOD

#endif // PATCHVIEW_H
