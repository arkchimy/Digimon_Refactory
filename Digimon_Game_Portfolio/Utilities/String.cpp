#include "stdafx.h"
#include "String.h"

wstring String::ToWString(string data)
{
	wstring temp;
	temp.assign(data.begin(), data.end());
	return temp;
}

string String::ToString(wstring data)
{
	string temp;
	temp.assign(data.begin(), data.end());
	return temp;
}
