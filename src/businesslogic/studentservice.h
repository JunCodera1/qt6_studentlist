#include <QIODevice>
#include <QObject>
#include <QByteArray>
#include <QDataStream>
#include <QString>

class StudentService : public QObject {
    Q_OBJECT
public:
    explicit StudentService(QObject *parent = nullptr);

    void addStudent(const Student &s);
    void updateFromServer(const Student &s);

signals:
    void studentAdded(const Student &s);

private:
    ClientSocket *socket;
};
