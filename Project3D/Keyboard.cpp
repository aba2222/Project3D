#include "Keyboard.h"

bool Keyboard::KeyIsPressed(unsigned char keycode) const noexcept {
	return keystates[keycode];
}

Keyboard::Event Keyboard::ReadKey() noexcept {
	if (keyBuffer.size() > 0u) {
		Keyboard::Event e = keyBuffer.front();
		keyBuffer.pop();
		return e;
	}
	return Keyboard::Event();
}

bool Keyboard::KeyIsEmpty() const noexcept {
	return keyBuffer.empty();
}

void Keyboard::FlushKey() noexcept {
	keyBuffer = std::queue<Event>();
}

char Keyboard::ReadChar() noexcept {
	if (charBuffer.size() > 0u) {
		unsigned char charcode = charBuffer.front();
		charBuffer.pop();
		return charcode;
	}
	return 0;
}

bool Keyboard::CharIsEmpty() const noexcept {
	return charBuffer.empty();
}

void Keyboard::FlushChar() noexcept {
	charBuffer = std::queue<char>();
}

void Keyboard::Flush() noexcept {
	FlushKey();
	FlushChar();
}

void Keyboard::EnbaleAutorepeat() noexcept {
	autorepeatEnabled = 1;
}

void Keyboard::DisaleAutorepeat() noexcept {
	autorepeatEnabled = 0;
}

bool Keyboard::AutorepeatIsEnbale() const noexcept {
	return autorepeatEnabled;
}

void Keyboard::OnKeyPressed(unsigned char keycode) noexcept {
	keystates[keycode] = 1;
	keyBuffer.push(Keyboard::Event(Keyboard::Event::Type::Press, keycode));
	TrimBuffer(keyBuffer);
}

void Keyboard::OnKeyReleased(unsigned char keycode) noexcept {
	keystates[keycode] = 0;
	keyBuffer.push(Keyboard::Event(Keyboard::Event::Type::Release, keycode));
	TrimBuffer(keyBuffer);
}

void Keyboard::OnChar(char character) noexcept {
	charBuffer.push(character);
	TrimBuffer(charBuffer);
}

void Keyboard::CleatState() noexcept {
	keystates.reset();
}

template<typename T>
void Keyboard::TrimBuffer(std::queue<T>& buffer) noexcept {
	while (buffer.size() > bufferSize) {
		buffer.pop();
	}
}
