#include "window.h"

#include <QtCharts/QLineSeries>
#include <QDebug>

#include <algorithm>
#include <math.h>


QLineSeries* make_ser(const std::vector<int>& arr) {
    auto s = new QLineSeries();
    for(size_t i = 0; i < arr.size(); ++i) {
        s->append(i, arr.at(i));
    }
    return s;
}

Window::Window(QObject *parent) : QObject(parent)
{
    widget = new QWidget();
    widget->resize(1000, 600);

    chart = new QChart();
    chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    hlayout = new QHBoxLayout;
    vlayout = new QVBoxLayout;
    img_label = new QLabel("");
    vlayout->addWidget(img_label);

    listWidget = new QListWidget;
    listWidget->addItem("bad");
    listWidget->addItem("good_0");
    listWidget->addItem("good_1");
    listWidget->addItem("good_2");
    listWidget->addItem("good_3");
    listWidget->addItem("good_4");
    listWidget->addItem("good_5");

    vlayout->addWidget(listWidget);
    connect(listWidget, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(isItemSelected(QListWidgetItem*)));

    result_label = new QLabel;

    vlayout->addWidget(result_label);
    verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
    vlayout->addItem(verticalSpacer);
    hlayout->addLayout(vlayout);

    hlayout->addWidget(chartView);

    widget->setLayout(hlayout);
    widget->show();

    redraw("good_0");
}

Window::~Window()
{
    delete widget;
    delete chartView;
    delete chart;
}

void Window::redraw(const QString& imgName)
{
    QImage img;
    img.load(":/imgs/" + imgName + ".png");
    bool result = imgAnalyser.analyze(img);
    if (result) {
        result_label->setText("good");
    } else {
        result_label->setText("bad");
    }

    img_label->setPixmap(QPixmap::fromImage(img));
    img_label->adjustSize();
}

void Window::isItemSelected(QListWidgetItem *item)
{
    qDebug() << item->text();
    redraw(item->text());
}
