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

#include "connectoritem.h"

#include <QBrush>
#include <QPainter>
#include <QGraphicsTextItem>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneDragDropEvent>

#include "log.h"
#include "mod/connector.h"
#include "mod/module.h"
#include "mod/model.h"

#include "valueedititem.h"
#include "moduleitem.h"
#include "patchgraphicsview.h"

namespace CSMOD {
namespace GUI {

ConnectorItem::ConnectorItem(ModuleItem * parent, CSMOD::Connector * con)
    :   QGraphicsRectItem(parent),
        moduleItem_     (parent),
        con_            (con),
        view_           (parent->view()),
        is_con_         (false),
        matches_        (false),
        last_value_     (-1)
{
    CSMOD_DEBUGF("ConnectorItem::ConnectorItem(" << parent << ", " << con << ")");

    setFlags(
          QGraphicsItem::ItemIsSelectable
        | QGraphicsItem::ItemIsFocusable
        //| QGraphicsItem::ItemClipsToShape
        );

    setBrush(QBrush(QColor(50,50,50)));
    setRect(0,0, 10, 10);

    // XXX no real strategy right now for handling connector labels

    // name label
    tname_ = new QGraphicsSimpleTextItem(QString::fromStdString(con_->name()), this);
    tname_->setPos(con_->dir() == CSMOD::Connector::IN?
                      12 :
                      -(tname_->boundingRect().width())
                      , -rect().height()*2/3);
    tname_->setBrush(QBrush(QColor(155,200,155)));

    if (hasValueDisplay())
    {
        // value label
        tvalue_ = new ValueEditItem(this);
        tvalue_->setPos(con_->dir() == CSMOD::Connector::IN?
                          12 :
                          -(tvalue_->boundingRect().width())
                          , tname_->boundingRect().y());
        // input editable
        tvalue_->setEditable(hasValueInput());

        if (hasValueInput())
        {
            // callback from value input
            tvalue_->setValueChanged([this](double v)
            {
                moduleItem_->view()->model()->setConnectorUserValue(con_, v);
            });

            // init display value
            auto vc = dynamic_cast<ValueConnector*>(con_);
            if (vc) tvalue_->setValue(vc->userValue());
        }
    }

    updateValueDisplay();
}

ConnectorItem::~ConnectorItem()
{
    CSMOD_DEBUGF("ConnectorItem::~ConnectorItem() this=" << this);
}

void ConnectorItem::setInfo(const String& info)
{
    view_->setInfo(info);
}

QPointF ConnectorItem::sceneConnectPoint() const
{
    if (con_->dir() == CSMOD::Connector::IN)
        return mapToScene(1, rect().height()/2);
    else
        return mapToScene(rect().width()-1, rect().height()/2);
}

void ConnectorItem::matchConnector(CSMOD::Connector * con)
{
    matches_ = (con_==0 || con==0)? false :
                con_->isConnectable(con);
}

// ---------------- info ------------------------

bool ConnectorItem::hasValueInput() const
{
    return ( (con_->dir() == CSMOD::Connector::IN)
             && (dynamic_cast<ValueConnector*>(con_) != 0)
             );
}

bool ConnectorItem::hasValueDisplay() const
{
    return true; //(con_->dir() == CSMOD::Connector::OUT);
}

// ---------------------- paint ---------------------------

void ConnectorItem::paint(QPainter *p, const QStyleOptionGraphicsItem * /*option*/, QWidget * /*widget*/)
{
    CSMOD_DEBUGE("ConnectorItem::paint(" << p << ")");

    if (matches_)
        p->setPen(QPen(QColor(200,200,200)));
    else
        p->setPen(Qt::NoPen);

    p->setBrush(QColor(50 + hasFocus() * 180,
                       100 + hasCursor() * 50,
                       50 + isSelected() * 100
                       ));
    p->drawRect(rect());
}

void ConnectorItem::updateValueDisplay()
{
    //CSMOD_DEBUGF("ConnectorItem::updateValueDisplay()");

    // udpate value display
    if (hasValueDisplay() && !hasValueInput())
    {
        QString v = con_->valueQString();
        if (v != last_value_)
        {
            tvalue_->setPlainText(v);

            // update alignment
            if (con_->dir() == CSMOD::Connector::OUT)
                tvalue_->setX(-(tvalue_->boundingRect().width()));
        }
        last_value_ = v;
    }
}

// ---------------------- events --------------------------

void ConnectorItem::mousePressEvent(QGraphicsSceneMouseEvent *e)
{
    CSMOD_DEBUGE("ConnectorItem::mousePressEvent(" << e->pos().x() << ", " << e->pos().y() << ")");

    // start connecting
    if (e->button() == Qt::LeftButton)
    {
        CSMOD_PATCH_INFO("Connector: " << moduleItem_->module()->name() << "." << con_->name()
                         << " [" << moduleItem_->module()->idName() << "." << con_->idName() << "]");

        is_con_ = true;
        view_->startConnect(this);
        e->accept();

        /*
        Drag *drag = new QDrag(this);
        QMimeData *mimeData = new QMimeData;

        mimeData->setData(mimeType, data);
        drag->setMimeData(mimeData);

        Qt::DropAction dropAction = drag->exec(Qt::CopyAction | Qt::MoveAction);
        */
    }
    QGraphicsRectItem::mousePressEvent(e);
}

void ConnectorItem::mouseMoveEvent(QGraphicsSceneMouseEvent *e)
{
    CSMOD_DEBUGE("ConnectorItem::mouseMoveEvent(" << e->pos().x() << ", " << e->pos().y() << ")");

    if (is_con_)
    {
        view_->moveConnect(e->scenePos());
    }

    QGraphicsRectItem::mouseMoveEvent(e);
}

void ConnectorItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *e)
{
    CSMOD_DEBUGE("ConnectorItem::mouseReleaseEvent(" << e->pos().x() << ", " << e->pos().y() << ") this=" << this);

    if (is_con_)
        view_->endConnect();
    is_con_ = false;

    QGraphicsRectItem::mouseReleaseEvent(e);
}



} // namespace GUI
} // namespace CSMOD
