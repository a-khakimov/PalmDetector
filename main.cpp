#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QVector>
#include <QImage>
#include <QDebug>

QT_CHARTS_USE_NAMESPACE

typedef struct {
    QVector<int> img1_d;
    QVector<int> img1_r;

    QVector<int> img2_d;
    QVector<int> img2_r;

    QVector<int> img3_d;
    QVector<int> img3_r;

    QVector<int> img4_d;
    QVector<int> img4_r;
} divided_img;


QLineSeries* make_series(const QPoint& point1, const QPoint& point2, const QImage& img) {
    QLineSeries* series = new QLineSeries();
    int x_counter = 0;
    int y_counter = 0;
    for(int j = point1.x(); j < point2.x(); ++j) {
        int average_y = 0;
        for(int i = point1.y(); i < point2.y(); ++i) {
            QRgb rgb = img.pixel(j, i);
            average_y += (qRed(rgb) + qGreen(rgb) + qBlue(rgb)) / 3;
            ++y_counter;
        }
        ++x_counter;
        series->append(x_counter, average_y / 1);
    }
    return series;
}


int main(int argc, char *argv[])
{

    QImage img;
    qDebug() << "image is loaded: " << img.load("C:\\Develop\\qimg_using\\good_0.png");
    qDebug() << "height:" << img.height();
    qDebug() << "width:" << img.width();

    QApplication a(argc, argv);

    int height = img.height();
    int width = img.width();

    QLineSeries *s1 = make_series(QPoint(0, 0), QPoint(width / 2, height / 2), img);
    QLineSeries *s2 = make_series(QPoint(width / 2, 0), QPoint(width, height / 2), img);
    QLineSeries *s3 = make_series(QPoint(width / 2, height / 2), QPoint(width, height), img);
    QLineSeries *s4 = make_series(QPoint(0, height / 2), QPoint(width / 2, height), img);
/*
    for(int j = 0; j < width; ++j) {
        int average_y = 0;
        for(int i = 0; i < height; ++i) {
            QRgb rgb = img.pixel(j, i);
            average_y += qRed(rgb);
        }
        red_series->append(j, average_y / height);
    }
*/

    //*series << QPointF(5, 1);

    QChart *chart = new QChart();
    chart->legend()->hide();


    chart->addSeries(s1);
    //chart->addSeries(s2);
    //chart->addSeries(s3);
    chart->addSeries(s4);


    chart->createDefaultAxes();

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    QMainWindow window;
    window.setCentralWidget(chartView);
    window.resize(800, 500);
    window.show();

    return a.exec();

}
