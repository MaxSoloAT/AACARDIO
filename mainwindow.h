#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "qcustomplot.h"
#include <QTimer>
#include <QtSerialPort>
#include <QSettings>
#include "maintab.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    float *y = new float[7]; // y from parsing
    float *D = new float[7]; // bias
    float height;
    float width;
    QFile file; //file for save data
    QTextStream out; // out to file
    QDate cd; //current data
    QTime ct; //current time
    QTabWidget *tabWidget;
    MainTab *mainTab;

    QString gn1, gn2, gn3, gn4, gn5, gn6, gn7;
private:
    Ui::MainWindow *ui;
    QTimer *dataTimer = new QTimer;
    QSerialPort *serial; // port info like list eth.
    QByteArray serial_queue; //all data from serial
    QSettings *cfg; //confoguration port and baudrate
    QCPItemStraightLine *infLine;
    long int key = 0; // means x for x axis
    void getPortList();
    void getBaudrate();
    void initSettingGraph(QLabel *graph_name, QLabel *graph_number, QLineEdit *min, QLineEdit *max, QString gn);
    void initSettingSlider(QColor col, QSlider *slider);

public slots:
    void realtimeDataSlot();

private slots:
    void on_pushButton_clicked();
    void readData();
    void on_horizontalSlider_valueChanged(int value);
    void on_pushButton_2_clicked();
    void clickedGraph(QMouseEvent *event);
    void on_graph_1_slider_valueChanged(int value);
    void on_graph_2_slider_valueChanged(int value);
    void on_graph_3_slider_valueChanged(int value);
    void on_graph_4_slider_valueChanged(int value);
    void on_graph_5_slider_valueChanged(int value);
    void on_graph_6_slider_valueChanged(int value);
    void on_graph_7_slider_valueChanged(int value);
    void on_graph_1_line_min_textChanged(const QString &arg1);
    void on_graph_1_line_max_textChanged(const QString &arg1);
    void on_graph_2_line_min_textChanged(const QString &arg1);
    void on_graph_2_line_max_textChanged(const QString &arg1);
    void on_graph_3_line_min_textChanged(const QString &arg1);
    void on_graph_3_line_max_textChanged(const QString &arg1);
    void on_graph_4_line_min_textChanged(const QString &arg1);
    void on_graph_4_line_max_textChanged(const QString &arg1);
    void on_graph_5_line_min_textChanged(const QString &arg1);
    void on_graph_5_line_max_textChanged(const QString &arg1);
    void on_graph_6_line_min_textChanged(const QString &arg1);
    void on_graph_6_line_max_textChanged(const QString &arg1);
    void on_graph_7_line_min_textChanged(const QString &arg1);
    void on_graph_7_line_max_textChanged(const QString &arg1);
};
#endif // MAINWINDOW_H
