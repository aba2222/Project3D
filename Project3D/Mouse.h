#pragma once
#include <queue>
#include <Windows.h>

class Mouse {
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
			Invalid
		};
	private:
		Type type;
		int x, y;
		bool leftIsPressed;
		bool rightIsPressed;
	public:
		Event() : type(Type::Invalid), leftIsPressed(0), rightIsPressed(0), x(0), y(0) {}
		Event(Type type, const Mouse& parent) : type(type), leftIsPressed(parent.leftIsPressed), rightIsPressed(parent.rightIsPressed),
												x(parent.x), y(parent.y){};
		bool IsValid() const noexcept {
			return type != Type::Invalid;
		}
		Type GetType() const noexcept {
			return type;
		}
		std::pair<int,int> GetPos() const noexcept {
			return { x, y };
		}
		int GetPosX() const noexcept {
			return x;
		}
		int GetPosY() const noexcept {
			return y;
		}
		bool LeftIsPress() const noexcept {
			return type == Type::LPress;
		}
		bool LeftIsRelease() const noexcept {
			return type == Type::LRelease;
		}
	};

	Mouse() = default;
	Mouse(const Mouse&) = delete;
	Mouse& operator=(const Mouse&) = delete;
	std::pair<int, int> GetPos() const noexcept;
	int GetPosX() const noexcept;
	int GetPosY() const noexcept;
	bool IsInWindow() const noexcept;
	bool LeftIsPressed() const noexcept;
	bool RightIsPressed() const noexcept;
	Mouse::Event Read() noexcept;
	bool IsEmpty() const noexcept {
		return buffer.empty();
	}
	void Flush() noexcept;

private:
	void OnMouseLeave() noexcept;
	void OnMouseEnter() noexcept;
	void OnMouseMove(int newx, int newy) noexcept;
	void OnLeftPressed(int x, int y) noexcept;
	void OnLeftReleased(int x, int y) noexcept;
	void OnRightPressed(int x, int y) noexcept;
	void OnRightReleased(int x, int y) noexcept;
	void OnWheelUp(int x, int y) noexcept;
	void OnWheelDown(int x, int y) noexcept;
	void TrimBuffer() noexcept;
	void OnWheelDeltaCarry(int x, int y, int delta) noexcept;

	static constexpr unsigned int bufferSize = 16u;
	int x, y;
	bool leftIsPressed = 0;
	bool rightIsPressed = 0;
	bool isInWindow = 0;
	int wheelDeltaCarry = 0;
	std::queue<Event> buffer;
};

