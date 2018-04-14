#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include<cstdlib>
#include<string>
#include<cstdio>
#include<cstring>
#include<iostream>
#include<algorithm>
#include <opencv2/opencv.hpp>
#include <opencv2/dnn.hpp>
#include <QThread>
#include <QElapsedTimer>
#include <QFileDialog>
using namespace std;
using namespace cv;
using namespace cv::dnn;

const int N = 300;
string classification;
string deploy;
string model;
string meansrc;
string label;


void int2str(const int &int_temp,string &string_temp)
{
        stringstream stream;
        stream<<int_temp;
        string_temp=stream.str();
}

char Judge(int i){
    string temp;
    int2str(i,temp);
    char line[N];
    FILE *fp;
    string cmd = classification+" "+deploy+" "+model+" "+meansrc+" "+label+" "+"ceshi/"+temp+".jpg";
    // system call
    const char *sysCommand = cmd.data();
    if ((fp = popen(sysCommand, "r")) == NULL) {
        cout << "error" << endl;
        return '-';
    }
    while (fgets(line, sizeof(line)-1, fp) != NULL){
        //cout << line << endl;
        for(int i=0;line[i];++i){
            if(line[i]=='"'&&line[i+2]=='"')
            {
                pclose(fp);
                return line[i+1];
            }
        }

    }
    pclose(fp);
    return '-';
}

int get_res(char n1,char n2,char n3,char n4)
{
    int ans[4];
    int t=0;
    if('1'==n1){
        ans[t]=0;
        ++t;
    }
    if('1'==n2){
        ans[t]=1;
        ++t;
    }
    if('1'==n3){
        ans[t]=2;
        ++t;
    }
    if('1'==n4){
        ans[t]=3;
        ++t;
    }

    if(0==t)
    {
        srand(time(0));
        int p = ((int)rand())%4;
        return p;
    }
    else if(1==t)
        return ans[0];
    else
    {
        srand(time(0));
        int p = ((int)rand())%t;
        return ans[p];
    }
}

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

void MainWindow::on_pushButton_clicked()
{
    QString qclassification=ui->lineEdit->text();
    QString qdeploy=ui->lineEdit_2->text();
    QString qmodel=ui->lineEdit_3->text();
    QString qmeansrc=ui->lineEdit_4->text();
    QString qlabel=ui->lineEdit_5->text();
    classification=qclassification.toStdString();
    deploy=qdeploy.toStdString();
    model=qmodel.toStdString();
    meansrc=qmeansrc.toStdString();
    label=qlabel.toStdString();

    ui->progressBar->setRange(0,1000);
    ui->progressBar->setValue(0);

        remove("result.txt");
        char src[20];
        char ans[20];
        FILE *file;
        char num1,num2,num3,num4;
        file = fopen("result.txt", "w");

        ui->label->setText("working...");
        for(int i=9000;i<10000;++i)
        {
            if(i%50==0)
            {
                QElapsedTimer et;
                et.start();
                while(et.elapsed()<300)
                    QCoreApplication::processEvents();
            }

            sprintf(src,"train/%04d.jpg",i);
            Mat img = imread(src);

            Mat cp1 = img(Rect(0,0,37,45));
            imwrite("ceshi/1.jpg",cp1);

            Mat cp2 = img(Rect(37,0,38,45));
            imwrite("ceshi/2.jpg",cp2);

            Mat cp3 = img(Rect(75,0,37,45));
            imwrite("ceshi/3.jpg",cp3);

            Mat cp4 = img(Rect(112,0,38,45));
            imwrite("ceshi/4.jpg",cp4);

            num1=Judge(1);
            num2=Judge(2);
            num3=Judge(3);
            num4=Judge(4);

            sprintf(ans,"%04d,%d",i,get_res(num1,num2,num3,num4));
            fprintf(file,"%s\n",ans);

            ui->progressBar->setValue(i-9000+1);
        }
        ui->label->setText("finished");
        fclose(file) ;
}

void MainWindow::on_toolButton_clicked()
{
    QString directory = QFileDialog::getOpenFileName(NULL,"select",".","*.bin");
    if(!directory.isEmpty())
    {
        ui->lineEdit->setText(directory);
    }
}

void MainWindow::on_toolButton_2_clicked()
{
    QString directory = QFileDialog::getOpenFileName(NULL,"select",".","*.prototxt");
    if(!directory.isEmpty())
    {
        ui->lineEdit_2->setText(directory);
    }
}

void MainWindow::on_toolButton_3_clicked()
{
    QString directory = QFileDialog::getOpenFileName(NULL,"select",".","*.caffemodel");
    if(!directory.isEmpty())
    {
        ui->lineEdit_3->setText(directory);
    }
}

void MainWindow::on_toolButton_4_clicked()
{
    QString directory = QFileDialog::getOpenFileName(NULL,"select",".","*.binaryproto");
    if(!directory.isEmpty())
    {
        ui->lineEdit_4->setText(directory);
    }
}

void MainWindow::on_toolButton_5_clicked()
{
    QString directory = QFileDialog::getOpenFileName(NULL,"select",".","*.txt");
    if(!directory.isEmpty())
    {
        ui->lineEdit_5->setText(directory);
    }
}
