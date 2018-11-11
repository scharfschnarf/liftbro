#include "workout.h"
#include <QJsonArray>

Workout::Workout(const std::initializer_list<Exercise> &list):
    m_exerlist{list.size()},
    m_finish_timestamp{}
{
    for (const Exercise &list_el: list)
        m_exerlist.push_back(list_el);
}

void Workout::insert_exercise(const Exercise &exer, unsigned int dest_pos) 
{
    if (m_exerlist.empty()) {
        m_exerlist.push_back(exer);
        return;
    }

    auto it_before = m_exerlist.begin() + dest_pos - 1;
    if ((it_before > m_exerlist.end()) || (it_before < m_exerlist.begin()))
        throw -1;
    m_exerlist.insert(it_before, exer);
}

void Workout::delete_exercise(unsigned int dest_pos)
{
    m_exerlist.erase(m_exerlist.begin() + dest_pos);
}

bool Workout::readJson(const QJsonObject &obj)
{
    if (!m_exerlist.empty())
        m_exerlist.clear();

    using namespace std::chrono;
    // Read timestamp
    if (obj.contains("timestamp")) {
        // TODO: consider using alternative to double (string?)
        long rawstamp = static_cast<long>(obj.value("timestamp").toDouble());
        duration<long> time_since_epoch{rawstamp};
        m_finish_timestamp = Timestamp{time_since_epoch};
    }

    if (obj.contains("exercises")) {
        QJsonArray exer_array = obj.value("exercises").toArray();
        Exercise tmp;
        for (auto it = exer_array.begin(); it != exer_array.end(); ++it) {
            tmp.readJson(it->toObject());
            m_exerlist.push_back(tmp);
        }
    }

    return true;
}

void Workout::writeJson(QJsonObject &obj) const
{
    long rawstamp = m_finish_timestamp.time_since_epoch().count();
    obj.insert("timestamp", static_cast<double>(rawstamp));

    QJsonArray ex_array;
    for (const auto& ex: m_exerlist) {
        QJsonObject ex_obj;
        ex.writeJson(ex_obj);
        ex_array.push_back(ex_obj);
    }
    obj.insert("exercises", ex_array);
}
