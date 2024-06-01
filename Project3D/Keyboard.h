#pragma once
#include "queue"
#include "bitset"

class Keyboard {
	friend class Window;
public:
	class Event {
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
		Event() : type(Type::Invalid), code(0u) {}
		Event(Type type, unsigned char code) : type(type), code(code) {};
		bool IsPress() const noexcept {
			return type == Type::Press;
		}
		bool IsRelease() const noexcept {
			return type == Type::Release;
		}
		bool IsValid() const noexcept {
			return type == Type::Invalid;
		}
		unsigned char GetCode() const noexcept {
			return code;
		}
	};

	Keyboard() = default;
	Keyboard(const Keyboard&) = delete;
	Keyboard& operator=(const Keyboard&) = delete;

	bool KeyIsPressed(unsigned char keycode) const noexcept;
	Event ReadKey() noexcept;
	bool KeyIsEmpty() const noexcept;
	void FlushKey() noexcept;

	char ReadChar() noexcept;
	bool CharIsEmpty() const noexcept;
	void FlushChar() noexcept;
	void Flush() noexcept;

	void EnbaleAutorepeat() noexcept;
	void DisaleAutorepeat() noexcept;
	bool AutorepeatIsEnbale() const noexcept;

private:
	void OnKeyPressed(unsigned char keycode) noexcept;
	void OnKeyReleased(unsigned char keycode) noexcept;
	void OnChar(char character) noexcept;
	void CleatState() noexcept;
	template<typename T>
	static void TrimBuffer(std::queue<T>& buffer) noexcept;

	static constexpr unsigned int nKeys = 256u;
	static constexpr unsigned int bufferSize = 16u;
	bool autorepeatEnabled = 0;
	std::bitset<nKeys> keystates;
	std::queue<Event> keyBuffer;
	std::queue<char> charBuffer;
};

