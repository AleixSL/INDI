#ifndef MYLABEL_H
#define MYLABEL_H

#include <QLabel>

class MyLabel: public QLabel
{
    Q_OBJECT
public:
    MyLabel(QWidget *parent);
    ~MyLabel();

public slots:
    void changeToRed();
    void changeToBlue();
};

#endif // MYLABEL_H
