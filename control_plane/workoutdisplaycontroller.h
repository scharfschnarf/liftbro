#ifndef WORKOUTDISPLAYCONTROLLER_H
#define WORKOUTDISPLAYCONTROLLER_H

#include <QAbstractItemModel>
#include <QHash>
#include "workout.h"
#include "setstorage.h"

class WorkoutDisplayController : public QAbstractItemModel
{
    Q_OBJECT
public:
    enum DisplayRoles {
        WarmupFlag = Qt::UserRole + 1,
        Reps,
        Weight,
        Time,
        Distance,
        Name,
        INVALID_ROLE
    };
    explicit WorkoutDisplayController(QObject *parent = nullptr,
                                      Workout *workout_ptr = nullptr);

    // Header:
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    // Basic functionality:
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;

    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    // Add/delete/reorder exercises
    // TODO: templated exercise add
    Q_INVOKABLE void appendExercise();
    Q_INVOKABLE void deleteExercise(const QModelIndex &index);
    //TODO: Q_INVOKABLE void moveExercise(const QModelIndex &from, unsigned to);
    // Add/remove sets to exercise
    Q_INVOKABLE void appendSet(const QModelIndex &index);
    Q_INVOKABLE void deleteSet(const QModelIndex &index);

    inline QHash<int, QByteArray> roleNames() const override;

    // Simplified bindings for QML workaround
    // TODO: remove workaround
    Q_INVOKABLE QVariant getParam(const QByteArray &what, int exercise, int set = 0)
    {
        DisplayRoles role;
        if (what == "name")
            role = Name;
        else if (what == "warmupFlag")
            role = WarmupFlag;
        else if (what == "reps")
            role = Reps;
        else if (what == "weight")
            role = Weight;
        else if (what == "distance")
            role = Distance;
        else if (what == "time")
            role = Time;
        else
            return QVariant{};

        return data(index(exercise, set), role);
    }
private:
    inline SetStorage::Member toSetStorage(DisplayRoles) const;
    inline DisplayRoles toDisplayRole(SetStorage::Member) const;
    // TODO: weak_ptr here
    Workout *m_workout_ptr;
};

// Inline member f-ns will follow --------------------

SetStorage::Member WorkoutDisplayController::toSetStorage(DisplayRoles role) const
{
    switch (role) {
    case WarmupFlag:
        return SetStorage::warmup_bit;
    case Reps:
        return SetStorage::reps;
    case Weight:
        return SetStorage::weight;
    case Time:
        return SetStorage::time;
    case Distance:
        return SetStorage::distance;
    default:
        return SetStorage::MEMBERS_END;
    }
}

WorkoutDisplayController::DisplayRoles WorkoutDisplayController::toDisplayRole(SetStorage::Member m) const
{
    switch(m) {
    case SetStorage::warmup_bit:
        return WarmupFlag;
    case SetStorage::reps:
        return Reps;
    case SetStorage::weight:
        return Weight;
    case SetStorage::distance:
        return Distance;
    case SetStorage::time:
        return Time;
    default:
        return INVALID_ROLE;
    }
}

QHash<int, QByteArray> WorkoutDisplayController::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[WarmupFlag] = "warmupFlag";
    roles[Reps] = "reps";
    roles[Weight] = "weight";
    roles[Distance] = "distance";
    roles[Time] = "time";
    roles[Name] = "name";
    return roles;
}

#endif // WORKOUTDISPLAYCONTROLLER_H
