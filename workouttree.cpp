#include "workouttree.h"
#include <stdexcept>
#include <string>
#include <algorithm>
#include <QJsonDocument>
#include <QJsonArray>

// Auxiliary function object to sort on WorkoutTree elements
struct PtrWorkoutLess
{
    bool operator() (const std::shared_ptr<Workout> lhs,
                     const std::shared_ptr<Workout> rhs) const
    {
        return (lhs->get_finish_time() < rhs->get_finish_time());
    }
};

WorkoutTree::~WorkoutTree() noexcept
{
    writeFile();
}

Workout & WorkoutTree::iterator::operator* ()
{
    // Ensure load_obj has been executed
    m_ptr->loadObj(*this);
    return **m_underlying;
}

WorkoutTree::WorkoutSharedPtr WorkoutTree::iterator::operator-> ()
{
    m_ptr->loadObj(*this);
    return *m_underlying;
}

void WorkoutTree::ensureSorted()
{
    if (std::is_sorted(std::begin(m_workouts), std::end(m_workouts), PtrWorkoutLess{}))
        return;
    sort();
}

void WorkoutTree::sort()
{
    std::sort(std::begin(m_workouts), std::end(m_workouts), PtrWorkoutLess{});
}

bool WorkoutTree::isObjLoaded(iterator) const
{
    return true;
}

void WorkoutTree::loadObj(iterator)
{
    // no-op, TODO: lazy loading
}

WorkoutTree::WorkoutTree(const char *filename):
    m_workouts{},
    m_stor_file{filename}
{
}

bool WorkoutTree::readFile()
{
    // Ensure file is open
    bool state = m_stor_file.open(QIODevice::ReadOnly);
    if (!state)
        return false;

    QByteArray fileContent = m_stor_file.readAll();
    if (fileContent.isEmpty())
        // Don't validate, jump out, there's just nothing in it
        return true;

    // Don't waste file descriptors
    m_stor_file.close();

    QJsonDocument doc = QJsonDocument::fromJson(fileContent);
    if (!doc.isObject())
        return false;

    QJsonObject docObj = doc.object();
    QJsonArray workoutArray = docObj.find("workoutArray")->toArray();
    for (auto it = workoutArray.begin(); it != workoutArray.end(); ++it) {
        QJsonObject obj = it->toObject();
        std::shared_ptr<Workout> newWorkout{new Workout};
        newWorkout->readJson(obj);
        m_workouts.emplace_back(std::move(newWorkout));
    }
    ensureSorted();
    return true;
}

bool WorkoutTree::writeFile()
{
    QJsonArray workoutArray;
    if (!m_workouts.empty()) {
        ensureSorted();
        for (auto it = m_workouts.begin(); it != m_workouts.end(); ++it) {
            QJsonObject obj;
            (*it)->writeJson(obj);
            workoutArray.push_back(obj);
        }
    }

    QJsonObject docObj;
    docObj.insert("workoutArray", workoutArray);

    QJsonDocument doc{docObj};

    if (m_stor_file.isOpen())
        m_stor_file.close();

    // Truncate file if JSON is valid
    bool state = m_stor_file.open(QIODevice::ReadWrite | QIODevice::Truncate);
    if (!state)
        return false;

    state = m_stor_file.write(doc.toJson(QJsonDocument::Compact));
    if (!state)
        return false;

    // Reopen file, read only
    m_stor_file.close();
    m_stor_file.open(QIODevice::ReadOnly);
    return true;
}

std::size_t WorkoutTree::size() const
{
    return m_workouts.size();
}

WorkoutTree::iterator WorkoutTree::begin()
{
    ensureSorted();
    return iterator{this, m_workouts.begin()};
}

WorkoutTree::iterator WorkoutTree::end()
{
    ensureSorted();
    return iterator{this, m_workouts.end()};
}

WorkoutTree::iterator WorkoutTree::beginInRange(Timestamp range_begin, Timestamp)
{
    ensureSorted();
    auto workout_tpl = std::make_shared<Workout>(range_begin);
    return iterator{this, std::lower_bound(m_workouts.begin(), m_workouts.end(),
                                           workout_tpl, PtrWorkoutLess{})};
}

WorkoutTree::iterator WorkoutTree::endInRange(Timestamp, Timestamp range_end)
{
    ensureSorted();
    auto workout_tpl = std::make_shared<Workout>(range_end);
    return iterator{this, std::upper_bound(m_workouts.begin(), m_workouts.end(),
                                           workout_tpl, PtrWorkoutLess{})};
}

WorkoutTree::iterator WorkoutTree::addWorkout(const Workout &toAdd)
{
    m_workouts.push_back(std::make_shared<Workout>(toAdd));
    sort();
    auto workout_tpl = std::make_shared<Workout>(toAdd.get_finish_time());
    return iterator{this, std::lower_bound(m_workouts.begin(), m_workouts.end(),
                                           workout_tpl, PtrWorkoutLess{})};
}

void WorkoutTree::deleteWorkout(iterator it)
{
    m_workouts.erase(it.m_underlying);
    ensureSorted();
}

#ifdef LAZY_LOAD_ENABLED
void WorkoutTree::load_obj(iterator it)
{
    if (is_obj_loaded(it))
        return;

    // Access underlying element and perform same calculations as in writeJson
    auto timestamp_to_find = (*(it.m_underlying))->get_finish_time();
    long time_since_epoch = timestamp_to_find.time_since_epoch().count();

    const std::string label_first_part  = "timestamp",
                      label_second_part = std::to_string(static_cast<double>(time_since_epoch));

    while (!m_stor_file.atEnd()) {
        QByteArray line_buf = m_stor_file.readLine();
        // UNFINISHED!!!
    }
}
#endif // LAZY_LOAD_ENABLED
