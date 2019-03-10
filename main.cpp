#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <vector>
#include <algorithm>
#include <QImage>
#include <QDebug>
#include <QHBoxLayout>
#include <QSpacerItem>
#include <QLabel>
#include <exception>

QT_CHARTS_USE_NAMESPACE

std::vector<int> prepare_ideal_array(const std::vector<int>& array) {
    unsigned int min = array.size() * 0;
    unsigned int max = array.size() * 0.5;
    int ideal_value = 1;

    std::vector<int> ideal;
    ideal.reserve(array.size());

    for(unsigned int i = min; i < max; ++i) {
        ideal[i] = ideal_value;
    }

    return ideal;
}

double gsl_stats_correlation(const std::vector<int>& data)
{
    std::vector<int> ideal = prepare_ideal_array(data);
    const int stride1 = 1;
    const int stride2 = 1;

    double sum_xsq = 0.0;
    double sum_ysq = 0.0;
    double sum_cross = 0.0;

    double mean_x = data[0 * stride1];
    double mean_y = ideal[0 * stride2];

    for(unsigned int i = 1; i < data.size(); ++i) {
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

std::pair<std::vector<int>, std::vector<int>> append_vec(const QImage& img, const int q_num)
{
    int minX = 0;
    int maxX = 0;
    int minY = 0;
    int maxY = 0;

    switch (q_num) {
    case 1:
        minX = 0;
        maxX = img.width() / 2;
        minY = 0;
        maxY = img.height() / 2;
        break;
    case 2:
        minX = img.width() / 2;
        maxX = img.width();
        minY = 0;
        maxY = img.height() / 2;
        break;
    case 3:
        minX = 0;
        maxX = img.width() / 2;
        minY = img.height() / 2;
        maxY = img.height();
        break;
    case 4:
        minX = img.width() / 2;
        maxX = img.width();
        minY = img.height() / 2;
        maxY = img.height();
        break;
    default:
        break;
    }

    std::vector<int> result1;
    for(int x = minX; x < maxX; ++x) {
        int average = 0;
        for(int y = minY; y < maxY; ++y) {
            QRgb rgb = img.pixel(x, y);
            average += (qRed(rgb) + qGreen(rgb) + qBlue(rgb)) / 3;
        }
        result1.push_back(average / (maxY - minY));
    }


    std::vector<int> result2;
    for(int x = minY; x < maxY; ++x) {
        int average = 0;
        for(int y = minX; y < maxX; ++y) {
            QRgb rgb = img.pixel(y, x);
            average += (qRed(rgb) + qGreen(rgb) + qBlue(rgb)) / 3;
        }
        result2.push_back(average / (maxY - minY));
    }

    if(q_num == 1) {
        std::reverse(std::begin(result1), std::end(result1));
        std::reverse(std::begin(result2), std::end(result2));
    }

    if(q_num == 2) {
        std::reverse(std::begin(result2), std::end(result2));
    }

    if(q_num == 3) {
        std::reverse(std::begin(result1), std::end(result1));
    }

    return std::pair<std::vector<int>, std::vector<int>>(result1, result2);
}

typedef struct {
    std::pair<std::vector<int>, std::vector<int>> fist;
    std::pair<std::vector<int>, std::vector<int>> secn;
    std::pair<std::vector<int>, std::vector<int>> thir;
    std::pair<std::vector<int>, std::vector<int>> four;
} arrs_t;

arrs_t img2arr(const QImage& img) {
    arrs_t arrs;
    arrs.fist = append_vec(img, 1);
    arrs.secn = append_vec(img, 2);
    arrs.thir = append_vec(img, 3);
    arrs.four = append_vec(img, 4);
    return arrs;
}

QLineSeries* make_ser(const std::vector<int>& arr) {
    auto s = new QLineSeries();
    for(size_t i = 0; i < arr.size(); ++i) {
        s->append(i, arr.at(i));
    }
    return s;
}

std::pair<std::vector<double>, std::vector<int>> get_max_and_corrs(const arrs_t& arrs)
{
    std::vector<double> correlation;

    correlation.push_back(gsl_stats_correlation(arrs.fist.first));
    correlation.push_back(gsl_stats_correlation(arrs.fist.second));
    correlation.push_back(gsl_stats_correlation(arrs.secn.first));
    correlation.push_back(gsl_stats_correlation(arrs.secn.second));
    correlation.push_back(gsl_stats_correlation(arrs.thir.first));
    correlation.push_back(gsl_stats_correlation(arrs.thir.second));
    correlation.push_back(gsl_stats_correlation(arrs.four.first));
    correlation.push_back(gsl_stats_correlation(arrs.four.second));

    std::vector<int> maximums;

    maximums.push_back(*std::max_element(arrs.fist.first.begin(), arrs.fist.first.end()));
    maximums.push_back(*std::max_element(arrs.fist.second.begin(), arrs.fist.second.end()));
    maximums.push_back(*std::max_element(arrs.secn.first.begin(), arrs.secn.first.end()));
    maximums.push_back(*std::max_element(arrs.secn.second.begin(), arrs.secn.second.end()));
    maximums.push_back(*std::max_element(arrs.thir.first.begin(), arrs.thir.first.end()));
    maximums.push_back(*std::max_element(arrs.thir.second.begin(), arrs.thir.second.end()));
    maximums.push_back(*std::max_element(arrs.four.first.begin(), arrs.four.first.end()));
    maximums.push_back(*std::max_element(arrs.four.second.begin(), arrs.four.second.end()));

    return std::pair<std::vector<double>, std::vector<int>>(correlation, maximums);
}

bool is_good(const std::vector<double>& correlation, const std::vector<int>& maximums)
{
    bool result = true;
    double min_corr = *std::min_element(correlation.begin(), correlation.end());
    if(min_corr < 0) {
        result = false;
    }
    double max_val = *std::max_element(maximums.begin(), maximums.end());
    if(max_val < 30) {
        result = false;
    }
    return result;
}

int main(int argc, char *argv[])
{
    QImage img;

    qDebug() << "image is loaded:" << img.load(":/imgs/good_0.png");
    //qDebug() << "image is loaded:" << img.load(":/imgs/good_1.png");
    //qDebug() << "image is loaded:" << img.load(":/imgs/good_2.png");
    //qDebug() << "image is loaded:" << img.load(":/imgs/good_3.png");
    //qDebug() << "image is loaded:" << img.load(":/imgs/good_4.png");
    //qDebug() << "image is loaded:" << img.load(":/imgs/good_5.png");
    //qDebug() << "image is loaded:" << img.load(":/imgs/bad.png");
    qDebug() << img;

    arrs_t arrs = img2arr(img);

    auto img_params = get_max_and_corrs(arrs);
    bool result = is_good(img_params.first, img_params.second);


    QApplication a(argc, argv);

    QChart *chart = new QChart();
    chart->legend()->hide();

    auto s1 = make_ser(arrs.fist.first);
    chart->addSeries(s1);
    auto s2 = make_ser(arrs.fist.second);
    chart->addSeries(s2);
    auto s3 = make_ser(arrs.secn.first);
    chart->addSeries(s3);
    auto s4 = make_ser(arrs.secn.second);
    chart->addSeries(s4);
    auto s5 = make_ser(arrs.thir.first);
    chart->addSeries(s5);
    auto s6 = make_ser(arrs.thir.second);
    chart->addSeries(s6);
    auto s7 = make_ser(arrs.four.first);
    chart->addSeries(s7);
    auto s8 = make_ser(arrs.four.second);
    chart->addSeries(s8);


    chart->createDefaultAxes();

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    QWidget window;

    QHBoxLayout *hlayout = new QHBoxLayout;
    QVBoxLayout *vlayout = new QVBoxLayout;
    QLabel* img_label = new QLabel("");
    img_label->setPixmap(QPixmap::fromImage(img));
    img_label->adjustSize();
    vlayout->addWidget(img_label);

    QLabel result_label;
    if(result) {
        result_label.setText("good");
    } else {
        result_label.setText("bad");
    }


    vlayout->addWidget(&result_label);
    QSpacerItem verticalSpacer(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
    vlayout->addItem(&verticalSpacer);
    hlayout->addLayout(vlayout);

    hlayout->addWidget(chartView);

    window.setLayout(hlayout);
    window.resize(1000, 200);
    window.show();

    return a.exec();

}
