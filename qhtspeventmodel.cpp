/*
 *  Copyright (C) 2012 Robert Meijers
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "qhtspeventmodel.h"

#include "qhtspchannel.h"

QHtspEventModel::QHtspEventModel(QHtspEventList *events, QObject *parent) :
    QAbstractTableModel(parent ? parent : events), m_events(events)
{
    connect(m_events, SIGNAL(added(QHtspEvent*)), this, SLOT(_addRow(QHtspEvent*)));
    connect(m_events, SIGNAL(removing(QHtspEvent*)), this, SLOT(_removeRow(QHtspEvent*)));
    connect(m_events, SIGNAL(groupAddEnded()), this, SLOT(_fetchMore()));

    m_fakeRowCount = 0;

    QHash<int, QByteArray> roles;
    roles[IdRole] = "id";
    roles[DescriptionRole] = "description";
    roles[StartRole] = "start";
    roles[StopRole] = "stop";
    roles[TitleRole] = "title";
    roles[ChannelRole] = "channel";
    roles[StartDateRole] = "startDate";
    setRoleNames(roles);
}

bool QHtspEventModel::canFetchMore(const QModelIndex &/*parent*/) const
{
    if(!m_events->channel() || m_events->count() == 0)
        return false;

    return m_events->at(m_events->count()-1)->nextEventId() > 0;
}

int QHtspEventModel::columnCount(const QModelIndex &/*parent*/) const
{
    return 6;
}

QVariant QHtspEventModel::data(const QModelIndex &index, int role) const
{
    QVariant result;

    if(index.row() < 0 || index.row() >= m_events->count())
        return "";

    QHtspEvent *event = m_events->at(index.row());

    if(role == Qt::DisplayRole)
    {
        switch(index.column())
        {
        case 0:
            result = event->id();
            break;
        case 1:
            result = event->description();
            break;
        case 2:
            result = event->start();
            break;
        case 3:
            result = event->stop();
            break;
        case 4:
            result = event->title();
            break;
        case 5:
            result = event->channel() ? event->channel()->name() : "";
            break;
        }
    }
    else if(role == IdRole)
    {
        result = event->id();
    }
    else if(role == DescriptionRole)
    {
        result = event->description();
    }
    else if(role == StartRole)
    {
        result = event->start();
    }
    else if(role == StopRole)
    {
        result = event->stop();
    }
    else if(role == TitleRole)
    {
        result = event->title();
    }
    else if(role == ChannelRole)
    {
        result = event->channel() ? event->channel()->name() : "";
    }
    else if(role == StartDateRole)
    {
        result = event->start().date();
    }

    return result;
}

void QHtspEventModel::fetchMore(const QModelIndex &/*parent*/)
{
    if(!m_fakeRowCount)
        m_events->channel()->fetchNextEvents(5);

    beginInsertRows(QModelIndex(), rowCount(), rowCount()+5);
    m_fakeRowCount += 5;
    endInsertRows();
}

QVariant QHtspEventModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role != Qt::DisplayRole || orientation != Qt::Horizontal)
        return QVariant();

    switch(section)
    {
    case 0:
        return QString("Id");
        break;
    case 1:
        return QString("Description");
        break;
    case 2:
        return QString("Start");
        break;
    case 3:
        return QString("Stop");
        break;
    case 4:
        return QString("Title");
        break;
    case 5:
        return QString("Channel");
    }

    return QVariant();
}

int QHtspEventModel::rowCount(const QModelIndex &/*parent*/) const
{
    return m_events->count() + m_fakeRowCount;
}

QHtspEvent *QHtspEventModel::getEventById(int id)
{
    return m_events->find(id);
}

QHtspEvent *QHtspEventModel::getEventByIndex(int index)
{
    if(index >= m_events->count())
        return 0;

    return m_events->at(index);
}

void QHtspEventModel::_addRow(QHtspEvent *event)
{
    int rows = m_events->count() - 1;
    if(m_fakeRowCount)
    {
        m_fakeRowCount--;
        emit dataChanged(createIndex(rows, 1), createIndex(rows, 5));
    }
    else
    {
        beginInsertRows(QModelIndex(), rows, rows);
        endInsertRows();
    }

    connect(event, SIGNAL(idChanged()), this, SLOT(_updateId()));
    connect(event, SIGNAL(channelIdChanged()), this, SLOT(_updateChannel()));
    connect(event, SIGNAL(descriptionChanged()), this, SLOT(_updateDescription()));
    connect(event, SIGNAL(startChanged()), this, SLOT(_updateStart()));
    connect(event, SIGNAL(stopChanged()), this, SLOT(_updateStop()));
    connect(event, SIGNAL(titleChanged()), this, SLOT(_updateTitle()));
}

void QHtspEventModel::_fetchMore()
{
    if(!m_fakeRowCount)
        return;

    if(m_events->at(m_events->count()-1)->nextEventId() < 0)
    {
        beginRemoveRows(QModelIndex(), m_events->count(), m_events->count()+m_fakeRowCount);
        m_fakeRowCount = 0;
        return;
    }

    m_events->channel()->fetchNextEvents(m_fakeRowCount);
}

void QHtspEventModel::_updateId()
{
    int row = m_events->indexOf((QHtspEvent*)QObject::sender());
    QModelIndex index = createIndex(row, 0);
    emit dataChanged(index, index);
}

void QHtspEventModel::_updateChannel()
{
    int row = m_events->indexOf((QHtspEvent*)QObject::sender());
    QModelIndex index = createIndex(row, 5);
    emit dataChanged(index, index);
}

void QHtspEventModel::_updateDescription()
{
    int row = m_events->indexOf((QHtspEvent*)QObject::sender());
    QModelIndex index = createIndex(row, 1);
    emit dataChanged(index, index);
}

void QHtspEventModel::_updateStart()
{
    int row = m_events->indexOf((QHtspEvent*)QObject::sender());
    QModelIndex index = createIndex(row, 2);
    emit dataChanged(index, index);
}

void QHtspEventModel::_updateStop()
{
    int row = m_events->indexOf((QHtspEvent*)QObject::sender());
    QModelIndex index = createIndex(row, 3);
    emit dataChanged(index, index);
}

void QHtspEventModel::_updateTitle()
{
    int row = m_events->indexOf((QHtspEvent*)QObject::sender());
    QModelIndex index = createIndex(row, 4);
    emit dataChanged(index, index);
}

void QHtspEventModel::_removeRow(QHtspEvent *event)
{
    disconnect(event, SIGNAL(idChanged()), this, SLOT(_updateId()));
    disconnect(event, SIGNAL(descriptionChanged()), this, SLOT(_updateDescription()));
    disconnect(event, SIGNAL(startChanged()), this, SLOT(_updateStart()));
    disconnect(event, SIGNAL(stopChanged()), this, SLOT(_updateStop()));
    disconnect(event, SIGNAL(titleChanged()), this, SLOT(_updateTitle()));

    int row = m_events->indexOf(event);
    beginRemoveRows(QModelIndex(), row, row);
    endRemoveRows();
}
