#include "DataClientLayer.h"

#include "Walnut/Application.h"
#include "Walnut/UI/UI.h"
#include "Walnut/Serialization/BufferStream.h"
#include "Walnut/Networking/NetworkingUtils.h"
#include "Walnut/Utils/StringUtils.h"

#include "misc/cpp/imgui_stdlib.h"

#include <yaml-cpp/yaml.h>

#include <iostream>
#include <fstream>

#include <windows.h>
#include <shobjidl.h> 

#include "UDPClient.h"

#include "ImPlot/implot.h"

#include <filesystem>




// utility structure for realtime plot
struct ScrollingBuffer {
	int MaxSize;
	int Offset;
	ImVector<ImVec2> Data;
	ScrollingBuffer(int max_size = 2000) {
		MaxSize = max_size;
		Offset = 0;
		Data.reserve(MaxSize);
	}
	void AddPoint(float x, float y) {
		if (Data.size() < MaxSize)
			Data.push_back(ImVec2(x, y));
		else {
			Data[Offset] = ImVec2(x, y);
			Offset = (Offset + 1) % MaxSize;
		}
	}
	void Erase() {
		if (Data.size() > 0) {
			Data.shrink(0);
			Offset = 0;
		}
	}
};

UDPClient lUDPClient("127.0.0.1", 20782);

void DataClientLayer::OnAttach()
{

}

void DataClientLayer::OnDetach()
{

}

void DataClientLayer::OnUIRender()
{
	//m_Console.OnUIRender();
	ImGui::ShowDemoWindow();
	ImPlot::CreateContext();
	ImPlot::ShowDemoWindow();
	DriverInputsStatus();
	StageStatus();
	BrakeData();
	if (!lUDPClient.isRunning_b)
	{
		lUDPClient.startClient();
	}

}


void DataClientLayer::DriverInputsStatus()
{
	ImGui::Begin("Driver Inputs");
	char buf[32];

	//*******************************Throttle
	sprintf(buf, "%d/%d", (int)(l_EASportsWRC.data.throttle*100), 100);
	ImGui::ProgressBar(l_EASportsWRC.data.throttle, ImVec2(0.0f, 0.0f),buf);
	ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
	ImGui::Text("Accel");
	//*******************************Brake
	sprintf(buf, "%d/%d", (int)(l_EASportsWRC.data.brake * 100), 100);
	ImGui::ProgressBar(l_EASportsWRC.data.brake, ImVec2(0.0f, 0.0f), buf);
	ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
	ImGui::Text("Brake");
	//*******************************Clutch
	sprintf(buf, "%d/%d", (int)(l_EASportsWRC.data.clutch * 100), 100);
	ImGui::ProgressBar(l_EASportsWRC.data.clutch, ImVec2(0.0f, 0.0f), buf);
	ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
	ImGui::Text("Clutch");
	//*******************************Handbrake
	sprintf(buf, "%d/%d", (int)(l_EASportsWRC.data.handbrake * 100), 100);
	ImGui::ProgressBar(l_EASportsWRC.data.handbrake, ImVec2(0.0f, 0.0f), buf);
	ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
	ImGui::Text("Handbrake");
	//*******************************Steering
	sprintf(buf, "%d", (int)(l_EASportsWRC.data.stear));
	ImGui::SliderFloat("Steering", &l_EASportsWRC.data.stear, -1.0f, 1.0f, "%.3f", 1);
	
	ImGui::End();
}

void DataClientLayer::StageStatus()
{
	ImGui::Begin("Stage Status");
	
	//*******************************Stage current distance
	char buf[32];
	float progression = (float)(l_EASportsWRC.data.lap_distance / l_EASportsWRC.data.track_length);
	sprintf(buf, "%d/%d", (int)(l_EASportsWRC.data.lap_distance), (int)l_EASportsWRC.data.track_length);
	ImGui::ProgressBar(progression, ImVec2(0.0f, 0.0f), buf);
	ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
	ImGui::Text("Distance");

	//*******************************Stage Time
	int displayCurrentMinutes = (int)l_EASportsWRC.data.current_minutes;
	int displayCurrentSeconds = (int)l_EASportsWRC.data.current_seconds;
	float displayMicroSeconds = (l_EASportsWRC.data.current_seconds - (int)l_EASportsWRC.data.current_seconds)*1000;
	ImGui::Text("Current Time: %d : %d : %1.0f", displayCurrentMinutes, displayCurrentSeconds , displayMicroSeconds);

	
	if (ImGui::Button("Store Run"))
	{
		//if (l_EASportsWRC.TelemetryData_v.current_time.size() != 0)
		{
			ImGui::OpenPopup("Store Run");
			// Always center this window when appearing
			ImVec2 center = ImGui::GetMainViewport()->GetCenter();
			ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
		}
	}
	ImGui::SameLine();
	if (ImGui::Button("Clear Run"))
	{
		l_EASportsWRC.ClearArray();
	}
	m_StoreRunModalOpen = ImGui::BeginPopupModal("Store Run", NULL, ImGuiWindowFlags_AlwaysAutoResize);
	if (m_StoreRunModalOpen) { StoreRunModal(); }

	ImGui::End();
}

void DataClientLayer::StoreRunModal()
{
	HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED |
		COINIT_DISABLE_OLE1DDE);
	static bool isPathEmpty = false;

	ImGui::Text("Storage location");
	ImGui::InputText("##Storage", &m_StoragePath);
	ImGui::SameLine();
	
	if (ImGui::Button("Select"))
	{
		if (SUCCEEDED(hr))
		{
			IFileOpenDialog* pFileOpen;

			// Create the FileOpenDialog object.
			hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL,
				IID_IFileOpenDialog, reinterpret_cast<void**>(&pFileOpen));
			
			DWORD dwOptions;
			if (SUCCEEDED(pFileOpen->GetOptions(&dwOptions)))
			{
				pFileOpen->SetOptions(dwOptions | FOS_PICKFOLDERS);
			}

			if (SUCCEEDED(hr))
			{
				// Show the Open dialog box.
				hr = pFileOpen->Show(NULL);

				// Get the file name from the dialog box.
				if (SUCCEEDED(hr))
				{
					IShellItem* pItem;
					hr = pFileOpen->GetResult(&pItem);
					if (SUCCEEDED(hr))
					{
						PWSTR pszFilePath;
						hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

						// Display the file name to the user.
						if (SUCCEEDED(hr))
						{
							std::wstring wss = pszFilePath;
							std::string sss(wss.begin(), wss.end());
							m_StoragePath = sss;
							CoTaskMemFree(pszFilePath);
						}
						pItem->Release();
					}
				}
				pFileOpen->Release();
			}
			CoUninitialize();
		}
		
	}
	
	ImGui::Text("File name");
	ImGui::InputText("##file", &m_StorageFileName);

	ImGui::Separator();
	
	if (isPathEmpty)
	{
		ImGui::Text("Please select a folder and insert a file name");
	}
	
	if (ImGui::Button("Save"))
	{
		if (0 == m_StorageFileName.size() || 0 == m_StoragePath.size())
		{
			isPathEmpty = true;
		}
		else
		{
			isPathEmpty = false;
			std::string l_SCompletePath = m_StoragePath + "\\" + m_StorageFileName + ".yaml";
			std::filesystem::path l_CompletePath = l_SCompletePath;
			l_EASportsWRC.StoreVector(l_CompletePath);
			ImGui::CloseCurrentPopup();
		}
	}
	
	ImGui::SameLine();
	if (ImGui::Button("Cancel")) { ImGui::CloseCurrentPopup(); }

	ImGui::EndPopup();

}

void DataClientLayer::BrakeData()
{
	static ScrollingBuffer BrakePos, BrakeTempbl, BrakeTempbr, BrakeTempfl, BrakeTempfr;
	float current_time = 0;

	ImGui::Begin("Brake Data");
	/********************************************************************************************/
	/*																							*/
	/*											Brake Plots										*/
	/*																							*/
	/********************************************************************************************/

	if (l_EASportsWRC.GetOnStage())
	{
		current_time = l_EASportsWRC.TelemetryData_v.current_time.back();
		float BrakePosition = l_EASportsWRC.TelemetryData_v.brake.back();
		
		BrakePos.AddPoint(current_time, BrakePosition);
		BrakeTempbl.AddPoint(current_time, l_EASportsWRC.TelemetryData_v.brake_temp_bl.back());
		BrakeTempbr.AddPoint(current_time, l_EASportsWRC.TelemetryData_v.brake_temp_br.back());
		BrakeTempfl.AddPoint(current_time, l_EASportsWRC.TelemetryData_v.brake_temp_fl.back());
		BrakeTempfr.AddPoint(current_time, l_EASportsWRC.TelemetryData_v.brake_temp_fr.back());
	}

	static float history = 10.0f;
	ImGui::SliderFloat("History", &history, 1, 30, "%.1f s");

	static ImPlotAxisFlags xflags = ImPlotAxisFlags_NoTickLabels | ImPlotAxisFlags_AutoFit;
	static ImPlotAxisFlags yflags = ImPlotAxisFlags_AutoFit;

	if (ImPlot::BeginPlot(" Brake Data ", ImVec2(-1, 150)))
	{
		ImPlot::SetupAxes(NULL, NULL, xflags, yflags);
		ImPlot::SetupAxisLimits(ImAxis_X1, current_time - history, current_time, ImGuiCond_Always);
		ImPlot::SetupAxisLimits(ImAxis_Y1, 0, 1.1);
		ImPlot::SetNextFillStyle(IMPLOT_AUTO_COL, 0.5f);
		if (l_EASportsWRC.GetOnStage())
		{
			ImPlot::PlotLine("Brake Pedal", &BrakePos.Data[0].x, &BrakePos.Data[0].y, BrakePos.Data.size(), 0,  BrakePos.Offset, 2 * sizeof(float));
			//std::cout << "Brake Position: " << BrakePos.Data[0].y << std::endl;
		}
		ImPlot::EndPlot();
	}

	if (ImPlot::BeginPlot(" Brake Temperature ", ImVec2(-1, 150)))
	{
		ImPlot::SetupAxes(NULL, NULL, xflags, yflags);
		ImPlot::SetupAxisLimits(ImAxis_X1, current_time - history, current_time, ImGuiCond_Always);
		ImPlot::SetupAxisLimits(ImAxis_Y1, 0, 100);
		ImPlot::SetNextFillStyle(IMPLOT_AUTO_COL, 0.5f);
		if (l_EASportsWRC.GetOnStage())
		{
			ImPlot::PlotLine("BL", &BrakeTempbl.Data[0].x, &BrakeTempbl.Data[0].y, BrakeTempbl.Data.size(), 0, BrakeTempbl.Offset, 2 * sizeof(float));
			ImPlot::PlotLine("BR", &BrakeTempbr.Data[0].x, &BrakeTempbr.Data[0].y, BrakeTempbr.Data.size(), 0, BrakeTempbr.Offset, 2 * sizeof(float));
			ImPlot::PlotLine("FL", &BrakeTempfl.Data[0].x, &BrakeTempfl.Data[0].y, BrakeTempfl.Data.size(), 0, BrakeTempfl.Offset, 2 * sizeof(float));
			ImPlot::PlotLine("FR", &BrakeTempfr.Data[0].x, &BrakeTempfr.Data[0].y, BrakeTempfr.Data.size(), 0, BrakeTempfr.Offset, 2 * sizeof(float));
			
		}
		ImPlot::EndPlot();
	}


	/********************************************************************************************/
	/*																							*/
	/*											Brake HeatMaps									*/
	/*																							*/
	/********************************************************************************************/

	static const char* xlabels[] = { "L","R"};
	static const char* ylabels[] = { "F","B"};
	static float values[2][2] = { {0,0},
								{0,0} };

	if (l_EASportsWRC.GetOnStage())// only need to check size of one
	{
		values[0][0] = l_EASportsWRC.TelemetryData_v.brake_temp_fl.back();
		values[0][1] = l_EASportsWRC.TelemetryData_v.brake_temp_fr.back();
		values[1][0] = l_EASportsWRC.TelemetryData_v.brake_temp_bl.back();
		values[1][1] = l_EASportsWRC.TelemetryData_v.brake_temp_br.back();
	}
	
	static float scale_min = 0;
	static float scale_max = 400;

	static ImPlotColormap map = ImPlotColormap_Jet;
	static ImPlotHeatmapFlags hm_flags = 0;
	static ImPlotAxisFlags axes_flags = ImPlotAxisFlags_Lock | ImPlotAxisFlags_NoGridLines | ImPlotAxisFlags_NoTickMarks;
	ImPlot::BustColorCache("##Heatmap1");
	ImPlot::PushColormap(map);

	if (ImPlot::BeginPlot("##Heatmap1", ImVec2(225, 225), ImPlotFlags_NoLegend | ImPlotFlags_NoMouseText)) {
		//ImPlot::PushColormap(map);
		ImPlot::SetupAxes(NULL, NULL, axes_flags, axes_flags);
		ImPlot::SetupAxisTicks(ImAxis_X1, 0 + 1.0 / 4.0, 1 - 1.0 / 4.0,2,xlabels);
		ImPlot::SetupAxisTicks(ImAxis_Y1, 1 - 1.0 / 4.0, 0 + 1.0 / 4.0,2, ylabels);
		ImPlot::PlotHeatmap("heat", values[0],2, 2, scale_min, scale_max, "%g", ImPlotPoint(0, 0), ImPlotPoint(1, 1), hm_flags);
		ImPlot::EndPlot();
	}
	ImGui::SameLine();
	ImPlot::ColormapScale("##HeatScale", scale_min, scale_max, ImVec2(60, 225));
	ImPlot::PopColormap();

	ImGui::End();
}

