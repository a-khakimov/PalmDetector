#ifndef IMAGEANALYSER_H
#define IMAGEANALYSER_H

#include <vector>

#include <QImage>

class ImageAnalyser
{
    typedef std::pair<std::vector<int>, std::vector<int>> arr_t;

    void img2arr(const QImage& img);
    bool is_good(const std::vector<double>& correlation, const std::vector<int>& maximums);
    arr_t append_vec(const QImage& img, const int q_num);
    std::vector<double> get_correlations();
    std::vector<int> get_maximums();
    std::vector<int> prepare_ideal_array(const std::vector<int>& array);
    double gsl_stats_correlation(const std::vector<int>& data);

    QImage image;
    std::vector<std::vector<int>> arrs;

public:
    ImageAnalyser();
    explicit ImageAnalyser(const QImage&);
    bool analyze(const QImage&);
    bool analyze();
    std::vector<std::vector<int>> data();

    virtual ~ImageAnalyser();
};

#endif // IMAGEANALYSER_H
