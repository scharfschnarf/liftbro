#ifndef AYT_H
#define AYT_H

#include <initializer_list>

namespace AYT
{
template <typename Parent,
          typename CmpType = std::size_t,
          std::size_t InstanceID = 0,
          std::size_t Size = 16>
struct AYT
{
    typedef Parent * (*TestFunction)(CmpType);

    constexpr static std::size_t AYTRunSize = Size;

    // Auxiliary structure to enable compile-time adding to array
    struct Add
    {
        constexpr Add (TestFunction fn)
        {
            _add(fn);
        }
    };

    static constexpr Parent * runOne(std::size_t id, CmpType arg)
    {
        if (m_array[id] == nullptr)
            return nullptr;
        else
            return m_array[id](arg);
    }

    static constexpr Parent * runAll(CmpType arg)
    {
        for (auto *currentTest: m_array) {
            if (currentTest == nullptr)
                return nullptr;

            Parent *testResult = (*currentTest)(arg);
            if (testResult != nullptr)
                return testResult;
        }
        return nullptr;
    }

private:
    static constexpr bool _add(TestFunction fn)
    {
        m_array[count] = fn;
        ++count;
        return true;
    }

    static TestFunction m_array[Size];
    static std::size_t count;
};

// Auxiliary
#define INIT_AYT(PARENT) \
    template<> \
    AYT::AYT<PARENT>::TestFunction AYT::AYT<PARENT>::m_array[] = {nullptr}; \
    template<> \
    std::size_t AYT::AYT<PARENT>::count = 0

} // ns AYT

#endif // AYT_H
