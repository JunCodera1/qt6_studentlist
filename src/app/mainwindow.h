#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "studentclient.h"

#include <QMainWindow>
#include <QWidget>
#include <QGridLayout>
#include <QPushButton>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QSpacerItem>
#include <QSizePolicy>
#include <QLineEdit>
#include <QSpinBox>
#include <QComboBox>
#include <QLabel>
#include <QTableWidget>
#include <QHeaderView>

#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>


class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    // Widgets
    QLineEdit *idEdit;
    QLineEdit *nameEdit;
    QSpinBox *ageSpin;
    QComboBox *majorBox;
    QTableWidget *table;

    // Methods
    void loadStudents();
    StudentClient *client;
};

#endif
