#ifndef WORKOUTLIST_H
#define WORKOUTLIST_H

#include <QAbstractListModel>

class WorkoutList : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit WorkoutList(QObject *parent = nullptr);

    // Header:
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

private:
};

#endif // WORKOUTLIST_H