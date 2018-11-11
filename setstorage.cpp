#include "setstorage.h"
#include <algorithm>

bool SetStorage::readJson(const QJsonObject &read_from)
{
    if (!m_data.empty())
        m_data.clear();

    for (unsigned labels_it = warmup_bit; labels_it < MEMBERS_END; ++labels_it) {
        const char *label = SET_STORAGE_MEMBER_LABELS[labels_it];
        if (read_from.contains(label))
            m_data[static_cast<Member>(labels_it)] = static_cast<unsigned int>(read_from[label].toDouble());
    }
    return true;
}

void SetStorage::writeJson(QJsonObject &write_to) const
{
    for (const auto &elem: m_data) {
        write_to.insert(
            SET_STORAGE_MEMBER_LABELS[elem.first],
            static_cast<double>(elem.second)
        );
    }
}

SetStorage::SetStorage(const std::initializer_list<std::pair<Member, FieldType>> &list)
{
    for (auto it = list.begin(); it != list.end(); ++it) {
        if (it->first < MEMBERS_END)
            m_data[it->first] = it->second;
    }
}
