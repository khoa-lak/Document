#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMqttClient>
#include <QImage>
#include <QTimer>

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
    void connectButtonClicked();
    void mqttConnected();
    void mqttDisconnected();
    void mqttMsgReceived(const QByteArray &msg, const QMqttTopicName &topic);

private:
    Ui::MainWindow *ui;
    QMqttClient *mqttClient;
    bool isConnected;
};
#endif // MAINWINDOW_H
