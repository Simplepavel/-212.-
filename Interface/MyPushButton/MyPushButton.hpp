#include <QPushButton>
#include <QString>
#include "../../BackEnd/Game/Figure/Figure.hpp"
#include <QMouseEvent>
#include <QPalette>
class MyPushButton : public QPushButton
{
    Q_OBJECT
    int row;
    int column;
    const Figure *figure;

public:
    MyPushButton(int r, int c);
    void SetFigure(const Figure *);
    void mousePressEvent(QMouseEvent *) override;
    void mouseReleaseEvent(QMouseEvent *) override;
    void setBackGroundColor();
    int get_row();
    int get_column();
    const Figure *get_figure();
};