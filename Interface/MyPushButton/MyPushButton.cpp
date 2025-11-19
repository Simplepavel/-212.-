#include "MyPushButton.hpp"

MyPushButton::MyPushButton(int r, int c) : row(r), column(c), figure(nullptr)
{
    setAutoFillBackground(true);
    setBackGroundColor();
    setFont(QFont("Calibri", 32));
}

void MyPushButton::SetFigure(const Figure *f)
{
    QPalette current_palette = palette();
    figure = f;
    setText(QString::fromStdString(figure->to_string()));
    if (figure->get_color() == FigureColor::WHITE)
    {
        current_palette.setColor(QPalette::ButtonText, QColor(200, 200, 200));
    }
    else
    {
        current_palette.setColor(QPalette::ButtonText, QColor(40, 40, 40));
    }
    setPalette(current_palette);
}

void MyPushButton::mousePressEvent(QMouseEvent *event)
{
    QPalette current_pallete = palette();
    current_pallete.setColor(QPalette::Button, QColor(178, 161, 144));
    setPalette(current_pallete);
    QPushButton::mousePressEvent(event);
}

void MyPushButton::mouseReleaseEvent(QMouseEvent *event)
{
    setBackGroundColor();
    QPushButton::mouseReleaseEvent(event);
}

void MyPushButton::setBackGroundColor()
{
    QPalette current_pallete = palette();
    bool color;
    if (row % 2 == 0)
    {
        if ((row * 8 + column) % 2 == 0)
            color = true;

        else
            color = false;
    }
    else
    {
        if ((row * 8 + column) % 2 == 0)
            color = false;
        else
            color = true;
    }
    current_pallete.setColor(QPalette::Button, (color ? QColor(255, 255, 255) : QColor(101, 67, 33)));
    setPalette(current_pallete);
}

int MyPushButton::get_row() { return row; }
int MyPushButton::get_column() { return column; }
const Figure *MyPushButton::get_figure() { return figure; }