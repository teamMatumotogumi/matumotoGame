#include "Keyboard.h"

uint8_t Keyboard::m_previous[256] = { 0 };
uint8_t Keyboard::m_current[256] = { 0 };

void Keyboard::Initialize()
{
    memset(m_previous, 0, sizeof(m_previous));
    memset(m_current, 0, sizeof(m_current));
}

void Keyboard::Update()
{
    memcpy(m_previous, m_current, sizeof(m_previous));
    GetKeyboardState(m_current);
}
