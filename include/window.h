#ifndef WINDOW_H
#define WINDOW_H

#include <QLabel>
#include <QObject>
#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QListWidget>
#include <QtCharts/QChartView>

#include "imageanalyser.h"

QT_CHARTS_USE_NAMESPACE

class Window : public QObject
{
    Q_OBJECT
public:
    explicit Window(QObject *parent = nullptr);
    ~Window();

private:
    QChart *chart;
    QWidget *widget;
    QChartView *chartView;
    QHBoxLayout *hlayout;
    QVBoxLayout *vlayout;
    QLabel* img_label;
    QSpacerItem* verticalSpacer;
    QLabel* result_label;
    QListWidget *listWidget;
    ImageAnalyser imgAnalyser;

    void redraw(const QString&);

private slots:
    void isItemSelected(QListWidgetItem *item);
};

#endif // WINDOW_H
