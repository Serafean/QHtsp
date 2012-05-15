#ifndef QHTSPTAGMODEL_H
#define QHTSPTAGMODEL_H

#include <QAbstractTableModel>
#include <QModelIndex>
#include <QVariant>

#include "qhtsptag.h"
#include "qhtsptaglist.h"

class QHtspTagModel : public QAbstractTableModel
{
    Q_OBJECT
    Q_PROPERTY(int count READ rowCount)

public:
    enum TagRoles {
        IdRole = Qt::UserRole + 1,
        NameRole,
        IconRole
    };

    explicit QHtspTagModel(QHtspTagList *tags, QObject *parent = 0);

    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;

    Q_INVOKABLE QHtspTag *getTagById(qint64 id);
    Q_INVOKABLE QHtspTag *getTagByIndex(int i);

private:
    QHtspTagList *m_tags;

private slots:
    void _addRow(QHtspTag *tag);
    void _updateIcon();
    void _updateId();
    void _updateName();
    void _removeRow(QHtspTag *tag);
};

#endif // QHTSPTAGMODEL_H
