#include "savedialog.h"
#include "ui_savedialog.h"
#include <opencv2/opencv.hpp>
#include <QtCore>
#include <QtGui>
#include <iostream>

using namespace std;
using namespace cv;

SaveDialog::SaveDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SaveDialog)
    {
        ui->setupUi(this);
        dirModel = new QFileSystemModel(this);
        dirModel->setRootPath("");
        ui->treeView->setModel(dirModel);
        ui->treeView->hideColumn(1);
        ui->treeView->hideColumn(2);
        ui->treeView->hideColumn(3);
        ui->lineEdit->setText("result");
    }

SaveDialog::~SaveDialog()
{
    delete ui;
}

void SaveDialog::on_cancelButton_clicked()
{
    this->hide();
}

void SaveDialog::on_okButton_clicked()
{
    this->path += "/" + ui->lineEdit->text() + ".jpg";
    imwrite(this->path.toStdString(), this->image);
    this->hide();
}

void SaveDialog::on_treeView_clicked(const QModelIndex &index)
{
    path = dirModel->filePath(index);
    cout << path.toStdString() << endl;
}

void SaveDialog::setImageToSave(Mat image)
{
    this->image = image;
}
