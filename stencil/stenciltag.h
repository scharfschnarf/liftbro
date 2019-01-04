#ifndef STENCILTAG_H
#define STENCILTAG_H

#include "../jsonable.h"
#include "../exercise.h"
#include "../setstorage.h"
#include "../tools/ayt.h"

#include <vector>
#include <memory>
#include <initializer_list>

// TODO: compile-time, if possible
// TODO: type-abstract, if possible

template <typename T>
class StencilTag : public Jsonable
{
public:
    StencilTag() = default;
    virtual ~StencilTag() noexcept = default;

    virtual const char *getTagName() const = 0;

    virtual void execute(T &) const = 0;
};

template <typename T>
class JoinTags: public StencilTag<T>
{
public:
    // MUST BE PARAMETER PACK
    template <typename... Tags>
    JoinTags(Tags &&... tagList)
    {
        // Copy every argument of ctor to heap and store pointer in a vector
        (appendTag(tagList), ...);
    }

    void appendTag(const StencilTag<T> &tag)
    {
        m_tags.push_back(std::make_shared(tag));
    }

    const char *getTagName() const override
    {
        return "JoinTag";
    }

    void execute(T &t) const override
    {
        for (const auto &tag: m_tags)
            tag->execute(t);
    }

    bool readJson(const QJsonObject &json)    override;
    void writeJson(QJsonObject &target) const override;
protected:
    std::vector<std::shared_ptr<StencilTag<T>>> m_tags;
};


#endif // STENCILTAG_H
