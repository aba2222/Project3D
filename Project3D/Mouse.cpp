#include "Mouse.h"

std::pair<int, int> Mouse::GetPos() const noexcept {
	return { x, y };
}
int Mouse::GetPosX() const noexcept {
	return x;
}
int Mouse::GetPosY() const noexcept {
	return y;
}

bool Mouse::IsInWindow() const noexcept {
	return isInWindow;
}

Mouse::Event Mouse::Read() noexcept {
	if (buffer.size() > 0u) {
		Mouse::Event e = buffer.front();
		buffer.pop();
		return e;
	}
	return Mouse::Event();
}

void Mouse::Flush() noexcept {
	buffer = std::queue<Event>();
}

void Mouse::OnMouseLeave() noexcept {
	isInWindow = 0;
	buffer.push(Mouse::Event(Mouse::Event::Type::Leave, *this));
	TrimBuffer();
}

void Mouse::OnMouseEnter() noexcept {
	isInWindow = 1;
	buffer.push(Mouse::Event(Mouse::Event::Type::Enter, *this));
	TrimBuffer();
}

void Mouse::OnMouseMove(int newx, int newy) noexcept {
	x = newx;
	y = newy;

	buffer.push(Mouse::Event(Mouse::Event::Type::Move, *this));
	TrimBuffer();
}

void Mouse::OnLeftPressed(int x, int y) noexcept {
	leftIsPressed = 1;

	buffer.push(Mouse::Event(Mouse::Event::Type::LPress, *this));
	TrimBuffer();
}

bool Mouse::LeftIsPressed() const noexcept {
	return leftIsPressed;
}

void Mouse::OnLeftReleased(int x, int y) noexcept {
	leftIsPressed = 0;

	buffer.push(Mouse::Event(Mouse::Event::Type::LRelease, *this));
	TrimBuffer();
}

void Mouse::OnRightPressed(int x, int y) noexcept {
	rightIsPressed = 1;

	buffer.push(Mouse::Event(Mouse::Event::Type::RPress, *this));
	TrimBuffer();
}

bool Mouse::RightIsPressed() const noexcept {
	return rightIsPressed;
}

void Mouse::OnRightReleased(int x, int y) noexcept {
	rightIsPressed = 0;

	buffer.push(Mouse::Event(Mouse::Event::Type::RRelease, *this));
	TrimBuffer();
}

void Mouse::OnWheelUp(int x, int y) noexcept {
	buffer.push(Mouse::Event(Mouse::Event::Type::WheelUp, *this));
	TrimBuffer();
}

void Mouse::OnWheelDown(int x, int y) noexcept {
	buffer.push(Mouse::Event(Mouse::Event::Type::WheelDown, *this));
	TrimBuffer();
}

void Mouse::TrimBuffer() noexcept {
	while (buffer.size() > bufferSize) {
		buffer.pop();
	}
}

void Mouse::OnWheelDeltaCarry(int x, int y, int delta) noexcept {
	wheelDeltaCarry += delta;
	while (wheelDeltaCarry >= WHEEL_DELTA) {
		wheelDeltaCarry -= WHEEL_DELTA;
		OnWheelUp(x, y);
	}
	while (wheelDeltaCarry <= WHEEL_DELTA) {
		wheelDeltaCarry += WHEEL_DELTA;
		OnWheelDown(x, y);
	}
}
