#include "ProfileButton.hpp"

ProfileButton::ProfileButton(QWidget *parent) : QPushButton(parent)
{
}

ProfileButton::ProfileButton(const QString &caption, int _id, QWidget *parent) : QPushButton(caption, parent), id(_id)
{
}

uint32_t ProfileButton::get_id() { return id; }
void ProfileButton::set_id(int new_id) { id = new_id; }