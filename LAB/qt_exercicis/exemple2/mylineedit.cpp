#include "mylineedit.h"

MyLineEdit::MyLineEdit(QWidget *parent) : QLineEdit(parent)
{

}

MyLineEdit::~MyLineEdit(){}

void MyLineEdit::tractaReturn() {
    emit enviaText(text());
}
