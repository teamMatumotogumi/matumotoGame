#include "InputControl.h"

InputControl::InputControl()
{
}

uint32_t InputControl::GetStateOffsetRelativeToDeviceRoot() const
{
    return uint32_t();
}

void* InputControl::GetCurrentStatePtr() const
{
    return nullptr;
}

void* InputControl::GetDefaultStatePtr() const
{
    return nullptr;
}

void* InputControl::GetNoiseMaskPtr() const
{
    return nullptr;
}

void* InputControl::GetPreviousFrameStatePtr() const
{
    return nullptr;
}

void InputControl::RefreshConfiguration()
{
}

void InputControl::RefreshConfigurationIfNeeded()
{
}

void InputControl::FinishSetup()
{
}

std::vector<std::string> InputControl::GetAliases() const
{
    return std::vector<std::string>();
}

std::vector<InputControl*> InputControl::GetChildren() const
{
    return std::vector<InputControl*>();
}

InputControl* InputControl::TryGetChildControl(const char* path) const
{
    return nullptr;
}

std::string InputControl::GetPath() const
{
    return std::string();
}

bool InputControl::IsSynthetic() const
{
    return false;
}

std::vector<std::string> InputControl::GetUsages() const
{
    return std::vector<std::string>();
}

int32_t InputControl::GetValueSizeInBytes() const
{
    return int32_t();
}

std::string InputControl::GetVariants() const
{
    return std::string();
}

bool InputControl::CompareValue(void* firstStatePtr, void* secondStatePtr) const
{
    return false;
}

float InputControl::EvaluateMagnitude() const
{
    return 0.0f;
}

float InputControl::EvaluateMagnitude(void* statePtr) const
{
    return 0.0f;
}

InputControl* InputControl::GetChildControl(const std::string& path) const
{
    return nullptr;
}

void InputControl::ReadValueFromStateIntoBuffer(void* statePtr, void* bufferPtr, int bufferSize)
{
}

void InputControl::WriteValueFromBufferIntoState(void* bufferPtr, int bufferSize, void* statePtr)
{
}
