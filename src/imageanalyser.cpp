#include "imageanalyser.h"
#include <math.h>
#include <QDebug>

using namespace std;

ImageAnalyser::ImageAnalyser()
{

}

ImageAnalyser::ImageAnalyser(const QImage& img) : image(img)
{

}

bool ImageAnalyser::analyze(const QImage& img)
{
    image = img;
    img2arr(image);
    auto correlaions = get_correlations();
    auto maximums = get_maximums();
    bool result = is_good(correlaions, maximums);
    return result;
}

bool ImageAnalyser::analyze()
{
    return analyze(image);
}

std::vector<std::vector<int> > ImageAnalyser::data()
{
    return arrs;
}

ImageAnalyser::~ImageAnalyser()
{

}

bool ImageAnalyser::is_good(const vector<double>& correlation, const vector<int>& maximums)
{
    bool result = true;
    double min_corr = *std::min_element(correlation.begin(), correlation.end());
    qDebug() << "min_corr:" << min_corr;
    if (min_corr < 0.5) {
        result = false;
    }
    double min_val = *std::min_element(maximums.begin(), maximums.end());
    qDebug() << "min_val:" << min_val;
    if (min_val < 30) {
        result = false;
    }
    return result;
}


void ImageAnalyser::img2arr(const QImage& img)
{
    arrs.clear();
    for (int i = 1; i <= 4; ++i) {
        arr_t arr = append_vec(img, i);
        arrs.push_back(arr.first);
        arrs.push_back(arr.second);
    }
}

ImageAnalyser::arr_t ImageAnalyser::append_vec(const QImage& img, const int q_num)
{
    int minX = 0, maxX = 0, minY = 0, maxY = 0;

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
    for (int x = minX; x < maxX; ++x) {
        int average = 0;
        for (int y = minY; y < maxY; ++y) {
            QRgb rgb = img.pixel(x, y);
            average += (qRed(rgb) + qGreen(rgb) + qBlue(rgb)) / 3;
        }
        result1.push_back(average / (maxY - minY));
    }


    std::vector<int> result2;
    for (int x = minY; x < maxY; ++x) {
        int average = 0;
        for (int y = minX; y < maxX; ++y) {
            QRgb rgb = img.pixel(y, x);
            average += (qRed(rgb) + qGreen(rgb) + qBlue(rgb)) / 3;
        }
        result2.push_back(average / (maxY - minY));
    }

    if (q_num == 1) {
        std::reverse(std::begin(result1), std::end(result1));
        std::reverse(std::begin(result2), std::end(result2));
    }

    if (q_num == 2) {
        std::reverse(std::begin(result2), std::end(result2));
    }

    if (q_num == 3) {
        std::reverse(std::begin(result1), std::end(result1));
    }

    return std::pair<std::vector<int>, std::vector<int>>(result1, result2);
}

std::vector<int> ImageAnalyser::prepare_ideal_array(const std::vector<int>& array)
{
    unsigned long min = static_cast<unsigned long>(array.size() * 0);
    unsigned long max = static_cast<unsigned long>(array.size() * 0.45);
    int ideal_value = 100;

    std::vector<int> ideal;
    ideal.resize(array.size());

    for(unsigned long i = min; i < max; ++i) {
        ideal[i] = ideal_value;
    }

    return ideal;
}

double ImageAnalyser::gsl_stats_correlation(const std::vector<int>& data)
{
    std::vector<int> ideal = prepare_ideal_array(data);
    const int stride1 = 1;
    const int stride2 = 1;

    double sum_xsq = 0.0;
    double sum_ysq = 0.0;
    double sum_cross = 0.0;

    double mean_x = data[0];
    double mean_y = ideal[0];

    for (unsigned int i = 1; i < data.size(); ++i) {
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

vector<double> ImageAnalyser::get_correlations()
{
    std::vector<double> correlations;

    for (auto array : arrs) {
        auto correlation = gsl_stats_correlation(array);
        correlations.push_back(correlation);
    }

    return correlations;
}

vector<int> ImageAnalyser::get_maximums()
{
    std::vector<int> maximums;

    for (auto array : arrs) {
        int maximum = *std::max_element(array.begin(), array.end());
        maximums.push_back(maximum);
    }

    return maximums;
}
