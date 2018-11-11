#ifndef SPAWNERFIELDS_H
#define SPAWNERFIELDS_H

#include "setstorage.h"

namespace SpawnerFields {

enum SetFieldOp {
    Static,        // set to constant value
    NthSet,           // change nth occurence to be equal to some value
    NFirstSet,        // change first n to be equal to
    NLastSet,         // change last n to be equal to
    NthAdd,
    NFirstAdd,
    NLastAdd,
    RecursiveAdd   // Increase every field by cumulative sum
};
#if 0
constexpr auto SET_FIELD_OP_TO_NAME(SetFieldOp f)
{
    switch(f) {
        case Static:        return "Static";
        case Nth:           return "Nth";
        case NFirst:        return "NFirst";
        case NLast:         return "NLast";
        case RecursiveAdd:  return "RecursiveAdd";
    }
    return "INVALID";
}

class FieldPack: public Jsonable
{
public:
    void run(SetStorage &apply_to, std::size_t index)
    {
        switch (m_op) {
            case Static:
                apply_to.set_field(m_affected, m_fieldmod);
                break;
            case Nth:
                if (index == m_index)
                    apply_to.set_field(m_affected, m_fieldmod);
                break;
            case NFirst:
                if (index < m_index)

        }
    }
protected:
    SetFieldOp m_op;
    SetStorage::Member m_affected;
    SetStorage::FieldType m_fieldmod;
    std::size_t m_index;
};

}; // namespace spawnerfields

#endif //0
#endif // SPAWNERFIELDS_H
