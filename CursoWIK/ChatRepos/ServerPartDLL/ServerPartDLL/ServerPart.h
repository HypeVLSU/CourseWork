#pragma once

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <Windows.h>
#include <fstream>
#include <Evolution.h>
#include <vector>
#include <boost/functional/hash/hash.hpp>

#pragma comment (lib, "Ws2_32.lib")
#include <algorithm>
#define MSG_LEN 512
#define HASH_K 5

extern "C"  __declspec(dllexport) void Recieving(SOCKET * Sock); // Получение сообщений

class Client // Класс клиента
{
private:
	SOCKET socket; // сокет
	std::string user_name; // имя
public:
	Client(SOCKET * in, std::string _name) :socket(*in), user_name(_name) {}
	Client() {}
	SOCKET GetSock() { return socket; }
	std::string GetName() { return user_name; }
	friend bool operator==(const Client& c1, const Client& c2)
	{
		return c1.socket == c2.socket;
	}
	friend bool operator!=(const Client& c1, const Client& c2)
	{
		return !(c1 == c2);
	}
};

extern "C"  __declspec(dllexport) void ReSendForAll(Client* source, std::string msg, std::vector<Client>& Connection);
extern "C"  __declspec(dllexport) void SystemSend(Client* client, std::vector<Client>& Connection, int & Count);
extern "C"  __declspec(dllexport) size_t GetHash(std::string input);