#pragma once
#include <Windows.h>
HANDLE GetNoitaHandle();
void PatchSeedGen(HANDLE hNoita);
void UnpatchSeedGen(HANDLE hNoita);
int ReadSeed(HANDLE hNoita);
void WriteSeed(HANDLE hNoita, int value);