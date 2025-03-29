#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    void on_toggleButton_clicked(); // Slot để xử lý sự kiện nhấn nút
private:
    Ui::MainWindow *ui;
    bool isOn; // Biến để theo dõi trạng thái bật/tắt
};
#endif // MAINWINDOW_H
