#ifndef WORKOUTTREE_H
#define WORKOUTTREE_H

#include <memory>
#include <vector>
#include <QFile>
#include <iterator>
#include "workout.h"
#include "filetree.h"

/* WorkoutTree
 * WorkoutTree packs entire save file into single entity/database
 * that has ownership of all workouts, workout templates
 * TODO: It supports lazy loading so that reading data from file is done
 * on demand
 */

constexpr char FILE_SAVE_PATH[] = "liftbro_saves.json";

// Auxiliary function object to sort on WorkoutTree elements
struct PtrWorkoutLess
{
    bool operator() (const std::shared_ptr<Workout> lhs,
                     const std::shared_ptr<Workout> rhs) const
    {
        return (lhs->get_finish_time() < rhs->get_finish_time());
    }
};

class WorkoutTree: public FileTree<Workout, PtrWorkoutLess>
{
public:
    // Delete copiers (heavy database object)
    WorkoutTree (const WorkoutTree &)            = delete;
    WorkoutTree &operator= (const WorkoutTree &) = delete;

    explicit WorkoutTree(const char fileName[] = FILE_SAVE_PATH);
    virtual ~WorkoutTree() noexcept = default;

    // Access workouts
    // All functions may call sort() if the vector elements count is unsorted
    // TODO: is const-correctness possible here?
    // WARNING: if object's timestamp is modified through iterators,
    //  the values inside may remain unsorted for some time
    iterator beginInRange(Timestamp range_begin, Timestamp range_end);
    iterator endInRange(Timestamp range_begin, Timestamp range_end);

    // Backward compability of interface
    iterator addWorkout(const Workout &toAdd = Workout{})
        {return addElem(toAdd);}
    void deleteWorkout(iterator it)
        {deleteElem(it);}
};

#endif // WORKOUTTREE_H
