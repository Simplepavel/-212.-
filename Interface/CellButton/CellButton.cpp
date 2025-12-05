#include "CellButton.hpp"

QList<QPixmap *> CellButton::ChessImages;

CellButton::CellButton(int r, int c) : row(r), column(c), figure(nullptr)
{
    setAutoFillBackground(true);
    setBackGroundColor();
}

void CellButton::SetFigure(const Figure *f)
{
    figure = f;
}

void CellButton::mousePressEvent(QMouseEvent *event)
{
    QPalette current_pallete = palette();
    current_pallete.setColor(QPalette::Button, QColor(178, 161, 144));
    setPalette(current_pallete);
    QPushButton::mousePressEvent(event);
}

void CellButton::mouseReleaseEvent(QMouseEvent *event)
{
    setBackGroundColor();
    QPushButton::mouseReleaseEvent(event);
}

void CellButton::setBackGroundColor()
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

int CellButton::get_row() { return row; }
int CellButton::get_column() { return column; }
const Figure *CellButton::get_figure() { return figure; }

void CellButton::LoadChessImages(int cell_Size)

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

void CellButton::paintEvent(QPaintEvent *e)
{
    QPushButton::paintEvent(e);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    uint8_t idx = figure->GetImgNum();
    if (idx < 255)
    {
        QPixmap *value = ChessImages[figure->GetImgNum()];
        painter.drawPixmap(0, 0, *value);
    }
}
