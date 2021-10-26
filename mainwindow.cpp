#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QPixmap icon(":/icons/icons/main_i.png");
    QIcon icon_i(icon);
    setWindowIcon(icon_i);
    //some app geometry
    QRect rec = QApplication::desktop()->screenGeometry();
    height = rec.height();
    width = rec.width();
    //setWindowState(Qt::WindowFullScreen);
    /*ui->customPlot->setGeometry(20,20,width-0.01*width,height-0.25*height);
    ui->groupBox->setGeometry(20,height-0.2*height,280,90);
    ui->groupBox_2->setGeometry(300,height-0.2*height,420,90);
    ui->horizontalSlider->setGeometry(740,height-0.2*height,420,50);
    ui->horizontalSlider->setStyleSheet("QSlider::handle:horizontal {background-color:red;}");
    ui->label_8->setGeometry(900,height-0.18*height,100,20);
    ui->pushButton_2->setGeometry(20,height-0.10*height,80,21);
    ui->pushButton_2->setStyleSheet("background-color: red");
    ui->label_8->setText("0 ms");
    ui->label_9->setText("pulse");
    ui->label_9->setGeometry(1000,height-0.08*height, 80,25);*/

/* adding tabs!!
    mainTab = new MainTab;
    tabWidget = new QTabWidget(this);
    tabWidget->addTab(mainTab,tr("first"));
    tabWidget->addTab(new QWidget(this),tr("second"));
    //tabWidget->setGeometry(100,100,width-200,height-100);*/


    //initSettingGraph(ui->groupBox->)
    //
    gn1 = "vibration";
    gn2 = "breath";
    gn3 = "kgr";
    gn4 = "pulse";
    gn5 = "nan";
    gn6 = "nan";
    gn7 = "nan";

    initSettingGraph(ui->graph_1_name, ui->graph_1_number, ui->graph_1_line_min, ui->graph_1_line_max, gn1);
    initSettingGraph(ui->graph_2_name, ui->graph_2_number, ui->graph_2_line_min, ui->graph_2_line_max, gn2);
    initSettingGraph(ui->graph_3_name, ui->graph_3_number, ui->graph_3_line_min, ui->graph_3_line_max, gn3);
    initSettingGraph(ui->graph_4_name, ui->graph_4_number, ui->graph_4_line_min, ui->graph_4_line_max, gn4);
    initSettingGraph(ui->graph_5_name, ui->graph_5_number, ui->graph_5_line_min, ui->graph_5_line_max, gn5);
    initSettingGraph(ui->graph_6_name, ui->graph_6_number, ui->graph_6_line_min, ui->graph_6_line_max, gn6);
    initSettingGraph(ui->graph_7_name, ui->graph_7_number, ui->graph_7_line_min, ui->graph_7_line_max, gn7);


    //work with file

    file.setFileName("Log.txt");
    file.open(QIODevice::WriteOnly);
    out.setDevice(&file);
    getPortList();
    getBaudrate();

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

    ui->customPlot->setOpenGl(true,16);
    ui->customPlot->legend->setVisible(true);
    ui->customPlot->legend->setFont(QFont("Helvetica",9));


    int l = 2;
    QColor col1(0, 0, 255);
    QPalette palette1;
    ui->customPlot->addGraph(); // blue line
    ui->customPlot->graph(0)->setPen(QPen(col1,l)); //vibration dark blue
    //ui->customPlot->graph(0)->setScatterStyle(QCPScatterStyle::ssCross);
    palette1.setColor(QPalette::Base, col1);
    palette1.setColor(QPalette::Text, Qt::white);
    initSettingSlider(col1, ui->graph_1_slider);
    ui->customPlot->graph(0)->setName("Acceleration");

    QColor col2(50, 0, 0);
    QPalette palette2;
    ui->customPlot->addGraph(); // red line
    ui->customPlot->graph(1)->setPen(QPen(col2,l)); // breath brown
    palette2.setColor(QPalette::Base, col2);
    palette2.setColor(QPalette::Text, Qt::white);
    initSettingSlider(col2, ui->graph_2_slider);
    ui->customPlot->graph(1)->setName("Breath");

    QColor col3(0, 0, 0);
    QPalette palette3;
    ui->customPlot->addGraph(); // red line
    ui->customPlot->graph(2)->setPen(QPen(col3,l));// kgr black
    palette3.setColor(QPalette::Base, col3);
    palette3.setColor(QPalette::Text, Qt::white);
    initSettingSlider(col3, ui->graph_3_slider);
    ui->customPlot->graph(2)->setName("GSR");

    QColor col4(140, 0, 0);
    QPalette palette4;
    ui->customPlot->addGraph(); // red line
    ui->customPlot->graph(3)->setPen(QPen(col4,l)); // pulse dark red
    palette4.setColor(QPalette::Base, col4);
    palette4.setColor(QPalette::Text, Qt::white);
    initSettingSlider(col4, ui->graph_4_slider);
    ui->customPlot->graph(3)->setName("Pulse");

    QColor col5(180, 180, 84);
    QPalette palette5;
    ui->customPlot->addGraph(); // red line
    ui->customPlot->graph(4)->setPen(QPen(col5,l)); // random dark
    palette5.setColor(QPalette::Base, col5);
    palette5.setColor(QPalette::Text, Qt::white);
    initSettingSlider(col5, ui->graph_5_slider);
    ui->customPlot->graph(4)->setName("--");

    QColor col6(200, 20, 150);
    QPalette palette6;
    ui->customPlot->addGraph(); // red line
    ui->customPlot->graph(5)->setPen(QPen(col6,l)); //
    palette6.setColor(QPalette::Base, col6);
    palette6.setColor(QPalette::Text, Qt::white);
    initSettingSlider(col6, ui->graph_6_slider);
    ui->customPlot->graph(5)->setName("---");


    QColor col7(125, 150, 140);
    ui->customPlot->addGraph(); // red line
    ui->customPlot->graph(6)->setPen(QPen(col7,l));
    QPalette palette7;
    palette7.setColor(QPalette::Base, col7);
    palette7.setColor(QPalette::Text, Qt::white);
    initSettingSlider(col7, ui->graph_7_slider);
    ui->customPlot->graph(6)->setName("----");

    ui->customPlot->setInteractions(QCP::iRangeDrag|QCP::iRangeZoom);
    // QSharedPointer<QCPAxisTickerTime> timeTicker(new QCPAxisTickerTime);
    // timeTicker->setTimeFormat("%h:%m:%s");
    //ui->customPlot->xAxis->setTicker(timeTicker);

    ui->customPlot->axisRect()->setupFullAxesBox();
    ui->customPlot->yAxis->setRange(-10, 5000);
   // ui->customPlot->hide();
    // make left and bottom axes transfer their ranges to right and top axes:
    connect(ui->customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->customPlot->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->customPlot->yAxis2, SLOT(setRange(QCPRange)));

    connect(ui->customPlot, SIGNAL(mousePress(QMouseEvent*)), SLOT(clickedGraph(QMouseEvent*)));
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


void MainWindow::getBaudrate()
{
    // Serial baudrate list.
    QList<qint32> baudlist = QSerialPortInfo::standardBaudRates();
    for (int i=0; i<baudlist.size(); i++) {
        qint32 baud_int = baudlist.at(i);
        QString baud_str = QString("%1").arg(baud_int);
        ui->Baudrate->addItem(baud_str);
    }
}

void MainWindow::initSettingGraph(QLabel *graph_name, QLabel *graph_number, QLineEdit *min, QLineEdit *max, QString gn)
{
    graph_name->setText(gn);
    min->setText("0");
    max->setText("100");
    graph_number->setText("0");

}

void MainWindow::initSettingSlider(QColor col, QSlider *slider)
{
    QPalette palette = slider->palette();
    palette.setColor(QPalette::Button,col);
    slider->setAutoFillBackground(true);
    slider->setPalette(palette);
}


void MainWindow::realtimeDataSlot()
{
  // static QTime time(QTime::currentTime());
    //calculate two new data points:
    //double key = time.elapsed()/1000.0; // time elapsed since start of demo, in seconds
 //  static double lastPointKey = 0;
  //  if (key-lastPointKey > 0.002) // at most add point every 2 ms
  //  {
      // add data to lines:
      /*  ui->customPlot->graph(0)->addData(key, y[0] + D[0]);
        ui->customPlot->graph(1)->addData(key, y[1] + D[1]);
        ui->customPlot->graph(2)->addData(key, y[2] + D[2]);
        ui->customPlot->graph(3)->addData(key, y[3] + D[3]);
        ui->customPlot->graph(4)->addData(key, y[4] + D[4]);
        ui->customPlot->graph(5)->addData(key, y[5] + D[5]);
        ui->customPlot->graph(6)->addData(key, y[6] + D[6]);*/
    //   ui->customPlot->graph(0)->addData(key, qSin(key)+qrand()/(double)RAND_MAX*1*qSin(key/0.3843));
     //  ui->customPlot->graph(1)->addData(key, qCos(key)+qrand()/(double)RAND_MAX*5*qSin(key/0.4364));
       // lastPointKey = key;
  //  }
    // rescale value (vertical) axis to fit the current data:
    //ui->customPlot->graph(0)->rescaleValueAxis();
    //ui->customPlot->graph(1)->rescaleValueAxis(true);
    // make key axis range scroll with the data (at a constant range size of 8):
  //  ui->customPlot->xAxis->setRange(key, 60, Qt::AlignRight);
   // ui->customPlot->replot();

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
    int j;
    long int first;
    QString second;
    long int third;
    while ( (j = serial_queue.indexOf("\r\n")) != -1 ) { //need to fix it requers to new data structures
        cd = QDate::currentDate();
        ct = QTime::currentTime();
        out<<cd.toString("yyyy-MM-dd");
        out<<" ";
        out<<ct.toString("hh:mm:ss.zzz");
        out<<" "<<key<<" "; //something wrong bellow works file
        out<<serial_queue;
        out<<"\r\n";

        // Extraction of the current processing line and eliminate it for next processing.
        QByteArray line = serial_queue.mid(0, j);
        serial_queue.remove(0, j+2);
        QList<QByteArray> elems = line.split('~');
        //ui->textEdit->setText(line);
        ui->textEdit->append(line);
        int n_elems = elems.length();

        for (int i=0; i < n_elems; i++) {
            first = elems[0].toInt();
            second = elems[1];
            third = elems[2].toInt();


            //double value = (double) elems[i].toDouble();
            //y[i] = value;
        }
/*
        //qDebug()<<"for end"<<endl;
        if(second == "v"){
            ui->customPlot->graph(0)->addData(first, third + D[0]);
            qDebug()<<first<<" VVVVV "<<second<<" "<<third<<endl;
            ui->customPlot->xAxis->setRange(first, 2500, Qt::AlignRight);
            ui->customPlot->replot();
        }
        if(second == "c"){
            qDebug()<<first<<" CCCCC "<<second<<" "<<third<<endl;
            ui->customPlot->graph(1)->addData(first, third + D[1]);
            ui->customPlot->xAxis->setRange(first, 2500, Qt::AlignRight);
            ui->customPlot->replot();
        }
        if(second == "g"){
            qDebug()<<first<<" HHHHH "<<second<<" "<<third<<endl;
            ui->customPlot->graph(2)->addData(first, third + D[2]);
            ui->customPlot->xAxis->setRange(first, 2500, Qt::AlignRight);
            ui->customPlot->replot();
        }
        if(second == "p"){
            qDebug()<<first<<" NNNN "<<second<<" "<<third<<endl;
            ui->customPlot->graph(3)->addData(first, third + D[3]);
            ui->customPlot->xAxis->setRange(first, 2500, Qt::AlignRight);
            ui->customPlot->replot();
        }
        if(second == "h"){
            qDebug()<<first<<" NNNN "<<second<<" "<<third<<endl;
            ui->customPlot->graph(4)->addData(first, third + D[4]);
            ui->customPlot->xAxis->setRange(first, 2500, Qt::AlignRight);
            ui->customPlot->replot();
        }
*/
        //ui->customPlot->graph(4)->addData(key, y[4] + D[4]);
        //ui->customPlot->graph(5)->addData(key, y[5] + D[5]);
       // ui->customPlot->replot();

        /*
        ui->label_9->setText(QString::number(y[3]));
        ui->customPlot->graph(0)->addData(key, y[0] + D[0]);
        ui->customPlot->graph(1)->addData(key, y[1] + D[1]);
        ui->customPlot->graph(2)->addData(key, y[2] + D[2]);
        ui->customPlot->graph(3)->addData(key, y[3] + D[3]);
        ui->customPlot->graph(4)->addData(key, y[4] + D[4]);
        ui->customPlot->graph(5)->addData(key, y[5] + D[5]);
        ui->customPlot->xAxis->setRange(key, 2500, Qt::AlignRight);
        ui->customPlot->replot();
        qDebug()<<key<<endl;
        key++;*/
    }
    if(second == "v"){
        ui->customPlot->graph(0)->addData(first, third + D[0]);
        //qDebug()<<first<<" VVVVV "<<second<<" "<<third<<endl;
        ui->customPlot->xAxis->setRange(first, 10000, Qt::AlignRight);
        ui->customPlot->replot();
    }
    if(second == "c"){
        //qDebug()<<first<<" CCCCC "<<second<<" "<<third<<endl;
        ui->customPlot->graph(1)->addData(first, third + D[1]);
        ui->customPlot->xAxis->setRange(first, 10000, Qt::AlignRight);
        ui->customPlot->replot();
    }
    if(second == "g"){
        //qDebug()<<first<<" HHHHH "<<second<<" "<<third<<endl;
        ui->customPlot->graph(2)->addData(first, third + D[2]);
        ui->customPlot->xAxis->setRange(first, 10000, Qt::AlignRight);
        ui->customPlot->replot();
    }
    if(second == "p"){
      //  qDebug()<<first<<" PPPP "<<second<<" "<<third<<endl;
        ui->customPlot->graph(3)->addData(first, third + D[3]);
        ui->customPlot->xAxis->setRange(first, 10000, Qt::AlignRight);
        ui->customPlot->replot();
    }
    if(second == "h"){
       // qDebug()<<first<<" HHHH "<<second<<" "<<third<<endl;
        ui->customPlot->graph(4)->addData(first, third + D[4]);
        ui->customPlot->xAxis->setRange(first, 10000, Qt::AlignRight);
        ui->customPlot->replot();
    }
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


void MainWindow::clickedGraph(QMouseEvent *event)
{   if(event->buttons() & Qt::RightButton)
    {
        QPoint point = event->pos();
        qDebug()<<ui->customPlot->xAxis->pixelToCoord(point.x())<<endl;
        qDebug()<<ui->customPlot->yAxis->pixelToCoord(point.y())<<endl;
        infLine = new QCPItemStraightLine(ui->customPlot);
        infLine->point1->setCoords(ui->customPlot->xAxis->pixelToCoord(point.x()),ui->customPlot->yAxis->pixelToCoord(point.y()));
        infLine->point2->setCoords(ui->customPlot->xAxis->pixelToCoord(point.x()),ui->customPlot->yAxis->pixelToCoord(point.y())+500);
        out<<"label: "<<ui->customPlot->xAxis->pixelToCoord(point.x())<<"\r\n"<<endl;
    }
}


void MainWindow::on_graph_1_slider_valueChanged(int value)
{
    ui->graph_1_number->setText(QString::number(value));
    D[0] = value;
}


void MainWindow::on_graph_2_slider_valueChanged(int value)
{
     ui->graph_2_number->setText(QString::number(value));
     D[1] = value;
}


void MainWindow::on_graph_3_slider_valueChanged(int value)
{
    ui->graph_3_number->setText(QString::number(value));
    D[2] = value;
}


void MainWindow::on_graph_4_slider_valueChanged(int value)
{
    ui->graph_4_number->setText(QString::number(value));
    D[3] = value;
}


void MainWindow::on_graph_5_slider_valueChanged(int value)
{
    ui->graph_5_number->setText(QString::number(value));
    D[4] = value;
}


void MainWindow::on_graph_6_slider_valueChanged(int value)
{
    ui->graph_6_number->setText(QString::number(value));
    D[5] = value;
}


void MainWindow::on_graph_7_slider_valueChanged(int value)
{
    ui->graph_7_number->setText(QString::number(value));
    D[6] = value;
}


void MainWindow::on_graph_1_line_min_textChanged(const QString &arg1)
{
    ui->graph_1_slider->setMinimum(arg1.toInt());
    ui->graph_1_slider->setValue(ui->graph_1_slider->value());
}


void MainWindow::on_graph_1_line_max_textChanged(const QString &arg1)
{
    ui->graph_1_slider->setMaximum(arg1.toInt());
    ui->graph_1_slider->setValue(ui->graph_1_slider->value());
}


void MainWindow::on_graph_2_line_min_textChanged(const QString &arg1)
{
    ui->graph_2_slider->setMinimum(arg1.toInt());
    ui->graph_2_slider->setValue(ui->graph_2_slider->value());
}


void MainWindow::on_graph_2_line_max_textChanged(const QString &arg1)
{
    ui->graph_2_slider->setMaximum(arg1.toInt());
    ui->graph_2_slider->setValue(ui->graph_2_slider->value());
}


void MainWindow::on_graph_3_line_min_textChanged(const QString &arg1)
{
    ui->graph_3_slider->setMinimum(arg1.toInt());
    ui->graph_3_slider->setValue(ui->graph_3_slider->value());
}


void MainWindow::on_graph_3_line_max_textChanged(const QString &arg1)
{
    ui->graph_3_slider->setMaximum(arg1.toInt());
    ui->graph_3_slider->setValue(ui->graph_3_slider->value());
}


void MainWindow::on_graph_4_line_min_textChanged(const QString &arg1)
{
    ui->graph_4_slider->setMinimum(arg1.toInt());
    ui->graph_4_slider->setValue(ui->graph_4_slider->value());
}


void MainWindow::on_graph_4_line_max_textChanged(const QString &arg1)
{
    ui->graph_4_slider->setMaximum(arg1.toInt());
    ui->graph_4_slider->setValue(ui->graph_4_slider->value());
}


void MainWindow::on_graph_5_line_min_textChanged(const QString &arg1)
{
    ui->graph_5_slider->setMinimum(arg1.toInt());
    ui->graph_5_slider->setValue(ui->graph_5_slider->value());
}


void MainWindow::on_graph_5_line_max_textChanged(const QString &arg1)
{
    ui->graph_5_slider->setMaximum(arg1.toInt());
    ui->graph_5_slider->setValue(ui->graph_5_slider->value());
}


void MainWindow::on_graph_6_line_min_textChanged(const QString &arg1)
{
    ui->graph_6_slider->setMinimum(arg1.toInt());
    ui->graph_6_slider->setValue(ui->graph_6_slider->value());
}


void MainWindow::on_graph_6_line_max_textChanged(const QString &arg1)
{
    ui->graph_6_slider->setMaximum(arg1.toInt());
    ui->graph_6_slider->setValue(ui->graph_6_slider->value());
}


void MainWindow::on_graph_7_line_min_textChanged(const QString &arg1)
{
    ui->graph_7_slider->setMinimum(arg1.toInt());
    ui->graph_7_slider->setValue(ui->graph_7_slider->value());
}


void MainWindow::on_graph_7_line_max_textChanged(const QString &arg1)
{
    ui->graph_7_slider->setMaximum(arg1.toInt());
    ui->graph_7_slider->setValue(ui->graph_7_slider->value());
}

