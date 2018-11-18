#include "workoutlist.h"

WorkoutList::WorkoutList(QObject *parent,
                         std::shared_ptr<WorkoutTree> modelParent)
    : QAbstractListModel(parent),
      m_wt{modelParent}
{
}

QVariant WorkoutList::headerData(int, Qt::Orientation orientation, int role) const
{
    if ((role != Qt::DisplayRole) && (role != DateTimeRole))
        return QVariant{};

    if (orientation != Qt::Orientation::Horizontal)
        return QVariant{};

    return QString{"Workouts"};
}

int WorkoutList::rowCount(const QModelIndex &parent) const
{
    // For list models only the root node (an invalid parent) should return the list's size. For all
    // other (valid) parents, rowCount() should return 0 so that it does not become a tree model.
    if (parent.isValid())
        return 0;

    return static_cast<int>(m_wt->size());
}

QVariant WorkoutList::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if ((role != Qt::DisplayRole) && (role != DateTimeRole))
        return QVariant{};

    // Go backwards, so that we don't pre-load historical workouts every time we try to fetch a list
    auto it = m_wt->end();
    for (auto decrement = static_cast<unsigned>(m_wt->size()) - index.row(); decrement > 0; --decrement)
        --it;

    return QVariant(it->get_finish_time());
}

QHash<int, QByteArray> WorkoutList::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[DateTimeRole] = "dateTime";
    return roles;
}

WorkoutList::~WorkoutList() noexcept
{
    m_wt->writeFile();
}
