#ifndef APPSERVER_H
#define APPSERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QList>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

class AppServer : public QTcpServer {
    Q_OBJECT
public:
    explicit AppServer(QObject *parent = nullptr);

protected:
    void incomingConnection(qintptr socketDescriptor) override;

private slots:
    void onReadyRead();
    void onDisconnected();

private:
    QList<QTcpSocket*> clients;

    void sendJson(QTcpSocket *client, const QJsonObject &obj);
    void broadcastJson(const QJsonObject &obj, QTcpSocket *exclude = nullptr);
    void sendAllStudents();

};

#endif
