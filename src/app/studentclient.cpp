#include "studentclient.h"
#include <QDebug>

StudentClient::StudentClient(QObject *parent) : QObject(parent){
    socket = new QTcpSocket(this);
    connect(socket, &QTcpSocket::readyRead, this, &StudentClient::onReadyRead);
}

void StudentClient::connectToServer(){
    socket->connectToHost(QHostAddress::LocalHost, 12345);
    if(!socket->waitForConnected(3000)){
        qDebug() << "Cannot connect to server:" << socket->errorString();
    }
    else{
        qDebug() << "Connected to server!";
    }
}

void StudentClient::sendMessage(const QString &msg){
    if(socket->state() == QAbstractSocket::ConnectedState){
        socket->write(msg.toUtf8() + "\n");
        socket->flush();
    }
}

void StudentClient::sendJson(const QJsonObject &obj){
    QJsonDocument doc(obj);
    QByteArray data = doc.toJson(QJsonDocument::Compact) + "\n";
    if(socket->state() == QAbstractSocket::ConnectedState){
        socket->write(data);
        socket->flush();
    }
}

void StudentClient::onReadyRead() {
    while(socket->canReadLine()){
        QByteArray line = socket->readLine().trimmed();

        emit messageReceived(QString::fromUtf8(line));

        QJsonParseError err;
        QJsonDocument doc = QJsonDocument::fromJson(line, &err);
        if(err.error == QJsonParseError::NoError && doc.isObject()){
            emit jsonReceived(doc.object());
        }
        else{
            qDebug() << "JSON parse error:" << err.errorString() << "->" << line;
        }
    }
}