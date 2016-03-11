#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    QLineEdit LEItem2,LEItem1;
    QMessageBox msgBox;
    QPushButton btnSubmit;
    QString strTe1, strTe2, info="";
    ~MainWindow();

private slots:
    void on_btnSubmit_clicked();

    void on_LEItem1_editingFinished();

    void on_LEItem2_editingFinished();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
