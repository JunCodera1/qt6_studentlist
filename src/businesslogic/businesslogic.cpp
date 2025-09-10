#include "businesslogic.h"
#include "../data/dbconnect.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

bool BusinessLogic::addStudent(const QString &id, const QString &name, int age, const QString &major, QString &errorMsg){
    if(id.isEmpty() || name.isEmpty()){
        errorMsg = "ID and Name cannot be blank !";
        return false;
    }

    if(age < 15){
        errorMsg = "Age must be >= 15";
        return false;
    }

    QSqlDatabase db = DbConnect::connect("student_connection");
    if(!db.isOpen()){
        errorMsg = "Cannot open DB";
    }

    QSqlQuery query(db);
    query.prepare("INSERT INTO students (id, name, age, major) VALUES (?, ?, ?, ?)");
    query.addBindValue(id);
    query.addBindValue(name);
    query.addBindValue(age);
    query.addBindValue(major);

    if (!query.exec()) {
        errorMsg = query.lastError().text();
        return false;
    }

    return true;
}

QList<QVariantMap> BusinessLogic::getAllStudents(QString &errorMsg) {
    QList<QVariantMap> result;
    QSqlDatabase db = DbConnect::connect("student_connection");
    if (!db.isOpen()) {
        errorMsg = "Cannot open db!";
        return result;
    }

    QSqlQuery query(db);
    if (!query.exec("SELECT id, name, age, major FROM students")) {
        errorMsg = query.lastError().text();
        return result;
    }

    while (query.next()) {
        QVariantMap student;
        student["id"] = query.value(0).toString();
        student["name"] = query.value(1).toString();
        student["age"] = query.value(2).toInt();
        student["major"] = query.value(3).toString();
        result.append(student);
    }

    return result;
}

bool BusinessLogic::deleteStudent(const QString &id, QString &errorMsg) {
    if (id.isEmpty()) {
        errorMsg = "ID cannot be blank!";
        return false;
    }

    QSqlDatabase db = DbConnect::connect("student_connection");
    if (!db.isOpen()) {
        errorMsg = "Cannot open db!";
        return false;
    }

    QSqlQuery query(db);
    query.prepare("DELETE FROM students WHERE id = ?");
    query.addBindValue(id);

    if (!query.exec()) {
        errorMsg = query.lastError().text();
        return false;
    }

    return true;
}

bool BusinessLogic::updateStudent(const QString &id, const QString &name, int age, const QString &major, QString &errorMsg){
    if (id.isEmpty()) {
        errorMsg = "ID cannot be blank";
        return false;
    }

    QSqlDatabase db = DbConnect::connect("student_connection");
    if (!db.isOpen()) {
        errorMsg = "Cannot open DB";
        return false;
    }

    QSqlQuery query(db);
    query.prepare("UPDATE students SET name = ?, age = ?, major = ? WHERE id = ?");
    query.addBindValue(name);
    query.addBindValue(age);
    query.addBindValue(major);
    query.addBindValue(id);

    if (!query.exec()) {
        errorMsg = query.lastError().text();
        return false;
    }

    return true;
}