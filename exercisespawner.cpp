#include "exercisespawner.h"

constexpr char SETS_COUNT_NAME[] = "setsCount";
constexpr char SET_DESCRIPTION[] = "setDesc";

ExerciseSpawner::ExerciseSpawner(const Exercise &make_from):
    m_sets {make_from.size()},
    m_sample {(m_sets == 0) ? SetStorage{} : make_from.get_set(1)}
{
}

void ExerciseSpawner::set_template(const Exercise &make_from)
{
    m_sets = make_from.size();
    if (m_sets > 0)
        m_sample = make_from.get_set(1);
    else
        m_sample = SetStorage{};
}

Exercise ExerciseSpawner::make_from_template() const
{
    Exercise e;
    for (std::size_t i = 0; i < m_sets; ++i)
        e.insert_set(m_sample, i + 1);

    return e;
}

bool ExerciseSpawner::readJson(const QJsonObject &json)
{
    if (json.contains(SETS_COUNT_NAME))
        m_sets = static_cast<std::size_t>(json.value(SETS_COUNT_NAME).toDouble());
    else
        m_sets = 0;

    if (json.contains(SET_DESCRIPTION))
        m_sample.readJson(json.value(SET_DESCRIPTION).toObject());
    else
        m_sample = SetStorage{};

    return true;
}

void ExerciseSpawner::writeJson(QJsonObject &obj) const
{
    obj.insert(SETS_COUNT_NAME, static_cast<double>(m_sets));
    QJsonObject set_json;
    m_sample.writeJson(set_json);
    obj.insert(SET_DESCRIPTION, set_json);
}
