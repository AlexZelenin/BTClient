#include "devicemodel.h"

#include <QDebug>


static const QLatin1String emptyAddress("00:00:00:00:00:00");

DeviceModel::DeviceModel(QObject* parent)
    : QAbstractListModel(parent)
{
}

void DeviceModel::clear()
{
    beginResetModel();
    m_devices.clear();
    endResetModel();
}

int DeviceModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_devices.size();
}

QVariant DeviceModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (index.row() >= m_devices.size())
        return QVariant();

    switch(role) {
    case Address: {
        return  m_devices.at(index.row()).address().toString();
    }
    case DeviceName: {
        return m_devices.at(index.row()).name();
    }
    default:
        return QVariant();
    }

    return QVariant();
}

QHash<int, QByteArray> DeviceModel::roleNames() const
{
    QHash<int, QByteArray> roleNames;

    roleNames[Address] = "address";
    roleNames[DeviceName] = "device_name";

    return roleNames;
}

void DeviceModel::addDevice(const BluetoothDeviceInfo &info)
{
    qDebug() << "Model add device: " << info.name();

    if (info.address().toString() == emptyAddress)
        return;

    bool contains = false;

    foreach(const BluetoothDeviceInfo &i, m_devices) {
        if (i.address().toString() == info.address().toString())
            contains = true;
    }

    if (!contains) {
        beginInsertRows(QModelIndex(), m_devices.size(), m_devices.size());
        m_devices.append(info);
        endInsertRows();
    }
}
