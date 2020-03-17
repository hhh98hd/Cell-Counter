#ifndef BROWSERDIALOG_H
#define BROWSERDIALOG_H

#include <QDialog>
#include <QtCore>
#include <QtGui>

namespace Ui
{
    class BrowserDialog;
}

class BrowserDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BrowserDialog(QWidget *parent = 0);
    ~BrowserDialog();

signals:
    void fileChosen(QString path);

private slots:
    void on_btn_dialog_cancel_clicked();

    void on_treeView_clicked(const QModelIndex &index);

    void on_btn_dialog_ok_clicked();

private:
    Ui::BrowserDialog *ui;
    QFileSystemModel* dirModel;
    QString path;
};

#endif // BROWSERDIALOG_H
