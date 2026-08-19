#include "globals.h"
#include "Bridge.h"
#include <string>
#include <iostream>
#include <stdlib.h>
#include <vector>
#include "ConsoleCol.h"
#include <iterator>
#include <fstream>
#include <intrin.h>
#include <Tlhelp32.h>
#include <CommCtrl.h>
#include <Wininet.h>
#include <fstream>
#include <sstream>
#include <Psapi.h>
#pragma comment(lib, "wininet.lib")
using namespace std;

using Bridge::m_rL;
using Bridge::m_L;

void PushGlobal(DWORD rL, lua_State* L, const char* s)
{
	r_lua_getglobal(rL, s);
	Bridge::push(rL, L, -1);
	lua_setglobal(L, s);
	r_lua_pop(rL, 1);
}

static int UserDataGC(lua_State *Thread) {
	void *UD = lua_touserdata(Thread, 1);
	int GarbageCount;
	if (m_rL) {
		r_lua_rawgeti(m_rL, LUA_REGISTRYINDEX, (int)UD);
		if (r_lua_type(m_rL, -1) <= R_LUA_TNIL) {
			lua_pushnil(Thread);
			lua_rawseti(Thread, LUA_REGISTRYINDEX, (int)UD);
			//printf(("GC: " + std::to_string(GarbageCount) + "\n").c_str());
		}
	}
	return 0;
}

int IndexGetRawMetatable(lua_State *L) {
	DWORD luaState = m_rL;
	double Num = lua_tonumber(L, lua_upvalueindex(1));
	printf("index calledd1\n");
	r_lua_rawgeti(luaState, LUA_REGISTRYINDEX, Num);
	r_lua_getmetatable(luaState, -1);
	r_lua_getfield(m_rL, -1, lua_tostring(L, -1));
	Bridge::push(L, luaState, -1);
	r_lua_settop(m_rL, 0);
	return 1;
}

int NewINdexGetRawmetatble(lua_State *L) {
	DWORD luaState = m_rL;
	double Num = lua_tonumber(L, lua_upvalueindex(1));
	const char* key = lua_tostring(L, -2);
	printf("key: %s\n", key);
	r_lua_rawgeti(luaState, LUA_REGISTRYINDEX, Num);
	r_lua_getmetatable(luaState, -1);
	r_lua_pushstring(luaState, key);
	Bridge::push(L, m_rL, -1);
	r_lua_settable(luaState, -3);
	r_lua_settop(m_rL, -2);
	return 1;
}

int Lua_WrapGlobal(lua_State* L) {
	std::string globalName = lua_tostring(L, -1);
	PushGlobal(m_rL, m_L, globalName.c_str());
	return 1;
}

static int GlobalsIndex(lua_State* Thread) {
	int RThread = r_lua_newthread(m_rL);
	r_lua_getglobal(RThread, lua_tostring(Thread, 2));
	if (r_lua_type(RThread, -1) > R_LUA_TNIL) {
		Bridge::push(RThread, Thread, -1);
		return 1;
	}
	return 0;
}

std::string replace(std::string subject, const std::string& search, const std::string& replace) {
	size_t pos = 0;
	while ((pos = subject.find(search, pos)) != std::string::npos) {
		subject.replace(pos, search.length(), replace);
		pos += replace.length();
	}
	return subject;
}

void Execute(std::string Source) {
	Source = "spawn(function() script = Instance.new('LocalScript')\r\n" + Source + "\r\nend)";
	Source = replace(Source, "game.Players.LocalPlayer:WaitForChild(\"PlayerGui\")", "game.CoreGui");
	Source = replace(Source, "game:GetObjects", "GetObjects");
	Source = replace(Source, "game:HttpGet", "HttpGet");
	Source = replace(Source, "game:HttpGetAsync", "HttpGet");
	Source = replace(Source, "Game:", "game:");
	Source = replace(Source, ", true))", "))");
	if (luaL_loadstring(m_L, Source.c_str())) {
		return;
	}
	else {
		lua_pcall(m_L, 0, 0, 0);
	}
}

void Execute_Script(std::string Source) {
	Execute(Source);
}

int epic_loadstring(lua_State* LS) {
	if (lua_gettop(LS) == 0) {
		luaL_error(LS, "'loadstring' needs 1 string argument.");
		return 0;
	}
	Execute_Script(std::string(lua_tostring(LS, -1)));
	return 0;
}

static int Custom_GetObjects(lua_State* L) {
	auto asset = lua_tostring(L, -1);
	auto hold = std::string("return {game:GetService('InsertService'):LoadLocalAsset('") + std::string(asset) + std::string("')}");
	luaL_dostring(L, hold.c_str());
	return 1;
}

/*static int Lua_GetObjects(lua_State* L) {
	r_lua_getfield(m_rL, -10002, "game");
	r_lua_getfield(m_rL, -1, "GetService");
	r_lua_pushvalue(m_rL, -2);
	r_lua_pushstring(m_rL, "InsertService");
	r_lua_call(m_rL, 2, 1);
	r_lua_getfield(m_rL, -1, "LoadLocalAsset");
	r_lua_pushvalue(m_rL, -2);
	r_lua_pushstring(m_rL, lua_tostring(L, -1));
	r_lua_call(m_rL, 2, 1);
	lua_createtable(L, 0, 0);
	Bridge::push(m_rL, L, -1);
	lua_insert(L, -1);
	return 1;
};*/

DWORD WINAPI input(PVOID lvpParameter)
{
	string WholeScript = "";
	HANDLE hPipe;
	char buffer[999999];
	DWORD dwRead;
	hPipe = CreateNamedPipe(TEXT("\\\\.\\pipe\\IDonotSuicideWithNiggers"), 
		PIPE_ACCESS_DUPLEX | PIPE_TYPE_BYTE | PIPE_READMODE_BYTE,
		PIPE_WAIT,
		1,
		999999,
		999999,
		NMPWAIT_USE_DEFAULT_WAIT,
		NULL);
	while (hPipe != INVALID_HANDLE_VALUE)
	{
		if (ConnectNamedPipe(hPipe, NULL) != FALSE)
		{
			while (ReadFile(hPipe, buffer, sizeof(buffer) - 1, &dwRead, NULL) != FALSE)
			{
				buffer[dwRead] = '\0';
				try {
					try {
						WholeScript = WholeScript + buffer;
					}
					catch (...) {
					}
				}
				catch (std::exception e) {

				}
				catch (...) {

				}
			}
			Execute_Script(WholeScript);
			WholeScript = "";
		}
		DisconnectNamedPipe(hPipe);
	}
}

inline std::string download_url(const std::string &url) {
	HINTERNET interwebs = InternetOpenA("Mozilla/5.0", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, NULL);
	std::string rtn;

	if (interwebs) {
		HINTERNET url_file = InternetOpenUrlA(interwebs, url.c_str(), NULL, NULL, NULL, NULL);

		if (url_file) {
			char buffer[2000];
			DWORD bytes_read;

			do {
				InternetReadFile(url_file, buffer, 2000, &bytes_read);
				rtn.append(buffer, bytes_read);
				memset(buffer, 0, 2000);
			} while (bytes_read);

			InternetCloseHandle(interwebs);
			InternetCloseHandle(url_file);
			std::string p = replace(rtn, "|n", "\r\n");
			return p;
		}
	}

	InternetCloseHandle(interwebs);
	std::string p = replace(rtn, "|n", "\r\n");
	return p;
}

int custom_httpget(lua_State* L) {
	std::string URL = std::string(lua_tostring(L, -1));
	if (URL.find("getauthticket") != string::npos) {
		luaL_error(L, "Auth Ticket Logger detected, Http request canceled.");
		return 0;
	}
	lua_pushstring(L, std::string(download_url(URL)).c_str());
	return 1;
}

/*
#include <windows.h>
#include <string>
#include <stdio.h>
using std::string;

#pragma comment(lib,"ws2_32.lib")

HINSTANCE hInst;
WSADATA wsaData;
void mParseUrl(char *mUrl, string &serverName, string &filepath, string &filename);
SOCKET connectToServer(char *szServerName, WORD portNum);
int getHeaderLength(char *content);
char *readUrl2(char *szUrl, long &bytesReturnedOut, char **headerOut);


int r_httpget(lua_State *L)
{
	if (!lua_isstring(L, 1))
		return 1;
	const int bufLen = 1024;
	const char *szUrl = lua_tostring(L, 1); // Don't mind this error, it will always error for some reason
	long fileSize;
	char *memBuffer, *headerBuffer;
	FILE *fp;
	memBuffer = headerBuffer = NULL;
	if (WSAStartup(0x101, &wsaData) != 0)
		return -1;
	memBuffer = readUrl2((char *)szUrl, fileSize, &headerBuffer);
	lua_pushstring(L, memBuffer);
	if (fileSize != 0)
	{
		fp = fopen("downloaded.file", "wb");
		fwrite(memBuffer, 1, fileSize, fp);
		fclose(fp);
		delete(memBuffer);
		delete(headerBuffer);
	}
	WSACleanup();
	return 1;
}

void mParseUrl(char *mUrl, string &serverName, string &filepath, string &filename)
{
	string::size_type n;
	string url = mUrl;

	if (url.substr(0, 7) == "http://")
		url.erase(0, 7);

	if (url.substr(0, 8) == "https://")
		url.erase(0, 8);

	n = url.find('/');
	if (n != string::npos)
	{
		serverName = url.substr(0, n);
		filepath = url.substr(n);
		n = filepath.rfind('/');
		filename = filepath.substr(n + 1);
	}

	else
	{
		serverName = url;
		filepath = "/";
		filename = "";
	}
}

SOCKET connectToServer(char *szServerName, WORD portNum)
{
	struct hostent *hp;
	unsigned int addr;
	struct sockaddr_in server;
	SOCKET conn;

	conn = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (conn == INVALID_SOCKET)
		return NULL;

	if (inet_addr(szServerName) == INADDR_NONE)
	{
		hp = gethostbyname(szServerName);
	}
	else
	{
		addr = inet_addr(szServerName);
		hp = gethostbyaddr((char*)&addr, sizeof(addr), AF_INET);
	}

	if (hp == NULL)
	{
		closesocket(conn);
		return NULL;
	}

	server.sin_addr.s_addr = *((unsigned long*)hp->h_addr);
	server.sin_family = AF_INET;
	server.sin_port = htons(portNum);
	if (connect(conn, (struct sockaddr*)&server, sizeof(server)))
	{
		closesocket(conn);
		return NULL;
	}
	return conn;
}

int getHeaderLength(char *content)
{
	const char *srchStr1 = "\r\n\r\n", *srchStr2 = "\n\r\n\r";
	char *findPos;
	int ofset = -1;

	findPos = strstr(content, srchStr1);
	if (findPos != NULL)
	{
		ofset = findPos - content;
		ofset += strlen(srchStr1);
	}

	else
	{
		findPos = strstr(content, srchStr2);
		if (findPos != NULL)
		{
			ofset = findPos - content;
			ofset += strlen(srchStr2);
		}
	}
	return ofset;
}

char *readUrl2(char *szUrl, long &bytesReturnedOut, char **headerOut)
{
	const int bufSize = 512;
	char readBuffer[bufSize], sendBuffer[bufSize], tmpBuffer[bufSize];
	char *tmpResult = NULL, *result;
	SOCKET conn;
	string server, filepath, filename;
	long totalBytesRead, thisReadSize, headerLen;
	mParseUrl(szUrl, server, filepath, filename);
	conn = connectToServer((char*)server.c_str(), 80);
	sprintf(tmpBuffer, "GET %s HTTP/1.0", filepath.c_str());
	strcpy(sendBuffer, tmpBuffer);
	strcat(sendBuffer, "\r\n");
	sprintf(tmpBuffer, "Host: %s", server.c_str());
	strcat(sendBuffer, tmpBuffer);
	strcat(sendBuffer, "\r\n");
	strcat(sendBuffer, "\r\n");
	send(conn, sendBuffer, strlen(sendBuffer), 0);
	totalBytesRead = 0;
	while (1)
	{
		memset(readBuffer, 0, bufSize);
		thisReadSize = recv(conn, readBuffer, bufSize, 0);

		if (thisReadSize <= 0)
			break;

		tmpResult = (char*)realloc(tmpResult, thisReadSize + totalBytesRead);

		memcpy(tmpResult + totalBytesRead, readBuffer, thisReadSize);
		totalBytesRead += thisReadSize;
	}

	headerLen = getHeaderLength(tmpResult);
	long contenLen = totalBytesRead - headerLen;
	result = new char[contenLen + 1];
	memcpy(result, tmpResult + headerLen, contenLen);
	result[contenLen] = 0x0;
	char *myTmp;

	myTmp = new char[headerLen + 1];
	strncpy(myTmp, tmpResult, headerLen);
	myTmp[headerLen] = NULL;
	delete(tmpResult);
	*headerOut = myTmp;

	bytesReturnedOut = contenLen;
	closesocket(conn);
	return(result);
}

*/

void writeMem(DWORD addr, char* val, int byten) {
	unsigned long oldp;
	VirtualProtect((LPVOID)addr, byten, PAGE_EXECUTE_READWRITE, &oldp);
	memcpy((LPVOID)addr, val, +byten);
	VirtualProtect((LPVOID)addr, byten, oldp, &oldp);
}

void ConsoleBypass(const char* Title) {
	DWORD superskiddoisachink;
	VirtualProtect((PVOID)&FreeConsole, 1, PAGE_EXECUTE_READWRITE, &superskiddoisachink);
	*(BYTE*)(&FreeConsole) = 0xC3;
	AllocConsole();
	SetConsoleTitleA(Title);
	freopen("CONOUT$", "w", stdout);
	freopen("CONIN$", "r", stdin);
	HWND ConsoleHandle = GetConsoleWindow();
	::SetWindowPos(ConsoleHandle, HWND_TOPMOST, 0, 0, 0, 0, SWP_DRAWFRAME | SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
	::ShowWindow(ConsoleHandle, SW_NORMAL);
}

void GetFile(const char* dllName, const char* fileName, char* buffer, int bfSize) {
	GetModuleFileName(GetModuleHandle(dllName), buffer, bfSize);
	if (strlen(fileName) + strlen(buffer) < MAX_PATH) {
		char* pathEnd = strrchr(buffer, '\\');
		strcpy(pathEnd + 1, fileName);
	}
	else {
		*buffer = 0;
	}
}



void r_lua_pushobject(DWORD pRobloxState, TValue *value) {
	auto &topa = *(TValue**)(pRobloxState + top);
	*topa = *value;
	++topa;
}

inline void r_lua_getservice(DWORD state, const char* s)
{
	r_lua_getfield(m_rL, -10002, "game");
	r_lua_getfield(m_rL, -1, "GetService");
	r_lua_pushvalue(m_rL, -2);
	r_lua_pushstring(m_rL, s);
	r_lua_pcall(m_rL, 2, 1, 0);
}

static int GC(lua_State *Thread) {
	void *UD = lua_touserdata(Thread, 1);
	if (m_rL) {
		r_lua_rawgeti(m_rL, LUA_REGISTRYINDEX, (int)UD);
		if (r_lua_type(m_rL, -1) <= R_LUA_TNIL) {
			lua_pushnil(Thread);
			lua_rawseti(Thread, LUA_REGISTRYINDEX, (int)UD);
		}
	}
	return 0;
}

static int EQ(lua_State *Thread) {
	lua_pushboolean(Thread, (*(DWORD *)lua_touserdata(Thread, 1) == *(DWORD *)lua_touserdata(Thread, 2)));
	return 1;
}

void r_lua_emptystack(DWORD luaState)
{
	r_lua_settop(luaState, 0);
};

void PushCClosure(const void* func, DWORD n) {
	try {
		r_lua_pushcclosure(m_rL, (int)func, n);
	}
	catch (...) {};
}

int custom_getrawmetatable(lua_State *L) {
	Bridge::push(L, m_rL, 1);
	if (!r_lua_getmetatable(m_rL, -1))
		lua_pushnil(L);
	Bridge::push(m_rL, L, -1);
	return 1;
}

int getRawMetaTable(lua_State *L) {
	Bridge::push(L, m_rL, 1);

	if (r_lua_getmetatable(m_rL, -1) == 0) {
		lua_pushnil(L);
		return 0;
	}
	Bridge::push(m_rL, L, -1);

	return 1;
}

r_TValue* r_gettop(int r_lua_State) {
	return *(r_TValue**)(r_lua_State + 16);
}
int setrawmetatable_impl(lua_State* r_lua_State) {
	if (r_lua_gettop(m_rL) < 2) {
		r_lua_pushboolean(m_rL, 0);
		r_lua_pushstring(m_rL, "2 or more arguments expected");
		return 2;
	}

	r_TValue* top = r_gettop(m_rL);
	r_TValue* obj1 = top - 1;
	r_TValue* obj0 = top - 2;

	if (!(obj0->tt == R_LUA_TTABLE || obj0->tt == R_LUA_TUSERDATA) || !(obj1->tt == R_LUA_TNIL || obj1->tt == R_LUA_TTABLE)) {
		r_lua_pushboolean(m_rL, 0);
		r_lua_pushstring(m_rL, "bad argument types");
		return 2;
	}

	r_lua_setmetatable(m_rL, 1);
	r_lua_pushboolean(m_rL, 1);
	return 1;
}

int getgenv(lua_State *L) {
	lua_pushvalue(L, LUA_GLOBALSINDEX);
	Bridge::push(m_rL, L, -1);
	return 1;
}

int getreg(lua_State *L) {
	lua_pushvalue(L, LUA_REGISTRYINDEX);
	Bridge::push(m_rL, L, -1);
	return 1;
}

int getrenv(lua_State* L) {
	lua_pushvalue(L, int(m_rL));
	Bridge::push(m_rL, L, -1);
	return 1;
}

void lua_setfield(DWORD L, int idx, const char *k) {
	r_lua_pushvalue(L, idx);
	if (r_lua_getmetatable(L, -1)) {
		r_lua_getfield(L, -1, "__newindex");
		r_lua_pushvalue(L, -3);
		r_lua_pushstring(L, k);
		r_lua_pushvalue(L, -6);
		r_lua_pop(L, 3);
	}
	else {
		std::cout << "0\n";
		r_lua_pop(L, 1);
		lua_setfield(L, idx, k);
	}
}

bool CompareData(const char* Data, const char* Pattern, const char* Mask) {
	while (*Mask) {
		if (*Mask != '?') {
			if (*Data != *Pattern) {
				return false;
			};
		};
		++Mask;
		++Data;
		++Pattern;
	};
	return true;
};


DWORD ScanForScriptContext(const char* SCVFT_Offsetted) {
	MEMORY_BASIC_INFORMATION BasicMemoryInformation = {};
	SYSTEM_INFO SystemInformation = {};
	GetSystemInfo(&SystemInformation);
	DWORD StartingMemorySearchPosition = (DWORD)SystemInformation.lpMinimumApplicationAddress;
	DWORD MaximumSearchBoundary = (DWORD)SystemInformation.lpMaximumApplicationAddress;
	do {
		while (VirtualQuery((void*)StartingMemorySearchPosition, &BasicMemoryInformation, sizeof(BasicMemoryInformation))) {
			if ((BasicMemoryInformation.Protect & PAGE_READWRITE) && !(BasicMemoryInformation.Protect & PAGE_GUARD)) {
				for (DWORD Key = (DWORD)(BasicMemoryInformation.BaseAddress); ((Key - (DWORD)(BasicMemoryInformation.BaseAddress) < BasicMemoryInformation.RegionSize)); ++Key) {
					if (CompareData((const char*)Key, SCVFT_Offsetted, "xxxx")) {
						return Key;
					};
				};
			};
			StartingMemorySearchPosition += BasicMemoryInformation.RegionSize;
		};
	} while (StartingMemorySearchPosition < MaximumSearchBoundary);
	return 0x0;
};

DWORD ScriptContext;
DWORD ScriptContextVFTable = x(0x1CFEE1C);

namespace Hook {
	void* placeHook(DWORD address, void* hook, bool revert = false) {
		DWORD oldprot;
		if (!revert) {
			void* oldmem = new void*;
			void* result = new void*;
			memcpy(oldmem, (void*)address, sizeof(void*) * 4);
			VirtualProtect((LPVOID)address, 1, PAGE_EXECUTE_READWRITE, &oldprot);
			*(char*)address = 0xE9; *(DWORD*)(address + 1) = (DWORD)hook - address - 5;
			memcpy(result, oldmem, sizeof(void*) * 4);
			VirtualProtect((LPVOID)address, 1, oldprot, &oldprot);
			return result;
		}
		else {
			VirtualProtect((LPVOID)address, 1, PAGE_EXECUTE_READWRITE, &oldprot);
			memcpy((void*)address, hook, sizeof(void*) * 4);
			VirtualProtect((LPVOID)address, 1, oldprot, &oldprot);
			return NULL;
		}
	}

}
DWORD RLSorg = 0;
DWORD hookaddr = 0x80FCF0; //gettop addr

int gettop(DWORD rState)
{
	RLSorg = rState;
	return (*(DWORD *)(rState + 20) - *(DWORD *)(rState + 24)) >> 4;
}


DWORD hookStateIndex(DWORD hooklocation, int offset)
{
	return (hooklocation + offset) + *(DWORD *)(hooklocation + offset);
}

void main()
{
	void* old = Hook::placeHook(x(hookaddr), gettop);
	do { Sleep(1); } while (RLSorg == 0);
	Hook::placeHook(x(hookaddr), old, 1);
	m_rL = RLSorg;
	m_L = luaL_newstate();
	seti(m_rL, 6);
	Bridge::VehHandlerpush();
	luaL_openlibs(m_L);
	luaL_newmetatable(m_L, "garbagecollector");
	lua_pushcfunction(m_L, UserDataGC);
	lua_setfield(m_L, -2, "__gc");
	lua_pushvalue(m_L, -1);
	lua_setfield(m_L, -2, "__index");
	PushGlobal(m_rL, m_L, "printidentity");
	PushGlobal(m_rL, m_L, "game");
	PushGlobal(m_rL, m_L, "Game");
	PushGlobal(m_rL, m_L, "workspace");
	PushGlobal(m_rL, m_L, "Workspace");
	PushGlobal(m_rL, m_L, "Axes");
	PushGlobal(m_rL, m_L, "BrickColor");
	PushGlobal(m_rL, m_L, "CFrame");
	PushGlobal(m_rL, m_L, "Color3");
	PushGlobal(m_rL, m_L, "ColorSequence");
	PushGlobal(m_rL, m_L, "ColorSequenceKeypoint");
	PushGlobal(m_rL, m_L, "NumberRange");
	PushGlobal(m_rL, m_L, "NumberSequence");
	PushGlobal(m_rL, m_L, "NumberSequenceKeypoint");
	PushGlobal(m_rL, m_L, "PhysicalProperties");
	PushGlobal(m_rL, m_L, "Ray");
	PushGlobal(m_rL, m_L, "Rect");
	PushGlobal(m_rL, m_L, "Region3");
	PushGlobal(m_rL, m_L, "Region3int16");
	PushGlobal(m_rL, m_L, "TweenInfo");
	PushGlobal(m_rL, m_L, "UDim");
	PushGlobal(m_rL, m_L, "UDim2");
	PushGlobal(m_rL, m_L, "Vector2");
	PushGlobal(m_rL, m_L, "Vector2int16");
	PushGlobal(m_rL, m_L, "Vector3");
	PushGlobal(m_rL, m_L, "Vector3int16");
	PushGlobal(m_rL, m_L, "Enum");
	PushGlobal(m_rL, m_L, "Faces");
	PushGlobal(m_rL, m_L, "Instance");
	PushGlobal(m_rL, m_L, "math");
	PushGlobal(m_rL, m_L, "warn");
	PushGlobal(m_rL, m_L, "typeof");
	PushGlobal(m_rL, m_L, "type");
	PushGlobal(m_rL, m_L, "spawn");
	PushGlobal(m_rL, m_L, "Spawn");
	PushGlobal(m_rL, m_L, "print");
	PushGlobal(m_rL, m_L, "printidentity");
	PushGlobal(m_rL, m_L, "ypcall");
	PushGlobal(m_rL, m_L, "Wait");
	PushGlobal(m_rL, m_L, "wait");
	PushGlobal(m_rL, m_L, "delay");
	PushGlobal(m_rL, m_L, "Delay");
	PushGlobal(m_rL, m_L, "tick");
	PushGlobal(m_rL, m_L, "LoadLibrary");
	PushGlobal(m_rL, m_L, "loadstring");
	PushGlobal(m_rL, m_L, "setrawmetatable");
	PushGlobal(m_rL, m_L, "getreg");
	PushGlobal(m_rL, m_L, "saveinstance");
	PushGlobal(m_rL, m_L, "getgenv");
	PushGlobal(m_rL, m_L, "DownloadString");
	PushGlobal(m_rL, m_L, "toclipboard");
	PushGlobal(m_rL, m_L, "Path");
	PushGlobal(m_rL, m_L, "setreadonly");
	PushGlobal(m_rL, m_L, "isreadonly");
	PushGlobal(m_rL, m_L, "httpget");
	lua_register(m_L, "Getobjects", Custom_GetObjects);
	lua_register(m_L, "loadstring", epic_loadstring);
	lua_register(m_L, "WrapGlobal", Lua_WrapGlobal);
	lua_register(m_L, "httpget", custom_httpget);
	lua_register(m_L, "GlobalsIndex", GlobalsIndex);
	lua_register(m_L, "EQ", EQ);
	lua_register(m_L, "GC", GC);
	lua_register(m_L, "IndexGetRawMetatable", IndexGetRawMetatable);
	lua_register(m_L, "NewINdexGetRawmetatble", NewINdexGetRawmetatble);
	lua_register(m_L, "getrawmetatable", getRawMetaTable);
	lua_register(m_L, "getrawmetatablec", custom_getrawmetatable);
	lua_register(m_L, "setrawmetatable", setrawmetatable_impl);
	lua_register(m_L, "getreg", getreg);
	lua_register(m_L, "getrenv", getrenv);
	lua_register(m_L, "getgenv", getgenv);
	r_lua_pop(m_rL, 2);	
	lua_newtable(m_L);
	lua_setglobal(m_L, "_G");
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)input, NULL, NULL, NULL);
	luaL_dostring(m_L, "function GetObjects(assetId) local obj = game:GetService(\"InsertService\"):LoadLocalAsset(assetId) return { obj }; end");
	luaL_dostring(m_L, "game.StarterGui:SetCore('SendNotification', {Duration = 4; Title='Vain'; Text='Welcome to Vain!'})");
}

unsigned int ProtectSections(HMODULE Module) {
	MODULEINFO ModuleInfo;
	GetModuleInformation(GetCurrentProcess(), Module, &ModuleInfo, sizeof(ModuleInfo));
	uintptr_t Address = reinterpret_cast<uintptr_t>(Module);
	uintptr_t TermAddress = Address + ModuleInfo.SizeOfImage;
	MEMORY_BASIC_INFORMATION MemoryInfo;


	while (Address < TermAddress) {
		VirtualQuery(reinterpret_cast<void*>(Address), &MemoryInfo, sizeof(MemoryInfo));
		if (MemoryInfo.State == MEM_COMMIT && (MemoryInfo.Protect & (PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY))) {
			DWORD OldProtection;
			VirtualProtect(reinterpret_cast<void*>(Address), MemoryInfo.RegionSize, PAGE_EXECUTE_READ, &OldProtection);
		}
		Address = reinterpret_cast<uintptr_t>(MemoryInfo.BaseAddress) + MemoryInfo.RegionSize;
	}

	VirtualQuery(reinterpret_cast<void*>(MemoryInfo.AllocationBase), &MemoryInfo, sizeof(MemoryInfo));
	if (MemoryInfo.State != MEM_COMMIT || !(MemoryInfo.Protect & PAGE_EXECUTE_READ))
		return 0x400;
	return MemoryInfo.RegionSize - 0x400;
}
__forceinline void UnlinkModule(HINSTANCE Module
) {
	unsigned long PEB_DATA = 0;
	_asm {
		pushad;
		pushfd;
		mov eax, fs:[30h]
			mov eax, [eax + 0Ch]
			mov PEB_DATA, eax

			InLoadOrderModuleList :
		mov esi, [eax + 0Ch]
			mov edx, [eax + 10h]

			LoopInLoadOrderModuleList :
			lodsd
			mov esi, eax
			mov ecx, [eax + 18h]
			cmp ecx, Module
			jne SkipA
			mov ebx, [eax]
			mov ecx, [eax + 4]
			mov[ecx], ebx
			mov[ebx + 4], ecx
			jmp InMemoryOrderModuleList

			SkipA :
		cmp edx, esi
			jne LoopInLoadOrderModuleList

			InMemoryOrderModuleList :
		mov eax, PEB_DATA
			mov esi, [eax + 14h]
			mov edx, [eax + 18h]

			LoopInMemoryOrderModuleList :
			lodsd
			mov esi, eax
			mov ecx, [eax + 10h]
			cmp ecx, Module
			jne SkipB
			mov ebx, [eax]
			mov ecx, [eax + 4]
			mov[ecx], ebx
			mov[ebx + 4], ecx
			jmp InInitializationOrderModuleList

			SkipB :
		cmp edx, esi
			jne LoopInMemoryOrderModuleList

			InInitializationOrderModuleList :
		mov eax, PEB_DATA
			mov esi, [eax + 1Ch]
			mov edx, [eax + 20h]

			LoopInInitializationOrderModuleList :
			lodsd
			mov esi, eax
			mov ecx, [eax + 08h]
			cmp ecx, Module
			jne SkipC
			mov ebx, [eax]
			mov ecx, [eax + 4]
			mov[ecx], ebx
			mov[ebx + 4], ecx
			jmp Finished

			SkipC :
		cmp edx, esi
			jne LoopInInitializationOrderModuleList

			Finished :
		popfd;
		popad;
	}
}

BOOL APIENTRY DllMain(HMODULE Module, DWORD Reason, void* Reserved)
{
	ProtectSections(Module);
	switch (Reason)
	{
	case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls(Module);
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)main, NULL, NULL, NULL);
		break;
	case DLL_PROCESS_DETACH:
		break;
	default: break;
	}

	return TRUE;
}