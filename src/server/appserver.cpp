#include "appserver.h"
#include "../businesslogic/businesslogic.h"
#include <QDebug>
#include <QJsonArray>

AppServer::AppServer(QObject *parent) : QTcpServer(parent){
    if (!listen(QHostAddress::Any, 12345)) {
        qDebug() << "Server failed to start:" << errorString();
    } else {
        qDebug() << "Server listening on 12345";
    }
}


void AppServer::incomingConnection(qintptr socketDescriptor){
    QTcpSocket *client = new QTcpSocket(this);
    client->setSocketDescriptor(socketDescriptor);

    clients << client;

    connect(client, &QTcpSocket::readyRead, this, &AppServer::onReadyRead);
    connect(client, &QTcpSocket::disconnected, this, &AppServer::onDisconnected);
    qDebug() << "Client connected:" << client->peerAddress().toString();
}

void AppServer::onReadyRead() {
    QTcpSocket *senderSocket = qobject_cast<QTcpSocket*>(sender());
    if(!senderSocket) return;

    while(senderSocket->canReadLine()) {
        QByteArray line = senderSocket->readLine().trimmed();
        if(line.isEmpty()) continue;

        QJsonParseError parseError;
        QJsonDocument doc = QJsonDocument::fromJson(line, &parseError);
        if(parseError.error != QJsonParseError::NoError) {
            qDebug() << "JSON parse error:" << parseError.errorString() << "->" << line;
            continue;
        }
        if(!doc.isObject()) continue;

        QJsonObject obj = doc.object();
        QString action = obj["action"].toString();
        qDebug() << "Received action:" << action;

        QString errorMsg;

        if(action == "add_student") {
            bool ok = BusinessLogic::addStudent(obj["id"].toString(),
                                                obj["name"].toString(),
                                                obj["age"].toInt(),
                                                obj["major"].toString(),
                                                errorMsg);

            QJsonObject res;
            res["action"] = "add_student_result";
            res["success"] = ok;
            res["error"] = errorMsg;
            sendJson(senderSocket, res);

            if(ok) {
                sendAllStudents();
            }
        }
        else if(action == "update_student") {
            if(BusinessLogic::updateStudent(obj["id"].toString(),
                                            obj["name"].toString(),
                                            obj["age"].toInt(),
                                            obj["major"].toString(),
                                            errorMsg)) {
                sendAllStudents();
            }
        }
        else if(action == "delete_student") {
            if(BusinessLogic::deleteStudent(obj["id"].toString(), errorMsg)) {
                sendAllStudents();
            }
        }
        else if(action == "get_all_students") {
            QList<QVariantMap> students = BusinessLogic::getAllStudents(errorMsg);
            QJsonArray arr;
            for(const auto &st : students) {
                QJsonObject s;
                s["id"] = st["id"].toString();
                s["name"] = st["name"].toString();
                s["age"] = st["age"].toInt();
                s["major"] = st["major"].toString();
                arr.append(s);
            }
            QJsonObject res;
            res["action"] = "get_all_students_result";
            res["students"] = arr;
            sendJson(senderSocket, res);
        }
    }
}


void AppServer::onDisconnected() {
    QTcpSocket *client = qobject_cast<QTcpSocket*>(sender());
    clients.removeAll(client);
    client->deleteLater();
    qDebug() << "Client disconnected";
}

void AppServer::sendJson(QTcpSocket *client, const QJsonObject &obj) {
    QJsonDocument doc(obj);
    client->write(doc.toJson(QJsonDocument::Compact) + "\n");
    client->flush();
}

void AppServer::broadcastJson(const QJsonObject &obj, QTcpSocket *exclude) {
    QJsonDocument doc(obj);
    QByteArray bytes = doc.toJson(QJsonDocument::Compact) + "\n";
    for (QTcpSocket *c : clients) {
        if (c != exclude) {
            c->write(bytes);
            c->flush();
        }
    }
}

void AppServer::sendAllStudents() {
    QString errorMsg;
    QList<QVariantMap> students = BusinessLogic::getAllStudents(errorMsg);

    QJsonArray arr;
    for(const auto &st : students) {
        QJsonObject s;
        s["id"] = st["id"].toString();
        s["name"] = st["name"].toString();
        s["age"] = st["age"].toInt();
        s["major"] = st["major"].toString();
        arr.append(s);
    }

    QJsonObject res;
    res["action"] = "get_all_students_result";
    res["students"] = arr;
    broadcastJson(res, nullptr);
}


