#ifndef GENERICEXC_H
#define GENERICEXC_H

#include <QString>
#include "Types.h"

class GenericExc {
public:
    static const u::uint32 ERR_NOT_SET; ///< Код ошибки не установлен

public:

    GenericExc(const QString& what, u::uint32 err = ERR_NOT_SET);

    GenericExc(const GenericExc& r);

    virtual ~GenericExc();

    GenericExc& operator=(const GenericExc& r);

public:

    virtual const QString& GetWhat() const {
        return m_what;
    }

    virtual u::uint32 GetErr() const {
        return m_err;
    }

private:
    QString m_what;
    u::uint32 m_err;
};


#endif // GENERICEXC_H
