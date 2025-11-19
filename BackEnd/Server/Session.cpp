#include "Session.hpp"

uint32_t Session::last_id = 1;
std::stack<uint32_t> Session::ids{};

Session::Session(const Player &p1, const Player &p2) : pl1(p1), pl2(p2)
{
    if (ids.empty())
    {
        id = last_id;
        ++last_id;
    }
    else
    {
        id = ids.top();
        ids.pop();
    }
}

Session::~Session()
{
    ids.push(id);
}
