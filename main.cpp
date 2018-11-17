#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QJsonDocument>
#include <QJsonArray>
#include <QFile>
#include <QByteArray>

#include "workout.h"
#include "workouttree.h"
#include "exercisespawner.h"
#include <iostream>
#include <string>
#include <iomanip>
#include <ctime>
#include <vector>

using std::cout;
using std::endl;

int main(int, char * [])
{
    WorkoutTree workouts;
    workouts.readFile();

    cout << "Workouts file has " << workouts.size() << "entries" << endl;
    Workout w{
        Exercise {
            SetStorage{{SetStorage::reps, 3}, {SetStorage::weight, 40}, {SetStorage::warmup_bit, true}},
            SetStorage{{SetStorage::reps, 5}, {SetStorage::weight, 60}, {SetStorage::warmup_bit, false}}
        },
        Exercise {
            SetStorage{{SetStorage::reps, 4}, {SetStorage::warmup_bit, true}},
            SetStorage{{SetStorage::reps, 10}, {SetStorage::warmup_bit, false}},
            SetStorage{{SetStorage::reps, 20}, {SetStorage::warmup_bit, false}}
        }
    };
    workouts.addWorkout(w);

    cout << "Listing contents of WorkoutTree: " << endl;
    for (WorkoutTree::iterator it = workouts.begin(); it != workouts.end(); ++it) {
        std::time_t tst = std::chrono::system_clock::to_time_t(it->get_finish_time());
        cout << "\t Workout" << endl
             << "\t\t Timestamp: " << std::put_time(std::localtime(&tst), "%F %T") << endl
             << "\t\t Exercises: " << endl;
        for (unsigned i = 1; i <= it->size(); ++i) {
            Exercise ex = it->get_exercise(i);
            cout << "\t\t\t Exercise " << i << endl
                 << "\t\t\t Sets: " << endl;
            for (unsigned j = 1; j <= ex.size(); ++j) {
                SetStorage set = ex.get_set(j);
                if (set.has_field(SetStorage::warmup_bit))
                    cout << "\t\t\t\t Warmup bit: " << set.get_field(SetStorage::warmup_bit) << endl;
                if (set.has_field(SetStorage::reps))
                    cout << "\t\t\t\t Repetitions: " << set.get_field(SetStorage::reps) << endl;
                if (set.has_field(SetStorage::weight))
                    cout << "\t\t\t\t Weight: " << set.get_field(SetStorage::weight) << endl;
                cout << "----" << endl;
            }
        }
    }

    workouts.writeFile();

#ifdef TEST_EXERCISE_SPAWNER
    Exercise e{
        SetStorage{{SetStorage::reps, 3}, {SetStorage::weight, 40}, {SetStorage::warmup_bit, true}},
        SetStorage{{SetStorage::reps, 5}, {SetStorage::weight, 60}, {SetStorage::warmup_bit, false}}
    };

    cout << "e size: " << e.size() << endl;
    ExerciseSpawner espawn{e};
    Exercise e2 = espawn.make_from_template();
    cout << "e2 size: " << e2.size() << endl
         << "e2, first set reps: " << e2.get_set(1).get_field(SetStorage::reps)
         << " and second: "        << e2.get_set(2).get_field(SetStorage::reps) << endl;
#endif
#if 0
    // Part 1. Test creating 5 different workouts
    std::vector<Workout> work_vec;
    for (int i = 0; i < 5; ++i) {
        Workout w;
        // Insert 5 exercises into each workout
        for (int j = 0; j < 5; ++j) {
            Exercise exer;
            // Insert j + 3 sets into each exercise
            for (int k = 0; k < (j + 3); ++k) {
                SetStorage s;
                s.set_field(SetStorage::warmup_bit,
                            (k < 2) ? 1 : 0);
                s.set_field(SetStorage::reps,
                            k + 3);
                if ((j % 2) == 0) {
                    s.set_field(SetStorage::weight,
                                k * 10 + j);
                } else {
                    s.set_field(SetStorage::time,
                                k * 5 + j);
                }
                exer.insert_set(s, k);
            }
            w.insert_exercise(exer, j);
        }
        w.mark_complete();
        work_vec.push_back(w);
    }

    // Part 2. Write json
    QFile file{"saves.json"};
    if (!file.open(QIODevice::ReadWrite)) {
        std::cout << "Error opening save file\n";
        return -1;
    }

    QJsonArray workout_arr;
    for (const auto &workout: work_vec) {
        QJsonObject work2json;
        workout.writeJson(work2json);
        workout_arr.push_back(work2json);
    }

    QJsonObject workouts_cont;
    workouts_cont.insert("workouts", workout_arr);
    QJsonDocument doc{workouts_cont};
    file.write(doc.toJson());

    QByteArray qba = doc.toJson();
    for (const auto &elem: qba) {
        std::cout << elem;
    }
    /*
    QByteArray saveData = file.readAll();

    std::cout << "JSON document read, parsing..." << std::endl;
    QJsonDocument doc_structure{QJsonDocument::fromJson(saveData)};


    if (doc_structure.isNull())
        std::cout << "parsing error" << std::endl;

    QJsonArray workout_array = doc_structure["workouts"].toArray();
    std::vector<Workout> workout_vec;
    for (auto it = workout_array.begin(); it != workout_array.end(); ++it) {
        Workout w;
        w.readJson(it->toObject());
        workout_vec.push_back(w);
    }

    std::cout << "JSON parsed, results: " << std::endl;
    for (unsigned i = 0; i < workout_vec.size(); ++i) {
        std::time_t finish_time = std::chrono::system_clock::to_time_t(workout_vec[i].get_finish_time());
        std::cout << "Workout #" << i << std::endl
                  << "\tTime: " << std::put_time(std::localtime(&finish_time), "%F %T") << std::endl
                  << "\tExercises: " << std::endl;
        for (unsigned j = 1; j <= workout_vec[i].size(); ++j) {
            std::cout << "\t\t sets_count: " << workout_vec[i][j].size() << std::endl
                      << "\t\t sets: " << std::endl;
            for (unsigned k = 1; k <= workout_vec[i][j].size(); ++k) {
                std::cout << "\t\t set " << k << std::endl;
                for (unsigned label_it = 0; label_it < static_cast<unsigned>(SetStorage::MEMBERS_END); ++label_it) {
                    unsigned int field;
                    if (workout_vec[i][j][k].get_field(static_cast<SetStorage::Member>(label_it), field)) {
                        std::cout << "\t\t\t" << SET_STORAGE_MEMBER_LABELS[label_it] << field << std::endl;
                    }
                }
            }
        }
    }*/

    /*
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
    */
#endif
}
