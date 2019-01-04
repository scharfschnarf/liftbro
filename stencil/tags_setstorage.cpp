#include "stencilfactory.h"
#include "tags_setstorage.h"
#include "tags_common.h"

INIT_AYT(StencilTag<SetStorage>);
AYT::AYT<StencilTag<SetStorage>>      SetStorageStencilAYT;
AYT::AYT<StencilTag<SetStorage>>::Add SetStorageStencilAYTHandles[] =
{
    AddEveryWorkout::createIf, SetEveryWorkout::createIf, SetGoodBit::createIf
};

bool AddEveryWorkout::readJson(const QJsonObject &data)
{
    if (data.contains(MEMBER_ID_STRING)) {
        int v = data.find(MEMBER_ID_STRING)->toInt();
        auto field = static_cast<SetStorage::Member>(v);

        if (data.contains(VALUE_ID_STRING)) {
            v = data.find(VALUE_ID_STRING)->toInt();
            m_where = field;
            m_what  = v;
            return true;
        }
    }
    m_where = SetStorage::MEMBERS_END;
    return false;
}

void AddEveryWorkout::writeJson(QJsonObject &target) const
{
    if (m_where < SetStorage::MEMBERS_END) {
        target.insert(MEMBER_ID_STRING, m_where);
        target.insert(VALUE_ID_STRING,  static_cast<int>(m_what));
    }
}

void SetEveryWorkout::writeJson(QJsonObject &target) const
{
    if (m_where < SetStorage::MEMBERS_END) {
        target.insert(MEMBER_ID_STRING, m_where);
        target.insert(VALUE_ID_STRING,  static_cast<int>(m_val));
    }
}

bool SetEveryWorkout::readJson(const QJsonObject &data)
{
    if (data.contains(MEMBER_ID_STRING)) {
        int v = data.find(MEMBER_ID_STRING)->toInt();
        auto field = static_cast<SetStorage::Member>(v);

        if (data.contains(VALUE_ID_STRING)) {
            v = data.find(VALUE_ID_STRING)->toInt();
            m_where = field;
            m_val  = v;
            return true;
        }
    }
    m_where = SetStorage::MEMBERS_END;
    return false;
}

void SetGoodBit::execute(SetStorage &s) const
{
    bool goodBit = true;
    const int enumDiff = SetStorage::planned_reps - SetStorage::reps;
    for (SetStorage::Member i: {SetStorage::reps, SetStorage::weight, SetStorage::distance, SetStorage::time}) {
        SetStorage::Member iPlanned = static_cast<SetStorage::Member>(i + enumDiff);
        if (s.has_field(iPlanned)) {
            SetStorage::FieldType lhs, rhs;
            lhs = s.get_field(i);
            rhs = s.get_field(iPlanned);
            if (lhs != rhs) {
                goodBit = false;
                break;
            }
        }
    }
    s.set_field(SetStorage::good_set_bit, goodBit);
}

bool SetGoodBit::readJson(const QJsonObject &)
{
    // Ignore everything - no params
    return true;
}

void SetGoodBit::writeJson(QJsonObject &) const
{
    // Ignore everything - no params
}
