#ifndef STUDENTCLIENT_H
#define STUDENTCLIENT_H

#include <QTcpSocket>
#include <QObject>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonParseError>


class StudentClient : public QObject{
    Q_OBJECT
    public:
        explicit StudentClient(QObject *parent = nullptr);

        void connectToServer();
        void sendMessage(const QString &msg);
        void sendJson(const QJsonObject &obj);

    signals:
        void messageReceived(const QString &msg);
        void jsonReceived(const QJsonObject &obj);
    
    private slots:
        void onReadyRead();

    private:
        QTcpSocket *socket;
};
#endif