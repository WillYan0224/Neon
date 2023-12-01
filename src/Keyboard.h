#include <queue>
#include <bitset>
#include <optional>

class Keyboard
{
	friend class Window;
public:
	class Event
	{
	public:
		enum class Type
		{
			Press,
			Release,
			Invalid
		};
	private:
		Type type;
		unsigned char code;
	public:
		Event()
			:
			type( Type::Invalid ), code( 0u )
		{}
		Event( Type type, unsigned code ) noexcept
			:
			type( type ), code( code )
		{}
		bool IsPress() const noexcept
		{
			return type == Type::Press;
		}
		bool IsRealise() const noexcept
		{
			return type == Type::Release;
		}
		bool IsValid() const noexcept
		{
			return type != Type::Invalid;
		}
		unsigned char GetCode() const noexcept
		{
			return code;
		}
	};
public:
	Keyboard() = default;
	Keyboard( const Keyboard& ) = delete; // no copy constructor
	Keyboard& operator=( const Keyboard& ) = delete;
	// key event
	bool KeyIsPressed(unsigned char keycode) const noexcept;
	std::optional<Event> ReadKey() noexcept;
	bool KeyIsEmpty() const noexcept;
	void FlushKey() noexcept;
	// char event
	std::optional<char> ReadChar() noexcept;
	bool CharIsEmpty() const noexcept;
	void FlushChar() noexcept;
	
	void Flush() noexcept;
	// autorepeat control
	void EnableAutoRepeat() noexcept;
	void DisableAutoRepeat() noexcept;
	bool AutorepeatEnabled() const noexcept;
private:
	void OnKeyPressed(unsigned char keycode ) noexcept;
	void OnKeyReleased(unsigned char keycode ) noexcept;
	void OnChar( char character ) noexcept;
	void ClearState() noexcept;
	template<typename T>
	static void TrimBuffer(std::queue<T>& buffer) noexcept;

private:
	static constexpr unsigned int nKey = 256u;
	static constexpr unsigned int bufferSize = 16u;
	bool autorepeatEnabled = false;
	std::bitset<nKey> keystates;
	std::queue<Event> keyBuffer;
	std::queue<char> charBuffer;
};

