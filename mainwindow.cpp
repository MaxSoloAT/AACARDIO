#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QRect rec = QApplication::desktop()->screenGeometry();
    height = rec.height();
    width = rec.width();
    /*QSize size = this->size();
    height = size.height();
    width =  size.width();*/
    //setFixedSize(width, height);
    setWindowState(Qt::WindowFullScreen);
    ui->customPlot->setGeometry(20,20,width-0.01*width,height-0.25*height);
    ui->groupBox->setGeometry(20,height-0.2*height,280,90);
    ui->groupBox_2->setGeometry(300,height-0.2*height,420,90);
    ui->horizontalSlider->setGeometry(740,height-0.2*height,420,50);
    ui->horizontalSlider->setStyleSheet("QSlider::handle:horizontal {background-color:red;}");
    ui->label_8->setGeometry(900,height-0.18*height,100,20);
    ui->pushButton_2->setGeometry(20,height-0.10*height,80,21);
    ui->pushButton_2->setStyleSheet("background-color: red");
    ui->label_8->setText("0 ms");
    file.setFileName("Log.txt");
    file.open(QIODevice::WriteOnly);


    // Serial port list.
    QList<QSerialPortInfo> portlist = QSerialPortInfo::availablePorts();
    for (int i=portlist.size()-1; i>=0; i--) {
        QString portname = portlist.at(i).portName();
        portname += ":";
        portname += portlist.at(i).description();
        ui->Port->addItem(portname);
    }

    // Serial baudrate list.
    QList<qint32> baudlist = QSerialPortInfo::standardBaudRates();
    for (int i=0; i<baudlist.size(); i++) {
        qint32 baud_int = baudlist.at(i);
        QString baud_str = QString("%1").arg(baud_int);
        ui->Baudrate->addItem(baud_str);
    }

    // Serial port settings.
    serial = new QSerialPort();
    connect(serial, SIGNAL(readyRead()), this, SLOT(readData()));

    // Load setting data.
    cfg = new QSettings("config.ini", QSettings::IniFormat);
    cfg->setIniCodec("UTF-8");

    // Load the serial port setting.
    QString portname = cfg->value("Serial/Port", "COM1").toString();
    int port_idx = ui->Port->findText(portname);
    if ( port_idx != -1 ) {
        ui->Port->setCurrentIndex( port_idx );
    }

    // Load the current settings.
    QString baudrate = cfg->value("Serial/Baudrate", "9800").toString();
    int baud_idx = ui->Baudrate->findText(baudrate);
    if ( baud_idx != -1 ) {
        ui->Baudrate->setCurrentIndex(baud_idx);
    }

    ui->customPlot->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

    ui->customPlot->addGraph(); // blue line
    ui->customPlot->graph(0)->setPen(QPen(QColor(40, 110, 255)));
    ui->customPlot->addGraph(); // red line
    ui->customPlot->graph(1)->setPen(QPen(QColor(255, 110, 40)));
    ui->customPlot->addGraph(); // red line
    ui->customPlot->graph(2)->setPen(QPen(QColor(125, 250, 140)));
    ui->customPlot->addGraph(); // red line
    ui->customPlot->graph(3)->setPen(QPen(QColor(100, 100, 140)));
    ui->customPlot->addGraph(); // red line
    ui->customPlot->graph(4)->setPen(QPen(QColor(200, 200, 140)));
    ui->customPlot->addGraph(); // red line
    ui->customPlot->graph(5)->setPen(QPen(QColor(30, 250, 140)));
    ui->customPlot->addGraph(); // red line
    ui->customPlot->graph(6)->setPen(QPen(QColor(125, 50, 40)));

    QSharedPointer<QCPAxisTickerTime> timeTicker(new QCPAxisTickerTime);
    timeTicker->setTimeFormat("%h:%m:%s");
    ui->customPlot->xAxis->setTicker(timeTicker);
    ui->customPlot->axisRect()->setupFullAxesBox();
    ui->customPlot->yAxis->setRange(-100, 1000);

    // make left and bottom axes transfer their ranges to right and top axes:
    connect(ui->customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->customPlot->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->customPlot->yAxis2, SLOT(setRange(QCPRange)));

    // setup a timer that repeatedly calls MainWindow::realtimeDataSlot:
    connect(dataTimer, SIGNAL(timeout()), this, SLOT(realtimeDataSlot()));
    dataTimer->start(0); // Interval 0 means to refresh as fast as possible

}

MainWindow::~MainWindow()
{
    file.close();
    delete ui;
}

void MainWindow::realtimeDataSlot()
{
    static QTime time(QTime::currentTime());
    // calculate two new data points:
    double key = time.elapsed()/1000.0; // time elapsed since start of demo, in seconds
    static double lastPointKey = 0;
    if (key-lastPointKey > 0.002) // at most add point every 2 ms
    {
      // add data to lines:
      ui->customPlot->graph(0)->addData(key, y[0] + D[0]);
      ui->customPlot->graph(1)->addData(key, y[1] + D[1]);
      ui->customPlot->graph(2)->addData(key, y[2] + D[2]);
      ui->customPlot->graph(3)->addData(key, y[3] + D[3]);
      ui->customPlot->graph(4)->addData(key, y[4] + D[4]);
      ui->customPlot->graph(5)->addData(key, y[5] + D[5]);
      ui->customPlot->graph(6)->addData(key, y[6] + D[6]);
      // rescale value (vertical) axis to fit the current data:
      //ui->customPlot->graph(0)->rescaleValueAxis();
      //ui->customPlot->graph(1)->rescaleValueAxis(true);
      lastPointKey = key;
    }
    // make key axis range scroll with the data (at a constant range size of 8):
    ui->customPlot->xAxis->setRange(key, 8, Qt::AlignRight);
    ui->customPlot->replot();

    // calculate frames per second:
    static double lastFpsKey;
    static int frameCount;
    ++frameCount;
    if (key-lastFpsKey > 2) // average fps over 2 seconds
    {

    }
}


void MainWindow::on_pushButton_clicked()
{
    if (serial->isOpen() ) {
        serial->close();
        ui->pushButton->setText("Connect");
        ui->statusbar->showMessage("The port is closed", 1000);
    } else {
        // Set port name from the combo box.
        QString portname = ui->Port->currentText();
        int port_sep_idx = portname.indexOf(":");
        portname = portname.left(port_sep_idx);
        serial->setPortName(portname);
        // Set baudrate from the combo box.
        qint32 baudrate = ui->Baudrate->currentText().toInt();
        serial->setBaudRate(baudrate);
        // Reset the starting line.

        if (serial->open(QIODevice::ReadWrite) == false ) { // Failed to open the port.
            ui->statusbar->showMessage("Failed to open COM port", 1000);
        } else {
            ui->pushButton->setText("Disconnect");
            ui->statusbar->showMessage("The port is opened.", 1000);
        }
    }
}


void MainWindow::readData()
{
    serial_queue += serial->readAll();
    QTextStream out(&file);

    int j;
    while ( (j = serial_queue.indexOf("\r\n")) != -1 ) {
        cd = QDate::currentDate();
        ct = QTime::currentTime();
        out<<cd.toString("yyyy-MM-dd");
        out<<" ";
        out<<ct.toString("hh:mm:ss.zzz");
        out<<" ";
        out<<serial_queue;
        out<<"\r\n";
        // Extraction of the current processing line and eliminate it for next processing.
        QByteArray line = serial_queue.mid(0, j);
        serial_queue.remove(0, j+2);
        // A process of current line.
        QList<QByteArray> elems = line.split(',');
        int n_elems = elems.length();

        for (int i=0; i < n_elems; i++) {
           // qDebug()<<"n_curves"<<i<<endl;
           double value = (double) elems[i].toDouble();
           y[i]=value;
           qDebug()<< value <<endl;
        }

    }

}

void MainWindow::on_lineEdit_textChanged(const QString &arg1)
{
    D[0] = arg1.toFloat();
}


void MainWindow::on_lineEdit_2_textChanged(const QString &arg1)
{
    D[1] = arg1.toFloat();
}


void MainWindow::on_lineEdit_3_textChanged(const QString &arg1)
{
    D[2] = arg1.toFloat();
}


void MainWindow::on_lineEdit_4_textChanged(const QString &arg1)
{
    D[3] = arg1.toFloat();
}


void MainWindow::on_lineEdit_5_textChanged(const QString &arg1)
{
    D[4] = arg1.toFloat();
}


void MainWindow::on_lineEdit_6_textChanged(const QString &arg1)
{
    D[5] = arg1.toFloat();
}



void MainWindow::on_lineEdit_7_textChanged(const QString &arg1)
{
    D[6] = arg1.toFloat();
}


void MainWindow::on_horizontalSlider_valueChanged(int value)
{
    ui->label_8->setText(QString::number(value)+" ms");
    dataTimer->start(value);
}



void MainWindow::on_pushButton_2_clicked()
{
    close();
}

