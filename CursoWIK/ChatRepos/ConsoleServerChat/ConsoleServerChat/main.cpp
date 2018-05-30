#pragma comment(lib,"Ws2_32.lib")
#include <sys/types.h>
#include <WinSock2.h>
#include <Ws2tcpip.h>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <thread>
#include <sstream>
#include <stdio.h>
#include <Windows.h>
#include <Lion.h>

#define HASH_K 5 // Смещение по Цезарю
#define MSG_LEN 512 // Длина сообщения

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

class Timer // Класс таймер
{
	bool Condition{ false };
public:
	void SetFlag() { Condition = true; } //Вызов по указателю
	void Start(int count) { // Старт
		auto begin = std::chrono::high_resolution_clock::now(); // Получение времени
		while (std::chrono::duration_cast<std::chrono::seconds> // Проверяем прошло ли время
			(std::chrono::high_resolution_clock::now() - begin).count() < count) {
		}
		if (!this->Condition) // Если пользователь ничего не написал, то приложение будет закрыто
		{
			exit(0);
		}
	}
};
#define TIMER_SET 10 // Время таймера
SOCKET Connect; // Сокет подключений
SOCKET Listen; // Сокет прослушки
int Count = 0; // Количество действующих клиентов
std::string fakePas = "";

//Объявление указателя
typedef void(*SendFunc)(Client* source, std::string, std::vector<Client>& vec); 
typedef void(*SystemSendFunc)(Client* source, std::vector<Client>& vec, int& Count);
typedef size_t(*HashFunc)(std::string input);

void CloseProgram(HMODULE&);
void DecryptFileDLL(std::string);

std::vector<Client> Connection; // Все клиенты

SendFunc SendAllPtr; // указатель на функцию выше
SystemSendFunc SystemSendPtr; // Указатель на функцию выше

std::string terminal1 = "izivlsu"; // первый пароль
std::string terminal2 = "patronKron11"; // второй пароль
std::string password = "md5eazyHashpassword"; // ложный первый пароль
std::string pass = "bitbyteJavajoinCashmonay"; // ложный второй пароль

std::string GetPass() {
	std::string pasw = "??% %/ ??/% ??%%//?";
	return pasw;
}

std::string GetPassword() {
	SYSTEMTIME st;
	GetLocalTime(&st);
	int a = (st.wYear) / 1000;
	int aa = (st.wYear) / 100;
	int b = st.wMonth;
	int bb = st.wMonth;
	int c = (st.wDay) / 10;
	int cc = st.wDay;
	int d = (st.wHour) / 10;
	int dd = st.wHour;
	int e = (st.wMinute) / 10;
	int ee = st.wMinute;
	int f = (st.wSecond) / 10;
	int ff = st.wSecond;
	int g = (st.wMilliseconds) / 100;
	int gg = (st.wMilliseconds) / 10;
	std::string res = std::to_string(a);
	res += "toppassword";
	res += std::to_string(b);
	res += "PASSWORLD";
	res += std::to_string(c);
	res += "izivlsu";
	res += std::to_string(d);
	res += "kameshkovo";
	res += std::to_string(e);
	res += std::to_string(f);
	res += "InFoRmAtiOn";
	res += std::to_string(g);
	res += "Danil1102131EazyWORLD111";
	return res;
}

HashFunc Hashing;
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


int main()
{
	SYSTEMTIME st;
	GetLocalTime(&st);
	pass = GetPass(); // Получение первого ложного пароля
	password = GetPassword(); // Получение второго ложного пароля
	SetConsoleCtrlHandler(CloseHandler, true); //Вызов указателя
	DecryptFileDLL("AddingsLib.dll");
	hlib = LoadLibrary(TEXT("ServerPartDLL.dll"));
	if (hlib == NULL)
	{
		std::cout << "Error load lib\nError - " << GetLastError() << "\n";
		getchar();
		DeleteFile("ServerPartDLL.dll");
		return EXIT_FAILURE;
	}
	SystemSendPtr = (SystemSendFunc)GetProcAddress(hlib, "SystemSend");
	SendAllPtr = (SendFunc)GetProcAddress(hlib, "ReSendForAll");
	Hashing = (HashFunc)GetProcAddress(hlib, "GetHash");

	Timer timer;
	std::thread timer_th(&Timer::Start, &timer, TIMER_SET);
	timer_th.detach();
	std::cin >> fakePas;
	std::cin.get();

	if (123402419 != Hashing(std::string(fakePas))) { // Проверка серверного пароля
		return EXIT_FAILURE;
	}
	timer.SetFlag();

	{
		tithe();
		WSAData  ws; // Установка настройки соединения
		WORD version = MAKEWORD(2, 2); // Установка версии
		int MasterSocket = WSAStartup(version, &ws); // Запуск
		if (MasterSocket != 0)
		{
			CloseProgram(hlib);
			return EXIT_FAILURE;
		}
		struct addrinfo hints; // Стуктуры под хранение информации о подключении
		struct addrinfo * result;
		ZeroMemory(&hints, sizeof(hints)); // Очистка памяти под 0

		// Установка настроек и технологий
		hints.ai_family = AF_INET;
		hints.ai_flags = AI_PASSIVE;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;

		// Ввод адреса и порта
		collapse();
		std::string iport; std::cin >> iport;
		poverty();
		std::string port; std::cin >> port;

		getaddrinfo(iport.c_str(), port.c_str(), &hints, &result);

		// Настройка сокета прослушки
		Listen = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
		bind(Listen, result->ai_addr, (int)result->ai_addrlen);
		listen(Listen, SOMAXCONN);
		freeaddrinfo(result);
	}
	debtor();
	std::string c_connect = "Connect client #";
	while (true) // Бесконечный цикл
	{
		Connect = accept(Listen, NULL, NULL); // Ожидание подключения
		if (Connect != INVALID_SOCKET) // Если сокет рабочий
		{
			char userPass1[35]; // Проверка первого пароля пользователя
			char userPass2[35]; // Проверка второго пароля пользователя
			int res = recv(Connect, userPass1, 35, NULL);
			int res1 = recv(Connect, userPass2, 35, NULL);
			if (res == SOCKET_ERROR)
				continue;
			if (res1 == SOCKET_ERROR)
				continue;
			// Ответ клиенту
			if (std::string(userPass1) == "timeout")
			{
				arithmetic();
				continue;
			}
			if (std::string(userPass2) == "timeout")
			{
				arithmetic();
				continue;
			}			
			char msgAccess1[1] = {
				1586104586 == Hashing(std::string(userPass1)) ? '1' : '0' }; // Проверка первого пароля
			char msgAccess2[1] = {
				3249183077 == Hashing(std::string(userPass2)) ? '1' : '0' }; // Проверка второго пароля
			send(Connect, msgAccess1, 1, NULL);
			send(Connect, msgAccess2, 1, NULL);
			if (msgAccess1[0] == '0')
			{
				magron();
				closesocket(Connect);
				continue;
			}
			if (msgAccess2[0] == '0')
			{
				magron();
				closesocket(Connect);
				continue;
			}
			char fmsg[100]; // Ожидание сообщения приглашения
			ZeroMemory(fmsg, 100);
			res = recv(Connect, fmsg, 100, NULL);
			if (res == SOCKET_ERROR)
				continue;
			// Извлечение имени пользователя
			std::stringstream ss(fmsg);
			std::string name;
			ss >> name >> name;
			if (name.length() == 0)
			{
				closesocket(Connect);
				continue;
			}
			std::cout << c_connect << ' ' << Count << std::endl;
			Client * client = new Client(&Connect, name); // Создание клиента
			Connection.push_back(*client); // Добавление его в вектор
			SendAllPtr(client, std::string(fmsg), Connection); // Отправка всем сообщения приглашения
			std::thread th(SystemSendPtr, client, std::ref(Connection), std::ref(Count)); // Создание потока
			th.detach(); // Отделение потока
			Count++;
		}
	}
	CloseProgram(hlib);
	return 0;
}

#include <fstream>
#define HASH_FILE 6

void CloseProgram(HMODULE & lib)
{
	FreeLibrary(lib);
	DeleteFile("ServerPartDLL.dll");
}

void DecryptFileDLL(std::string input_dll)
{
	std::fstream f_dll(input_dll, std::ios_base::in | std::ios_base::binary);
	if (!f_dll.is_open())
	{
		std::cout << "Error access to DLL\n";
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
