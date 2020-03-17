#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "browserdialog.h"
#include "watershedsegmenter.h"
#include "savedialog.h"
#include <opencv2/opencv.hpp>

using namespace cv;

WatershedSegmenter segmenter;
QString lastPath = "", currentPath = "";
Mat result;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
    {
        browser = new BrowserDialog();
        save = new SaveDialog();
        ui->setupUi(this);
        ui->le_path->setReadOnly(true);
        ui->startButton->setEnabled(false);
        ui->saveButton->setEnabled(false);
        ui->cellCount->setStyleSheet("QLabel { color: blue; }");
        connect(browser, SIGNAL(fileChosen(QString)), this, SLOT(onFileChosen(QString)));
    }

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btn_browser_clicked()
{
    browser->show();
}

void MainWindow::onFileChosen(QString path)
{
    currentPath = path;
    segmenter.setSegmentNum(0);
    ui->le_path->setText(path);
    ui->startButton->setEnabled(true);
    QPixmap img(path);

    /* auto scaling */
    int w = ui->img->width();
    int h = ui->img->height();
    ui->img->setPixmap(img.scaled(w, h, Qt::KeepAspectRatio));
    segmenter.loadImage(path.toStdString());
}

QPixmap MainWindow::cvMat2QPixmap(Mat mat)
{
    QPixmap pixmap;
        if(mat.type()==CV_8UC1)
        {
            // Set the color table (used to translate colour indexes to qRgb values)
            QVector<QRgb> colorTable;
            for (int i=0; i<256; i++)
                colorTable.push_back(qRgb(i,i,i));
            // Copy input Mat
            const uchar *qImageBuffer = (const uchar*)mat.data;
            // Create QImage with same dimensions as input Mat
            QImage img(qImageBuffer, mat.cols, mat.rows, mat.step, QImage::Format_Indexed8);
            img.setColorTable(colorTable);
            pixmap = QPixmap::fromImage(img);
        }
        // 8-bits unsigned, NO. OF CHANNELS=3
        if(mat.type()==CV_8UC3)
        {
            // Copy input Mat
            const uchar *qImageBuffer = (const uchar*)mat.data;
            // Create QImage with same dimensions as input Mat
            QImage img(qImageBuffer, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
            pixmap = QPixmap::fromImage(img.rgbSwapped());
        }
        return pixmap;
}

void MainWindow::on_startButton_clicked()
{
    ui->saveButton->setEnabled(true);
    QPixmap imgage;
    if(lastPath != currentPath)
    {
        if(ui->checkBox->isChecked())
        {
            result = segmenter.segment(21, true);
        }
        else
        {
            result = segmenter.segment(21, false);
        }
        save->setImageToSave(result);
        imgage = cvMat2QPixmap(result);
        int w = ui->img->width();
        int h = ui->img->height();
        ui->img->setPixmap(imgage.scaled(w, h, Qt::KeepAspectRatio));
        int count =  segmenter.getSegmentNum();
        ui->cellCount->setText(QString::number(count));
    }
    lastPath = currentPath;
}

void MainWindow::on_saveButton_clicked()
{
    save->show();
}

void MainWindow::on_resetButton_clicked()
{
    ui->img->clear();
    ui->img->setText("No Image");
    ui->img->setAlignment(Qt::AlignCenter);
    ui->cellCount->setText(QString::number(0));
    ui->startButton->setEnabled(false);
    ui->saveButton->setEnabled(false);
    lastPath = "", currentPath = "";
}
