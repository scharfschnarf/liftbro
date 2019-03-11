#include <QHBoxLayout>

#include "mainwindow.h"
#include <iostream>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    setupWidgets();

}

void MainWindow::setupWidgets()
{
    m_workoutTree = std::make_shared<WorkoutTree>();
    m_workoutTree->readFile();

    std::cout << "Added workoutTree with " << m_workoutTree->size() << " elements" << std::endl;
    m_listModel = new WorkoutList{this, m_workoutTree};
    std::cout << "Added listModel with " << m_listModel->rowCount() << " elements" << std::endl;

    QFrame *frame = new QFrame;
    QHBoxLayout *layout = new QHBoxLayout{frame};

    m_listView = new QListView;
    m_listView->setViewMode(QListView::ListMode);
    m_listView->setModel(m_listModel);
    layout->addWidget(m_listView);
    setCentralWidget(frame);
}

MainWindow::~MainWindow()
{

}
