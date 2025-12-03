#include "RoundedAvatar.hpp"

RoundedAvatar::RoundedAvatar(int width, int height, QWidget *parent) : QWidget(parent), img_height(height), img_width(width)
{
    setFixedSize(height, width);
}

void RoundedAvatar::setPixmap(const QPixmap &argv)
{
    QRect rectcrop(0, 0, img_width, img_height);
    rectcrop.moveCenter(argv.rect().center());
    img = argv.copy(rectcrop);
}

void RoundedAvatar::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    QPainterPath path;
    path.addEllipse(rect());   // Вписываем окружность
    painter.setClipPath(path); // Устанавливаем область обрезания по кру
    painter.drawPixmap(rect(), img);
}
