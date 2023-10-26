#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QLabel>
#include <QHostAddress>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void on_sendMsg_clicked();

    void on_conBtn_clicked();

    void on_disConBtn_clicked();

private:
    Ui::MainWindow *ui;
    QTcpSocket *m_tcp;
    QLabel *m_status;
};
#endif // MAINWINDOW_H
