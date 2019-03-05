#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QVector>
#include <QImage>
#include <QDebug>
#include <exception>

QT_CHARTS_USE_NAMESPACE

double
gsl_stats_correlation(const int data1[], const size_t stride1,
                      const int data2[], const size_t stride2,
                      const size_t n)
{
    size_t i;
    double sum_xsq = 0.0;
    double sum_ysq = 0.0;
    double sum_cross = 0.0;
    double ratio;
    double delta_x, delta_y;
    double mean_x, mean_y;
    double r;

    mean_x = data1[0 * stride1];
    mean_y = data2[0 * stride2];

    for(i = 1; i < n; ++i) {
        ratio = i / (i + 1.0);
        delta_x = data1[i * stride1] - mean_x;
        delta_y = data2[i * stride2] - mean_y;
        sum_xsq += delta_x * delta_x * ratio;
        sum_ysq += delta_y * delta_y * ratio;
        sum_cross += delta_x * delta_y * ratio;
        mean_x += delta_x / (i + 1.0);
        mean_y += delta_y / (i + 1.0);
    }

    r = sum_cross / (sqrt(sum_xsq) * sqrt(sum_ysq));

    return r;
}


QLineSeries* make_series(const QPoint& point1, const QPoint& point2, const QImage& img) {
    QLineSeries* series = new QLineSeries();
    int x_counter = 0;
    int y_counter = point2.x() - point1.x();
    for(int j = point1.x(); j < point2.x(); ++j) {
        int average_y = 0;
        for(int i = point1.y(); i < point2.y(); ++i) {
            QRgb rgb = img.pixel(j, i);
            average_y += (qRed(rgb) + qGreen(rgb) + qBlue(rgb)) / 3;
        }
        ++x_counter;
        series->append(x_counter, average_y / y_counter);
    }
    return series;
}


int main(int argc, char *argv[])
{
    QImage img;

    qDebug() << "image is loaded:" << img.load("D:\\Develop\\find_points\\imgs\\good_1.png");
    qDebug() << "height:" << img.height();
    qDebug() << "width:" << img.width();

    QApplication a(argc, argv);

    int height = img.height();
    int width = img.width();

    QLineSeries *s1 = make_series(QPoint(0, 0), QPoint(width / 2, height / 2), img);
    QLineSeries *s2 = make_series(QPoint(width / 2, 0), QPoint(width, height / 2), img);
    QLineSeries *s3 = make_series(QPoint(width / 2, height / 2), QPoint(width, height), img);
    QLineSeries *s4 = make_series(QPoint(0, height / 2), QPoint(width / 2, height), img);

    QChart *chart = new QChart();
    chart->legend()->hide();


    chart->addSeries(s1);
    chart->addSeries(s2);
    chart->addSeries(s3);
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
