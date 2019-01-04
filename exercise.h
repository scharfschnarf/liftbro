#ifndef EXERCISE_H
#define EXERCISE_H

#include <vector>
#include <initializer_list>
#include <string>
#include "setstorage.h"
#include "jsonable.h"

struct ExerciseMetaFields
{
    bool goodWorkout;
};

class Exercise : public Jsonable
{
public:
    Exercise(const std::string &name, const std::initializer_list<SetStorage> &);

    // Explicitly default The Big Six
    Exercise()                            = default;
    Exercise(const Exercise &)            = default;
    Exercise(Exercise &&)                 = default;
    Exercise &operator=(const Exercise &) = default;
    Exercise &operator=(Exercise &&)      = default;
    virtual ~Exercise() noexcept          = default;

    virtual bool readJson(const QJsonObject &);
    virtual void writeJson(QJsonObject &) const;

    SetStorage get_set(std::size_t which) const
    {
        return m_setlist.at(which);
    }

    SetStorage &get_ref(std::size_t which)
    {
        return m_setlist.at(which);
    }

    // Auxiliary functions that prevent copy creation when single field from single member
    // needs to be extracted
    SetStorage::FieldType get_field(std::size_t which, SetStorage::Member member) const
    {
        return m_setlist.at(which).get_field(member);
    }

    void set_field(std::size_t which, SetStorage::Member member, SetStorage::FieldType value)
    {
        m_setlist.at(which).set_field(member, value);
    }

    SetStorage &operator[] (unsigned int which)
    {
        return m_setlist[which];
    }

    std::size_t size() const
    {
        return m_setlist.size();
    }

    void insert_set(const SetStorage &, std::size_t dest_position);
    void delete_set(std::size_t dest_set);

    std::string get_name() const
    {
        return m_name;
    }

    void set_name(const std::string &new_name)
    {
        m_name = new_name;
    }

    ExerciseMetaFields generateEmf() const;
protected:
    std::vector<SetStorage> m_setlist;
    std::string m_name;
};

#endif // EXERCISE_H
