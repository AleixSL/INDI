#ifndef MYLINEEDIT_H
#define MYLINEEDIT_H

#include <QLineEdit>

class MyLineEdit: public QLineEdit
{
        Q_OBJECT
public:
    MyLineEdit(QWidget *parent);
    ~MyLineEdit();

public slots:
    void tractaReturn();

signals:
    void enviaText(const QString &);
};

#endif // MYLINEEDIT_H
