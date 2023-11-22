#pragma once
#include <unordered_map>
#include <Windows.h>
#include <string>


class WindowsMessageMap
{
public:
	WindowsMessageMap() noexcept;
	std::string operator()(DWORD msg, LPARAM lParam, WPARAM wParam) const;
private:
	std::unordered_map<DWORD, std::string> map;

};