#ifndef EXERCISESPAWNER_H
#define EXERCISESPAWNER_H

#include <QJsonObject>

#include "jsonable.h"
#include "exercise.h"
#include "setstorage.h"

// [!] NO MODIFIERS FOR NOW

class ExerciseSpawner : public Jsonable
{
public:
    ExerciseSpawner() = default;
    ExerciseSpawner(const Exercise &);

    void set_template(const Exercise &);
    Exercise make_from_template() const;

    // read_json and write_json will be compacted
    virtual bool readJson(const QJsonObject &json);
    virtual void writeJson(QJsonObject &target) const;
protected:
    std::size_t m_sets;
    SetStorage m_sample;
};

#endif // EXERCISESPAWNER_H
