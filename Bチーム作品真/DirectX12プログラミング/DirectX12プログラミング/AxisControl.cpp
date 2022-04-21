#include "AxisControl.h"

AxisControl::AxisControl()
    : m_clamp(Clamp::None)
    , m_clampConstant(0.0f)
    , m_clampMin(-1.0f)
    , m_clampMax(+1.0f)
    , m_normalizeMin(-1.0f)
    , m_normalizeMax(+1.0f)
    , m_normalizeZero(0.0f)
    , m_scaleFactor(1.0f)
    , m_invert(false)
    , m_normalize(false)
    , m_scale(false)
{
}

bool AxisControl::CompareValue(void* firstStatePtr, void* secondStatePtr) const
{
    return false;
}

float AxisControl::EvaluateMagnitude(void* statePtr) const
{
    return 0.0f;
}

float AxisControl::ReadUnprocessedValueFromState(void* statePtr)
{
    return 0.0f;
}

void AxisControl::WriteValueIntoState(const float& value, void* statePtr)
{
}

void AxisControl::FinishSetup()
{
}

float AxisControl::Preprocess(float value)
{
    return 0.0f;
}
