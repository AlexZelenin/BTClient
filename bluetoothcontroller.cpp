#include "bluetoothcontroller.h"

#include <QBluetoothTransferManager>
#include <QBluetoothTransferRequest>

#include <QFile>
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
        qDebug() << "LocalName: " << m_localDevice->name();
            m_localDevice->powerOn();
        }
    connect(m_discoveryAgent, SIGNAL(deviceDiscovered(QBluetoothDeviceInfo)),
            this, SLOT(deviceDiscovered(QBluetoothDeviceInfo)));

    m_discoveryAgent->setInquiryType(QBluetoothDeviceDiscoveryAgent::GeneralUnlimitedInquiry);

    m_discoveryAgent->start();
}

void BluetoothController::pushData(const QString &address)
{
    requestPairing(QBluetoothAddress(address));
//    m_socket->connectToService(QBluetoothAddress(address), QBluetoothUuid(serviceUuid));

//    connect(m_socket, SIGNAL(error(QBluetoothSocket::SocketError)),
//                        this, SLOT(socketError(QBluetoothSocket::SocketError)));
//    connect(m_socket, SIGNAL(connected()), this, SLOT(socketConnected()));
//    connect(m_socket, SIGNAL(disconnected()), this, SLOT(socketDisconnected()));
//    connect(m_socket, SIGNAL(readyRead()), this, SLOT(socketRead()));
//    connect(m_socket, SIGNAL(stateChanged(QBluetoothSocket::SocketState)), this, SLOT(socketStateChanged()));

    //m_socket->open(QIODevice::WriteOnly);

    /*QBluetoothTransferManager *transferManager = new QBluetoothTransferManager(this);

    // Create the transfer request and file to be sent
    QBluetoothAddress remoteAddress(address);
    QBluetoothTransferRequest request(remoteAddress);
    QFile *file = new QFile("testfile.txt");

    // Ask the transfer manager to send it
    QBluetoothTransferReply *reply = transferManager->put(request, file);
    if (reply->error() == QBluetoothTransferReply::NoError) {

        // Connect to the reply's signals to be informed about the status and do cleanups when done
        connect(reply, SIGNAL(finished(QBluetoothTransferReply*)),
                this, SLOT(transferFinished(QBluetoothTransferReply*)));
        connect(reply, SIGNAL(error(QBluetoothTransferReply::TransferError)),
                this, SLOT(error(QBluetoothTransferReply::TransferError)));
    } else {
        qWarning() << "Cannot push testfile.txt:" << reply->errorString();
    }*/
}

void BluetoothController::deviceDiscovered(const QBluetoothDeviceInfo &device)
{
    qDebug() << "Found new device:" << device.name() << '(' << device.address().toString() << ')';
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
}

void BluetoothController::socketDisconnected()
{
    qDebug() << "Socket disconnected";
}

void BluetoothController::socketRead()
{
    qDebug() << "Socket read";
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
  //...
  m_socket = new QBluetoothSocket(QBluetoothServiceInfo::RfcommProtocol, this);
  connect(m_socket, &QBluetoothSocket::connected, this, &BluetoothController::socketConnected);
  connect(m_socket, &QBluetoothSocket::readyRead, this, &BluetoothController::socketRead);
  m_socket->connectToService(deviceInfo, 1);
}
