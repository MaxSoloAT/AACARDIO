#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //some app geometry
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
    ui->label_9->setText("pulse");
    ui->label_9->setGeometry(1000,height-0.08*height, 80,25);

    //work with file
    file.setFileName("Log.txt");
    file.open(QIODevice::WriteOnly);

    getPortList();
    getBaubrate();




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

    //ui->customPlot->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    //ui->customPlot->setOpenGl(true);

    int l = 1;
    QColor col1(0, 0, 255);
    QPalette palette1;
    ui->customPlot->addGraph(); // blue line
    ui->customPlot->graph(0)->setPen(QPen(col1,l)); //vibration dark blue
    //ui->customPlot->graph(0)->setScatterStyle(QCPScatterStyle::ssCross);
    palette1.setColor(QPalette::Base, col1);
    palette1.setColor(QPalette::Text, Qt::white);
    ui->lineEdit->setPalette(palette1);
    ui->lineEdit->setText("0");

    QColor col2(50, 0, 0);
    QPalette palette2;
    ui->customPlot->addGraph(); // red line
    ui->customPlot->graph(1)->setPen(QPen(col2,l)); // breath brown
    palette2.setColor(QPalette::Base, col2);
    palette2.setColor(QPalette::Text, Qt::white);
    ui->lineEdit_2->setPalette(palette2);
    ui->lineEdit_2->setText("0");

    QColor col3(0, 0, 0);
    QPalette palette3;
    ui->customPlot->addGraph(); // red line
    ui->customPlot->graph(2)->setPen(QPen(col3,l));// kgr black
    palette3.setColor(QPalette::Base, col3);
    palette3.setColor(QPalette::Text, Qt::white);
    ui->lineEdit_3->setPalette(palette3);
    ui->lineEdit_3->setText("0");

    QColor col4(140, 0, 0);
    QPalette palette4;
    ui->customPlot->addGraph(); // red line
    ui->customPlot->graph(3)->setPen(QPen(col4,l)); // pulse dark red
    palette4.setColor(QPalette::Base, col4);
    palette4.setColor(QPalette::Text, Qt::white);
    ui->lineEdit_4->setPalette(palette4);
    ui->lineEdit_4->setText("0");

    QColor col5(180, 180, 84);
    QPalette palette5;
    ui->customPlot->addGraph(); // red line
    ui->customPlot->graph(4)->setPen(QPen(col5,l)); // random dark
    palette5.setColor(QPalette::Base, col5);
    palette5.setColor(QPalette::Text, Qt::white);
    ui->lineEdit_5->setPalette(palette5);
    ui->lineEdit_5->setText("0");

    QColor col6(200, 20, 150);
    QPalette palette6;
    ui->customPlot->addGraph(); // red line
    ui->customPlot->graph(5)->setPen(QPen(col6,l)); //
    palette6.setColor(QPalette::Base, col6);
    palette6.setColor(QPalette::Text, Qt::white);
    ui->lineEdit_6->setPalette(palette6);
    ui->lineEdit_6->setText("0");


    QColor col7(125, 150, 140);
    ui->customPlot->addGraph(); // red line
    ui->customPlot->graph(6)->setPen(QPen(col7,l));
    QPalette palette7;
    palette7.setColor(QPalette::Base, col7);
    palette7.setColor(QPalette::Text, Qt::white);
    ui->lineEdit_7->setPalette(palette7);
    ui->lineEdit_7->setText("0");

    ui->customPlot->setInteractions(QCP::iRangeDrag|QCP::iRangeZoom);
   // QSharedPointer<QCPAxisTickerTime> timeTicker(new QCPAxisTickerTime);
   // timeTicker->setTimeFormat("%h:%m:%s");
    //ui->customPlot->xAxis->setTicker(timeTicker);

    ui->customPlot->axisRect()->setupFullAxesBox();
    ui->customPlot->yAxis->setRange(-10, 5000);

    // make left and bottom axes transfer their ranges to right and top axes:
    connect(ui->customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->customPlot->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->customPlot->yAxis2, SLOT(setRange(QCPRange)));
    // connect(this, SIGNAL(readData()), this, SLOT(realtimeDataSlot()));
    //ALOOOOOOOOOOO
    // setup a timer that repeatedly calls MainWindow::realtimeDataSlot:
    //connect(dataTimer, SIGNAL(timeout()), this, SLOT(realtimeDataSlot2()));
   // dataTimer->start(1000); // Interval 0 means to refresh as fast as possible
    //ui->customPlot->setNoAntialiasingOnDrag(true);
}

MainWindow::~MainWindow()
{
    file.close();
    delete dataTimer;
    delete [] y;
    delete [] D;
    delete ui;
}

void MainWindow::getPortList()
{
    // Serial port list.
    QList<QSerialPortInfo> portlist = QSerialPortInfo::availablePorts();
    for (int i=portlist.size()-1; i>=0; i--) {
        QString portname = portlist.at(i).portName();
        portname += ":";
        portname += portlist.at(i).description();
        ui->Port->addItem(portname);
    }
}

void MainWindow::getBaubrate()
{
    // Serial baudrate list.
    QList<qint32> baudlist = QSerialPortInfo::standardBaudRates();
    for (int i=0; i<baudlist.size(); i++) {
        qint32 baud_int = baudlist.at(i);
        QString baud_str = QString("%1").arg(baud_int);
        ui->Baudrate->addItem(baud_str);
    }
}


void MainWindow::realtimeDataSlot()
{
   static QTime time(QTime::currentTime());
    //calculate two new data points:
    double key = time.elapsed()/1000.0; // time elapsed since start of demo, in seconds
   static double lastPointKey = 0;
    if (key-lastPointKey > 0.002) // at most add point every 2 ms
    {
      // add data to lines:
      /*  ui->customPlot->graph(0)->addData(key, y[0] + D[0]);
        ui->customPlot->graph(1)->addData(key, y[1] + D[1]);
        ui->customPlot->graph(2)->addData(key, y[2] + D[2]);
        ui->customPlot->graph(3)->addData(key, y[3] + D[3]);
        ui->customPlot->graph(4)->addData(key, y[4] + D[4]);
        ui->customPlot->graph(5)->addData(key, y[5] + D[5]);
        ui->customPlot->graph(6)->addData(key, y[6] + D[6]);*/
       ui->customPlot->graph(0)->addData(key, qSin(key)+qrand()/(double)RAND_MAX*1*qSin(key/0.3843));
       ui->customPlot->graph(1)->addData(key, qCos(key)+qrand()/(double)RAND_MAX*5*qSin(key/0.4364));
        lastPointKey = key;
    }
    // rescale value (vertical) axis to fit the current data:
    //ui->customPlot->graph(0)->rescaleValueAxis();
    //ui->customPlot->graph(1)->rescaleValueAxis(true);
    // make key axis range scroll with the data (at a constant range size of 8):
    ui->customPlot->xAxis->setRange(key, 60, Qt::AlignRight);
    ui->customPlot->replot();

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
            double value = (double) elems[i].toDouble();
            y[i] = value;
           // qDebug()<< value <<endl;
        }

    }
    ui->label_9->setText(QString::number(y[3]));
    static int key = 0;
    ui->customPlot->graph(0)->addData(key, y[0] + D[0]);
    ui->customPlot->graph(1)->addData(key, y[1] + D[1]);
    ui->customPlot->graph(2)->addData(key, y[2] + D[2]);
    ui->customPlot->graph(3)->addData(key, y[3] + D[3]);
    ui->customPlot->graph(4)->addData(key, y[4] + D[4]);
    ui->customPlot->graph(5)->addData(key, y[5] + D[5]);
    ui->customPlot->xAxis->setRange(key, 2500, Qt::AlignRight);
    ui->customPlot->replot();
    key++;
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

