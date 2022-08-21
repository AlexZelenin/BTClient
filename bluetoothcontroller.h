#ifndef BLUETOOTHCONTROLLER_H
#define BLUETOOTHCONTROLLER_H

#include <QObject>
#include <QtBluetooth/QBluetoothDeviceInfo>
#include <QBluetoothServiceDiscoveryAgent>
#include <QBluetoothLocalDevice>
#include <QBluetoothTransferReply>
#include <QBluetoothSocket>
#include <QBluetoothUuid>
#include <QBluetoothSocket>
#include <QFile>
#include <QModelIndex>


class BluetoothController : public QObject
{
    Q_OBJECT
public:
    explicit BluetoothController(QObject *parent = nullptr);
    ~BluetoothController();

    Q_INVOKABLE void startDiscovery(const QBluetoothUuid &uuid);
    Q_INVOKABLE QBluetoothServiceInfo service() const;
    Q_INVOKABLE void fileSelect();
    Q_INVOKABLE void sendFile(const QString& info, const QString& filepath);
    Q_INVOKABLE void stopButton();
    Q_INVOKABLE void startConnect(const QString& address);
    Q_INVOKABLE void sendData(const QString& data);

private:
    QString addressToName(const QBluetoothAddress &address) const;

signals:
    void addDevice(const QBluetoothServiceInfo &info);
    void openFile(const QString&);
    void transferProgress(qint64, qint64);
    void sockConnect();

public Q_SLOTS:
    void startDiscovery();

private slots:
    void serviceDiscovered(const QBluetoothServiceInfo &serviceInfo);

    void pairingFinished(const QBluetoothAddress &address,QBluetoothLocalDevice::Pairing pairing);
    void pairingError(QBluetoothLocalDevice::Error error);
    void displayPin(const QBluetoothAddress &address, QString pin);
    void displayConfirmation(const QBluetoothAddress &address, QString pin);
    void displayConfReject();
    void displayConfAccepted();

    void socketConnected();
    void socketRead();
    void socketError(QBluetoothSocket::SocketError error);

private:
    QBluetoothServiceDiscoveryAgent *m_discoveryAgent;
    QBluetoothServiceInfo m_service;
    QMap<QString, QBluetoothServiceInfo> m_discoveredServices;
    QFile *m_file;
    QBluetoothLocalDevice *m_localDevice;
    bool m_pairingError;

    QBluetoothSocket *m_socket;
};

#endif // BLUETOOTHCONTROLLER_H
