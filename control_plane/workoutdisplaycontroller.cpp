#include "workoutdisplaycontroller.h"
#include <iostream>

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
            return (*m_workout_ptr)[s].get_name().c_str();
        break;
    }
    case Qt::Horizontal: // sets
        return ("Set " + std::to_string(section)).c_str();
    }
    return QVariant{};
}

QModelIndex WorkoutDisplayController::index(int row, int column, const QModelIndex &) const
{
    unsigned m_row = static_cast<unsigned>(row);
    unsigned m_col = static_cast<unsigned>(column);

    std::cerr << "## in WDC::index(), row (exercise) " << m_row << ", column (set) " << m_col << std::endl;

    unsigned max_row = static_cast<unsigned>(m_workout_ptr->size());
    if (m_row < max_row) {
        unsigned max_col = static_cast<unsigned>((*m_workout_ptr)[m_row].size());
        if (m_col < max_col)
            return createIndex(row, column);
    }
    std::cerr << "## in WDC::index(): INVALID INDEX" << std::endl;
    return QModelIndex{};
}

QModelIndex WorkoutDisplayController::parent(const QModelIndex &) const
{
    return QModelIndex{};
}

int WorkoutDisplayController::rowCount(const QModelIndex &parent) const
{
    //if (!parent.isValid())
    //    return 0;

    auto rv = static_cast<int>(m_workout_ptr->size());
    std::cerr << "in WDC rowCount, returning " << rv << std::endl;
    return rv;
}

int WorkoutDisplayController::columnCount(const QModelIndex &parent) const
{
    //if (!parent.isValid())
    //    return 0;

    unsigned exercise_id = static_cast<unsigned>(parent.row());
    auto rv = static_cast<int>((*m_workout_ptr)[exercise_id].size());
    std::cerr << "in WDC columnCount, returning " << rv << std::endl;
    return rv;
}

QVariant WorkoutDisplayController::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        std::cerr << "in WDC data, index (" << index.row() << "," << index.column() << ")invalid, ending!" << std::endl;
        return QVariant();
    }

    auto exer_copy = m_workout_ptr->get_exercise(static_cast<unsigned>(index.row()));
    unsigned set_id = static_cast<unsigned>(index.column());

    //std::cerr << "in WDC data, set_id = " << set_id << std::endl;

    if (role == Name) {
        QString rv{exer_copy.get_name().c_str()};
      //  std::cerr << "in WDC data, returning Name = " << rv.toStdString() << std::endl;
        return rv;
    }

    SetStorage::Member r = toSetStorage(static_cast<DisplayRoles>(role));
    if (role != SetStorage::MEMBERS_END) {
        // Check if the role first exists before calling it
        if (exer_copy.get_set(set_id).has_field(r)) {
            auto rv = static_cast<unsigned>(exer_copy.get_field(set_id, r));
           // std::cerr << "in WDC data, returning set field " << set_id
             //         << " = " << rv << std::endl;
            return rv;
        } else
            return QVariant{};
    }

    std::cerr << "in WDC data, role invalid, ending!" << std::endl;
    return QVariant();
}

QVariant WorkoutDisplayController::getParam(DisplayRoles role, int exercise, int set) const
{
    std::cerr << "### WDC getParam: displayRole = " << role << ", from exercise = " << exercise << ", set = " << set << std::endl;
    QVariant rv = data(index(exercise, set), role);

    // Two checks
    // If it was parameter from SetStorage, modify return value
    // to 0 in case of WarmupFlag as default walue
    // to -1 in other cases
    if (rv == QVariant{}) {
        switch(role) {
        case WarmupFlag:
            return false;
        case Reps:
        case Weight:
        case Distance:
        case Time:
        case PlannedReps:
        case PlannedTime:
        case PlannedWeight:
        case PlannedDistance:
            return -1;
        default:
            return QVariant{};
        }
    }

    return rv;
}

void WorkoutDisplayController::setParam(const QVariant &newValue, DisplayRoles role, int exercise, int set)
{
    switch (role) {
    case Reps:
    case Weight:
    case Distance:
    case Time:
        if (newValue == -1) {
            auto ind = index(exercise, set);
            if (!ind.isValid())
                return;

            (*m_workout_ptr)[exercise].get_ref(set).remove_field(toSetStorage(role));
            dataChanged(ind, ind, {role});
            break;
        }
        [[clang::fallthrough]]; // fallthrough to set data if that's not the case
    default:
        setData(index(exercise, set), newValue, role);
    }
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
        beginRemoveRows(which.parent(),
                        static_cast<int>(which.row()), static_cast<int>(which.row() + 1));
        m_workout_ptr->delete_exercise(static_cast<std::size_t>(which.row() + 1));
        endRemoveRows();
    }
}

bool WorkoutDisplayController::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid())
        return false;

    auto exercise_id = static_cast<unsigned>(index.row());
    if (role == Name) {
        (*m_workout_ptr)[exercise_id].set_name(value.toString().toStdString());
    } else {
        auto set_id      = static_cast<unsigned>(index.column());

        auto set_stor_mem = toSetStorage(static_cast<DisplayRoles>(role));
        if (set_stor_mem == SetStorage::MEMBERS_END)
            return false;

        bool ok = false;
        unsigned val = value.toUInt(&ok);
        if (!ok)
            return false;

        (*m_workout_ptr)[exercise_id].set_field(set_id, set_stor_mem, val);
    }
    dataChanged(index, index, {role});
    return true;
}

void WorkoutDisplayController::appendSet(const QModelIndex &index)
{
    if (!index.isValid())
        return;

    auto e_id = static_cast<unsigned>(index.row());
    auto s_id = static_cast<std::size_t>(index.column());

    beginInsertColumns(index.parent(), s_id, s_id + 1);
    SetStorage set;
    m_workout_ptr->get_exercise(e_id).insert_set(set, s_id);
    endInsertColumns();
}

void WorkoutDisplayController::deleteSet(int row, int set)
{
    auto ind = index(row, set);
    if (!ind.isValid())
        return;

    auto e_id = static_cast<unsigned>(ind.row());
    auto s_id = static_cast<std::size_t>(ind.column());
    beginRemoveColumns(ind.parent(), s_id, s_id + 1);
    (*m_workout_ptr)[e_id].delete_set(s_id);
    endRemoveColumns();
}

