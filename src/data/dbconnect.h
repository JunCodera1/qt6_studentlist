#ifndef DBCONNECT_H
#define DBCONNECT_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QString>

class DbConnect {
    public:
        static QSqlDatabase connect(const QString &dbName = "students.db");

    private:
        DbConnect() = default;
};

#endif