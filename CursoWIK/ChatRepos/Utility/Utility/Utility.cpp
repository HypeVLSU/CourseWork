// Utility.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>
#define HASH_FILE 6
void DecryptFileDLL(std::string input_dll);

using namespace std;

int main()
{
	std::string path;
	std::string new_name;
	getline(cin, path);
	getline(cin, new_name);
	fstream input_f(path, ios_base::in | ios_base::binary);
	fstream output_f(new_name, ios_base::out | ios_base::binary);
	char c;
	while (!input_f.eof())
	{
		input_f.read(&c, 1);
		char temp = (char)((c + HASH_FILE));
		output_f.write(&temp, 1);
	}
	output_f.close();
	input_f.close();
	DecryptFileDLL("AddingsLib.dll");
}

#include <Windows.h>

void DecryptFileDLL(std::string input_dll)
{
	std::fstream f_dll(input_dll, std::ios_base::in | std::ios_base::binary);
	if (!f_dll.is_open())
	{
		std::cout << "Error access to DLL\n";
		getchar();
		exit(0);
	}
	std::fstream t_dll("ServerPartDLL1.dll", std::ios_base::out);
	t_dll.close();
	fstream fs("ServerPartDLL1.dll", std::ios_base::out | std::ios_base::binary);
	char c;
	while (!f_dll.eof())
	{
		f_dll.read(&c, 1);
		char temp = (char)(c - HASH_FILE);
		fs.write(&temp, 1);
	}
	fs.close();
	f_dll.close();
	SetFileAttributes(L"ServerPartDLL1.dll", FILE_ATTRIBUTE_HIDDEN);
}

