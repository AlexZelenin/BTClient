#include "bluetoothcontroller.h"

#include <QBluetoothTransferManager>
#include <QBluetoothTransferRequest>
#include <QBluetoothLocalDevice>
#include <QFileDialog>
#include <QMessageBox>e

#include <QDebug>

static const QLatin1String serviceUuid("00001101-0000-1000-8000-00805F9B34FB");

QMap<QString, QBluetoothDeviceInfo> gDevs;

BluetoothController::BluetoothController(QObject *parent)
    : QObject{parent}
{

    m_discoveryAgent = new QBluetoothDeviceDiscoveryAgent(this);
   // m_discoveryAgent->setLowEnergyDiscoveryTimeout(5000);
    m_localDevice = new QBluetoothLocalDevice(this);
    m_socket = new QBluetoothSocket(this);

    connect(m_socket, &QBluetoothSocket::connected, this, &BluetoothController::socketConnected);
}

void BluetoothController::startScanDevices()
{
    qDebug() << "start: " << m_localDevice->address().toString();
   // if(m_localDevice->hostMode() == QBluetoothLocalDevice::HostPoweredOff){
        m_localDevice->powerOn();
   // }
    connect(m_discoveryAgent, SIGNAL(deviceDiscovered(QBluetoothDeviceInfo)),
            this, SLOT(deviceDiscovered(QBluetoothDeviceInfo)));

   // m_discoveryAgent->setInquiryType(QBluetoothDeviceDiscoveryAgent::GeneralUnlimitedInquiry);

    m_discoveryAgent->start(QBluetoothDeviceDiscoveryAgent::ClassicMethod);
}

void BluetoothController::stopScanDevices()
{
    m_discoveryAgent->stop();
}

void BluetoothController::pushData(const QString &address)
{
    qDebug() << "address: " << address;

    QBluetoothTransferManager transferManager;

    QBluetoothAddress remoteAddress(address);
    QBluetoothTransferRequest request(remoteAddress);

    //foreach(QFile *file, m_files) {

    QFile* const file = m_files.at(0);

    QBluetoothTransferReply *reply = transferManager.put(request, file);

    Q_ASSERT(reply && "https://forum.qt.io/topic/125736/qbluetoothtransfer-not-sending-file");
qWarning() << "Cannot push file: " << reply->errorString();
    if (reply->error()) {
        qWarning() << "Cannot push file: " << reply->errorString();
        reply->deleteLater();
    }

    connect(reply, SIGNAL(finished(QBluetoothTransferReply*)),
            this, SLOT(transferFinished(QBluetoothTransferReply*)));
    connect(reply, SIGNAL(error(QBluetoothTransferReply::TransferError)),
            this, SLOT(error(QBluetoothTransferReply::TransferError)));
    //}
}

void BluetoothController::attachFile()
{
    const QString& filepath = QFileDialog::getOpenFileName(nullptr, "Select file", ".");
    m_files.append(new QFile(filepath));
    emit fileAttached(filepath);
}

void BluetoothController::startConnect(const QString& address)
{
    requestPairing(QBluetoothAddress(address));
}

void BluetoothController::deviceDiscovered(const QBluetoothDeviceInfo &device)
{
    qDebug() << "Found new device:" << device.name() << '(' << device.address().toString() << ')';
    if (m_localDevice->pairingStatus(device.address()) == QBluetoothLocalDevice::Paired)  {
        emit connected();
    }

    gDevs[device.address().toString()] = device;

    emit addDevice(device);
}

void BluetoothController::transferFinished(QBluetoothTransferReply * replay)
{
    Q_UNUSED(replay)
}

void BluetoothController::error(QBluetoothTransferReply::TransferError error)
{
    switch (error) {
    case QBluetoothTransferReply::NoError:
        break;
    case QBluetoothTransferReply::UnknownError:
        qDebug() << "UnknownError";
        break;
    case QBluetoothTransferReply::FileNotFoundError:
        qDebug() << "FileNotFoundError";
        break;
    case QBluetoothTransferReply::HostNotFoundError:
        qDebug() << "HostNotFoundError";
        break;
    case QBluetoothTransferReply::UserCanceledTransferError:
        qDebug() << "UserCanceledTransferError";
        break;
    case QBluetoothTransferReply::IODeviceNotReadableError:
        qDebug() << "IODeviceNotReadableError";
        break;
    case QBluetoothTransferReply::ResourceBusyError:
        qDebug() << "ResourceBusyError";
        break;
    case QBluetoothTransferReply::SessionError:
        qDebug() << "SessionError";
        break;
    }
}

void BluetoothController::socketError(QBluetoothSocket::SocketError error)
{
    switch(error) {
    case QBluetoothSocket::NoSocketError:
        qDebug() << "QBluetoothSocket::NoSocketError";
        break;
    case QBluetoothSocket::UnknownSocketError:
        qDebug() << "QBluetoothSocket::UnknownSocketError";
        break;
    case QBluetoothSocket::RemoteHostClosedError:
        qDebug() << "QBluetoothSocket::RemoteHostClosedError";
        break;
    case QBluetoothSocket::HostNotFoundError:
        qDebug() << "QBluetoothSocket::HostNotFoundError";
        break;
    case QBluetoothSocket::ServiceNotFoundError:
        qDebug() << "QBluetoothSocket::ServiceNotFoundError";
        break;
    case QBluetoothSocket::NetworkError:
        qDebug() << "QBluetoothSocket::NetworkError";
        break;
    case QBluetoothSocket::UnsupportedProtocolError:
        qDebug() << "QBluetoothSocket::UnsupportedProtocolError";
        break;
    case QBluetoothSocket::OperationError:
        qDebug() << "QBluetoothSocket::OperationError";
        break;
    }
}

void BluetoothController::socketConnected()
{
    qDebug() << "Socket connected";
    emit connected();
}

void BluetoothController::socketDisconnected()
{
    qDebug() << "Socket disconnected";
}

void BluetoothController::socketStateChanged()
{
    qDebug() << "Socket state changed";
}

void BluetoothController::requestPairing(const QBluetoothAddress &address)
{
    if (m_localDevice->pairingStatus(address) != QBluetoothLocalDevice::Paired) {
        m_localDevice->requestPairing(address, QBluetoothLocalDevice::Paired);
        qDebug() << "is not paired: " << address.toString();
        connect(m_localDevice, &QBluetoothLocalDevice::pairingDisplayConfirmation, this, [](const QBluetoothAddress &address, QString pin){
            QMessageBox::information(nullptr, "Confirm device", pin);
        });
    }

    startClient(address);
}

void BluetoothController::startClient(const QBluetoothAddress &deviceInfo) {
    qDebug() << "Connect to: " << deviceInfo.toString();
    auto dev = gDevs[deviceInfo.toString()];
    QBluetoothServiceInfo info;
    info.setDevice(dev);

    qDebug() << "Registered devs: " << info.device().address().toString();
    m_socket->connectToService(info);
    //m_socket->connectToService(deviceInfo,  QBluetoothUuid(QBluetoothUuid::Rfcomm));
}
