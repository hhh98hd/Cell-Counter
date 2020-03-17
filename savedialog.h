#ifndef SAVEDIALOG_H
#define SAVEDIALOG_H

#include <QDialog>
#include <QtCore>
#include <QtGui>
#include <opencv2/opencv.hpp>

using namespace cv;

namespace Ui {
class SaveDialog;
}

class SaveDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SaveDialog(QWidget *parent = 0);
    ~SaveDialog();

    void setImageToSave(Mat image);

private slots:
    void on_cancelButton_clicked();

    void on_okButton_clicked();

    void on_treeView_clicked(const QModelIndex &index);

private:
    Ui::SaveDialog *ui;
    QFileSystemModel* dirModel;
    QString path;
    Mat image;
};

#endif // SAVEDIALOG_H
