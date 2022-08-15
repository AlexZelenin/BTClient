#include "bluetoothcontroller.h"

#include <QBluetoothDeviceDiscoveryAgent>
#include <QBluetoothLocalDevice>
#include <QDebug>


BluetoothController::BluetoothController(QObject *parent)
    : QObject{parent}
{
    QBluetoothLocalDevice localDevice;

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

          //startServiceDiscovery();

      } else {
          qDebug() << "bluetooth not available!";
         // emit showStatusMessage( "Bluetooth: not available!" );
      }
}

void BluetoothController::startScanDevices()
{
    // Create a discovery agent and connect to its signals
        QBluetoothDeviceDiscoveryAgent *discoveryAgent = new QBluetoothDeviceDiscoveryAgent(this);
        connect(discoveryAgent, SIGNAL(deviceDiscovered(QBluetoothDeviceInfo)),
                this, SLOT(deviceDiscovered(QBluetoothDeviceInfo)));

        // Start a discovery

            discoveryAgent->start();
}

void BluetoothController::deviceDiscovered(const QBluetoothDeviceInfo &device)
{
    qDebug() << "Found new device:" << device.name() << '(' << device.address().toString() << ')';
}
