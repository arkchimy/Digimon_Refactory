#pragma once
class String
{
public:
	static wstring ToWString(string data);
	static string ToString(wstring data);
};