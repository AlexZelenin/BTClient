#ifndef BLUETOOTHCONTROLLER_H
#define BLUETOOTHCONTROLLER_H

#include <QObject>
#include <QtBluetooth/QBluetoothDeviceInfo>
#include <QBluetoothDeviceDiscoveryAgent>
#include <QBluetoothLocalDevice>
#include <QBluetoothTransferReply>
#include <QBluetoothSocket>
#include <QFile>
#include <QModelIndex>


class BluetoothController : public QObject
{
    Q_OBJECT
public:
    explicit BluetoothController(QObject *parent = nullptr);

    Q_INVOKABLE void startScanDevices();
    Q_INVOKABLE void stopScanDevices();
    Q_INVOKABLE void pushData(const QString& address, const QString& file);
    Q_INVOKABLE void attachFile();

public slots:
    void startConnect(const QString& address);

    void deviceDiscovered(const QBluetoothDeviceInfo &device);
    void transferFinished(QBluetoothTransferReply*);
    void error(QBluetoothTransferReply::TransferError);

    void socketError(QBluetoothSocket::SocketError);
    void socketConnected();
    void socketDisconnected();
    void socketStateChanged();

    void requestPairing(const QBluetoothAddress &address);
    void startClient(const QBluetoothAddress &deviceInfo);

signals:
    void addDevice(const QBluetoothDeviceInfo&);
    void connected();
    void fileAttached(const QString&); 
    void transferProgress(qint64, qint64);

signals:

private:
    QBluetoothSocket *m_socket;
    QBluetoothDeviceDiscoveryAgent *m_discoveryAgent;
    QBluetoothLocalDevice *m_localDevice;
    QList<QFile*> m_files;
    QBluetoothTransferManager *m_transferManager;

};

#endif // BLUETOOTHCONTROLLER_H
