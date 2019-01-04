#ifndef TAGS_SETSTORAGE_H
#define TAGS_SETSTORAGE_H

#include "stenciltag.h"

extern AYT::AYT<StencilTag<SetStorage>> SetStorageStencilAYT;

enum SetStorageTags
{
    AddEveryWorkout,
    SetEveryWorkout,
    SetGoodBit,
    SET_STORAGE_TAGS_END
};
extern AYT::AYT<StencilTag<SetStorage>>::Add SetStorageStencilAYTHandles[SET_STORAGE_TAGS_END];

class AddEveryWorkout: public StencilTag<SetStorage>
{
public:
    AddEveryWorkout(SetStorage::Member    where = SetStorage::MEMBERS_END,
                    SetStorage::FieldType what  = 0):
        m_where{where},
        m_what {what}
    {
    }

    static StencilTag<SetStorage> *createIf(std::size_t val)
    {
        if (val == SetStorageTags::AddEveryWorkout)
            return static_cast<StencilTag<SetStorage> *>(new AddEveryWorkout);
        else
            return nullptr;
    }

    const char *getTagName() const override
    {
        return "AddEveryWorkout";
    }

    void execute(SetStorage &s) const override
    {
        auto val = s.get_field(m_where) + m_what;
        s.set_field(m_where, val);
    }

    bool readJson(const QJsonObject &json)    override;
    void writeJson(QJsonObject &target) const override;
private:
    SetStorage::Member m_where;
    SetStorage::FieldType m_what;
};

class SetEveryWorkout: public StencilTag<SetStorage>
{
public:
    SetEveryWorkout(SetStorage::Member    where = SetStorage::MEMBERS_END,
                    SetStorage::FieldType value = 0):
        m_where{where},
        m_val  {value}
    {
    }

    static StencilTag<SetStorage> *createIf(std::size_t val)
    {
        if (val == SetStorageTags::SetEveryWorkout)
            return static_cast<StencilTag<SetStorage> *>(new SetEveryWorkout);
        else
            return nullptr;
    }

    const char *getTagName() const override
    {
        return "SetEveryWorkout";
    }

    void execute(SetStorage &s) const override
    {
        s.set_field(m_where, m_val);
    }

    bool readJson(const QJsonObject &json)    override;
    void writeJson(QJsonObject &target) const override;
private:
    SetStorage::Member m_where;
    SetStorage::FieldType m_val;
};

class SetGoodBit: public StencilTag<SetStorage>
{
public:
    SetGoodBit() = default;

    static StencilTag<SetStorage> *createIf(std::size_t val)
    {
        if (val == SetStorageTags::SetGoodBit)
            return static_cast<StencilTag<SetStorage> *>(new SetGoodBit);
        else
            return nullptr;
    }

    const char *getTagName() const override
    {
        return "SetGoodBit";
    }

    void execute(SetStorage &s)         const override;
    bool readJson(const QJsonObject &json)    override;
    void writeJson(QJsonObject &target) const override;
};

#endif // TAGS_SETSTORAGE_H
