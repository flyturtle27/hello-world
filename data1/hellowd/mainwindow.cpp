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
#include <QElapsedTimer>
#include <QDir>
#include <QFileDialog>
using namespace std;
using namespace cv;
using namespace cv::dnn;

const int N = 300;
//int wrong=0;
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

char judge1(int i){
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
            if(line[i]=='"'&&line[i+2]=='"'&&line[i+1]!='+'&&line[i+1]!='*'&&line[i+1]!='!')
            {
                pclose(fp);
                return line[i+1];
            }
        }

    }
    pclose(fp);
    //wrong++;
    return '1';
}

char judge2(int i){
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
            if(line[i]=='"'&&line[i+2]=='"'&&(line[i+1]=='+'||line[i+1]=='*'||line[i+1]=='!'))
            {
                pclose(fp);
                return line[i+1];
            }
        }

    }
    pclose(fp);
    //wrong++;
    return '+';
}

int char2int(char e)
{
    if(e>='0'&&e<='9')
        return e-48;
    else
        return e-55;
}

int calculate(int e1,int e2,char k)
{
    if('+'==k)
        return e1+e2;
    else
        return e1-e2;
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
        char res[20];
        char num1,num2,num3;
        char cal1,cal2;
        int n1,n2,n3;
        int ans;
        FILE *file;
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

            sprintf(src,"oyz/%04d.jpg",i);
            Mat img = imread(src);

                    Mat cp1 = img(Rect(5,0,80,80));
                    imwrite("ceshi/1.jpg",cp1);

                    Mat cp2 = img(Rect(65,0,80,80));
                    imwrite("ceshi/2.jpg",cp2);

                    Mat cp3 = img(Rect(135,0,80,80));
                    imwrite("ceshi/3.jpg",cp3);

                    Mat cp4 = img(Rect(205,0,80,80));
                    imwrite("ceshi/4.jpg",cp4);

                    Mat cp5 = img(Rect(265,0,80,80));
                    imwrite("ceshi/5.jpg",cp5);

                    num1=judge1(1);
                    cal1=judge2(2);
                    num2=judge1(3);
                    cal2=judge2(4);
                    num3=judge1(5);

                    n1=char2int(num1);
                    n2=char2int(num2);
                    n3=char2int(num3);

                    if('!'==cal1)
                        cal1='-';

                    if('!'==cal2)
                        cal2='-';

                    ans=-99999;
                    if('*'==cal1&&'*'==cal2)
                    {
                        ans=n1*n2*n3;
                    } else if('*'==cal1)
                    {
                        ans=calculate(n1*n2,n3,cal2);
                    } else if('*'==cal2)
                    {
                        ans=calculate(n1,n2*n3,cal1);
                    } else
                    {
                        ans=calculate(calculate(n1,n2,cal1),n3,cal2);
                    }

            sprintf(res,"%04d,%d%c%d%c%d=%d",i,n1,cal1,n2,cal2,n3,ans);
            fprintf(file,"%s\n",res);

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
