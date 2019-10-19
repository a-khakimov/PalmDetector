#ifndef IMAGEANALYSER_H
#define IMAGEANALYSER_H

#include <vector>

#include <QImage>

class ImageAnalyser
{
    typedef struct {
        std::pair<std::vector<int>, std::vector<int>> fist;
        std::pair<std::vector<int>, std::vector<int>> secn;
        std::pair<std::vector<int>, std::vector<int>> thir;
        std::pair<std::vector<int>, std::vector<int>> four;
    } arrs_t;

    arrs_t img2arr(const QImage& img);
    bool is_good(const std::vector<double>& correlation, const std::vector<int>& maximums);
    std::pair<std::vector<int>, std::vector<int>> append_vec(const QImage& img, const int q_num);
    std::pair<std::vector<double>, std::vector<int>> get_max_and_corrs(const arrs_t& arrs);
    std::vector<int> prepare_ideal_array(const std::vector<int>& array);
    double gsl_stats_correlation(const std::vector<int>& data);

    QImage image;

public:
    ImageAnalyser();
    explicit ImageAnalyser(const QImage&);
    bool analyze(const QImage&);
    bool analyze();

    virtual ~ImageAnalyser();
};

#endif // IMAGEANALYSER_H
