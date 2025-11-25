#include "MyPushButton.hpp"

QList<QPixmap *> MyPushButton::ChessImages;

MyPushButton::MyPushButton(int r, int c) : row(r), column(c), figure(nullptr)
{
    setAutoFillBackground(true);
    setBackGroundColor();
}

void MyPushButton::SetFigure(const Figure *f)
{
    figure = f;
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

void MyPushButton::LoadChessImages(int cell_Size)

{
    QList<QString> FigureNames{"King.png", "Queen.png", "Rook.png", "Bishop.png", "Knight.png", "Pawn.png"};
    for (auto i = FigureNames.begin(); i != FigureNames.end(); ++i)
    {
        QString filename = "Static/Chess/Black" + *i;
        QPixmap img(filename);
        QPixmap *new_img = new QPixmap(img.scaled(cell_Size, cell_Size, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
        ChessImages.push_back(new_img);
    }

    for (auto i = FigureNames.begin(); i != FigureNames.end(); ++i)
    {
         QString filename = "Static/Chess/White" + *i;
        QPixmap img(filename);
        QPixmap *new_img = new QPixmap(img.scaled(cell_Size, cell_Size, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
        ChessImages.push_back(new_img);
    }
}

void MyPushButton::paintEvent(QPaintEvent *e)
{
    QPushButton::paintEvent(e);
    QPainter painter(this);
    uint8_t idx = figure->GetImgNum();
    if (idx < 255)
    {
        QPixmap *value = ChessImages[figure->GetImgNum()];
        painter.drawPixmap(0, 0, *value);
    }
}
