#pragma once

#include <queue>
#include <bitset>

class Keyboard
{

	friend class Window;

public:
	class Event 
	{
	public:
		enum class Type {
			Press,
			Release,
			Invalid
		};
	private:
		Type type;
		unsigned char code;
	public:
		Event() : type(Type::Invalid), code(0)
		{

		}

		Event(Type type, unsigned char keycode) : type(type), code(keycode)
		{

		}

		bool IsPress() { return type == Type::Press; };
		bool IsRelease() { return type == Type::Release; };
		bool IsValid() { return type != Type::Invalid; };

		unsigned char GetCode() { return code; }
	};

public:
	Keyboard() = default;
	Keyboard(const Keyboard&) = delete;
	Keyboard& operator=(const Keyboard&) = delete;

	bool IsKeyPressed(unsigned char keycode) const;
	bool IsKeyEmpty() const;
	void FlushKey();
	Event ReadKey();

	char ReadChar();
	bool IsCharEmpty() const;
	void FlushChar();
	void Flush();

	void EnableAutorepeat();
	void DisableAutorepeat();
	bool IsAutorepeatEnabled();

private:
	void OnKeyPressed(unsigned char keycode);
	void OnKeyReleased(unsigned char keycode);
	void OnChar(unsigned char character);
	void ClearState();

	template<typename T>
	static void TrimBuffer(std::queue<T>& buffer);

private:
	static constexpr unsigned int nKeys = 256u;
	static constexpr unsigned int bufferSize = 16u;
	
	bool autoRepeatEnabled = false;

	std::bitset<nKeys> keyStates;
	std::queue<Event> keyBuffer;
	std::queue<unsigned char> charBuffer;

};