#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <memory>
#include <QMainWindow>
#include <QListView>
#include "../workouttree.h"
#include "../control_plane/workoutlist.h"


class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    virtual ~MainWindow();
signals:

public slots:

private:
    void setupWidgets();

    std::shared_ptr<WorkoutTree> m_workoutTree;
    WorkoutList *m_listModel;
    QListView *m_listView;
};

#endif // MAINWINDOW_H
