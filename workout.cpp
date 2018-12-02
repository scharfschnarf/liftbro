#include "workout.h"
#include <QJsonArray>

constexpr char TIMESTAMP_RW_FORMAT_STRING[] = "yyyy-M-d H:m:s";

Workout::Workout(const std::initializer_list<Exercise> &list,
                 Timestamp now):
    m_exerlist{},
    m_finish_timestamp{now}
    //m_loaded{list.size() > 0}
{
    for (const Exercise &list_el: list)
        m_exerlist.push_back(list_el);
}

void Workout::insert_exercise(const Exercise &exer, std::size_t dest_pos)
{
    //m_loaded = true;
    if (m_exerlist.empty()) {
        // Skip iterator calculations
        m_exerlist.push_back(exer);
        return;
    }

    auto it_before = m_exerlist.begin() + dest_pos;
    if ((it_before > m_exerlist.end()) || (it_before < m_exerlist.begin()))
        throw -1;
    m_exerlist.insert(it_before, exer);
}

void Workout::delete_exercise(std::size_t dest_pos)
{
    m_exerlist.erase(m_exerlist.begin() + dest_pos);
}

bool Workout::readJson(const QJsonObject &obj)
{
    if (!m_exerlist.empty())
        m_exerlist.clear();

    // Read timestamp
    if (obj.contains("timestamp")) {
        QString time_string = obj.value("timestamp").toString();
        m_finish_timestamp = QDateTime::fromString(time_string, TIMESTAMP_RW_FORMAT_STRING);
    }

    if (obj.contains("exercises")) {
        QJsonArray exer_array = obj.value("exercises").toArray();
        for (auto it = exer_array.begin(); it != exer_array.end(); ++it) {
            Exercise tmp;
            tmp.readJson(it->toObject());
            m_exerlist.push_back(tmp);
        }
    }

    //m_loaded = true;
    return true;
}

void Workout::writeJson(QJsonObject &obj) const
{
    obj.insert("timestamp", m_finish_timestamp.toString(TIMESTAMP_RW_FORMAT_STRING));

    QJsonArray ex_array;
    for (const auto& ex: m_exerlist) {
        QJsonObject ex_obj;
        ex.writeJson(ex_obj);
        ex_array.push_back(ex_obj);
    }
    obj.insert("exercises", ex_array);
}
