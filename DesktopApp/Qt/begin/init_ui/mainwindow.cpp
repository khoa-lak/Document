#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow), isOn(false)
{
    ui->setupUi(this);
    connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::on_toggleButton_clicked);
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::on_toggleButton_clicked() {
    if (isOn) {
        // Chuyển sang trạng thái tắt
        ui->pushButton->setText("Off");
        ui->pushButton->setStyleSheet("background-color: red; color: white; font-size: 16px;");
        ui->label->setText("Máy đang trạng thái tắt");
        isOn = false;
    } else {
        // Chuyển sang trạng thái bật
        ui->pushButton->setText("On");
        ui->pushButton->setStyleSheet("background-color: green; color: white; font-size: 16px;");
        ui->label->setText("Máy đang trạng thái bật");
        isOn = true;
    }
}
