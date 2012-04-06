#ifndef QHTSPTAG_H
#define QHTSPTAG_H

#include <QExplicitlySharedDataPointer>
#include <QObject>
#include <QSharedData>
#include <QString>

#include "qhtspchannellist.h"
#include "qhtspchannelmodel.h"
#include "qhtspmessage.h"

class QHtsp;
class QHtspTagData : public QObject, public QSharedData
{
    Q_OBJECT

public:
    QHtspTagData(QHtsp *htsp, QObject *parent = 0);
    QHtspTagData(const QHtspTagData &other, QObject *parent = 0);
    ~QHtspTagData() { }

    QHtspChannelList *channels;
    QHtsp *htsp;
    QString iconUrl;
    qint64 id;
    QString name;

    void setIconUrl(QString url);
    void setId(qint64 id);
    void setName(QString name);

    void parseMessage(QHtspMessage &message);

signals:
    void iconUrlChanged();
    void idChanged();
    void nameChanged();

private:
    void _updateChannels(QList<qint64> *ids);
};

class QHtspTag : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QHtspChannelList *channels READ channels)
    Q_PROPERTY(QHtspChannelModel *channelsModel READ channelsModel)
    Q_PROPERTY(QString iconUrl READ iconUrl WRITE setIconUrl NOTIFY iconUrlChanged)
    Q_PROPERTY(qint64 id READ id WRITE setId NOTIFY idChanged)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)

public:
    explicit QHtspTag(QHtsp *htsp, QObject *parent = 0);
    QHtspTag(QHtspMessage &message, QHtsp *htsp, QObject *parent = 0);
    QHtspTag(const QHtspTag &tag, QObject *parent = 0);
    
    QHtspChannelList *channels();
    QHtspChannelModel *channelsModel();
    QString iconUrl();
    qint64 id();
    QString name();

    void setIconUrl(QString url);
    void setId(qint64 id);
    void setName(QString name);

    void update(QHtspMessage &message);

signals:
    void iconUrlChanged();
    void idChanged();
    void nameChanged();

private:
    QExplicitlySharedDataPointer<QHtspTagData> d;
    QHtspChannelModel *m_channelsModel;

    void _connectSignals();
};

#endif // QHTSPTAG_H
