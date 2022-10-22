#pragma once

#include <queue>


class Mouse
{

	friend class Window;

public:
	class Event {

	public:
		enum class Type {
			LPress,
			LRelease,
			RPress,
			RRelease,
			WheelUp,
			WheelDown,
			Move,
			Enter,
			Leave,
			Invalid,
		};

	private:
		Type type;
		int x, y; // coordinates
		bool IsLeftPressed = false;
		bool IsRightPressed = false;

	public:
		Event() : type(Type::Invalid), x(0), y(0) {};
		Event(Type type, const Mouse& parent)
			:
			type(type),
			IsLeftPressed(parent.IsLeftPressed),
			IsRightPressed(parent.IsRightPressed),
			x(parent.x),
			y(parent.y)
		{

		}

		Type GetType() const { return type; }
		int GetPosX() const { return x; }
		int GetPosY() const { return y;}
		bool LeftIsPressed() const { return IsLeftPressed; }
		bool RightIsPressed() const { return IsRightPressed; }

	};


public:
	Mouse() = default;
	Mouse(const Mouse&) = delete;
	Mouse& operator=(const Mouse&) = delete;
	int GetPosX() const;
	int GetPosY() const;
	bool IsInWindow() const;
	bool LeftIsPressed() const;
	bool RightIsPressed() const;
	Mouse::Event Read();
	bool IsEmpty() const;
	void Flush();

private:
	void OnMouseMove(int x, int y);
	void OnMouseLeave();
	void OnMouseEnter();
	void OnLeftPressed(int x, int y);
	void OnLeftReleased(int x, int y);
	void OnRightPressed(int x, int y);
	void OnRightReleased(int x, int y);
	void OnWheelUp(int x, int y);
	void OnWheelDown(int x, int y);
	void TrimBuffer();

private:
	static constexpr unsigned int bufferSize = 16u;
	int x, y; // coordinates
	bool IsLeftPressed = false;
	bool IsRightPressed = false;
	bool bIsInWindow = false;
	std::queue<Event> buffer;
};

