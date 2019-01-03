#include "workouttree.h"
#include <stdexcept>
#include <string>
#include <algorithm>
#include <QJsonDocument>
#include <QJsonArray>

WorkoutTree::WorkoutTree(const char *filename):
    FileTree<Workout, PtrWorkoutLess>{filename}
{
}

WorkoutTree::iterator WorkoutTree::beginInRange(Timestamp range_begin, Timestamp)
{
    ensureSorted();
    auto workout_tpl = std::make_shared<Workout>(range_begin);
    return makeIterator(std::lower_bound(m_members.begin(), m_members.end(),
                                         workout_tpl, PtrWorkoutLess{}));
}

WorkoutTree::iterator WorkoutTree::endInRange(Timestamp, Timestamp range_end)
{
    ensureSorted();
    auto workout_tpl = std::make_shared<Workout>(range_end);
    return makeIterator(std::upper_bound(m_members.begin(), m_members.end(),
                                         workout_tpl, PtrWorkoutLess{}));
}
