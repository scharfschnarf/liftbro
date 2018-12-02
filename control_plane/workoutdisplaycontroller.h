#ifndef WORKOUTDISPLAYCONTROLLER_H
#define WORKOUTDISPLAYCONTROLLER_H

#include <QAbstractItemModel>
#include <QHash>
#include <QQmlEngine>
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
    Q_ENUM(DisplayRoles)

    explicit WorkoutDisplayController(QObject *parent = nullptr,
                                      Workout *workout_ptr = nullptr);

    static void declareQML() {
        qmlRegisterType<WorkoutDisplayController>("WDCData", 1, 0, "WDC");
    }

    // Add/delete/reorder exercises
    // TODO: templated exercise add
    Q_INVOKABLE void appendExercise();
    Q_INVOKABLE void deleteExercise(const QModelIndex &index);
    //TODO: Q_INVOKABLE void moveExercise(const QModelIndex &from, unsigned to);
    // Add/remove sets to exercise
    Q_INVOKABLE void appendSet(const QModelIndex &index);
    Q_INVOKABLE void deleteSet(const QModelIndex &index);

    // Simplified bindings for QML workaround
    // TODO: remove workaround
    //Q_INVOKABLE QVariant getParam(const QByteArray &what, int exercise, int set = 0);
    Q_INVOKABLE QVariant getParam(DisplayRoles role, int exercise, int set = 0) const;
    Q_INVOKABLE void setParam(const QVariant &newValue, DisplayRoles role, int exercise, int set = 0);
protected:
    // QT Model interface
    // There's no need to expose it anymore
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

#endif // WORKOUTDISPLAYCONTROLLER_H
