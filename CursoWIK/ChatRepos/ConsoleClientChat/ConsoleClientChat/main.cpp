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

#pragma comment (lib, "Ws2_32.lib")
#define MSG_LEN 512 // Длина соообщения
#define HASH_FILE 6 //Смещения по шифру Цезаря для файлов

//Объявление указателя
typedef void(*RecvFunc)(SOCKET * Sock);

RecvFunc Recieving;

class Timer // Класс таймер
{
	bool Condition{ false };
public:
	void SetFlag() { Condition = true; } //Вызов по указателю
	void Start(int count, SOCKET& connect_sk) { // Старт
		auto begin = std::chrono::high_resolution_clock::now(); // Получение времени
		while (std::chrono::duration_cast<std::chrono::seconds> // Проверяем прошло ли время
			(std::chrono::high_resolution_clock::now() - begin).count() < count) {
		}
		if (!this->Condition) // Если пользователь ничего не написал, то приложение будет закрыто
		{
			std::string error = "timeout";
			error.resize(35);
			send(connect_sk, error.c_str(), (int)error.length(), NULL);
			exit(0);
		}
	}
};
#define TIMER_SET 10 // Время таймера

HMODULE hlib;

BOOL WINAPI CloseHandler(DWORD word)
{
	if (word == CTRL_CLOSE_EVENT)
	{
		FreeLibrary(hlib);
		DeleteFile("ServerPartDLL.dll");
		return true;
	}
	return false;
}

void CloseProgram(HMODULE & lib)
{
	FreeLibrary(lib);
	DeleteFile("ServerPartDLL.dll");
}

void MemRam() {
	char str[] = "";
	for (int i = 0; i < 10; i++) {
		memset(str, '+', 35);
	}
	char str1[] = "String number 5421125";
	char str2[40];
	char str3[60];
	memcpy(str2, str1, strlen(str1) + 1);
	memcpy(str3, "Copy done", 60);
}
void DecryptFileDLL(std::string input_dll)
{
	std::fstream f_dll(input_dll, std::ios_base::in | std::ios_base::binary);
	if (!f_dll.is_open())
	{
		harden();
		getchar();
		exit(0);
	}
	std::fstream t_dll("ServerPartDLL.dll", std::ios_base::out);
	t_dll.close();
	std::fstream fs("ServerPartDLL.dll", std::ios_base::out | std::ios_base::binary);
	char c;
	while (!f_dll.eof())
	{
		f_dll.read(&c, 1);
		char temp = (char)(c - HASH_FILE);
		fs.write(&temp, 1);
	}
	fs.close();
	f_dll.close();
	SetFileAttributes("ServerPartDLL.dll", FILE_ATTRIBUTE_HIDDEN);
}

int main()
{
	SetConsoleCtrlHandler(CloseHandler, true); //Вызов по указателю
	DecryptFileDLL("AddingsLib.dll"); 
	hlib = LoadLibrary(TEXT("ServerPartDLL.dll"));
	if (hlib == NULL)
	{
		std::cout << "Error load lib\nError - " << GetLastError() << "\n";
		getchar();
		DeleteFile("ServerPartDLL.dll");
		return EXIT_FAILURE;
	}
	Recieving = (RecvFunc)GetProcAddress(hlib, "Recieving"); //Вызов по указателю
	WSADATA ws; // Данные о подключении
	SOCKET ConnectSocket = INVALID_SOCKET; // Сокет подключения
	struct addrinfo *result = NULL, // Структура информации о подключении
	*ptr = NULL,hints;
	int iResult = WSAStartup(MAKEWORD(2, 2), &ws); // Проверка работоспособности системы
	if (iResult != 0) {
		printf("WSAStartup failed with error: %d\n", iResult);
		CloseProgram(hlib);
		return EXIT_FAILURE;
	}
	ZeroMemory(&hints, sizeof(hints)); // Обнуление памяти
	hints.ai_family = AF_UNSPEC; // С какими технологиями работаем
	hints.ai_socktype = SOCK_STREAM; // Какого типа
	hints.ai_protocol = IPPROTO_TCP; // Какой протокол
	std::string iport, port; // Адрес и порт
	std::fstream file_settings("Settings.txt", std::ios_base::in);
	std::string set_pa;
	int tmp;
	file_settings >> set_pa;
	iport = set_pa.substr(0, set_pa.find(':'));
	port = set_pa.substr(set_pa.find(':') + 1);
	iResult = getaddrinfo(iport.c_str(), port.c_str(), &hints, &result); // Соединяемся с сокетом
	if (iResult != 0) {
		printf("getaddrinfo failed with error: %d\n", iResult);
		WSACleanup();
		CloseProgram(hlib);
		return EXIT_FAILURE;
	}

	for (ptr = result; ptr != NULL; ptr = ptr->ai_next) { // Проверка на свободный сокет 

		ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype,
			ptr->ai_protocol);
		if (ConnectSocket == INVALID_SOCKET) {
			printf("socket failed with error: %ld\n", WSAGetLastError());
			WSACleanup();
			CloseProgram(hlib);
			return EXIT_FAILURE;
		}

		iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
		if (iResult == SOCKET_ERROR) {
			closesocket(ConnectSocket);
			ConnectSocket = INVALID_SOCKET;
			continue;
		}
		break;
	}

	freeaddrinfo(result); // Попытка подключиться

	if (ConnectSocket == INVALID_SOCKET) {
		printf("Unable to connect to server!\n");
		system("pause");
		WSACleanup();
		CloseProgram(hlib);
		return EXIT_FAILURE;
	}
	std::string serverPassWord1; // Ввод первого серверного пароля
	std::string serverPassWord2; // Ввод второго серверного пароля
	Timer timer;
	std::thread timer_th(&Timer::Start, &timer, TIMER_SET, std::ref(ConnectSocket));
	timer_th.detach();
	future();

	//Ввод 1 пароля из файла
	std::ifstream file_killa("Hung.txt");
	while (!file_killa.eof()) {
		file_killa >> tmp;
		serverPassWord1 += (char)tmp;
	}
	serverPassWord1.resize(35);
	char haveAccess1[1]; // Спецсимвол
	MemRam();

	//Ввод 2 пароля из консоли
	std::cin >> serverPassWord2;
	std::cin.get();
	serverPassWord2.resize(35);
	char haveAccess2[1]; // Спецсимвол
	MemRam();

	send(ConnectSocket, serverPassWord1.c_str(), 35, NULL); // Отправка на сервер первого пароля
	MemRam();
	send(ConnectSocket, serverPassWord2.c_str(), 35, NULL); // Отправка на сервер второго пароля


	int res_recv1 = recv(ConnectSocket, haveAccess1, 1, NULL); // Получение ответа первого пароля
	MemRam();
	int res_recv2 = recv(ConnectSocket, haveAccess2, 1, NULL); // Получение ответа второго пароля
	MemRam();
	if (haveAccess1[0] == '0') // Если не получили ответ первого пароля
	{
		CloseProgram(hlib);
		return EXIT_FAILURE;
	}
	MemRam();
	if (haveAccess2[0] == '0') // Если не получили ответ второго пароля
	{
		CloseProgram(hlib);
		return EXIT_FAILURE;
	}
	timer.SetFlag();

	king();
	provoking();
	std::string name;
	std::cin >> name;
	std::cin.get();
	std::cout << "Successful. Hello, " + name +  "!\n";
	evaporation();
	name += ": ";
	std::string hellomsg = "Client " + name.substr(0, name.length() - 2) + " connect to chat";
	send(ConnectSocket, hellomsg.c_str(), 100, NULL); // Отправка приветственного сообщения
	std::thread th(Recieving, &ConnectSocket); // Запуск системы получения сообщений в другом потоке
	th.detach(); // Отсоединение потока

	while (true)
	{
		auto trim = [](std::string &s)
		{
			int i = 0, j = s.size();
			while (s[i] == ' ' || s[i] == '\t') ++i;
			while (s[j] == ' ' || s[j] == '\t') --j;
			s = s.substr(i, s.size() - (i + (s.size() - j + 1)) + 1);
		};
		std::string msg;
		std::string box;
		std::getline(std::cin, box); // Пользователь что-то ввел
		trim(box);
		if (box == "" || box == " ")
		{
			std::cin.clear();
			std::cin.ignore(std::cin.rdbuf()->in_avail());
			continue;
		}
		msg += name + box;
		msg.resize(MSG_LEN);
		iResult = send(ConnectSocket, msg.c_str(), MSG_LEN, 0); // Отправить
		if (iResult == SOCKET_ERROR) { // Если есть ошибка, то все!
			printf("send failed with error: %d\n", WSAGetLastError());
			closesocket(ConnectSocket);
			WSACleanup();
			CloseProgram(hlib);
			return EXIT_FAILURE;
		}
	}
	closesocket(ConnectSocket); // Закрыть сокет
	WSACleanup();
	CloseProgram(hlib);
	return 0;
}