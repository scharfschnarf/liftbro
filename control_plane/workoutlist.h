#ifndef WORKOUTLIST_H
#define WORKOUTLIST_H

#include <QAbstractListModel>
#include <QHash>
#include <memory>
#include "workouttree.h"

class WorkoutList : public QAbstractListModel
{
    Q_OBJECT

public:
    enum WorkoutListRoles {
        DateTimeRole = Qt::UserRole + 1
    };
    explicit WorkoutList(QObject *parent = nullptr,
                         std::shared_ptr<WorkoutTree> modelParent = nullptr);
    ~WorkoutList() noexcept override;
    // Header:
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    QHash<int, QByteArray> roleNames() const override;
private:
    std::shared_ptr<WorkoutTree> m_wt;
};



#endif // WORKOUTLIST_H
