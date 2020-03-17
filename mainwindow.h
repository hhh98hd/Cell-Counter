#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "browserdialog.h"
#include "savedialog.h"
#include <opencv2/opencv.hpp>

using namespace cv;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void onFileChosen(QString path);

private slots:
    void on_btn_browser_clicked();

    void on_startButton_clicked();

    void on_saveButton_clicked();

    void on_resetButton_clicked();

private:
    Ui::MainWindow *ui;
    BrowserDialog* browser;
    SaveDialog* save;

    QPixmap cvMat2QPixmap(Mat mat);
};

#endif // MAINWINDOW_H
