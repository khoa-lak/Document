#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow), isConnected(false)
{
    ui->setupUi(this);

    //khởi tạo mqtt
    mqttClient = new QMqttClient(this);
    mqttClient->setHostname("mqtt.iotoom.com");
    mqttClient->setPort(1883);
    mqttClient->setUsername("lab");
    mqttClient->setPassword("IotoomVN");
    mqttClient->setClientId("application-show-video");
    //kết nối các sự kiện của mqtt
    connect(mqttClient, &QMqttClient::connected,this,&MainWindow::mqttConnected);
    connect(mqttClient,&QMqttClient::disconnected,this,&MainWindow::mqttDisconnected);
    connect(mqttClient,&QMqttClient::messageReceived,this,&MainWindow::mqttMsgReceived);
    //kết nối sự kiện nút nhấn
    connect(ui->connectBtn,&QPushButton::clicked,this,&MainWindow::connectButtonClicked);

}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::connectButtonClicked(){
    qDebug()<<"btn clicked";
    if(!isConnected){
        qDebug()<<"btn clicked on";
        mqttClient->connectToHost();
        ui->connectBtn->setText("disconnect");
    }else{
        qDebug()<<"btn clicked false";
        mqttClient->disconnectFromHost();
        ui->connectBtn->setText("connect");
    }
}
void MainWindow::mqttConnected(){
    qDebug()<<"connected to mqtt";
    isConnected = true;
    mqttClient->subscribe(QMqttTopicFilter("devices/esp32cam/video"),1);
}
void MainWindow::mqttDisconnected(){
    qDebug()<<"disconnected from mqtt";
    isConnected = false;
    ui->connectBtn->setText("connect");
}
void MainWindow::mqttMsgReceived(const QByteArray &msg, const QMqttTopicName &topic){
    if(topic.name() == "devices/esp32cam/video"){
        QImage image;
        if(image.loadFromData(msg,"JPEG")){
            ui->videoLabel->setPixmap(QPixmap::fromImage(image).scaled(ui->videoLabel->size(),Qt::KeepAspectRatio));
        }else{
            qDebug()<<"failed to load image";
        }

    }
}
