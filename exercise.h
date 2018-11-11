#ifndef EXERCISE_H
#define EXERCISE_H

#include <vector>
#include <initializer_list>
#include "setstorage.h"
#include "jsonable.h"

class Exercise : public Jsonable
{
public:
    Exercise(const std::initializer_list<SetStorage> &);

    // Explicitly default The Big Six
    Exercise()                            = default;
    Exercise(const Exercise &)            = default;
    Exercise(Exercise &&)                 = default;
    Exercise &operator=(const Exercise &) = default;
    Exercise &operator=(Exercise &&)      = default;
    virtual ~Exercise() noexcept          = default;

    virtual bool readJson(const QJsonObject &);
    virtual void writeJson(QJsonObject &) const;

    SetStorage get_set(unsigned int which) const
    {
        return m_setlist.at(which - 1);
    }

    SetStorage &operator[] (unsigned int which)
    {
        return m_setlist[which - 1];
    }

    std::size_t size() const
    {
        return m_setlist.size();
    }

    void insert_set(const SetStorage &, std::size_t dest_position);
    void delete_set(std::size_t dest_set);
protected:
    std::vector<SetStorage> m_setlist;
};

#endif // EXERCISE_H
