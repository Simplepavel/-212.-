#include <QWidget>
#include <QPixmap>
#include <string>
#include <QPaintEvent>
#include <QPainter>
#include <QPainterPath>
#include <QRectF>
#include <QRect>
#include <iostream>
class RoundedAvatar : public QWidget
{
    Q_OBJECT
    QPixmap img;
    int img_height;
    int img_width;

public:
    RoundedAvatar(QWidget *parent = nullptr) : QWidget(parent) {};
    RoundedAvatar(int, int, QWidget *parent = nullptr);
    void setPixmap(const QPixmap &argv);
    void paintEvent(QPaintEvent *);
};