#include "dbconnect.h"
#include <QSqlError>
#include <QDebug>

QSqlDatabase DbConnect::connect(const QString &connectionName) {
    QSqlDatabase db;
    if (QSqlDatabase::contains(connectionName)) {
        db = QSqlDatabase::database(connectionName);
    } else {
        db = QSqlDatabase::addDatabase("QSQLITE", connectionName);
        db.setDatabaseName("students.db");
    }

    if (!db.isOpen() && !db.open()) {
        qDebug() << "DB error:" << db.lastError().text();
    } else {
        qDebug() << "Database connected to students.db";
    }

    return db;
}

