#include "workoutdisplaycontroller.h"

WorkoutDisplayController::WorkoutDisplayController(QObject *parent, Workout *workout)
    : QAbstractItemModel(parent),
      m_workout_ptr(workout)
{
}

QVariant WorkoutDisplayController::headerData(int section, Qt::Orientation orientation, int) const
{
    unsigned s = static_cast<unsigned>(section);
    switch (orientation) {
    case Qt::Vertical:  // exercises
    {
        int max_section = static_cast<int>(m_workout_ptr->size());
        if ((section >= 0) && (section < max_section))
            return m_workout_ptr->get_exercise(s).get_name().c_str();
        break;
    }
    case Qt::Horizontal: // sets
        return ("Set " + std::to_string(section)).c_str();
    }
    return QVariant{};
}

QModelIndex WorkoutDisplayController::index(int row, int column, const QModelIndex &) const
{
    unsigned m_row = static_cast<unsigned>(row) + 1;
    unsigned m_col = static_cast<unsigned>(column) + 1;

    unsigned max_row = static_cast<unsigned>(m_workout_ptr->size() + 1);
    if ((m_row > 0) && (m_row < max_row)) {
        unsigned max_col = static_cast<unsigned>(m_workout_ptr->get_exercise(m_row).size() + 1);
        if ((m_col > 0) && (m_col < max_col))
            return createIndex(row, column);
    }

    return QModelIndex{};
}

QModelIndex WorkoutDisplayController::parent(const QModelIndex &) const
{
    return QModelIndex{};
}

int WorkoutDisplayController::rowCount(const QModelIndex &parent) const
{
    if (!parent.isValid())
        return 0;

    return static_cast<int>(m_workout_ptr->size());
}

int WorkoutDisplayController::columnCount(const QModelIndex &parent) const
{
    if (!parent.isValid())
        return 0;

    unsigned exercise_id = static_cast<unsigned>(parent.row()) + 1;
    return static_cast<int>(m_workout_ptr->get_exercise(exercise_id).size());
}

QVariant WorkoutDisplayController::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    auto exer_copy = m_workout_ptr->get_exercise(static_cast<unsigned>(index.row()) + 1);
    unsigned set_id = static_cast<unsigned>(index.column() + 1);

    SetStorage::Member r = toSetStorage(static_cast<DisplayRoles>(role));
    if (role != SetStorage::MEMBERS_END)
        return static_cast<unsigned>(exer_copy.get_field(set_id, r));

    return QVariant();
}

Qt::ItemFlags WorkoutDisplayController::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsEnabled | Qt::ItemIsEditable | Qt::ItemNeverHasChildren;
}

void WorkoutDisplayController::appendExercise()
{
    beginInsertRows(QModelIndex{},
                    static_cast<int>(m_workout_ptr->size() - 1),
                    static_cast<int>(m_workout_ptr->size()));
    m_workout_ptr->insert_exercise(Exercise{}, m_workout_ptr->size());
    endInsertRows();
}

void WorkoutDisplayController::deleteExercise(const QModelIndex &which)
{
    if ((which.row() >= 0) && (which.row() < static_cast<int>(m_workout_ptr->size()))) {
        beginRemoveRows(QModelIndex{},
                        static_cast<int>(which.row()), static_cast<int>(which.row() + 1));
        m_workout_ptr->delete_exercise(static_cast<std::size_t>(which.row() + 1));
        endRemoveRows();
    }
}

bool WorkoutDisplayController::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid())
        return false;

    auto exercise_id = static_cast<unsigned>(index.row() + 1);
    auto set_id      = static_cast<unsigned>(index.column() + 1);

    auto set_stor_mem = toSetStorage(static_cast<DisplayRoles>(role));
    if (set_stor_mem == SetStorage::MEMBERS_END)
        return false;

    bool ok = false;
    unsigned val = value.toUInt(&ok);
    if (!ok)
        return false;

    m_workout_ptr->get_exercise(exercise_id)
                  .get_set(set_id)
                  .set_field(set_stor_mem, val);
    dataChanged(index, index, {role});
    return true;
}

void WorkoutDisplayController::appendSet(const QModelIndex &index)
{
    if (!index.isValid())
        return;

    auto e_id = static_cast<unsigned>(index.row() + 1);
    auto s_id = static_cast<std::size_t>(index.column() + 1);

    SetStorage set;
    m_workout_ptr->get_exercise(e_id).insert_set(set, s_id);
    dataChanged(index, index);
}
