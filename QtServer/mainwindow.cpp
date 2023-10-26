#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 状态栏
    ui->statusbar->addWidget(new QLabel("连接状态:"));
    m_status = new QLabel(this);
    m_status->setStyleSheet("color:red");
    m_status->setText("无连接");
    ui->statusbar->addWidget(m_status);
    ui->sendMsg->setDisabled(true);
    ui->portEdit->setDisabled(false);

    //默认端口
    ui->portEdit->setText("8899");
    //创建监听的服务器对象
    m_s = new QTcpServer(this);
    //监听连接
    connect(m_s, &QTcpServer::newConnection, this, [=](){
        m_tcp = m_s->nextPendingConnection();
        ui->record->append("已成功连接到客户端...");
        m_status->setStyleSheet("color:green");
        m_status->setText("客户端连接中");
        ui->sendMsg->setDisabled(false);

        //检测是否有数据发送
        connect(m_tcp, QTcpSocket::readyRead, this, [=](){
            QByteArray data = m_tcp->readAll();
            ui->record->append("客户端say:" + data);
        });
        //检查是否断开连接
        connect(m_tcp, &QTcpSocket::disconnected, this, [=](){
            m_tcp->close();
            m_tcp->deleteLater();
            m_status->setStyleSheet("color:red");
            m_status->setText("无连接");
            ui->record->append("已断开...");
            ui->sendMsg->setDisabled(true);
        });
    });



}

MainWindow::~MainWindow()
{
    delete ui;
}

//开始监听
void MainWindow::on_setListen_clicked()
{
    ui->portEdit->setDisabled(true);
    unsigned short port = ui->portEdit->text().toUShort();
    m_s->listen(QHostAddress::Any, port);
    ui->setListen->setDisabled(true);
}

//发送数据
void MainWindow::on_sendMsg_clicked()
{
    QString msg = ui->msg->toPlainText();
    m_tcp->write(msg.toUtf8());
    ui->record->append("服务器say:" + msg);
}
