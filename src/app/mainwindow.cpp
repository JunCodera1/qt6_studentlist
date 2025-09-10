#include "mainwindow.h"
#include "../data/dbconnect.h"
#include "../businesslogic/businesslogic.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("Student List");
    resize(800, 600);

    client = new StudentClient(this);
    client->connectToServer();

    QWidget *central = new QWidget(this);
    setCentralWidget(central);

    QVBoxLayout *mainLayout = new QVBoxLayout(central);

    QGridLayout *formLayout = new QGridLayout();
    QLabel *idLabel = new QLabel("ID:");
    QLabel *nameLabel = new QLabel("Name:");
    QLabel *ageLabel = new QLabel("Age:");
    QLabel *majorLabel = new QLabel("Major:");

    idEdit = new QLineEdit();
    nameEdit = new QLineEdit();
    ageSpin = new QSpinBox();
    ageSpin->setRange(15, 100);
    majorBox = new QComboBox();
    majorBox->addItems({"Computer Science", "Math", "Physics", "Literature"});

    formLayout->addWidget(idLabel, 0, 0);
    formLayout->addWidget(idEdit, 0, 1);
    formLayout->addWidget(nameLabel, 1, 0);
    formLayout->addWidget(nameEdit, 1, 1);
    formLayout->addWidget(ageLabel, 2, 0);
    formLayout->addWidget(ageSpin, 2, 1);
    formLayout->addWidget(majorLabel, 3, 0);
    formLayout->addWidget(majorBox, 3, 1);

    mainLayout->addLayout(formLayout);

    table = new QTableWidget(0, 4, central);
    table->setHorizontalHeaderLabels({"ID", "Name", "Age", "Major"});
    table->horizontalHeader()->setStretchLastSection(true);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    mainLayout->addWidget(table);

    QGridLayout *grid = new QGridLayout();
    QPushButton *createBtn = new QPushButton("Create");
    QPushButton *updateBtn = new QPushButton("Update");
    QPushButton *delBtn = new QPushButton("Delete");
    QPushButton *reloadBtn = new QPushButton("Reload");

    grid->addWidget(createBtn, 0, 0);
    grid->addWidget(updateBtn, 0, 1);
    grid->addWidget(delBtn, 1, 0);
    grid->addWidget(reloadBtn, 1, 1);
    mainLayout->addLayout(grid);

    central->setLayout(mainLayout);

    loadStudents();

    connect(client, &StudentClient::jsonReceived, this, [=](const QJsonObject &obj){
        QString action = obj["action"].toString();
        if(action == "student_added" || action == "student_updated" || action == "student_deleted"){
            loadStudents();
        } else if (action == "get_all_students_result"){
            table->setRowCount(0);
            QJsonArray arr = obj["students"].toArray();
            for(const auto &item : arr){
                QJsonObject st = item.toObject();
                int row = table->rowCount();
                table->insertRow(row);
                table->setItem(row, 0, new QTableWidgetItem(st["id"].toString()));
                table->setItem(row, 1, new QTableWidgetItem(st["name"].toString()));
                table->setItem(row, 2, new QTableWidgetItem(QString::number(st["age"].toInt())));
                table->setItem(row, 3, new QTableWidgetItem(st["major"].toString()));
            }
        }
    });

    connect(createBtn, &QPushButton::clicked, this, [=]() {
        QJsonObject obj;
        obj["action"] = "add_student";
        obj["id"] = idEdit->text();
        obj["name"] = nameEdit->text();
        obj["age"] = ageSpin->value();
        obj["major"] = majorBox->currentText();
        client->sendJson(obj);
    });


    connect(updateBtn, &QPushButton::clicked, this, [=]() {
        QJsonObject obj;
        obj["action"] = "update_student";
        obj["id"] = idEdit->text();
        obj["name"] = nameEdit->text();
        obj["age"] = ageSpin->value();
        obj["major"] = majorBox->currentText();
        client->sendJson(obj);
    });

    connect(delBtn, &QPushButton::clicked, this, [=]() {
        QJsonObject obj;
        obj["action"] = "delete_student";
        obj["id"] = idEdit->text();
        client->sendJson(obj);
    });

    connect(reloadBtn, &QPushButton::clicked, this, [=]() {
        QJsonObject obj;
        obj["action"] = "get_all_students";
        client->sendJson(obj);
    });

    connect(table, &QTableWidget::cellClicked, this, [=](int row, int) {
        idEdit->setText(table->item(row, 0)->text());
        nameEdit->setText(table->item(row, 1)->text());
        ageSpin->setValue(table->item(row, 2)->text().toInt());
        majorBox->setCurrentText(table->item(row, 3)->text());
    });
}

MainWindow::~MainWindow() {}

void MainWindow::loadStudents() {
    table->setRowCount(0);
    QString errorMsg;
    QList<QVariantMap> students = BusinessLogic::getAllStudents(errorMsg);
    if (!errorMsg.isEmpty()) {
        QMessageBox::critical(this, "Error", errorMsg);
        return;
    }
    for (const auto &student : students) {
        int row = table->rowCount();
        table->insertRow(row);
        table->setItem(row, 0, new QTableWidgetItem(student["id"].toString()));
        table->setItem(row, 1, new QTableWidgetItem(student["name"].toString()));
        table->setItem(row, 2, new QTableWidgetItem(QString::number(student["age"].toInt())));
        table->setItem(row, 3, new QTableWidgetItem(student["major"].toString()));
    }
}
