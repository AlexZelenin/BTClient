#ifndef BLUETOOTHCONTROLLER_H
#define BLUETOOTHCONTROLLER_H

#include <QObject>
#include <QtBluetooth/QBluetoothDeviceInfo>


class BluetoothController : public QObject
{
    Q_OBJECT
public:
    explicit BluetoothController(QObject *parent = nullptr);

    Q_INVOKABLE void startScanDevices();

public slots:

    void deviceDiscovered(const QBluetoothDeviceInfo &device);

signals:

};

#endif // BLUETOOTHCONTROLLER_H
