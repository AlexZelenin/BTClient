#include "bluetoothcontroller.h"

#include <qbluetoothdeviceinfo.h>
#include <qbluetoothaddress.h>
#include <qbluetoothtransferrequest.h>
#include <qbluetoothtransferreply.h>
#include <qbluetoothlocaldevice.h>

#include <QBluetoothTransferManager>
#include <QBluetoothTransferRequest>
#include <QBluetoothLocalDevice>
#include <QFileDialog>
#include <QMessageBox>
#include <QPointer>


#include <QDebug>

#include "uuid_services.h"


BluetoothController::BluetoothController(QObject *parent)
    : QObject{parent}
{
    m_localDevice = new QBluetoothLocalDevice(this);
    QBluetoothAddress adapterAddress = m_localDevice->address();

    qDebug() << adapterAddress.toString();

    m_discoveryAgent = new QBluetoothServiceDiscoveryAgent(adapterAddress);

    connect(m_discoveryAgent, SIGNAL(serviceDiscovered(QBluetoothServiceInfo)),
            this, SLOT(serviceDiscovered(QBluetoothServiceInfo)));

    connect(m_localDevice, SIGNAL(pairingDisplayPinCode(QBluetoothAddress,QString)),
            this, SLOT(displayPin(QBluetoothAddress,QString)));
    connect(m_localDevice, SIGNAL(pairingDisplayConfirmation(QBluetoothAddress,QString)),
            this, SLOT(displayConfirmation(QBluetoothAddress,QString)));
    connect(m_localDevice, SIGNAL(pairingFinished(QBluetoothAddress,QBluetoothLocalDevice::Pairing)),
            this, SLOT(pairingFinished(QBluetoothAddress,QBluetoothLocalDevice::Pairing)));
    connect(m_localDevice, SIGNAL(error(QBluetoothLocalDevice::Error)),
            this, SLOT(pairingError(QBluetoothLocalDevice::Error)));

    m_socket = new QBluetoothSocket(QBluetoothServiceInfo::RfcommProtocol);

    connect(m_socket, SIGNAL(error(QBluetoothSocket::SocketError)), this, SLOT(socketError(QBluetoothSocket::SocketError)));

}

BluetoothController::~BluetoothController()
{
    delete m_discoveryAgent;
    delete m_localDevice;
}

void BluetoothController::startDiscovery(const QBluetoothUuid &uuid)
{
    if (m_discoveryAgent->isActive())
        m_discoveryAgent->stop();

    //m_discoveryAgent->setUuidFilter(uuid);
    m_discoveryAgent->start();

    if (!m_discoveryAgent->isActive() ||
            m_discoveryAgent->error() != QBluetoothServiceDiscoveryAgent::NoError) {
        return;
    }
}

QBluetoothServiceInfo BluetoothController::service() const
{
    return m_service;
}

void BluetoothController::serviceDiscovered(const QBluetoothServiceInfo &serviceInfo)
{

    qDebug() << "Discovered service on"
             << serviceInfo.device().name() << serviceInfo.device().address().toString();
    qDebug() << "\tService name:" << serviceInfo.serviceName();
    qDebug() << "\tDescription:"
             << serviceInfo.attribute(QBluetoothServiceInfo::ServiceDescription).toString();
    qDebug() << "\tProvider:"
             << serviceInfo.attribute(QBluetoothServiceInfo::ServiceProvider).toString();
    qDebug() << "\tL2CAP protocol service multiplexer:"
             << serviceInfo.protocolServiceMultiplexer();
    qDebug() << "\tRFCOMM server channel:" << serviceInfo.serverChannel();


    QString remoteName;
    if (serviceInfo.device().name().isEmpty())
        remoteName = serviceInfo.device().address().toString();
    else
        remoteName = serviceInfo.device().name();

    const QBluetoothAddress address = serviceInfo.device().address();
    for (QBluetoothServiceInfo &info : m_discoveredServices) {
        if (info.device().address() == address){
            info = serviceInfo;
            return;
        }
    }

    m_discoveredServices.insert(serviceInfo.device().address().toString(), serviceInfo);
    emit addDevice(serviceInfo);
}

void BluetoothController::startDiscovery()
{
    startDiscovery(QBluetoothUuid(QBluetoothUuid::ObexObjectPush));
}

void BluetoothController::fileSelect()
{
    const QString filepath = QFileDialog::getOpenFileName();
    emit openFile(filepath);
}

void BluetoothController::sendFile(const QString& address, const QString& filepath)
{
    QBluetoothTransferManager mgr;
    QBluetoothTransferRequest req(m_discoveredServices[address].device().address());

    m_file = new QFile(filepath);

    QBluetoothTransferReply *reply = mgr.put(req, m_file);

    reply->setParent(this);

    if (reply->error() != QBluetoothTransferReply::NoError){
        qDebug() << "Failed to send file";
        reply->deleteLater();
        return;
    }

    connect(reply, SIGNAL(transferProgress(qint64,qint64)), this, SIGNAL(transferProgress(qint64,qint64)));
}

void BluetoothController::stopButton()
{
    m_discoveryAgent->stop();
}

void BluetoothController::startConnect(const QString& address)
{
    if (m_discoveredServices.contains(address)) {
        m_socket->disconnectFromService();
        qDebug() << "Address: " << QBluetoothAddress(address) << " contains";
        m_socket->connectToService(QBluetoothAddress(address), QBluetoothUuid(QBluetoothUuid::SerialPort), QIODevice::ReadWrite);

        qDebug()  << m_socket->socketType();

        connect(m_socket, &QBluetoothSocket::connected, this, &BluetoothController::socketConnected);
        connect(m_socket, &QBluetoothSocket::readyRead, this, &BluetoothController::socketRead);
    }
}

void BluetoothController::sendData(const QString &data)
{
    const QByteArray bytes = data.toUtf8();
    m_socket->write(bytes, bytes.length());
}

QString BluetoothController::addressToName(const QBluetoothAddress &address) const
{
    for (const QBluetoothServiceInfo &info : m_discoveredServices) {
        if (info.device().address() == address)
            return info.device().name();
    }
    return address.toString();
}

void BluetoothController::displayPin(const QBluetoothAddress &address, QString pin)
{
    Q_UNUSED(address);
    Q_UNUSED(pin);
}

void BluetoothController::displayConfirmation(const QBluetoothAddress &address, QString pin)
{
    Q_UNUSED(address);
    Q_UNUSED(pin);
    //if (m_pindisplay)
     //   m_pindisplay->deleteLater();
    //m_pindisplay = new pinDisplay(QString("Confirm this pin is the same"), pin, this);
    //connect(m_pindisplay, SIGNAL(accepted()), this, SLOT(displayConfAccepted()));
    //connect(m_pindisplay, SIGNAL(rejected()), this, SLOT(displayConfReject()));
    //m_pindisplay->setOkCancel();
    //m_pindisplay->show();
}

void BluetoothController::displayConfAccepted()
{
    m_localDevice->pairingConfirmation(true);
}
void BluetoothController::displayConfReject()
{
    m_localDevice->pairingConfirmation(false);
}

void BluetoothController::pairingFinished(const QBluetoothAddress &address, QBluetoothLocalDevice::Pairing status)
{
    QBluetoothServiceInfo service;

    if (m_discoveredServices.contains(address.toString())) {
        service = m_discoveredServices.value(address.toString());
    }

    QMessageBox msgBox;
    if (m_pairingError) {
        msgBox.setText("Pairing failed with " + address.toString());
    } else if (status == QBluetoothLocalDevice::Paired
               || status == QBluetoothLocalDevice::AuthorizedPaired) {
        msgBox.setText("Paired successfully with " + address.toString());
    } else {
        msgBox.setText("Pairing released with " + address.toString());
    }


    m_pairingError = false;
    msgBox.exec();
}

void BluetoothController::pairingError(QBluetoothLocalDevice::Error error)
{
    if (error != QBluetoothLocalDevice::PairingError)
        return;

    m_pairingError = true;
    pairingFinished(m_service.device().address(), QBluetoothLocalDevice::Unpaired);
}

void BluetoothController::socketConnected()
{
    emit sockConnect();
}

void BluetoothController::socketRead()
{
    QByteArray data = m_socket->readAll();
    qDebug() << "Incomming data: " << data;
}

void BluetoothController::socketError(QBluetoothSocket::SocketError error)
{
    switch (error) {
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

