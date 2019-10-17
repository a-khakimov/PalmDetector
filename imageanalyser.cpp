#include "imageanalyser.h"
#include <math.h>
#include <QDebug>

ImageAnalyser::ImageAnalyser()
{

}

ImageAnalyser::ImageAnalyser(const QImage& img) : image(img)
{
}

bool ImageAnalyser::analyze(const QImage& img)
{
    image = img;
    arrs_t arrs = img2arr(image);
    auto img_params = get_max_and_corrs(arrs);
    bool result = is_good(img_params.first, img_params.second);
    return result;
}

bool ImageAnalyser::analyze()
{
    return analyze(image);
}

ImageAnalyser::~ImageAnalyser()
{

}

bool ImageAnalyser::is_good(const std::vector<double>& correlation, const std::vector<int>& maximums)
{
    bool result = true;
    double min_corr = *std::min_element(correlation.begin(), correlation.end());
    qDebug() << "min_corr:" << min_corr;
    if (min_corr < 0.5) {
        result = false;
    }
    double max_val = *std::max_element(maximums.begin(), maximums.end());
    qDebug() << "max_val:" << max_val;
    if (max_val < 30) {
        result = false;
    }
    return result;
}


ImageAnalyser::arrs_t ImageAnalyser::img2arr(const QImage& img)
{
    arrs_t arrs;
    arrs.fist = append_vec(img, 1);
    arrs.secn = append_vec(img, 2);
    arrs.thir = append_vec(img, 3);
    arrs.four = append_vec(img, 4);
    return arrs;
}

std::pair<std::vector<int>, std::vector<int>> ImageAnalyser::append_vec(const QImage& img, const int q_num)
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

std::pair<std::vector<double>, std::vector<int>> ImageAnalyser::get_max_and_corrs(const arrs_t& arrs)
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

    qDebug() << QVector<double>::fromStdVector(correlation);

    std::vector<int> maximums;

    maximums.push_back(*std::max_element(arrs.fist.first.begin(), arrs.fist.first.end()));
    maximums.push_back(*std::max_element(arrs.fist.second.begin(), arrs.fist.second.end()));
    maximums.push_back(*std::max_element(arrs.secn.first.begin(), arrs.secn.first.end()));
    maximums.push_back(*std::max_element(arrs.secn.second.begin(), arrs.secn.second.end()));
    maximums.push_back(*std::max_element(arrs.thir.first.begin(), arrs.thir.first.end()));
    maximums.push_back(*std::max_element(arrs.thir.second.begin(), arrs.thir.second.end()));
    maximums.push_back(*std::max_element(arrs.four.first.begin(), arrs.four.first.end()));
    maximums.push_back(*std::max_element(arrs.four.second.begin(), arrs.four.second.end()));

    qDebug() << QVector<int>::fromStdVector(maximums);

    return std::pair<std::vector<double>, std::vector<int>>(correlation, maximums);
}
