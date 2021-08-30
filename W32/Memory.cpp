#include "Memory.h"
#include <string>
#include <tlhelp32.h>
HANDLE GetNoitaHandle()
{
	const std::string NoitaProc = "noita.exe";
	PROCESSENTRY32 processEntry;
	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	if (Process32First(snapshot, &processEntry))
	{
		while (Process32Next(snapshot, &processEntry)) {
			auto procName = std::string(processEntry.szExeFile);
			if (procName != NoitaProc) {
				continue;
			}
			else {
				return OpenProcess(PROCESS_ALL_ACCESS, false, processEntry.th32ProcessID);
			}
		}

	}

	return INVALID_HANDLE_VALUE;
}

// seed patches
// noita.exe + 3A8DC4 - A3 50E8FE00           -   mov [noita.exe+BEE850],eax
// noita.exe + 279077 - C7 05 50E8FE00 00000000 - mov [noita.exe + BEE850], 00000000

void PatchSeedGen(HANDLE hNoita)
{
	DWORD oldProtect, oldProtect2;
	VirtualProtectEx(hNoita, (void*)0x7a8dc4, 5, PAGE_EXECUTE_READWRITE, &oldProtect);
	VirtualProtectEx(hNoita, (void*)0x679077, 10, PAGE_EXECUTE_READWRITE, &oldProtect2);

	BYTE buffer1[5] = { 0x90, 0x90, 0x90, 0x90, 0x90 };
	BYTE buffer2[10] = { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 };

	WriteProcessMemory(hNoita, (void*)0x7a8dc4, buffer1, sizeof(buffer1), nullptr);
	WriteProcessMemory(hNoita, (void*)0x679077, buffer2, sizeof(buffer2), nullptr);

	VirtualProtectEx(hNoita, (void*)0x679077, 10, oldProtect2, &oldProtect2);
	VirtualProtectEx(hNoita, (void*)0x7a8dc4, 5, oldProtect, &oldProtect);

	FlushInstructionCache(hNoita, (void*)0x7a8dc4, 5);
	FlushInstructionCache(hNoita, (void*)0x679077, 10);
}

void UnpatchSeedGen(HANDLE hNoita)
{
	DWORD oldProtect, oldProtect2;
	VirtualProtectEx(hNoita, (void*)0x7a8dc4, 5, PAGE_EXECUTE_READWRITE, &oldProtect);
	VirtualProtectEx(hNoita, (void*)0x679077, 10, PAGE_EXECUTE_READWRITE, &oldProtect2);

	BYTE buffer1[5] = { 0xA3, 0x50, 0xE8, 0xFE, 0x00 };
	BYTE buffer2[10] = { 0xC7, 0x05, 0x50, 0xE8, 0xFE, 0x00, 0x00, 0x00, 0x00, 0x00 };

	WriteProcessMemory(hNoita, (void*)0x7a8dc4, buffer1, sizeof(buffer1), nullptr);
	WriteProcessMemory(hNoita, (void*)0x679077, buffer2, sizeof(buffer2), nullptr);

	VirtualProtectEx(hNoita, (void*)0x679077, 10, oldProtect2, &oldProtect2);
	VirtualProtectEx(hNoita, (void*)0x7a8dc4, 5, oldProtect, &oldProtect);

	FlushInstructionCache(hNoita, (void*)0x7a8dc4, 5);
	FlushInstructionCache(hNoita, (void*)0x679077, 10);
}

int ReadSeed(HANDLE hNoita)
{
	int seed;
	SIZE_T bytesRead;
	if (ReadProcessMemory(hNoita, (void*)0x00FEE850, &seed, sizeof(seed), &bytesRead)) {
		return seed;
	}
	return 0;
}

void WriteSeed(HANDLE hNoita, int value)
{
	WriteProcessMemory(hNoita, (void*)0x00FEE850, &value, sizeof(int), nullptr);
}