#ifndef EXERCISE_H
#define EXERCISE_H

#include <vector>
#include <initializer_list>
#include "setstorage.h"
#include "jsonable.h"

class Exercise : public Jsonable
{
public:
    Exercise() = default;
    Exercise(const std::initializer_list<SetStorage> &);
    virtual ~Exercise() noexcept = default;

    virtual bool readJson(const QJsonObject &);
    virtual void writeJson(QJsonObject &) const;

    SetStorage get_set(unsigned int which) const
    {
        return m_setlist.at(which - 1);
    }

    std::size_t size() const
    {
        return m_setlist.size();
    }

    SetStorage &operator[] (unsigned int which)
    {
        return m_setlist[which - 1];
    }

    void insert_set(const SetStorage &, unsigned int dest_position);
    void delete_set(unsigned int dest_set);
protected:
    std::vector<SetStorage> m_setlist;
};

#endif // EXERCISE_H
