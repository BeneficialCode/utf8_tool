#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <Windows.h>
#include <tchar.h>
#include <locale>
#include <regex>


std::string WstringToUtf8(const std::wstring& wstr) {
	int len = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), wstr.size(), nullptr, 0, nullptr, nullptr);
	len += 1;
	std::unique_ptr<char[]> buffer = std::make_unique<char[]>(len);
	memset(buffer.get(), 0, sizeof(char) * len);
	WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), wstr.size(), buffer.get(), len, nullptr, nullptr);
	std::string str(buffer.get());
	auto data = str.data();
	return str;
}

std::wstring Utf8ToWstring(const std::string& str) {
	int len = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, nullptr, 0);
	len += 1;
	std::unique_ptr<wchar_t[]> buffer = std::make_unique<wchar_t[]>(len);
	memset(buffer.get(), 0, sizeof(wchar_t) * len);
	MultiByteToWideChar(CP_UTF8, 0, str.c_str(), str.size(), buffer.get(), len);
	std::wstring wstr(buffer.get());
	return wstr;
}

std::string WstringToString(const std::wstring& wstr) {
	int len = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), wstr.size(), nullptr, 0, nullptr, nullptr);
	len += 1;
	std::unique_ptr<char[]> buffer = std::make_unique<char[]>(len);
	memset(buffer.get(), 0, sizeof(char) * len);
	WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), wstr.size(), buffer.get(), len, nullptr, nullptr);
	std::string str(buffer.get());
	return str;
}

int wmain(int argc, wchar_t* argv[], wchar_t* envp[])
{
	std::setlocale(LC_CTYPE, "");
	std::wstring cmd = argv[1];
	

	if (!_wcsicmp(L"-e", cmd.c_str())) {
		//std::wcout << wstr << std::endl;
		std::wstring wstr = argv[2];
		std::string utf8 = WstringToUtf8(wstr);
		wstr = Utf8ToWstring(utf8);
		for (int i = 0; i < utf8.length(); i++) {
			printf("\\%u", utf8[i] & 0xFF);
		}
		printf("\n");
	}
	else if (!_wcsicmp(L"-d", cmd.c_str())) {
		// -d \230\137\167\229\168\129\232\133\149
		//printf("%ws\n", wstr.c_str());
		for (int i = 0; i < argc - 2; i++) {
			std::wstring wstr = argv[2 + i];
			std::string str = WstringToString(wstr);
			std::regex pattern("\\d{3}"); // Æ¥ÅäÈý¸öÊý×Ö
			std::smatch result;
			std::string utf8;


			std::vector<char> vchars;
			while (std::regex_search(str, result, pattern)) {
				ULONG value = std::stoi(result.str());
				vchars.push_back(value & 0xFF);
				str = result.suffix();
			}
			utf8.resize(vchars.size());
			for (int i = 0; i < vchars.size(); i++) {
				utf8[i] = vchars[i] & 0xFF;
			}

			wstr = Utf8ToWstring(utf8);
			std::wcout << wstr << std::endl;
		}
	}
	system("pause");
	return 0;
}