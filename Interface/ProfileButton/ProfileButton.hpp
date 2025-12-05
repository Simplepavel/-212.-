#include <QPushButton>
#include <QWidget>
#include <cstdint>
#include <QString>

class ProfileButton : public QPushButton
{
private:
    uint32_t id;

public:
    ProfileButton(QWidget *parent = nullptr);
    ProfileButton(const QString &caption, int _id = 0, QWidget *parent = nullptr);
    uint32_t get_id();
    void set_id(int _id);
};