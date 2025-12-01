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
    QPixmap img;

public:
    RoundedAvatar(QWidget *parent = nullptr) {};
    void setPixmap(const QPixmap &argv, int height, int width);
    void paintEvent(QPaintEvent *);
};