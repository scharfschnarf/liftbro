#include "exercise.h"
#include <algorithm>
#include <QJsonArray>

Exercise::Exercise(const std::initializer_list<SetStorage> &list):
    m_setlist{list.size()}
{
    std::for_each(list.begin(), list.end(),
                  [this] (const SetStorage &s) {
                      this->m_setlist.push_back(s);
                  });
}

void Exercise::insert_set(const SetStorage &new_set, unsigned int dest_pos)
{
    m_setlist.insert(m_setlist.begin() + dest_pos,
                     new_set);
}

void Exercise::delete_set(unsigned int dest_set)
{
    if (dest_set > size())
        return;

    m_setlist.erase(m_setlist.begin() + dest_set - 1);
}

bool Exercise::readJson(const QJsonObject &obj)
{
    if (!obj.contains("sets_list"))
        return false;

    QJsonArray set_arr{obj.value("sets_list")};
    for (const auto json_obj: set_arr) {
        if (!json_obj.isObject())
            continue;
        SetStorage set;
        set.readJson(std::move(json_obj.toObject()));
        m_setlist.emplace_back(std::move(set));
    }
    return true;
}

void Exercise::writeJson(QJsonObject &target) const
{
    QJsonArray set_arr;
    for (const SetStorage &set: m_setlist) {
        QJsonObject set_obj;
        set.writeJson(set_obj);
        set_arr.push_back(set_obj);
    }

    target.insert("sets_list", set_arr);
}
