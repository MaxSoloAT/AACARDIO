#include "maintab.h"

MainTab::MainTab(QWidget *parent) : QWidget(parent)
{
    label1 = new QLabel;
    label2 = new QLabel;
    //this->setGeometry(0,0,200,200);
    //label1->setGeometry(0,0,50,52);
    //label2->setGeometry(100,100)
    label1->setText("sdadasdadasd");
    label2->setText("222222222222a");
    QVBoxLayout *lay = new QVBoxLayout;
    lay->addWidget(label1);
    lay->addWidget(label2);
    setLayout(lay);

}
