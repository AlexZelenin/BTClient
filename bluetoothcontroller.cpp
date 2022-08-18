#include "bluetoothcontroller.h"

#include <QBluetoothTransferManager>
#include <QBluetoothTransferRequest>
#include <QFileDialog>

#include <QDebug>

static const QLatin1String serviceUuid("00001101-0000-1000-8000-00805F9B34FB");

BluetoothController::BluetoothController(QObject *parent)
    : QObject{parent}
{
    m_discoveryAgent = new QBluetoothDeviceDiscoveryAgent(this);
    m_localDevice = new QBluetoothLocalDevice(this);
    m_socket = new QBluetoothSocket(QBluetoothServiceInfo::RfcommProtocol, this);
}

void BluetoothController::startScanDevices()
{

    if(m_localDevice->hostMode() == QBluetoothLocalDevice::HostPoweredOff){
        m_localDevice->powerOn();
    }
    connect(m_discoveryAgent, SIGNAL(deviceDiscovered(QBluetoothDeviceInfo)),
            this, SLOT(deviceDiscovered(QBluetoothDeviceInfo)));

    m_discoveryAgent->setInquiryType(QBluetoothDeviceDiscoveryAgent::GeneralUnlimitedInquiry);

    m_discoveryAgent->start();
}

void BluetoothController::stopScanDevices()
{
    m_discoveryAgent->stop();
}

void BluetoothController::pushData(const QString &address)
{

    m_transferManager = new QBluetoothTransferManager(this);

    QBluetoothAddress remoteAddress(address);
    QBluetoothTransferRequest request(remoteAddress);

    foreach(QFile *file, m_files) {
        QBluetoothTransferReply *reply = m_transferManager->put(request, file);

        if (reply->error() == QBluetoothTransferReply::NoError) {

            connect(reply, SIGNAL(finished(QBluetoothTransferReply*)),
                    this, SLOT(transferFinished(QBluetoothTransferReply*)));
            connect(reply, SIGNAL(error(QBluetoothTransferReply::TransferError)),
                    this, SLOT(error(QBluetoothTransferReply::TransferError)));
        } else {
            qWarning() << "Cannot push file: " << reply->errorString();
        }
    }
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
    if (m_localDevice->pairingStatus(address) == QBluetoothLocalDevice::Paired) {
        startClient(address);
    } else {
        m_localDevice->requestPairing(address, QBluetoothLocalDevice::Paired);
    }
}

void BluetoothController::startClient(const QBluetoothAddress &deviceInfo) {
    connect(m_socket, &QBluetoothSocket::connected, this, &BluetoothController::socketConnected);

    m_socket->connectToService(deviceInfo, QBluetoothUuid(serviceUuid));
}
