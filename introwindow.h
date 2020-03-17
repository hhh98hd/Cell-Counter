#ifndef INTROWINDOW_H
#define INTROWINDOW_H

#include <QMainWindow>

namespace Ui {
class IntroWindow;
}

class IntroWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit IntroWindow(QWidget *parent = 0);
    ~IntroWindow();

private:
    Ui::IntroWindow *ui;
    void keyPressEvent(QKeyEvent* ev);
};

#endif // INTROWINDOW_H
