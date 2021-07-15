/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.12.8
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDial>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLCDNumber>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QHBoxLayout *horizontalLayout;
    QGridLayout *gridLayout;
    QLCDNumber *hoursLcdNumber;
    QSpacerItem *horizontalSpacer_6;
    QLCDNumber *minLcdNumber;
    QSpacerItem *horizontalSpacer_3;
    QDial *hourDial;
    QDial *minDial;
    QSpacerItem *horizontalSpacer_2;
    QSpacerItem *horizontalSpacer;
    QPushButton *exitButton;
    QLabel *minutesLabel;
    QSpacerItem *horizontalSpacer_5;
    QSpacerItem *horizontalSpacer_4;
    QLabel *hoursLabel;
    QMenuBar *menubar;
    QMenu *menuEx1;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(376, 314);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        horizontalLayout = new QHBoxLayout(centralwidget);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        gridLayout = new QGridLayout();
        gridLayout->setSpacing(1);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setSizeConstraint(QLayout::SetMinAndMaxSize);
        gridLayout->setContentsMargins(1, 1, 1, 1);
        hoursLcdNumber = new QLCDNumber(centralwidget);
        hoursLcdNumber->setObjectName(QString::fromUtf8("hoursLcdNumber"));

        gridLayout->addWidget(hoursLcdNumber, 1, 0, 1, 2);

        horizontalSpacer_6 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_6, 1, 2, 1, 2);

        minLcdNumber = new QLCDNumber(centralwidget);
        minLcdNumber->setObjectName(QString::fromUtf8("minLcdNumber"));

        gridLayout->addWidget(minLcdNumber, 1, 4, 1, 2);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_3, 1, 6, 1, 3);

        hourDial = new QDial(centralwidget);
        hourDial->setObjectName(QString::fromUtf8("hourDial"));
        hourDial->setMaximum(23);

        gridLayout->addWidget(hourDial, 2, 0, 1, 3);

        minDial = new QDial(centralwidget);
        minDial->setObjectName(QString::fromUtf8("minDial"));
        minDial->setMaximum(59);

        gridLayout->addWidget(minDial, 2, 3, 1, 4);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_2, 2, 8, 1, 1);

        horizontalSpacer = new QSpacerItem(208, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 3, 0, 1, 7);

        exitButton = new QPushButton(centralwidget);
        exitButton->setObjectName(QString::fromUtf8("exitButton"));

        gridLayout->addWidget(exitButton, 3, 7, 1, 2);

        minutesLabel = new QLabel(centralwidget);
        minutesLabel->setObjectName(QString::fromUtf8("minutesLabel"));

        gridLayout->addWidget(minutesLabel, 0, 0, 1, 2);

        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_5, 0, 2, 1, 2);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_4, 0, 6, 1, 1);

        hoursLabel = new QLabel(centralwidget);
        hoursLabel->setObjectName(QString::fromUtf8("hoursLabel"));

        gridLayout->addWidget(hoursLabel, 0, 4, 1, 2);

        gridLayout->setRowStretch(0, 1);
        gridLayout->setRowStretch(1, 1);
        gridLayout->setRowStretch(2, 1);
        gridLayout->setRowStretch(3, 1);
        gridLayout->setColumnStretch(0, 1);
        gridLayout->setColumnStretch(1, 1);
        gridLayout->setColumnStretch(2, 1);
        gridLayout->setColumnStretch(3, 1);
        gridLayout->setColumnStretch(4, 1);
        gridLayout->setColumnStretch(5, 1);
        gridLayout->setColumnStretch(6, 1);
        gridLayout->setColumnStretch(7, 1);
        gridLayout->setColumnStretch(8, 1);
        gridLayout->setColumnMinimumWidth(0, 1);
        gridLayout->setColumnMinimumWidth(1, 1);
        gridLayout->setColumnMinimumWidth(2, 1);
        gridLayout->setColumnMinimumWidth(3, 1);
        gridLayout->setColumnMinimumWidth(4, 1);
        gridLayout->setColumnMinimumWidth(5, 1);
        gridLayout->setColumnMinimumWidth(6, 1);
        gridLayout->setColumnMinimumWidth(7, 1);
        gridLayout->setColumnMinimumWidth(8, 1);
        gridLayout->setRowMinimumHeight(0, 1);
        gridLayout->setRowMinimumHeight(1, 1);
        gridLayout->setRowMinimumHeight(2, 1);
        gridLayout->setRowMinimumHeight(3, 1);

        horizontalLayout->addLayout(gridLayout);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 376, 22));
        menuEx1 = new QMenu(menubar);
        menuEx1->setObjectName(QString::fromUtf8("menuEx1"));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        menubar->addAction(menuEx1->menuAction());

        retranslateUi(MainWindow);
        QObject::connect(hourDial, SIGNAL(sliderMoved(int)), hoursLcdNumber, SLOT(display(int)));
        QObject::connect(minDial, SIGNAL(sliderMoved(int)), minLcdNumber, SLOT(display(int)));
        QObject::connect(exitButton, SIGNAL(clicked()), MainWindow, SLOT(close()));

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", nullptr));
        exitButton->setText(QApplication::translate("MainWindow", "Exit", nullptr));
        minutesLabel->setText(QApplication::translate("MainWindow", "Minutes", nullptr));
        hoursLabel->setText(QApplication::translate("MainWindow", "Hours", nullptr));
        menuEx1->setTitle(QApplication::translate("MainWindow", "Ex1", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
