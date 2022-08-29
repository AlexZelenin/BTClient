#ifndef BLUETOOTHCONTROLLER_H
#define BLUETOOTHCONTROLLER_H

#include <QObject>
#include <QFile>
#include <QModelIndex>

// https://github.com/nholthaus/win-bluetooth
#include <win-bluetooth>

#include "globals.h"

class BluetoothController : public QObject
{
    Q_OBJECT
public:
    explicit BluetoothController(QObject *parent = nullptr);
    ~BluetoothController();

    Q_INVOKABLE void startDiscovery(const BluetoothUuid &uuid);
    Q_INVOKABLE BluetoothDeviceInfo service() const;
    Q_INVOKABLE void fileSelect();
    Q_INVOKABLE void sendFile(const QString& info, const QString& filepath);
    Q_INVOKABLE void stopButton();
    //Q_INVOKABLE void startConnect(const QString& address) const;
    Q_INVOKABLE void sendData(const QString& deviceName, const QString& port, const QString& data);

private:
    QString addressToName(const BluetoothAddress &address) const;

signals:
    void addDevice(const BluetoothDeviceInfo&);
    void openFile(const QString&);
    void transferProgress(qint64, qint64);
    void sockConnect();
    void discovered();
    void stopDiscovery();
    void exitPrgrsDlg();

public Q_SLOTS:
    void startDiscovery();

private slots:
    void serviceDiscovered(const BluetoothDeviceInfo &serviceInfo);

    void pairingFinished(const BluetoothAddress &address,BluetoothLocalDevice::Pairing pairing);
    void pairingError(BluetoothLocalDevice::Error error);
    void displayPin(const BluetoothAddress &address, QString pin);
    void displayConfirmation(const BluetoothAddress &address, QString pin);
    void displayConfReject();
    void displayConfAccepted();

    void transferFinished();

    void socketConnected();
    void socketRead();
    void socketError(BluetoothSocket::SocketError error);
    void socketDisconnected();

private:
    BluetoothDeviceDiscoveryAgent *m_discoveryAgent;
    BluetoothDeviceInfo m_service;
    QMap<QString, BluetoothDeviceInfo> m_discoveredServices;
    QFile *m_file;
    BluetoothLocalDevice *m_localDevice;
    bool m_pairingError;

    BluetoothTransferManager *m_manager;
    BluetoothTransferReply *m_reply;

    BluetoothSocket *m_socket;
};

#endif // BLUETOOTHCONTROLLER_H
