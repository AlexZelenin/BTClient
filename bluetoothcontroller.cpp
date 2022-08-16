#include "bluetoothcontroller.h"

#include <QBluetoothDeviceDiscoveryAgent>
#include <QBluetoothLocalDevice>
#include <QBluetoothTransferManager>
#include <QBluetoothTransferRequest>

#include <QFile>
#include <QDebug>


BluetoothController::BluetoothController(QObject *parent)
    : QObject{parent}
{
    /* QBluetoothLocalDevice localDevice;

      // Check if Bluetooth is available on this device
      if (localDevice.isValid()) {

          // Turn Bluetooth on
          localDevice.powerOn();

          // Read local device name
          auto localDeviceName = localDevice.address().toString();
          qDebug() << "local bluetooth device name: " << localDeviceName;
          // Make it visible to others
          //localDevice.setHostMode(QBluetoothLocalDevice::HostDiscoverable);

          // Get connected devices
          auto remotes = localDevice.connectedDevices();

          qDebug() << "remotes count: " << remotes.count();


          //startServiceDiscovery();

      } else {
          qDebug() << "bluetooth not available!";
         // emit showStatusMessage( "Bluetooth: not available!" );
      } */
}

void BluetoothController::startScanDevices()
{

    QBluetoothDeviceDiscoveryAgent *discoveryAgent = new QBluetoothDeviceDiscoveryAgent(this);
    connect(discoveryAgent, SIGNAL(deviceDiscovered(QBluetoothDeviceInfo)),
            this, SLOT(deviceDiscovered(QBluetoothDeviceInfo)));

    discoveryAgent->start();
}

void BluetoothController::pushData(const QString &address)
{

    QBluetoothTransferManager *transferManager = new QBluetoothTransferManager(this);

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
    }
}

void BluetoothController::deviceDiscovered(const QBluetoothDeviceInfo &device)
{
    qDebug() << "Found new device:" << device.name() << '(' << device.address().toString() << ')';
    emit addDevice(device);
    QBluetoothUuid uuid(QBluetoothUuid::OBEXFileTransfer);
    qDebug() << "UUID: " << uuid.toString();
        m_socket = new QBluetoothSocket(this);
        m_socket->connectToService(device.address(), uuid);
}

void BluetoothController::transferFinished(QBluetoothTransferReply * replay)
{
}

void BluetoothController::error(QBluetoothTransferReply::TransferError error)
{
    switch (error) {
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
