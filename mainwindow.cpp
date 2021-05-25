#include <QMdiSubWindow>
#include "spritesplitter.h"
#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setCentralWidget(&mdi_area);

    connect(ui->actionSpriteSplitter,&QAction::triggered,this,&MainWindow::slotSpriteSplitter);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::slotSpriteSplitter()
{
    QMdiSubWindow *sub_win = new QMdiSubWindow;
    SpriteSplitter *widget = new SpriteSplitter;
    sub_win->setWidget(widget);
    mdi_area.addSubWindow(sub_win);
    sub_win->show();
}

/*
void MainWindow::FindRowPair(const Mat &mat, vector<pair<int,int>> &rowpairs)
{
    rowpairs.clear();
    if(mat.empty()||mat.type()!=CV_8UC4)
        return;

    const unsigned char *ptr;
    int i,j;
    bool firstrow = false;
    bool secondrow = false;
    pair<int,int> rowpair;

    for(i=0;i<mat.rows;++i)
    {
        ptr = mat.ptr<unsigned char>(i);
        unsigned int sum = 0;
        for(j=0;j<mat.cols;++j)
        {
            sum += ptr[j*4+3];
        }
        if(sum!=0 && firstrow==false && secondrow == false)
        {
            firstrow = true;
            rowpair.first = i;
        }
        if(sum==0 && firstrow==true && secondrow==false)
        {
            firstrow = false;
            rowpair.second = i;
            rowpairs.push_back(rowpair);
        }
    }
}

void MainWindow::FindColumnPair(const Mat &mat, vector<pair<int,int>> &columnpairs)
{
    columnpairs.clear();
    if(mat.empty()||mat.type()!=CV_8UC4)
        return;

    const unsigned char *ptr;
    int i,j;
    bool firstcolumn = false;
    bool secondcolumn = false;
    pair<int,int> columnpair;

    for(j=0;j<mat.cols;++j)
    {
        int sum = 0;
        for(i=0;i<mat.rows;++i)
        {
            ptr = mat.ptr<unsigned char>(i);
            sum += ptr[j*4+3];
        }
        if(sum!=0 && firstcolumn==false && secondcolumn==false)
        {
            firstcolumn = true;
            columnpair.first = j;
        }
        if(sum==0 && firstcolumn==true && secondcolumn==false)
        {
            firstcolumn = false;
            columnpair.second =j;
            if(columnpair.second-columnpair.first>10)
                columnpairs.push_back(columnpair);
        }
    }
}

void MainWindow::SpriteSeparate()
{
    if(operator_data==nullptr)
        return;

    Mat src_mat = QDataModelInstance::Instance()->GetData(windowTitle().toStdString().c_str())->GetOriginalImage().clone();
    if(src_mat.empty())
        return;

    vector<pair<int,int>> rowpairs;
    vector<pair<int,int>> columnpairs;
    FindRowPair(src_mat,rowpairs);
    FindColumnPair(src_mat(Rect(0,rowpairs[8].first,src_mat.cols,rowpairs[8].second-rowpairs[8].first)),columnpairs);

    for(size_t i=0;i<rowpairs.size();++i)
    {
        cv::line(src_mat,Point(0,rowpairs[i].first), Point(src_mat.cols,rowpairs[i].first),Scalar(128,0,128,255));
        cv::line(src_mat,Point(0,rowpairs[i].second), Point(src_mat.cols,rowpairs[i].second),Scalar(128,0,128,255));
    }

    RNG rng(256);
    for(size_t i=0;i<columnpairs.size();++i)
    {
        Scalar color = Scalar(rand()%257,rand()%257,rand()%257,255);
        cv::line(src_mat,Point(columnpairs[i].first,rowpairs[8].first), Point(columnpairs[i].first,rowpairs[8].second),color);
        cv::line(src_mat,Point(columnpairs[i].second,rowpairs[8].first), Point(columnpairs[i].second,rowpairs[8].second),color);
    }

    QFile file(QApplication::applicationDirPath()+"/Champion.json");
    if(!file.open(QIODevice::WriteOnly))
    {
         qDebug() << "File open failed!";
    }
    else
    {
        qDebug() <<"File open successfully!";
    }

    QJsonDocument jdoc;
    QJsonObject obj;
    QJsonArray arr;
    obj["Name"] = "Champion";
    for(size_t i=0;i<columnpairs.size();++i)
    {
        QJsonObject member;
        member["Left"] = QString("%1").arg(columnpairs[i].first);
        member["Top"] = QString("%1").arg(rowpairs[8].first);
        member["Width"] = QString("%1").arg(columnpairs[i].second-columnpairs[i].first);
        member["Height"] = QString("%1").arg(rowpairs[8].second-rowpairs[8].first);
        arr.append(member);
    }

    obj["Walk"] = arr;

    jdoc.setObject(obj);
    file.write(jdoc.toJson(QJsonDocument::Indented));
    file.close();




    imshow("contours",src_mat);
    imwrite("rowcol.png",src_mat);
    waitKey(0);
}
*/
