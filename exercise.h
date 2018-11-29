#ifndef EXERCISE_H
#define EXERCISE_H

#include <vector>
#include <initializer_list>
#include <string>
#include "setstorage.h"
#include "jsonable.h"

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

    SetStorage get_set(unsigned int which) const
    {
        return m_setlist.at(which - 1);
    }

    // Auxiliary function that prevents copy creation when single field from single member
    // needs to be extracted
    SetStorage::FieldType get_field(unsigned int which, SetStorage::Member member) const
    {
        return m_setlist.at(which - 1).get_field(member);
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

    std::string get_name() const
    {
        return m_name;
    }

    void set_name(const std::string &new_name)
    {
        m_name = new_name;
    }
protected:
    std::vector<SetStorage> m_setlist;
    std::string m_name;
};

#endif // EXERCISE_H
