#include "Keyboard.h"

bool Keyboard::IsKeyPressed(unsigned char keycode) const
{
	return keyStates[keycode];
}

bool Keyboard::IsKeyEmpty() const
{
	return keyBuffer.empty();
}

void Keyboard::FlushKey()
{
	keyBuffer = std::queue<Event>();
}

Keyboard::Event Keyboard::ReadKey()
{
	if (keyBuffer.size() > 0u) {
		Keyboard::Event e = keyBuffer.front();
		keyBuffer.pop();
		return e;
	}
	return Keyboard::Event();
}

char Keyboard::ReadChar()
{
	if (charBuffer.size() > 0u) {
		unsigned char charcode = charBuffer.front();
		charBuffer.pop();
		return charcode;
	}
	return 0;
}

bool Keyboard::IsCharEmpty() const
{
	return charBuffer.empty();
}

void Keyboard::FlushChar()
{
	charBuffer = std::queue<unsigned char>();
}

void Keyboard::Flush()
{
	FlushKey();
	FlushChar();
}

void Keyboard::EnableAutorepeat()
{
	this->autoRepeatEnabled = true;
}

void Keyboard::DisableAutorepeat()
{
	this->autoRepeatEnabled = false;
}

bool Keyboard::IsAutorepeatEnabled()
{
	return this->autoRepeatEnabled;
}

void Keyboard::OnKeyPressed(unsigned char keycode)
{
	keyStates[keycode] = true;
	keyBuffer.push(Keyboard::Event(Keyboard::Event::Type::Press, keycode));
	TrimBuffer(keyBuffer);
}

void Keyboard::OnKeyReleased(unsigned char keycode)
{
	keyStates[keycode] = false;
	keyBuffer.push(Keyboard::Event(Keyboard::Event::Type::Release, keycode));
	TrimBuffer(keyBuffer);
}

void Keyboard::OnChar(unsigned char character)
{
	charBuffer.push(character);
	TrimBuffer(charBuffer);
}

void Keyboard::ClearState()
{
	keyStates.reset();
}


template<typename T>
void Keyboard::TrimBuffer(std::queue<T>& buffer)
{
	while (buffer.size() > bufferSize) {
		buffer.pop();
	}
}
