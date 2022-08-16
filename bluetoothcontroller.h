#ifndef BLUETOOTHCONTROLLER_H
#define BLUETOOTHCONTROLLER_H

#include <QObject>
#include <QtBluetooth/QBluetoothDeviceInfo>
#include <QBluetoothTransferReply>
#include <QBluetoothSocket>

class BluetoothController : public QObject
{
    Q_OBJECT
public:
    explicit BluetoothController(QObject *parent = nullptr);

    Q_INVOKABLE void startScanDevices();
    Q_INVOKABLE void pushData(const QString& address);

public slots:
    void deviceDiscovered(const QBluetoothDeviceInfo &device);
    void transferFinished(QBluetoothTransferReply*);
    void error(QBluetoothTransferReply::TransferError);

signals:
    void addDevice(const QBluetoothDeviceInfo&);

signals:

private:
    QBluetoothSocket *m_socket;

};

#endif // BLUETOOTHCONTROLLER_H
