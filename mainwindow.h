#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "qcustomplot.h"
#include <QTimer>
#include <QtSerialPort>
#include <QSettings>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    float *y = new float[7];
    float *D = new float[7];
    float height;
    float width;
    QFile file;
    QDate cd;
    QTime ct;

private:
    Ui::MainWindow *ui;

    QTimer *dataTimer = new QTimer;
    QSerialPort *serial;
    QByteArray serial_queue;
    QSettings *cfg;

public slots:
    void realtimeDataSlot();
private slots:
    void on_pushButton_clicked();
    void readData();
    void on_lineEdit_textChanged(const QString &arg1);
    void on_lineEdit_2_textChanged(const QString &arg1);
    void on_lineEdit_3_textChanged(const QString &arg1);
    void on_lineEdit_4_textChanged(const QString &arg1);
    void on_lineEdit_5_textChanged(const QString &arg1);
    void on_lineEdit_6_textChanged(const QString &arg1);
    void on_lineEdit_7_textChanged(const QString &arg1);
    void on_horizontalSlider_valueChanged(int value);
    void on_pushButton_2_clicked();
};
#endif // MAINWINDOW_H
