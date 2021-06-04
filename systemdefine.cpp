#include "systemdefine.h"

void FindRowPair(const QImage &mat, vector<pair<int,int>> &rowpairs)
{
    rowpairs.clear();
//    if(mat.empty()||mat.type()!=CV_8UC4)
    if(mat.format()!=QImage::Format_ARGB32)
        return;

    const unsigned char *ptr;
    int i,j;
    bool firstrow = false;
    bool secondrow = false;
    pair<int,int> rowpair;

    for(i=0;i<mat.height();++i)
    {
//        ptr = mat.ptr<unsigned char>(i);
        ptr = mat.scanLine(i);
        unsigned int sum = 0;
        for(j=0;j<mat.width();++j)
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

void FindColumnPair(const QImage &mat, vector<pair<int,int>> &columnpairs)
{
    columnpairs.clear();
//    if(mat.empty()||mat.type()!=CV_8UC4)
    if(mat.format()!=QImage::Format_ARGB32)
        return;

    const unsigned char *ptr;
    int i,j;
    bool firstcolumn = false;
    bool secondcolumn = false;
    pair<int,int> columnpair;

    for(j=0;j<mat.width();++j)
    {
        int sum = 0;
        for(i=0;i<mat.height();++i)
        {
//            ptr = mat.ptr<unsigned char>(i);
            ptr = mat.scanLine(i);
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

QImage* CopyImageROI(const QRect &rect, const QImage &image)
{
    if(image.format() != QImage::Format_ARGB32)
        return nullptr;
    if(rect.width()>image.width() || rect.height()>image.height())
        return nullptr;

    QImage *result = new QImage(rect.size(),image.format());

    for(int i=0;i<rect.height();++i)
    {
//        memcpy(result->scanLine(i),image.scanLine(rect.y()+i)+rect.x() ,rect.width()*image.depth()>>3);
        for(int j=0;j<rect.width();++j)
        {
            result->setPixel(j,i,image.pixel(j+rect.x(),i+rect.y()));
        }
    }

    return result;
}

SpriteRectangle* FindRowAndColumnPair(const QImage &img, const QRect &rect)
{
    if(img.isNull())
        return nullptr;

    QImage *roi = CopyImageROI(rect,img);
    if(roi == nullptr)
        return nullptr;

    SpriteRectangle *result = new SpriteRectangle;

    if(result==nullptr)
        return nullptr;

    result->image_area = rect;

    FindRowPair(*roi,result->rowpairs);
    for(int i=0;i<result->rowpairs.size();++i)
    {
        QRect row_rect;
        vector<pair<int,int>> cols;
        row_rect.setX(0);
        row_rect.setY(result->rowpairs[i].first);
        row_rect.setWidth(rect.width());
        row_rect.setHeight(result->rowpairs[i].second-result->rowpairs[i].first);
        QImage *roi_roi = CopyImageROI(row_rect,*roi);
        if(roi_roi != nullptr)
            FindColumnPair(*roi_roi,cols);
        result->columnpairs.push_back(cols);
        if(roi_roi!=nullptr)
            delete roi_roi;
    }

    delete roi;

    return result;
}

//void SpriteSeparate()
//{
//    if(operator_data==nullptr)
//        return;

//    Mat src_mat = QDataModelInstance::Instance()->GetData(windowTitle().toStdString().c_str())->GetOriginalImage().clone();
//    if(src_mat.empty())
//        return;

//    vector<pair<int,int>> rowpairs;
//    vector<pair<int,int>> columnpairs;
//    FindRowPair(src_mat,rowpairs);
//    FindColumnPair(src_mat(Rect(0,rowpairs[8].first,src_mat.cols,rowpairs[8].second-rowpairs[8].first)),columnpairs);

//    for(size_t i=0;i<rowpairs.size();++i)
//    {
//        cv::line(src_mat,Point(0,rowpairs[i].first), Point(src_mat.cols,rowpairs[i].first),Scalar(128,0,128,255));
//        cv::line(src_mat,Point(0,rowpairs[i].second), Point(src_mat.cols,rowpairs[i].second),Scalar(128,0,128,255));
//    }

//    RNG rng(256);
//    for(size_t i=0;i<columnpairs.size();++i)
//    {
//        Scalar color = Scalar(rand()%257,rand()%257,rand()%257,255);
//        cv::line(src_mat,Point(columnpairs[i].first,rowpairs[8].first), Point(columnpairs[i].first,rowpairs[8].second),color);
//        cv::line(src_mat,Point(columnpairs[i].second,rowpairs[8].first), Point(columnpairs[i].second,rowpairs[8].second),color);
//    }

//    QFile file(QApplication::applicationDirPath()+"/Champion.json");
//    if(!file.open(QIODevice::WriteOnly))
//    {
//         qDebug() << "File open failed!";
//    }
//    else
//    {
//        qDebug() <<"File open successfully!";
//    }

//    QJsonDocument jdoc;
//    QJsonObject obj;
//    QJsonArray arr;
//    obj["Name"] = "Champion";
//    for(size_t i=0;i<columnpairs.size();++i)
//    {
//        QJsonObject member;
//        member["Left"] = QString("%1").arg(columnpairs[i].first);
//        member["Top"] = QString("%1").arg(rowpairs[8].first);
//        member["Width"] = QString("%1").arg(columnpairs[i].second-columnpairs[i].first);
//        member["Height"] = QString("%1").arg(rowpairs[8].second-rowpairs[8].first);
//        arr.append(member);
//    }

//    obj["Walk"] = arr;

//    jdoc.setObject(obj);
//    file.write(jdoc.toJson(QJsonDocument::Indented));
//    file.close();




//    imshow("contours",src_mat);
//    imwrite("rowcol.png",src_mat);
//    waitKey(0);
//}
