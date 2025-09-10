#include "studentservice.h"

void StudentService::addStudent(const Student &s) {
    QByteArray data;
    QDataStream out(&data, QIODevice::WriteOnly);
    out << QString("ADD") << s.id << s.name << s.age << s.major;
    socket->send(data);
}

void StudentService::updateFromServer(const Student &s) {
    emit studentAdded(s);
}
