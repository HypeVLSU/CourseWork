#include "ServerPart.h"

void Recieving(SOCKET * Sock) // Получение сообщений
{
	char msg[MSG_LEN]; // контейнер под сообщения
	for (;; Sleep(75)) // бесконечный цикл получения
	{
		int iResult = recv(*Sock, msg, MSG_LEN, 0); // Ожидание сообщения
		if (iResult > 0) // Если что-то есть, то вывод
			std::cout << msg << std::endl;
	}
}

void ReSendForAll(Client* source, std::string msg, std::vector<Client>& Connection) // Отправить всем
{
	msg.resize(MSG_LEN); // Установка длины сообщения
	for (auto i = Connection.begin(); i != Connection.end(); i++) // обход всех в векторе и отправка им сообщений
		if (*i != *source && i->GetSock() != INVALID_SOCKET)
			send(i->GetSock(), msg.c_str(), MSG_LEN, NULL);
}

void SystemSend(Client* client, std::vector<Client>& Connection, int & Count) // Система отправки
{
	for (;; Sleep(75)) // Бесконечный цикл
	{
		char msg[MSG_LEN]; // сообщения
		ZeroMemory(msg, sizeof(char) * MSG_LEN); // очистка памяти
		int iResult = recv(client->GetSock(), msg, MSG_LEN, NULL); // получение сообщения
		std::cout << msg << std::endl; // вывод в консоль
		if (iResult == SOCKET_ERROR) // если ошибочный сокет
		{
			auto iter = std::find(Connection.begin(), Connection.end(), *client);
			if (iter != Connection.end())
				Connection.erase(iter);
			std::string msgOut = "User " + client->GetName() + " out"; // Сообщение выхода
			ReSendForAll(client, msgOut, Connection); // Отправить всем
			std::cout << msgOut << std::endl;
			closesocket(client->GetSock()); // Закрыть его
			delete client; // удалить клиента
			Count--;
			break;
			return;
		}
		else
			ReSendForAll(client, std::string(msg), Connection); // Отправить всем
	}
	std::cout << "Сlose Connect\n";
}

size_t GetHash(std::string input) // Получить хэш пароля через смещение
{
	boost::hash<std::string> h;
	return h(input);
}