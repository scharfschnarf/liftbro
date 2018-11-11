#ifndef SETSTORAGE_H
#define SETSTORAGE_H

#include "jsonable.h"
#include <unordered_map>
#include <initializer_list>

constexpr unsigned int STORAGE_LABELS_LENGTH = 16;
constexpr static char SET_STORAGE_MEMBER_LABELS[][STORAGE_LABELS_LENGTH] = {
    "warmup_bit",
    "reps",
    "weight",
    "time",
    "distance"
    ""
};

/*
 * SetStorage
 * a class to store information about single set within a given exercise
 * The supported set information now are: reps, weight, distance and time
 * Note that all of these are unit-agnostic, so it's up for higher level to decide
 * whether for example weight is in kg or lbs
 *
 * is_warmup is to mark the set as warmup, because sometimes warmup sets have
 * specific settings, and will be displayed differently
 *
 * TODO: user-defined field types
 */

class SetStorage : public Jsonable
{
public:
    enum Member {
        warmup_bit, reps, weight, time, distance, MEMBERS_END
    };

    typedef long int FieldType;

    SetStorage() = default;
    SetStorage(const std::initializer_list<std::pair<Member, FieldType>> &);
    virtual ~SetStorage() = default;

    void set_field(Member which, FieldType value)
    {
        m_data[which] = value;
    }

    FieldType get_field(Member which) const
    {
        return m_data.at(which);
    }

    bool has_field(Member which) const
    {
        return (m_data.count(which) > 0);
    }

    virtual bool readJson(const QJsonObject &read_from);
    virtual void writeJson(QJsonObject &write_to) const;
protected:
    std::unordered_map<Member, FieldType> m_data;
};

#endif // SETSTORAGE_H
