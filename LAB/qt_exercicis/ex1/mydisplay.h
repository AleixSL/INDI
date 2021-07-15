#ifndef MYDISPLAY_H
#define MYDISPLAY_H

#include <QLCDNumber>


class MyDisplay
{
    Q_OBJECT
public:
    MyDisplay(QWidget *parent);
    ~MyDisplay();
public slots:
    void changeColor(int n);
    void setToZero();
};

#endif // MYDISPLAY_H
