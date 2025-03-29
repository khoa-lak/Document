#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QLabel>

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onButtonClicked(); // Slot để xử lý sự kiện nhấn nút

private:
    QPushButton *pushButton;
    QLabel *label;
    bool isOn; // Biến để theo dõi trạng thái bật/tắt
};

#endif // MAINWINDOW_H
