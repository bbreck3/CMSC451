#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QLineEdit>
#include <QMessageBox>
#include <QDebug>

//static QLineEdit test;
QString test;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}




//QString info = te1.plainText + " " + te2.plainText;
//msgBox.setText(info);



//click button to display content
void MainWindow::on_btnSubmit_clicked()
{
       qDebug() << "Debugging: " + info + "testing: " + test;
       QMessageBox::information(this,"Input",info);
       QMessageBox::information(this,"Input",test);
       info=""; //reset the info to the empty string
}


//once the textField has been selected and is finished being edited:
//Grab the info


//once the textField has been selected and is finished being edited:
//Grab the info


void MainWindow::on_LEItem1_editingFinished()
{
     //QString test = LEItem1.size();
    //ui-><nameOfItem>->text :: has to be in this format
    qDebug() << ui->LEItem1->text();
    strTe1 = ui->LEItem1->text();
    info+=strTe1;
    info+=" ";
}

void MainWindow::on_LEItem2_editingFinished()
{
    //ui-><nameOfItem>->text :: has to be in this format
    qDebug() << ui->LEItem2->text();
    strTe2 = ui->LEItem2->text();

    //testing string manipulation
    test = strTe2;
    qDebug() << test;
    info+=strTe2;
    info+=" ";
}
