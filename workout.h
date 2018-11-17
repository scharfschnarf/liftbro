#ifndef WORKOUT_H
#define WORKOUT_H

#include "jsonable.h"
#include "exercise.h"

#include <vector>
#include <chrono>
#include <initializer_list>

typedef std::chrono::time_point<std::chrono::system_clock> Timestamp;

// TODO: lazy loading

class Workout : public Jsonable
{
public:
    explicit Workout(Timestamp now = std::chrono::system_clock::now()):
        m_exerlist{},
        m_finish_timestamp{now}
       // m_loaded{false}
    {
    }

    virtual ~Workout() noexcept = default;

    Workout(const std::initializer_list<Exercise> &,
            Timestamp now = std::chrono::system_clock::now());

    virtual bool readJson(const QJsonObject &);
    virtual void writeJson(QJsonObject &) const;

    Timestamp get_finish_time() const
    {
        return m_finish_timestamp;
    }

    void mark_complete(Timestamp when = std::chrono::system_clock::now())
    {
        m_finish_timestamp = when;
       // m_loaded = true;
    }

    std::size_t size() const
    {
        return m_exerlist.size();
    }

    Exercise get_exercise(unsigned int which) const
    {
        return m_exerlist.at(which - 1);
    }

    Exercise &operator[] (unsigned int which)
    {
        return m_exerlist[which - 1];
    }

    void insert_exercise(const Exercise &, unsigned int dest_pos);
    void delete_exercise(unsigned int dest_pos);

    bool is_populated()
    {
        return true;
       // return m_loaded;
    }
protected:
    std::vector<Exercise> m_exerlist;
    Timestamp m_finish_timestamp;
    // TODO: lazy loading
    // bool m_loaded;                     // field for lazy load
};

#endif // WORKOUT_H
