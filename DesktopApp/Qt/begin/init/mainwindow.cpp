#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), isOn(false) // Khởi tạo trạng thái ban đầu là tắt
{
    // Thiết lập kích thước cửa sổ chính
    setFixedSize(400, 300);

    // Tạo và thiết lập PushButton
    pushButton = new QPushButton(this);
    pushButton->setText("Off"); // Text ban đầu
    pushButton->setStyleSheet("QPushButton { background-color: red; color: white; font-size: 16px; }"); // Nền đỏ, chữ trắng
    pushButton->setGeometry(150, 150, 100, 40); // Vị trí (x, y) và kích thước (width, height)
    pushButton->setFont(QFont("Arial", 12, QFont::Bold)); // Font chữ

    // Tạo và thiết lập Label
    label = new QLabel(this);
    label->setText("Máy đang trạng thái tắt"); // Text ban đầu
    label->setStyleSheet("QLabel { background-color: red; color: white; font-size: 16px; }"); // Nền đỏ, chữ trắng
    label->setGeometry(80, 100, 240, 30); // Vị trí (x, y) và kích thước (width, height)
    label->setFont(QFont("Arial", 12, QFont::Bold)); // Font chữ
    label->setAlignment(Qt::AlignCenter); // Căn giữa text trong label

    // Kết nối sự kiện nhấn nút với slot
    connect(pushButton, &QPushButton::clicked, this, &MainWindow::onButtonClicked);
}

MainWindow::~MainWindow() {
}

void MainWindow::onButtonClicked() {
    if (isOn) {
        // Chuyển sang trạng thái tắt
        pushButton->setText("Off");
        pushButton->setStyleSheet("QPushButton { background-color: red; color: white; font-size: 16px; }"); // Nền đỏ, chữ trắng
        label->setText("Máy đang trạng thái tắt");
        label->setStyleSheet("QLabel { background-color: red; color: white; font-size: 16px; }"); // Nền đỏ, chữ trắng

        isOn = false;
    } else {
        // Chuyển sang trạng thái bật
        pushButton->setText("On");
        pushButton->setStyleSheet("QPushButton { background-color: green; color: white; font-size: 16px; }"); // Nền xanh lá, chữ trắng
        label->setText("Máy đang trạng thái bật");
        label->setStyleSheet("QLabel { background-color: green; color: white; font-size: 16px; }"); // Nền đỏ, chữ trắng

        isOn = true;
    }
}
