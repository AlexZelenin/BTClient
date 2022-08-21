#include "devicemodel.h"

#include <QDebug>
#include <QBluetoothAddress>
#include <QBluetoothDeviceInfo>

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

    if (role == Address)
        return m_devices.at(index.row()).device().address().toString();
    if (role == DeviceName)
        return m_devices.at(index.row()).device().name();
    else
        return QVariant();
}

QHash<int, QByteArray> DeviceModel::roleNames() const
{
    QHash<int, QByteArray> roleNames;

    roleNames[Address] = "address";
    roleNames[DeviceName] = "device_name";

    return roleNames;
}

void DeviceModel::addDevice(const QBluetoothServiceInfo &info)
{
    qDebug() << "Model add device: " << info.device().name();

    bool contains = false;

    foreach(const QBluetoothServiceInfo &i, m_devices) {
        if (i.device().address().toString() == info.device().address().toString())
            contains = true;
    }

    if (!contains) {
        beginInsertRows(QModelIndex(), m_devices.size(), m_devices.size());
        m_devices.append(info);
        endInsertRows();
    }
}
