#include "workout.h"
#include <QJsonArray>
#include <sstream>
#include <chrono>
#include <iomanip>
#include <string>

constexpr char TIMESTAMP_RW_FORMAT_STRING[] = "%Y %m %d %H %M %S";

Workout::Workout(const std::initializer_list<Exercise> &list,
                 Timestamp now):
    m_exerlist{},
    m_finish_timestamp{now}
    //m_loaded{list.size() > 0}
{
    for (const Exercise &list_el: list)
        m_exerlist.push_back(list_el);
}

void Workout::insert_exercise(const Exercise &exer, unsigned int dest_pos) 
{
    //m_loaded = true;
    if (m_exerlist.empty()) {
        // Skip iterator calculations
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
        std::string time_string = obj.value("timestamp").toString().toStdString();
        std::istringstream time_stream{time_string};
        std::tm t{};
        time_stream >> std::get_time(&t, TIMESTAMP_RW_FORMAT_STRING);
        m_finish_timestamp = std::chrono::system_clock::from_time_t(std::mktime(&t));
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
    std::ostringstream time_str;
    std::time_t tst = std::chrono::system_clock::to_time_t(m_finish_timestamp);
    time_str << std::put_time(std::localtime(&tst), TIMESTAMP_RW_FORMAT_STRING);
    obj.insert("timestamp", time_str.str().c_str());

    QJsonArray ex_array;
    for (const auto& ex: m_exerlist) {
        QJsonObject ex_obj;
        ex.writeJson(ex_obj);
        ex_array.push_back(ex_obj);
    }
    obj.insert("exercises", ex_array);
}
