#include "browserdialog.h"
#include "mainwindow.h"
#include "ui_browserdialog.h"

BrowserDialog::BrowserDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BrowserDialog)
    {
        ui->setupUi(this);
        dirModel = new QFileSystemModel(this);
        dirModel->setRootPath("");
        ui->treeView->setModel(dirModel);
        ui->treeView->hideColumn(1);
        ui->treeView->hideColumn(2);
        ui->treeView->hideColumn(3);
    }

BrowserDialog::~BrowserDialog()
{
    delete ui;
}

void BrowserDialog::on_btn_dialog_cancel_clicked()
{
    this->hide();
}

void BrowserDialog::on_treeView_clicked(const QModelIndex &index)
{
    path = dirModel->filePath(index);
}


void BrowserDialog::on_btn_dialog_ok_clicked()
{
    emit fileChosen(path);
    this->hide();
}

