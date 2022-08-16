#ifndef BLUETOOTHCONTROLLER_H
#define BLUETOOTHCONTROLLER_H

#include <QObject>
#include <QtBluetooth/QBluetoothDeviceInfo>
#include <QBluetoothDeviceDiscoveryAgent>
#include <QBluetoothLocalDevice>
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

    void socketError(QBluetoothSocket::SocketError);
    void socketConnected();
    void socketDisconnected();
    void socketRead();
    void socketStateChanged();

    void requestPairing(const QBluetoothAddress &address);
    void startClient(const QBluetoothAddress &deviceInfo);

signals:
    void addDevice(const QBluetoothDeviceInfo&);

signals:

private:
    QBluetoothSocket *m_socket;
    QBluetoothDeviceDiscoveryAgent *m_discoveryAgent;
    QBluetoothLocalDevice *m_localDevice;

};

#endif // BLUETOOTHCONTROLLER_H
