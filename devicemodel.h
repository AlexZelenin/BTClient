#ifndef DEVICEMODEL_H
#define DEVICEMODEL_H

#include <QObject>
#include <QAbstractListModel>
#include <QList>

#include "globals.h"

// https://github.com/nholthaus/win-bluetooth
#include <win-bluetooth>

class DeviceModel : public QAbstractListModel
{
    Q_OBJECT
public:

    enum Roles {
      Address = Qt::UserRole + 1,
      DeviceName
    };

    explicit DeviceModel(QObject* parent = nullptr);

    Q_INVOKABLE void clear();

public:
    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QHash<int, QByteArray> roleNames() const;

public slots:
    void addDevice(const BluetoothDeviceInfo &info);

private:
    QList<BluetoothDeviceInfo> m_devices;

};

#endif // DEVICEMODEL_H
