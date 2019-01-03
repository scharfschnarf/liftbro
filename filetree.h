#ifndef FILETREE_H
#define FILETREE_H

#include <memory>
#include <vector>
#include <QFile>
#include <initializer_list>
#include <iterator>
#include <functional>

// tpl instantiation
#include <algorithm>
#include <QJsonDocument>
#include <QJsonArray>

#include "jsonable.h"

/* FileTree
 * FileTree packs a save file into single entity/database
 * that has ownership of all members
 * TODO: It supports lazy loading so that reading data from file is done
 * on demand
 * NOTE TO SELF: it must be std::vector - std::set wouldn't support changing timestamps in workouts
 * TODO: T can be Jsonable-independent (uniform file IO interface)
 */

template <typename T, typename TLess = std::less<T>>
class FileTree: public Jsonable
{
protected:
    typedef std::shared_ptr<T> MemberSharedPtr;
public:
    class iterator
    {
    private:
        typedef typename std::vector<MemberSharedPtr>::iterator UnderlyingIterator;
        friend class FileTree<T, TLess>;
    public:
        // STD definitions follow
        typedef std::bidirectional_iterator_tag iterator_category;
        typedef T    value_type;
        typedef T&   reference;
        //typedef WorkoutPtr pointer;
        typedef std::size_t difference_type;

        iterator operator++()    {++m_underlying; return *this;}
        iterator operator--()    {--m_underlying; return *this;}
        iterator operator++(int) {iterator cur = *this; ++m_underlying; return cur;}
        iterator operator--(int) {iterator cur = *this; --m_underlying; return cur;}

        T &operator*();
        MemberSharedPtr operator->();

        bool operator== (const iterator &rhs) {return m_underlying == rhs.m_underlying;}
        bool operator!= (const iterator &rhs) {return !(*this == rhs);}
    private:
        iterator(FileTree<T, TLess> *ptr, UnderlyingIterator it):
            m_underlying{it}, m_ptr{ptr}
        {}

        UnderlyingIterator m_underlying;
        FileTree<T, TLess> *m_ptr;
    };

    // Delete copiers (heavy database object)
    FileTree    (const FileTree<T> &)            = delete;
    FileTree<T> &operator= (const FileTree<T> &) = delete;

    explicit FileTree(const char fileName[]);
    virtual ~FileTree() noexcept;

    virtual bool readJson(const QJsonObject &json);
    virtual void writeJson(QJsonObject &target) const;

    bool readFile();
    bool writeFile();

    // Access elements
    // All functions may call sort() if the vector elements count is unsorted
    // TODO: is const-correctness possible here?
    // WARNING: if object's timestamp is modified through iterators,
    //  the values inside may remain unsorted for some time
    std::size_t size() const
    {
        return m_members.size();
    }

    iterator begin() { ensureSorted(); return iterator{this, m_members.begin()}; }
    iterator end()   { ensureSorted(); return iterator{this, m_members.end()}; }

    // Modifiers
    // All functions here call sort()
    inline iterator addElem(const T &toAdd = T{});
    inline void deleteElem(iterator);
protected:
    inline void ensureSorted()
    {
        if (!std::is_sorted(std::begin(m_members), std::end(m_members), TLess{}))
            sort();
    }
    inline void sort()
    {
        std::sort(std::begin(m_members), std::end(m_members), TLess{});
    }

    inline iterator makeIterator(typename iterator::UnderlyingIterator it)
    {
        return iterator{this, it};
    }


    // Internal, auxiliary functions
    // Functions handling lazy loading
    // TODO: Lazy loading
    bool isObjLoaded(iterator /*it*/) const;
    void loadObj(iterator);

    std::vector<MemberSharedPtr> m_members;
    QFile m_stor_file;
};

// TEMPLATE INSTANTIATION FUNCTIONS FOLLOW --------------------------------------------

template <typename T, typename TLess>
typename FileTree<T,TLess>::iterator
    FileTree<T,TLess>::addElem(const T &toAdd)
{
    auto shared = std::make_shared<T>(toAdd);
    m_members.push_back(shared);
    sort();

    // Find again an iterator to the just added element (std::lower_bound because sorted)
    return iterator{this, std::lower_bound(m_members.begin(), m_members.end(),
                                           shared, TLess{})};
}

template <typename T, typename TLess>
void FileTree<T,TLess>::deleteElem(iterator it)
{
    m_members.erase(it.m_underlying);
    ensureSorted();
}


template <typename T, typename TLessCall>
FileTree<T,TLessCall>::~FileTree<T, TLessCall>() noexcept
{
    writeFile();
}

template <typename T, typename TLessCall>
T & FileTree<T,TLessCall>::iterator::operator* ()
{
    m_ptr->loadObj(*this);
    return **m_underlying;
}

template <typename T, typename TLessCall>
typename FileTree<T,TLessCall>::MemberSharedPtr
    FileTree<T,TLessCall>::iterator::operator-> ()
{
    m_ptr->loadObj(*this);
    return *m_underlying;
}

template <typename T, typename TLessCall>
bool FileTree<T,TLessCall>::isObjLoaded(iterator) const
{
    // no-op, TODO: lazy loading
    return true;
}

template <typename T, typename TLessCall>
void FileTree<T,TLessCall>::loadObj(iterator)
{
    // no-op, TODO: lazy loading
    return;
}

template <typename T, typename TLessCall>
FileTree<T,TLessCall>::FileTree(const char *filename):
    m_members{},
    m_stor_file{filename}
{
}

template <typename T, typename TLess>
bool FileTree<T,TLess>::readJson(const QJsonObject &json)
{
    // Silent insignificant assumption:
    // First element will be the one with array
    QJsonArray objArr = json.begin()->toArray();
    for (auto elem: objArr) {
        QJsonObject obj = elem.toObject();
        MemberSharedPtr al{new T{}};
        if (al->readJson(obj) == false)
            return false;
        m_members.push_back(std::move(al));
    }

    return true;
}

template <typename T, typename TLess>
void FileTree<T,TLess>::writeJson(QJsonObject &target) const
{
    QJsonArray objArr;
    for (const auto &elem: m_members) {
        QJsonObject obj;
        elem->writeJson(obj);
        objArr.push_back(obj);
    }
    target.insert("FT", objArr);
}

template <typename T, typename TLess>
bool FileTree<T,TLess>::readFile()
{
    // Ensure file is open for read only
    if (m_stor_file.isOpen())
        m_stor_file.close();
    bool state = m_stor_file.open(QIODevice::ReadOnly);
    if (!state)
        return false;

    QByteArray fileContent = m_stor_file.readAll();
    if (fileContent.isEmpty())
        return true;
    m_stor_file.close();

    QJsonDocument doc = QJsonDocument::fromJson(fileContent);
    if (!doc.isObject())
        return false;

    return readJson(doc.object());
}

template <typename T, typename TLess>
bool FileTree<T,TLess>::writeFile()
{
    QJsonObject docObj;
    writeJson(docObj);

    QJsonDocument doc{std::move(docObj)};

    // Ensure file is open for write
    if (m_stor_file.isOpen())
        m_stor_file.close();
    bool state = m_stor_file.open(QIODevice::ReadWrite | QIODevice::Truncate);
    if (!state)
        return false;

    state = m_stor_file.write(doc.toJson(QJsonDocument::Compact));
    if (!state)
        return false;

    m_stor_file.close();
    return true;
}


#endif // FILETREE_H
