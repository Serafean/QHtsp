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

#ifndef QHTSPEVENTMODEL_H
#define QHTSPEVENTMODEL_H

#include <QAbstractTableModel>

#include "qhtspevent.h"
#include "qhtspeventlist.h"

class QHtspChannel;

class QHtspEventModel : public QAbstractTableModel
{
    Q_OBJECT
    Q_PROPERTY(int count READ rowCount)
public:
    enum ChannelRoles {
        IdRole = Qt::UserRole + 1,
        DescriptionRole,
        StartRole,
        StopRole,
        TitleRole,
        ChannelRole,
        StartDateRole
    };

    explicit QHtspEventModel(QHtspEventList *events, QObject *parent = 0);
    
    bool canFetchMore(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    void fetchMore(const QModelIndex &parent);
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;

    Q_INVOKABLE QHtspEvent *getEventById(int id);
    Q_INVOKABLE QHtspEvent *getEventByIndex(int index);

signals:
    
private:
    QHtspEventList *m_events;
    int m_fakeRowCount;

private slots:
    void _addRow(QHtspEvent *event);
    void _fetchMore();
    void _updateId();
    void _updateChannel();
    void _updateDescription();
    void _updateStart();
    void _updateStop();
    void _updateTitle();
    void _removeRow(QHtspEvent *event);
};

#endif // QHTSPEVENTMODEL_H
