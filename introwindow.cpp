#include "introwindow.h"
#include "mainwindow.h"
#include "ui_introwindow.h"
#include <QKeyEvent>
#include <iostream>

using namespace std;

IntroWindow::IntroWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::IntroWindow)
    {
        ui->setupUi(this);
        /* load logo image */
        QPixmap logo("./uet_logo.png");
        ui->label_uet_logo->setPixmap(logo);
    }

IntroWindow::~IntroWindow()
{
    delete ui;
}

void IntroWindow::keyPressEvent(QKeyEvent *ev)
{
    /* any key pressed */
    if(ev->key())
    {
        MainWindow* w = new MainWindow();
        this->destroy();
        w->show();
    }
}





