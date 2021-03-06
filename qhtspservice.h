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

#ifndef QHTSPSERVICE_H
#define QHTSPSERVICE_H

#include <QExplicitlySharedDataPointer>
#include <QObject>
#include <QSharedData>
#include <QString>

#include "qhtspmessage.h"
#include "qhtspchannel.h"

class QHtspChannel;
class QHtspServiceData : public QObject, public QSharedData
{
    Q_OBJECT

public:
    explicit QHtspServiceData(QHtspChannel *channel);
    QHtspServiceData(const QHtspServiceData &other);
    ~QHtspServiceData() { }

    qint64 caid;
    QString caname;
    QHtspChannel *channel;
    QString name;
    qint64 type;

    void setCaid(qint64 caid);
    void setCaname(QString caname);
    void setName(QString name);
    void setType(qint64 type);

    void parseMessage(QHtspMessage &message);

signals:
    void caidChanged();
    void canameChanged();
    void nameChanged();
    void typeChanged();
};

class QHtspService : public QObject
{
    Q_OBJECT
    Q_ENUMS(Type)
    Q_PROPERTY(qint64 caid READ caid WRITE setCaid NOTIFY caidChanged)
    Q_PROPERTY(QString caname READ caname WRITE setCaname NOTIFY canameChanged)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(Type type READ type WRITE setType NOTIFY typeChanged)

public:
    enum Type {
        RadioType,
        SdTvType,
        HdTvType
    };

    explicit QHtspService(QHtspChannel *channel, QObject *parent = 0);
    QHtspService(QHtspMessage &message, QHtspChannel *channel, QObject *parent = 0);
    QHtspService(const QHtspService &service, QObject *parent = 0);
    
    qint64 caid();
    QString caname();
    QString name();
    Type type();

    void setCaid(qint64 caid);
    void setCaname(QString caname);
    void setName(QString name);
    void setType(Type type);

signals:
    void caidChanged();
    void canameChanged();
    void nameChanged();
    void typeChanged();

private:
    QExplicitlySharedDataPointer<QHtspServiceData> d;

    void _connectSignals();
};

#endif // QHTSPSERVICE_H
