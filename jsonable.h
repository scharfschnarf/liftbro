#ifndef JSONABLE_H
#define JSONABLE_H

#include <QJsonObject>

/* Jsonable
 * A class that defines abstract behaviour of being able
 * to read/write itself from JSON file
 */

class Jsonable
{
public:
    virtual bool readJson(const QJsonObject &json)    = 0;
    virtual void writeJson(QJsonObject &target) const = 0;
    virtual ~Jsonable() noexcept = default;
};

#endif // JSONABLE_H
