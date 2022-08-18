#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "bluetoothcontroller.h"
#include "devicemodel.h"

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    QApplication app(argc, argv);

    BluetoothController controller;
    DeviceModel model;

    QObject::connect(&controller, SIGNAL(addDevice(QBluetoothDeviceInfo)),
                     &model, SLOT(addDevice(QBluetoothDeviceInfo)));

    QQmlApplicationEngine engine;

    engine.rootContext()->setContextProperty("controller", &controller);
    engine.rootContext()->setContextProperty("devicemodel", &model);

    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
