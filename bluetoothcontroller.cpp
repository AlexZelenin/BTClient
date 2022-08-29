#include "bluetoothcontroller.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QPointer>
#include <QTimer>

#include <QDebug>


BluetoothController::BluetoothController(QObject *parent)
    : QObject{parent}
{
    m_localDevice = new BluetoothLocalDevice(this);
    BluetoothAddress adapterAddress = m_localDevice->address();

    qDebug() << "Adapter address: " << adapterAddress.toString();

    m_discoveryAgent = new BluetoothDeviceDiscoveryAgent(adapterAddress);

    connect(m_discoveryAgent, &BluetoothDeviceDiscoveryAgent::deviceDiscovered, this, &BluetoothController::serviceDiscovered);
    connect(m_localDevice, &BluetoothLocalDevice::pairingDisplayPinCode, this, &BluetoothController::displayPin);
    connect(m_localDevice, &BluetoothLocalDevice::pairingDisplayConfirmation, this, &BluetoothController::displayConfirmation);
    connect(m_localDevice, &BluetoothLocalDevice::pairingFinished, this, &BluetoothController::pairingFinished);
    connect(m_localDevice, &BluetoothLocalDevice::error, this, &BluetoothController::pairingError);

    //m_socket = new BluetoothSocket;

    //connect(m_socket, &BluetoothSocket::connected, this, &BluetoothController::socketConnected);
    //connect(m_socket, &BluetoothSocket::readyRead, this, &BluetoothController::socketRead);
    //connect(m_socket, &BluetoothSocket::error, this, &BluetoothController::socketError);
}

BluetoothController::~BluetoothController()
{
    m_discoveryAgent->deleteLater();
}

void BluetoothController::startDiscovery(const BluetoothUuid &uuid)
{

    if (m_discoveryAgent->isActive())
        m_discoveryAgent->stop();

    //m_discoveryAgent->setUuidFilter(uuid);
    m_discoveryAgent->start();

    if (!m_discoveryAgent->isActive() ||
            m_discoveryAgent->error() != BluetoothDeviceDiscoveryAgent::NoError) {
        return;
    }

}

BluetoothDeviceInfo BluetoothController::service() const
{
    return m_service;
}

void BluetoothController::serviceDiscovered(const BluetoothDeviceInfo &serviceInfo)
{


    QString remoteName;
    if (serviceInfo.name().isEmpty())
        remoteName = serviceInfo.address().toString();
    else
        remoteName = serviceInfo.name();

    const BluetoothAddress address = serviceInfo.address();
    for (BluetoothDeviceInfo &info : m_discoveredServices) {
        if (info.address() == address){
            info = serviceInfo;
            return;
        }
    }

    m_discoveredServices.insert(serviceInfo.address().toString(), serviceInfo);
    emit addDevice(serviceInfo);

}

void BluetoothController::startDiscovery()
{
    startDiscovery(BluetoothUuid(Protocol::OBEX));
}

void BluetoothController::fileSelect()
{
    const QString filepath = QFileDialog::getOpenFileName();
    emit openFile(filepath);
}

static void doDeleteLater(QFile* obj) {
    obj->deleteLater();
}

void BluetoothController::sendFile(const QString& address, const QString& filepath)
{
    m_manager = new BluetoothTransferManager;
    BluetoothTransferRequest req(m_discoveredServices[address].address());

    QSharedPointer<QFile> file = QSharedPointer<QFile>(new QFile, doDeleteLater);
    file->setFileName(filepath);
    QFileInfo file_info(filepath);

    req.setAttribute(BluetoothTransferRequest::Attribute::NameAttribute, file_info.fileName());

    m_reply = m_manager->put(req, file);

    m_reply->setParent(this);

    if (m_reply->error() != BluetoothTransferReply::NoError){
        qDebug() << "Failed to send file";
        m_reply->deleteLater();
        return;
    }

    connect(m_reply, &BluetoothTransferReply::transferProgress, this, &BluetoothController::transferProgress, Qt::QueuedConnection);
    connect(m_reply, &BluetoothTransferReply::finished, this, &BluetoothController::transferFinished, Qt::QueuedConnection);
}

void BluetoothController::stopButton()
{
    m_discoveryAgent->stop();
}

#if 0
void BluetoothController::startConnect(const QString& address) const
{
    /*if (m_discoveredServices.contains(address)) {
        m_socket->disconnectFromService();
        qDebug() << "Address: " << address << " contains";
        m_socket->connectToService(BluetoothAddress(address), port.toInt());

        qDebug()  << m_socket->socketType();
    }*/
}
#endif

void BluetoothController::sendData(const QString& deviceName, const QString& port, const QString &data)
{
    qDebug() << "Send to: " << deviceName;

    BluetoothAddress address(deviceName);
    m_socket = new BluetoothSocket;

    if (BluetoothSocket::SocketState::UnconnectedState == m_socket->state()) {
        qDebug() << "Socket unconnected";
    } else {
        qDebug() << "Socket already connected";
    }


    connect(m_socket, &BluetoothSocket::connected, this, [this, data](){
        qDebug() << "Socket connected";
        m_socket->write(data.toUtf8(), data.length());
    }, Qt::QueuedConnection);

    connect(m_socket, QOverload<BluetoothSocket::SocketError>::of(&BluetoothSocket::error), this,
            &BluetoothController::socketError, Qt::QueuedConnection);

    connect(m_socket, &BluetoothSocket::readyRead, this, &BluetoothController::socketRead, Qt::QueuedConnection);

    connect(m_socket, &BluetoothSocket::disconnected, this, &BluetoothController::socketDisconnected, Qt::QueuedConnection);

    QTimer::singleShot(100, [this, address, port]()
    {
        m_socket->connectToService(address, port.toUInt());
    });

}

QString BluetoothController::addressToName(const BluetoothAddress &address) const
{

    for (const BluetoothDeviceInfo &info : m_discoveredServices) {
        if (info.address() == address)
            return info.name();
    }
    return address.toString();
}

void BluetoothController::displayPin(const BluetoothAddress &address, QString pin)
{
    Q_UNUSED(address);
    Q_UNUSED(pin);
}

void BluetoothController::displayConfirmation(const BluetoothAddress &address, QString pin)
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

void BluetoothController::transferFinished()
{
    qDebug() << "Transfer finished";
    m_reply->deleteLater();
    m_manager->deleteLater();
    //emit exitPrgrsDlg();
}

void BluetoothController::displayConfReject()
{
    m_localDevice->pairingConfirmation(false);
}

void BluetoothController::pairingFinished(const BluetoothAddress &address, BluetoothLocalDevice::Pairing status)
{
    QMessageBox msgBox;
    if (m_pairingError) {
        msgBox.setText("Pairing failed with " + address.toString());
    } else if (status == BluetoothLocalDevice::Paired
               || status == BluetoothLocalDevice::AuthorizedPaired) {
        msgBox.setText("Paired successfully with " + address.toString());
    } else {
        msgBox.setText("Pairing released with " + address.toString());
    }


    m_pairingError = false;
    msgBox.exec();
}

void BluetoothController::pairingError(BluetoothLocalDevice::Error error)
{
    if (error != BluetoothLocalDevice::PairingError)
        return;

    m_pairingError = true;
    pairingFinished(m_service.address(), BluetoothLocalDevice::Unpaired);
}

void BluetoothController::socketConnected()
{
    qDebug() << "Socket connected";

    emit sockConnect();
}

void BluetoothController::socketRead()
{
    const QByteArray data = m_socket->readAll();
    qDebug() << "Incomming data: " << data;
}

void BluetoothController::socketError(BluetoothSocket::SocketError error)
{

    qDebug() << "Socket error: " << error;

    switch (error) {
    case BluetoothSocket::SocketError::NoSocketError:
        qDebug() << "QBluetoothSocket::NoSocketError";
        break;
    case BluetoothSocket::SocketError::UnknownSocketError:
        qDebug() << "QBluetoothSocket::UnknownSocketError";
        break;
    case BluetoothSocket::SocketError::RemoteHostClosedError:
        qDebug() << "QBluetoothSocket::RemoteHostClosedError";
        break;
    case BluetoothSocket::SocketError::HostNotFoundError:
        qDebug() << "QBluetoothSocket::HostNotFoundError";
        break;
    case BluetoothSocket::SocketError::ServiceNotFoundError:
        qDebug() << "QBluetoothSocket::ServiceNotFoundError";
        break;
    case BluetoothSocket::SocketError::NetworkError:
        qDebug() << "QBluetoothSocket::NetworkError";
        break;
    case BluetoothSocket::SocketError::UnsupportedProtocolError:
        qDebug() << "QBluetoothSocket::UnsupportedProtocolError";
        break;
    case BluetoothSocket::SocketError::OperationError:
        qDebug() << "QBluetoothSocket::OperationError";
        break;
    }

}

void BluetoothController::socketDisconnected()
{
    qDebug() << "Socket disconnected";
    m_socket->deleteLater();
}

