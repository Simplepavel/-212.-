#include <QPushButton>
#include <QString>
#include "../../BackEnd/Game/Figure/Figure.hpp"
#include <QMouseEvent>
#include <QPalette>
#include <QPainter>
#include <QPaintEvent>
#include <QDebug>
class MyPushButton : public QPushButton
{
    Q_OBJECT
    int row;
    int column;
    const Figure *figure;
    static QList<QPixmap *> ChessImages;

public:
    static void LoadChessImages(int);
    MyPushButton(int r, int c);
    void SetFigure(const Figure *);
    void mousePressEvent(QMouseEvent *) override;
    void mouseReleaseEvent(QMouseEvent *) override;
    void setBackGroundColor();
    int get_row();
    int get_column();
    const Figure *get_figure();
    void paintEvent(QPaintEvent *) override;
};