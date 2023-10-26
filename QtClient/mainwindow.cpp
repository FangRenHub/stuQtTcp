#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("客户端");

    // 状态栏
    ui->statusbar->addWidget(new QLabel("连接状态:"));
    m_status = new QLabel(this);
    m_status->setStyleSheet("color:red");
    m_status->setText("无连接");
    ui->statusbar->addWidget(m_status);
    ui->sendMsg->setDisabled(true);
    ui->disConBtn->setDisabled(true);

    //默认端口和ip
    ui->portEdit->setText("8899");
    ui->ipEdit->setText("127.0.0.1");

    //创建客户端套接字对象
    m_tcp = new QTcpSocket(this);

    //检查是否断开连接
    connect(m_tcp, &QTcpSocket::disconnected, this, [=](){
        m_tcp->close();
        m_status->setStyleSheet("color:red");
        m_status->setText("无连接");
        ui->record->append("断开连接...");
        ui->sendMsg->setDisabled(true);
        ui->disConBtn->setDisabled(true);
        ui->conBtn->setDisabled(false);
    });

    //监听消息
    connect(m_tcp, &QTcpSocket::readyRead, this, [=](){
        QByteArray data = m_tcp->readAll();
        ui->record->append("服务器say:" + data);
    });

    connect(m_tcp, &QTcpSocket::connected, this, [=](){
        m_status->setStyleSheet("color:green");
        m_status->setText("已连接");
        ui->record->append("已成功连接到服务器...");
        ui->sendMsg->setDisabled(false);
        ui->disConBtn->setDisabled(false);
        ui->conBtn->setDisabled(true);
    });

}

MainWindow::~MainWindow()
{
    delete ui;
}


//发送数据
void MainWindow::on_sendMsg_clicked()
{
    QString msg = ui->msg->toPlainText();
    m_tcp->write(msg.toUtf8());
    ui->record->append("客户端say:" + msg);
}

void MainWindow::on_conBtn_clicked()
{
    QString ip = ui->ipEdit->text();
    unsigned short port = ui->portEdit->text().toUShort();
    m_tcp->connectToHost(QHostAddress(ip),port);
}

void MainWindow::on_disConBtn_clicked()
{
    m_tcp->close();
    ui->disConBtn->setDisabled(true);
    ui->conBtn->setDisabled(false);
}
