#include "GenericExc.h"

const u::uint32 GenericExc::ERR_NOT_SET = (u::uint32)-1;

GenericExc::GenericExc(const QString& what, u::uint32 err)
    : m_what(what)
    , m_err(err) {
}

GenericExc::GenericExc(const GenericExc& r)
    : m_what(r.m_what)
    , m_err(r.m_err) {
}

GenericExc::~GenericExc() {
}

GenericExc& GenericExc::operator=(const GenericExc& r) {
    if (this != &r) {
        m_what = r.m_what;
        m_err = r.m_err;
    }
    return *this;
}
