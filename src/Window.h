#pragma once 

#include "NeonWin.h"
#include "NeonExcpetion.h"
#include "Keyboard.h"
#include "Mouse.h"

#define NEONWND_EXCEPT( hr ) Window::Exception( __LINE__, __FILE__, hr )
#define NEONWND_LAST_EXCEPT( hr ) Window::Exception( __LINE__, __FILE__, GetLastError() )

class Window
{
public:
	class Exception : public NeonException
	{
	public:
		Exception(int line, const char* file, HRESULT hr) noexcept;
		const char* what() const noexcept override;
		virtual const char* GetType() const noexcept;
		static std::string TranslateErrorCode( HRESULT hr ) noexcept;
		HRESULT GetErrorCode() const noexcept;
		std::string GetErrorString() const noexcept;
	private:
		HRESULT hr;
	};


private:
	// Singleton : Need one class only
	class WindowClass
	{
	private:
		WindowClass() noexcept;
		~WindowClass();
		WindowClass( const WindowClass&)  = delete; // No copy constructor
		WindowClass& operator=( const WindowClass& ) = delete;
		static constexpr const char* wndClassName = "Neon DirectX11 Engine Window";
		static WindowClass wndClass; // Singleton
		HINSTANCE hInst;
	public:
		static const char* GetName() noexcept;
		static HINSTANCE GetInstance() noexcept;
	};
public:
	Window( int width, int height, const char* name ) ;
	~Window();
	Window( const Window&)  = delete; // No copy constructor
	Window& operator=( const Window& ) = delete;
	void SetTitle(const std::string& title);
private:	
	// Functional programming 
	static LRESULT CALLBACK HandleMsgSetup( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam ) noexcept;
	static LRESULT CALLBACK HandleMsgThunk( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam ) noexcept;
	LRESULT HandleMsg( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

public:
	Keyboard kbd;
	Mouse mouse;
private:
	int width;
	int height;
	HWND hWnd;
};