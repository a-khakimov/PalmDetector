#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QVector>
#include <QImage>
#include <QDebug>
#include <exception>

QT_CHARTS_USE_NAMESPACE

QVector<int> prepare_ideal_array(const QVector<int>& array) {
    int min = 60;
    int max = 80;
    int ideal_value = 500;

    QVector<int> ideal;
    ideal.reserve(array.size());

    for(int i = min; i < max; ++i) {
        ideal[i] = ideal_value;
    }

    return ideal;
}

double gsl_stats_correlation(const QVector<int>& data)
{
    QVector<int> ideal = prepare_ideal_array(data);
    const int stride1 = 1;
    const int stride2 = 1;

    double sum_xsq = 0.0;
    double sum_ysq = 0.0;
    double sum_cross = 0.0;

    double mean_x = data[0 * stride1];
    double mean_y = ideal[0 * stride2];

    for(int i = 1; i < data.size(); ++i) {
        double ratio = i / (i + 1.0);
        double delta_x = data[i * stride1] - mean_x;
        double delta_y = ideal[i * stride2] - mean_y;
        sum_xsq += delta_x * delta_x * ratio;
        sum_ysq += delta_y * delta_y * ratio;
        sum_cross += delta_x * delta_y * ratio;
        mean_x += delta_x / (i + 1.0);
        mean_y += delta_y / (i + 1.0);
    }

    double r = sum_cross / (sqrt(sum_xsq) * sqrt(sum_ysq));

    return r;
}


QVector<QVector<int>> get_quad(const QPoint& point1, const QPoint& point2, const QImage& img) {
    QVector<QVector<int>> quad;
    for(int j = point1.x(); j < point2.x(); ++j) {
        QVector<int> line;
        for(int i = point1.y(); i < point2.y(); ++i) {
            QRgb rgb = img.pixel(j, i);
            int average_y = (qRed(rgb) + qGreen(rgb) + qBlue(rgb)) / 3;
            line.append(average_y);
        }
        quad.append(line);
    }
    return quad;
}

QPair<QLineSeries*, QLineSeries*> make_series(const QVector<QVector<int>>& quad)
{
    QLineSeries *line_horizontal = new QLineSeries();
    QLineSeries *line_vertical = new QLineSeries();

    line_horizontal->setName("horizontal");
    line_vertical->setName("vertical");

    for(int i = 0; i < quad.size(); ++i) {
        int average = 0;
        for(int j = 0; j < quad[i].size(); ++j) {
            average += quad[i][j];
        }
        line_horizontal->append(i, average / quad[i].size());
    }

    for(int i = 0; i < quad[i].size(); ++i) {
        int average = 0;

        for(int j = 0; j < quad.size(); ++j) {
            average += quad[j][i];
        }
        line_vertical->append(i, average / quad.size());
    }

    return QPair<QLineSeries*, QLineSeries*>(line_horizontal, line_vertical);
}


int main(int argc, char *argv[])
{
    QImage img;

    qDebug() << "image is loaded:" << img.load("C:\\Develop\\qimg_using\\imgs\\good_1.png");
    qDebug() << "height:" << img.height();
    qDebug() << "width:" << img.width();

    QApplication a(argc, argv);

    int height = img.height();
    int width = img.width();

    auto q1 = get_quad(QPoint(0, 0), QPoint(width / 2, height / 2), img);
    auto q2 = get_quad(QPoint(width / 2, 0), QPoint(width, height / 2), img);
    auto q3 = get_quad(QPoint(width / 2, height / 2), QPoint(width, height), img);
    auto q4 = get_quad(QPoint(0, height / 2), QPoint(width / 2, height), img);

    auto s1 = make_series(q1);
    auto s2 = make_series(q2);
    auto s3 = make_series(q3);
    auto s4 = make_series(q4);

    QChart *chart = new QChart();
    chart->legend()->hide();


    chart->addSeries(s1.first);
    chart->addSeries(s1.second);

    chart->addSeries(s2.first);
    chart->addSeries(s2.second);

    chart->addSeries(s3.first);
    chart->addSeries(s3.second);

    chart->addSeries(s4.first);
    chart->addSeries(s4.second);


    chart->createDefaultAxes();

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    QMainWindow window;
    window.setCentralWidget(chartView);
    window.resize(800, 500);
    window.show();

    return a.exec();

}
