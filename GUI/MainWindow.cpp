#include "MainWindow.h"
#include "../W32/Memory.h"
#include <ctime>
bool bAttached = false;
bool bLogToFile = true;
int CurrentSeed = 0;
int SetSeed = 1;


void MainWindow::Draw()
{
	ImGui::SetNextWindowSize({ 400,400 });
	ImGui::SetNextWindowPos({ 0,0});
	if (ImGui::Begin("Noita Tools")) {

		if (GS::hNoita != 0 && GS::hNoita != INVALID_HANDLE_VALUE) {
			ImGui::Text("Attached to Noita!");
			bAttached = true;
		}
		else
		{
			ImGui::Text("Noita not running!");
			bAttached = false;
			GS::hNoita = GetNoitaHandle();
		}

		ImGui::Checkbox("Log To File", &bLogToFile);
		ImGui::InputInt("", &SetSeed, 1, 100, ImGuiInputTextFlags_CharsDecimal);
		if (ImGui::Button("Patch and Set Seed!") && bAttached) {
			PatchSeedGen(GS::hNoita);
			WriteSeed(GS::hNoita, SetSeed);
		}
		if (ImGui::Button("Remove Patch")) {
			UnpatchSeedGen(GS::hNoita);
		}

		if (ImGui::Button("Exit")) {
			exit(0);
		}

		if (bAttached) {
			CurrentSeed = ReadSeed(GS::hNoita);
			ImGui::Text("Current Seed: %d", CurrentSeed);
			if (ImGui::IsItemClicked()) {
				GS::CopyToClipboard("%d", CurrentSeed);
			}
			ImGui::Text("Seed History");
			for (int seed : GS::seedHistory) {
				ImGui::Text("%d", seed);
				if (ImGui::IsItemClicked()) {
					GS::CopyToClipboard("%d", seed);
				}
			}
			if (bLogToFile && CurrentSeed && CurrentSeed != GS::gameSeed) {
				GS::gameSeed = CurrentSeed;
				GS::seedHistory.push_back(CurrentSeed);
				time_t rawtime;
				struct tm timeinfo;
				char buffer[80];

				time(&rawtime);
				localtime_s(&timeinfo, &rawtime);

				strftime(buffer, sizeof(buffer), "%d-%m-%Y %H:%M:%S", &timeinfo);
				std::string time_string(buffer);

				GS::logFile << time_string << "| seed: " << CurrentSeed << std::endl;
			}
		}
		ImGui::End();
	}
}
