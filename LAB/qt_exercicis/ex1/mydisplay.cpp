#include "mydisplay.h"

MyDisplay::MyDisplay() : QLCDNumber(parent)
{

}

MyDisplay::~MyDisplay() {}

void MyDisplay::changeColor(int i) {
    if(n % 2 == 0) {
        setSegmentStyle("")
    }
}

void MyDisplay::setToZero() {

}
