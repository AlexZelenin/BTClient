#ifndef GLOBALS_H
#define GLOBALS_H

#include <QObject>
#include <QString>

class DEVICE_INFO : public QObject {
    Q_OBJECT;
public:

    explicit DEVICE_INFO(QObject* parent = Q_NULLPTR) : QObject(parent) {}
    ~DEVICE_INFO() {}

    QString address;
    QString device_name;

};

#endif // GLOBALS_H
