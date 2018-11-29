#include "exercise.h"
#include <algorithm>
#include <QJsonArray>

Exercise::Exercise(const std::string &name, const std::initializer_list<SetStorage> &list):
    m_name{name}
{
    std::copy(list.begin(), list.end(), std::back_inserter(m_setlist));
}

void Exercise::insert_set(const SetStorage &new_set, std::size_t dest_pos)
{
    if (m_setlist.empty()) {
        m_setlist.push_back(new_set);
        return;
    }

    auto it_before = m_setlist.begin() + (dest_pos - 1);
    if ((it_before > m_setlist.end()) || (it_before < m_setlist.begin()))
        throw 'a';

    m_setlist.insert(it_before, new_set);
}

void Exercise::delete_set(std::size_t dest_set)
{
    if (dest_set > size())
        return;

    m_setlist.erase(m_setlist.begin() + dest_set - 1);
}

bool Exercise::readJson(const QJsonObject &obj)
{
    if (!obj.contains("sets_list"))
        return false;

    QJsonArray set_arr = obj.find("sets_list")->toArray();
    for (auto it = set_arr.begin(); it != set_arr.end(); ++it) {
        SetStorage set;
        set.readJson(it->toObject());
        m_setlist.push_back(set);
    }

    if (obj.contains("name")) {
        QString name = obj.find("name")->toString();
        m_name = name.toStdString();
    } else
        m_name = "";

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

    if (!m_name.empty())
        target.insert("name", m_name.c_str());

    target.insert("sets_list", set_arr);
}
