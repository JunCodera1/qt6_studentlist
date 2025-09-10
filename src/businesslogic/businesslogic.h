#ifndef BUSINESSLOGIC_H
#define BUSINESSLOGIC_H

#include <QString>
#include <QList>
#include <QVariantMap>

class BusinessLogic {
    public:
        static bool addStudent(const QString &id, const QString &name, int age, const QString &major, QString &errorMsg);

        static QList<QVariantMap> getAllStudents(QString &errorMsg);

        static bool deleteStudent(const QString &id, QString &errorMsg);

        static bool updateStudent(const QString &id, const QString &name, int age, const QString &major, QString &errorMsg);
};

#endif