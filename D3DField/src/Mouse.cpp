#include "Mouse.h"

int Mouse::GetPosX() const
{
    return x;
}

int Mouse::GetPosY() const
{
    return y;
}

bool Mouse::IsInWindow() const
{
    return bIsInWindow;
}

bool Mouse::LeftIsPressed() const
{
    return IsLeftPressed;
}

bool Mouse::RightIsPressed() const
{
    return IsRightPressed;
}

Mouse::Event Mouse::Read()
{
    if (buffer.size() > 0u) {
        Mouse::Event e = buffer.front();
        buffer.pop();
        return e;
    }
    return Mouse::Event();
}

bool Mouse::IsEmpty() const
{
    return buffer.empty();
}

void Mouse::Flush()
{
    buffer = std::queue<Event>();
}

void Mouse::OnMouseMove(int x, int y)
{
    this->x = x;
    this->y = y;

    buffer.push(Mouse::Event(Mouse::Event::Type::Move, *this));
    TrimBuffer();
}

void Mouse::OnMouseLeave()
{
    bIsInWindow = false;
    buffer.push(Mouse::Event(Mouse::Event::Type::Leave, *this));
    TrimBuffer();
}

void Mouse::OnMouseEnter()
{
    bIsInWindow = true;
    buffer.push(Mouse::Event(Mouse::Event::Type::Enter, *this));
    TrimBuffer();
}

void Mouse::OnLeftPressed(int x, int y)
{
    this->IsLeftPressed = true;

    buffer.push(Mouse::Event(Mouse::Event::Type::LPress, *this));
    TrimBuffer();
}

void Mouse::OnLeftReleased(int x, int y)
{
    this->IsLeftPressed = false;

    buffer.push(Mouse::Event(Mouse::Event::Type::LRelease, *this));
    TrimBuffer();
}

void Mouse::OnRightPressed(int x, int y)
{
    this->IsRightPressed = true;

    buffer.push(Mouse::Event(Mouse::Event::Type::RPress, *this));
    TrimBuffer();
}

void Mouse::OnRightReleased(int x, int y)
{
    this->IsRightPressed = false;

    buffer.push(Mouse::Event(Mouse::Event::Type::RPress, *this));
    TrimBuffer();
}

void Mouse::OnWheelUp(int x, int y)
{
    buffer.push(Mouse::Event(Mouse::Event::Type::WheelUp, *this));
    TrimBuffer();
}

void Mouse::OnWheelDown(int x, int y)
{
    buffer.push(Mouse::Event(Mouse::Event::Type::WheelDown, *this));
    TrimBuffer();
}

void Mouse::TrimBuffer()
{
    while (buffer.size() > bufferSize) {
        Mouse::Event ev = buffer.front();
        buffer.pop();
    }
}
