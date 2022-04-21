#include "ReferenceCounter.h"


ReferenceCounter::ReferenceCounter()
    : m_referenceCount(1)
{
}


ULONG __stdcall ReferenceCounter::Release()
{
    if (--m_referenceCount == 0L)
    {
        delete this;
        return 0;
    }

    return m_referenceCount;
}

