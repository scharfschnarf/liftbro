#ifndef WORKOUTTREE_H
#define WORKOUTTREE_H

#include <memory>
#include <vector>
#include <QFile>
#include <iterator>
#include "workout.h"

/* WorkoutTree
 * WorkoutTree packs entire save file into single entity/database
 * that has ownership of all workouts, workout templates
 * TODO: It supports lazy loading so that reading data from file is done
 * on demand
 */

constexpr char FILE_SAVE_PATH[] = "liftbro_saves.json";

class WorkoutTree
{
private:
    typedef std::shared_ptr<Workout> WorkoutSharedPtr;
public:
    class iterator
    {
    private:
        typedef typename std::vector<WorkoutSharedPtr>::iterator UnderlyingIterator;
        friend class WorkoutTree;
    public:
        // STD definitions follow
        typedef std::bidirectional_iterator_tag iterator_category;
        typedef Workout    value_type;
        typedef Workout&   reference;
        //typedef WorkoutPtr pointer;
        typedef std::size_t difference_type;

        iterator operator++()    {++m_underlying; return *this;}
        iterator operator--()    {--m_underlying; return *this;}
        iterator operator++(int) {iterator cur = *this; ++m_underlying; return cur;}
        iterator operator--(int) {iterator cur = *this; --m_underlying; return cur;}

        Workout &operator*();
        WorkoutSharedPtr operator->();

        bool operator== (const iterator &rhs) {return m_underlying == rhs.m_underlying;}
        bool operator!= (const iterator &rhs) {return !(*this == rhs);}
    private:
        iterator(WorkoutTree *ptr, UnderlyingIterator it):
            m_underlying{it}, m_ptr{ptr}
        {}

        UnderlyingIterator m_underlying;
        WorkoutTree *m_ptr;
    };

    // Delete copiers (heavy database object)
    WorkoutTree (const WorkoutTree &)            = delete;
    WorkoutTree &operator= (const WorkoutTree &) = delete;

    explicit WorkoutTree(const char fileName[] = FILE_SAVE_PATH);
    bool readFile();
    bool writeFile();

    // Access workouts
    // All functions may call sort() if the vector elements count is unsorted
    // TODO: is const-correctness possible here?
    // WARNING: if object's timestamp is modified through iterators,
    //  the values inside may remain unsorted for some time
    std::size_t size() const;
    iterator begin();
    iterator end();
    iterator beginInRange(Timestamp range_begin, Timestamp range_end);
    iterator endInRange(Timestamp range_begin, Timestamp range_end);

    // Modifiers
    // All functions here call sort()
    iterator addWorkout(const Workout &toAdd = Workout{});
    void deleteWorkout(iterator);
private:
    void ensureSorted();
    void sort();

    // Internal, auxiliary functions
    // Functions handling lazy loading
    // TODO: Lazy loading
    bool isObjLoaded(iterator /*it*/) const;
    void loadObj(iterator);

    std::vector<WorkoutSharedPtr> m_workouts;
    QFile m_stor_file;
#ifdef LAZY_LOAD_ENABLED
    // Helpers with read and write
    void load_whole_file();
    void load_headlines();
#endif // LAZY_LOAD_ENABLED
};

#endif // WORKOUTTREE_H
