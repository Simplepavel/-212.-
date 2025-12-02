#include "RoundedAvatar.hpp"

void RoundedAvatar::setPixmap(const QPixmap &argv, int height, int width)
{
    resize(width, height);
    QRect rectcrop(0, 0, width, height);
    rectcrop.moveCenter(img.rect().center());
    img = argv.copy(rectcrop);
    update();
}

void RoundedAvatar::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    QPainterPath path;
    path.addEllipse(img.rect()); // Вписываем окружность
    painter.setClipPath(path);   // Устанавливаем область обрезания по кругу
    painter.drawPixmap(0, 0, img);
}