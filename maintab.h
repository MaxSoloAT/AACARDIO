#ifndef MAINTAB_H
#define MAINTAB_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>

class MainTab : public QWidget
{
    Q_OBJECT
public:
    explicit MainTab(QWidget *parent = nullptr);
    QLabel *label1;
    QLabel *label2;
signals:

};

#endif // MAINTAB_H
