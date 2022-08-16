#ifndef DEVICEMODEL_H
#define DEVICEMODEL_H

#include <QObject>
#include <QAbstractListModel>
#include <QBluetoothDeviceInfo>
#include <QList>


class DeviceModel : public QAbstractListModel
{
    Q_OBJECT
public:

    enum Roles {
      Address = Qt::UserRole + 1,
      DeviceName
    };

    explicit DeviceModel(QObject* parent = nullptr);


    // QAbstractItemModel interface
public:
    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QHash<int, QByteArray> roleNames() const;

public slots:
    void addDevice(const QBluetoothDeviceInfo& info);

private:
    QList<QBluetoothDeviceInfo> m_devices;

};

#endif // DEVICEMODEL_H
