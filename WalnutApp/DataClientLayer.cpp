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

#include "src/EASportsWRC/EASportsWRC.h"

#include "ImPlot/implot.h"

#include <filesystem>

template <typename T>
inline T RandomRange(T min, T max) {
	T scale = rand() / (T)RAND_MAX;
	return min + scale * (max - min);
}

static ImVec4 RandomColor() {
	ImVec4 col;
	col.x = RandomRange(0.0f, 1.0f);
	col.y = RandomRange(0.0f, 1.0f);
	col.z = RandomRange(0.0f, 1.0f);
	col.w = 1.0f;
	return col;
}


EASportsWRC l_EASportsWRC("127.0.0.1", 20782);

/***

Handlers

***/

void DataClientLayer::OnAttach()
{
	ImPlot::CreateContext();
}

void DataClientLayer::OnDetach()
{

}

void DataClientLayer::OnUIRender()
{
	//ImGui::ShowDemoWindow();
	//ImPlot::ShowDemoWindow();
	StageStatus();
	if (m_ShowBrakeData) BrakeData();
	if (m_LoadRunAndShowMultiSignalPlot) LoadRunAndMultiSignalPlot();
	if (m_ShowDriverInputStatus) DriverInputsStatus();
	if (m_ShowPositionPlot) VehiclePosition();
	if (m_ShowShiftLight) ShiftLight();
	if (m_ShowMultiSignalPlotLive) MultiSignalPlotLive();
	if (!l_EASportsWRC.isRunning_b)
	{
		l_EASportsWRC.startClient();
	}
}

/***

Windows

***/

void DataClientLayer::DriverInputsStatus()
{
	ImGui::Begin("Driver Inputs",&m_ShowDriverInputStatus);
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
		l_EASportsWRC.ClearMap();
	}
	m_StoreRunModalOpen = ImGui::BeginPopupModal("Store Run", NULL, ImGuiWindowFlags_AlwaysAutoResize);
	if (m_StoreRunModalOpen) { StoreRunModal(); }

	ImGui::End();
}

void DataClientLayer::BrakeData()
{
	static ScrollingBuffer BrakePos, BrakeTempbl, BrakeTempbr, BrakeTempfl, BrakeTempfr;
	float current_time = 0;

	ImGui::Begin("Brake Data", &m_ShowBrakeData);
	/********************************************************************************************/
	/*																							*/
	/*											Brake Plots										*/
	/*																							*/
	/********************************************************************************************/

	if (l_EASportsWRC.GetOnStage() && l_EASportsWRC.TelemetryData_v.brake_temp_bl.size() != 0)
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
		if (l_EASportsWRC.GetOnStage() && l_EASportsWRC.TelemetryData_v.brake_temp_bl.size() != 0)
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
		if (l_EASportsWRC.GetOnStage() && l_EASportsWRC.TelemetryData_v.brake_temp_bl.size() != 0)
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

	static const char* xlabels[] = { "Left","Right"};
	static const char* ylabels[] = { "Front","Back"};
	static float values[2][2] = { {0,0},
								{0,0} };

	if (l_EASportsWRC.GetOnStage() && l_EASportsWRC.TelemetryData_v.brake_temp_bl.size() != 0)// only need to check size of one
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

void DataClientLayer::LoadRunAndMultiSignalPlot()
{
	static ImPlotSubplotFlags flags = ImPlotSubplotFlags_ShareItems | ImPlotSubplotFlags_LinkAllX;
	static int rows = 4;
	static int cols = 4;
	static int id[] = { 0,1,2,3,4,5 };
	static int curj = -1;
	
	static bool show_rows_cols = false;

	const int dnd_size = EASPORTS_DATA_SIZE;
	static DragAndDropItem dnd[dnd_size];
	static EAtelemetryfloat_t time;
	
	/*------------------------------------------ Begin widgets---------------------------------------------------------------------*/
	ImGui::Begin("Signal Plots", &m_LoadRunAndShowMultiSignalPlot, ImGuiWindowFlags_MenuBar);
	
	/*------------------------------------------ Begin child for DnD gen-----------------------------------------------------------*/
	// child window to serve as initial source for our DND items
	ImGui::BeginChild("DND_LEFT", ImVec2(225, -1));

	if (!l_EASportsWRC.m_EAtelemetryMap.EAtelemetryfloatMap.empty())
	{	
		if (ImGui::Button("Reset Data")) {
			for (int k = 0; k < dnd_size; ++k)
				dnd[k].Reset();
			//dndx = dndy = NULL;
		}
		for (int it = 0; it < dnd_size; it++)
		{
			if (dnd[it].Plt > 0)
				continue;
			ImPlot::ItemIcon(dnd[it].Color); ImGui::SameLine();
			ImGui::Selectable(dnd[it].SignalName.c_str(), false, 0, ImVec2(225, 0));
			if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None)) {
				ImGui::SetDragDropPayload("MY_DND", &it, sizeof(int));
				ImPlot::ItemIcon(dnd[it].Color); ImGui::SameLine();
				ImGui::TextUnformatted(dnd[it].SignalName.c_str());
				ImGui::EndDragDropSource();
			}
		}
	}
	else
	{
		ImGui::Text("Load a file or start a run");
	}

	ImGui::EndChild();
	/*------------------------------------------ End child for DnD gen-------------------------------------------------------------*/

	/*------------------------------------------ Begin Drag and Drop target--------------------------------------------------------*/
	if (ImGui::BeginDragDropTarget()) {
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("MY_DND")) {
			int i = *(int*)payload->Data; dnd[i].Reset();
		}
		ImGui::EndDragDropTarget();
	}
	/*------------------------------------------ End Drag and Drop target----------------------------------------------------------*/
	
	ImGui::SameLine();
	///ImGui::BeginChild("DND_RIGHT", ImVec2(-1, -1));
	/*------------------------------------------ Begin Subplots--------------------------------------------------------------------*/
	if (ImPlot::BeginSubplots("##ItemSharing", rows, cols, ImVec2(-1, -1), flags)) 
	{
		for (int i = 0; i < rows * cols; ++i) {
			if (ImPlot::BeginPlot("")) {
				for (int k = 0; k < dnd_size; k++)
				{
					if (dnd[k].Plt == 1 && dnd[k].Data.size() > 0 && dnd[k].SubPlotId == i) {
						ImPlot::SetAxis(dnd[k].Yax);
						ImPlot::SetNextLineStyle(dnd[k].Color);
 						ImPlot::PlotLine(dnd[k].SignalName.c_str(), &dnd[k].DataVec2[0].x, &dnd[k].DataVec2[0].y, dnd[k].DataVec2.size(), 0, 0, 2 * sizeof(float));
						// allow legend item labels to be DND sources
						if (ImPlot::BeginDragDropSourceItem(dnd[k].SignalName.c_str())) {
							ImGui::SetDragDropPayload("MY_DND", &k, sizeof(int));
							ImPlot::ItemIcon(dnd[k].Color); ImGui::SameLine();
							ImGui::TextUnformatted(dnd[k].SignalName.c_str());
							ImPlot::EndDragDropSource();
						}
					}
				}
				// allow the main plot area to be a DND target
				if (ImPlot::BeginDragDropTargetPlot()) {
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("MY_DND")) {
						int Z = *(int*)payload->Data; 
						dnd[Z].Plt = 1; 
						dnd[Z].SubPlotId = i;
						dnd[Z].Yax = ImAxis_Y1;
					}
					ImPlot::EndDragDropTarget();
				}
				ImPlot::EndPlot();
			}
		}
		ImPlot::EndSubplots();
	}
	//ImGui::EndChild();
	/*------------------------------------------ End Subplots----------------------------------------------------------------------*/
	
	/*------------------------------------------ Begin Top menu for Load and config------------------------------------------------*/
	/*Menu for changing amount of plots*/
	if (ImGui::BeginMenuBar()) {
		if (ImGui::BeginMenu("Config")) {
			ImGui::MenuItem("Change Row/Columns", NULL, &show_rows_cols);
			ImGui::MenuItem("Load", NULL, &m_LoadRunModalRequest);
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}
	if (show_rows_cols)
	{
		ImGui::Begin("Configure Rows & Columns", &show_rows_cols);
	
		ImGui::InputInt("Rows", &rows);
		ImGui::InputInt("Columns", &cols);
		if (ImGui::Button("Done"))
		{
			show_rows_cols = false;
		}
		ImGui::End();
	}
	else if (m_LoadRunModalRequest)
	{
		//start popup
		ImGui::OpenPopup("Load Run");
		// Always center this window when appearing
		ImVec2 center = ImGui::GetMainViewport()->GetCenter();
		ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
	
		m_LoadRunModalOpen = ImGui::BeginPopupModal("Load Run", NULL, ImGuiWindowFlags_AlwaysAutoResize);
		if (m_LoadRunModalOpen) 
		{ 
			l_EASportsWRC.ClearMap();//Clear possible previous runs
			LoadRunModal();
		}
		
		if (!l_EASportsWRC.m_EAtelemetryMap.EAtelemetryfloatMap.empty())
		{
			// Workflow:
			// fill DnD vector
			// insert into list
			// val is a vector with the data for the specified key
			// the Data vector is a EAtelemetrydouble_t vector which takes the x (time defined above, and the data from the map file
			// Since Data is a EAtelemetrydouble_t vector, we need to iterate over it and fill it with the vector members from the map

			static int curr_id = 0;
			time = l_EASportsWRC.m_EAtelemetryMap.EAtelemetryfloatMap["Current time"];
			for (auto const& [key, val] : l_EASportsWRC.m_EAtelemetryMap.EAtelemetrybyteMap)
			{
				dnd[curr_id].Data.resize(val.size());
				dnd[curr_id].Color = RandomColor();
				dnd[curr_id].SignalName = key;
				std::transform(val.begin(), val.end(), dnd[curr_id].Data.begin(), [](int x) { return (float)x; });
				if (!dnd[curr_id].DataVec2.empty()) { dnd[curr_id].DataVec2.clear(); } //Clear possible previous runs
				for (int i = 0; i < dnd[curr_id].Data.size(); i++)
				{
					dnd[curr_id].DataVec2.push_back(ImVec2(time[i], dnd[curr_id].Data[i]));
				}
				curr_id++;
			}
			for (auto const& [key, val] : l_EASportsWRC.m_EAtelemetryMap.EAtelemetrydoubleMap)
			{
				dnd[curr_id].Data.resize(val.size());
				dnd[curr_id].Color = RandomColor();
				dnd[curr_id].SignalName = key;
				if (!dnd[curr_id].DataVec2.empty()) { dnd[curr_id].DataVec2.clear(); } //Clear possible previous runs
				std::transform(val.begin(), val.end(), dnd[curr_id].Data.begin(), [](double x) { return (float)x; });
				for (int i = 0; i < dnd[curr_id].Data.size(); i++)
				{
					dnd[curr_id].DataVec2.push_back(ImVec2(time[i], dnd[curr_id].Data[i]));
				}
				curr_id++;
			}
			for (auto const& [key, val] : l_EASportsWRC.m_EAtelemetryMap.EAtelemetryfloatMap)
			{
				dnd[curr_id].Data.resize(val.size());
				dnd[curr_id].Color = RandomColor();
				dnd[curr_id].SignalName = key;
				if (!dnd[curr_id].DataVec2.empty()) { dnd[curr_id].DataVec2.clear(); } //Clear possible previous runs
				std::transform(val.begin(), val.end(), dnd[curr_id].Data.begin(), [](float x) { return (float)x; });
				for (int i = 0; i < dnd[curr_id].Data.size(); i++)
				{
					dnd[curr_id].DataVec2.push_back(ImVec2(time[i], dnd[curr_id].Data[i]));
				}
				curr_id++;
			}
			
			curr_id = 0;
		}

	}
	/*------------------------------------------ End Top menu for Load and config--------------------------------------------------*/
	
	ImGui::End();
}

void DataClientLayer::VehiclePosition()
{
	static bool isSetup = true;

	static ImVector<ImVec2> VehiclePosition;
	ImGui::Begin("Vehicle Position", &m_ShowPositionPlot);

	if (ImGui::Button("Reset") || l_EASportsWRC.data.lap_distance == 0)
	{
		VehiclePosition.clear();
		//dndx = dndy = NULL;
	}
	if (ImPlot::BeginPlot("Vehicle Position", ImVec2(-1, -1)))
	{
		if (l_EASportsWRC.GetOnStage() && l_EASportsWRC.TelemetryData_v.VehPosZ.size() != 0)
		{
			VehiclePosition.push_back(ImVec2(l_EASportsWRC.TelemetryData_v.VehPosZ.back(), l_EASportsWRC.TelemetryData_v.VehPosX.back()));
			ImPlot::PlotLine("Vehicle Position", &VehiclePosition[0].x, &VehiclePosition[0].y, VehiclePosition.size(), 0, 0, 2 * sizeof(float));
			
		}
		ImPlot::EndPlot();
	}

	ImGui::End();
}

void DataClientLayer::ShiftLight()
{
	static ScrollingBuffer PosX, PosY, PosZ, PosXZ;
	float current_time = 0;
	static bool isOpen;
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoResize |   ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoDocking;
	ImGui::Begin("Shift Lights", &m_ShowShiftLight, window_flags);

	/********************************************************************************************/
	/*																							*/
	/*											Shift HeatMaps									*/
	/*																							*/
	/********************************************************************************************/

	static const char* xlabels[] = { " "};
	static const char* ylabels[] = { " "};
	static float values = 0;
	static float scale_min = 0;
	static float scale_max = 5000;
	static ImPlotColormap map = ImPlotColormap_Jet;
	
	if (l_EASportsWRC.GetOnStage() && l_EASportsWRC.TelemetryData_v.rpm.size() != 0)// only need to check size of one
	{
		values = l_EASportsWRC.TelemetryData_v.rpm.back();
		//Black when not in range
		if (l_EASportsWRC.TelemetryData_v.rpm.back() < ((2 * l_EASportsWRC.TelemetryData_v.shiftlightStart.back()) - l_EASportsWRC.TelemetryData_v.shiftlightEnd.back()))
		{
			map = ImPlotColormap_Greys;
			scale_min = -5000;
			scale_max = 0;
		}
		//its in range
		else if (l_EASportsWRC.TelemetryData_v.rpm.back() >= ((2 * l_EASportsWRC.TelemetryData_v.shiftlightStart.back()) - l_EASportsWRC.TelemetryData_v.shiftlightEnd.back()))
		{
			map = ImPlotColormap_Jet;
			scale_max = l_EASportsWRC.TelemetryData_v.shiftlightEnd.back();
			scale_min = (1.8 * l_EASportsWRC.TelemetryData_v.shiftlightStart.back()) - scale_max;
		}
		//show blue when in regen (only for rally 1)
		//TODO: check if regen only really hits with 50% brake pressure (still prototype
		//if (0 == l_EASportsWRC.TelemetryData_v.throttle.back() && l_EASportsWRC.TelemetryData_v.brake.back() >= 0.5)
		//{
		//	//Regan is active, show blue
		//	map = ImPlotColormap_Jet;
		//	scale_min = l_EASportsWRC.TelemetryData_v.max_rpm.back();
		//	scale_max = l_EASportsWRC.TelemetryData_v.max_rpm.back();
		//}
	}


	static ImPlotHeatmapFlags hm_flags = 0;
	static ImPlotAxisFlags axes_flags = ImPlotAxisFlags_NoTickLabels | ImPlotAxisFlags_Lock | ImPlotAxisFlags_NoGridLines | ImPlotAxisFlags_NoTickMarks | ImPlotAxisFlags_NoLabel;
	ImPlot::BustColorCache("##Heatmap1");
	ImPlot::PushColormap(map);

	if (ImPlot::BeginPlot("##Heatmap1", ImVec2(225, 225), ImPlotFlags_NoLegend | ImPlotFlags_NoMouseText)) {
		//ImPlot::PushColormap(map);
		ImPlot::SetupAxes(NULL, NULL, axes_flags, axes_flags);
		ImPlot::PlotHeatmap("heat", &values, 1, 1, scale_min, scale_max, "%g", ImPlotPoint(0, 0), ImPlotPoint(1, 1), hm_flags);
		ImPlot::EndPlot();
	}
	ImGui::SameLine();
	ImPlot::PopColormap();

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

void DataClientLayer::LoadRunModal()
{
	HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED |
		COINIT_DISABLE_OLE1DDE);
	static bool isPathEmpty = false;
	static std::filesystem::path lPath;

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
				pFileOpen->SetOptions(dwOptions | FOS_FILEMUSTEXIST);
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
							std::filesystem::path path(m_StoragePath);
							m_StorageFileName = path.filename().string(); // "file"
							m_StoragePath = path.parent_path().string(); // "/home/dir1/dir2/dir3/dir4"
							lPath = path;
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

	if (ImGui::Button("Load"))
	{
		ImGui::Text("This operation might take some time, please wait");
		l_EASportsWRC.GenerateMapFromYAML(lPath);
		ImGui::CloseCurrentPopup();
		m_LoadRunModalRequest = false;
	}

	ImGui::SameLine();
	if (ImGui::Button("Cancel")) 
	{ 
		ImGui::CloseCurrentPopup();
		m_LoadRunModalRequest = false;
	}

	ImGui::EndPopup();
}

void DataClientLayer::MultiSignalPlotLive()
{
	static ImPlotSubplotFlags flags = ImPlotSubplotFlags_ShareItems | ImPlotSubplotFlags_LinkAllX;
	static int rows = 4;
	static int cols = 4;

	static bool isSetup = true;

	static bool show_rows_cols = false;

	const int dnd_size = EASPORTS_DATA_SIZE;
	static DragAndDropItem dnd[dnd_size];
	static EAtelemetryfloat_t time;

	/*------------------------------------------ Begin Setup-----------------------------------------------------------------------*/
	if (isSetup && l_EASportsWRC.GenerateMapFromLiveData()) // Initialize dnd structures
	{
		// Workflow:
		// fill DnD vector
		// insert into list
		// val is a vector with the data for the specified key
		// the Data vector is a EAtelemetrydouble_t vector which takes the x (time defined above, and the data from the map file
		// Since Data is a EAtelemetrydouble_t vector, we need to iterate over it and fill it with the vector members from the map

		static int curr_id = 0;
		time = l_EASportsWRC.m_EAtelemetryMap.EAtelemetryfloatMap["Current time"];
		for (auto const& [key, val] : l_EASportsWRC.m_EAtelemetryMap.EAtelemetrybyteMap)
		{
			dnd[curr_id].Data.resize(val.size());
			dnd[curr_id].Color = RandomColor();
			dnd[curr_id].SignalName = key;
			std::transform(val.begin(), val.end(), dnd[curr_id].Data.begin(), [](int x) { return (float)x; });
			if (!dnd[curr_id].DataVec2.empty()) { dnd[curr_id].DataVec2.clear(); } //Clear possible previous runs
			for (int i = 0; i < dnd[curr_id].Data.size(); i++)
			{
				dnd[curr_id].DataVec2.push_back(ImVec2(time[i], dnd[curr_id].Data[i]));
			}
			curr_id++;
		}
		for (auto const& [key, val] : l_EASportsWRC.m_EAtelemetryMap.EAtelemetrydoubleMap)
		{
			dnd[curr_id].Data.resize(val.size());
			dnd[curr_id].Color = RandomColor();
			dnd[curr_id].SignalName = key;
			if (!dnd[curr_id].DataVec2.empty()) { dnd[curr_id].DataVec2.clear(); } //Clear possible previous runs
			std::transform(val.begin(), val.end(), dnd[curr_id].Data.begin(), [](double x) { return (float)x; });
			for (int i = 0; i < dnd[curr_id].Data.size(); i++)
			{
				dnd[curr_id].DataVec2.push_back(ImVec2(time[i], dnd[curr_id].Data[i]));
			}
			curr_id++;
		}
		for (auto const& [key, val] : l_EASportsWRC.m_EAtelemetryMap.EAtelemetryfloatMap)
		{
			dnd[curr_id].Data.resize(val.size());
			dnd[curr_id].Color = RandomColor();
			dnd[curr_id].SignalName = key;
			if (!dnd[curr_id].DataVec2.empty()) { dnd[curr_id].DataVec2.clear(); } //Clear possible previous runs
			std::transform(val.begin(), val.end(), dnd[curr_id].Data.begin(), [](float x) { return (float)x; });
			for (int i = 0; i < dnd[curr_id].Data.size(); i++)
			{
				dnd[curr_id].DataVec2.push_back(ImVec2(time[i], dnd[curr_id].Data[i]));
			}
			curr_id++;
		}
		curr_id = 0;
		isSetup = false;
	}
	else if (l_EASportsWRC.UpdateMapFromLiveData()) // update everytime with latest data from the map
	{
		time = l_EASportsWRC.m_EAtelemetryMap.EAtelemetryfloatMap["Current time"];
		for (auto const& [key, val] : l_EASportsWRC.m_EAtelemetryMap.EAtelemetrybyteMap) // loop over  map
		{
			for (auto & curr_dnd : dnd) // iterate over drag and drop items
			{
				if (curr_dnd.SignalName == key) //found corresponding dnd item to map
				{
					curr_dnd.DataVec2.push_back(ImVec2(time.back(), (float)val.back()));
				}
			}
		}
		for (auto const& [key, val] : l_EASportsWRC.m_EAtelemetryMap.EAtelemetrydoubleMap) // loop over  map
		{
			for (auto& curr_dnd : dnd) // iterate over drag and drop items
			{
				if (curr_dnd.SignalName == key) //found corresponding dnd item to map
				{
					curr_dnd.DataVec2.push_back(ImVec2(time.back(), (float)val.back()));
				}
			}
		}
		for (auto const& [key, val] : l_EASportsWRC.m_EAtelemetryMap.EAtelemetryfloatMap) // loop over  map
		{
			for (auto& curr_dnd : dnd) // iterate over drag and drop items
			{
				if (curr_dnd.SignalName == key) //found corresponding dnd item to map
				{
					curr_dnd.DataVec2.push_back(ImVec2(time.back(), (float)val.back()));
				}
			}
		}
	}
	/*------------------------------------------ End   Setup-----------------------------------------------------------------------*/



	/*------------------------------------------ Begin widgets---------------------------------------------------------------------*/
	ImGui::Begin("Signal Plots", &m_ShowMultiSignalPlotLive, ImGuiWindowFlags_MenuBar);

	/*------------------------------------------ Begin child for DnD gen-----------------------------------------------------------*/
	// child window to serve as initial source for our DND items
	ImGui::BeginChild("DND_LEFT", ImVec2(225, -1));

	if (!l_EASportsWRC.m_EAtelemetryMap.EAtelemetryfloatMap.empty())
	{
		if (ImGui::Button("Reset Data")) {
			for (int k = 0; k < dnd_size; ++k)
			{
				dnd[k].Reset();
				dnd[k].DataVec2.clear();
				//dndx = dndy = NULL;
			}
				
		}
		for (int it = 0; it < dnd_size; it++)
		{
			if (dnd[it].Plt > 0)
				continue;
			ImPlot::ItemIcon(dnd[it].Color); ImGui::SameLine();
			ImGui::Selectable(dnd[it].SignalName.c_str(), false, 0, ImVec2(225, 0));
			if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None)) {
				ImGui::SetDragDropPayload("MY_DND", &it, sizeof(int));
				ImPlot::ItemIcon(dnd[it].Color); ImGui::SameLine();
				ImGui::TextUnformatted(dnd[it].SignalName.c_str());
				ImGui::EndDragDropSource();
			}
		}
	}

	ImGui::EndChild();
	/*------------------------------------------ End child for DnD gen-------------------------------------------------------------*/

	/*------------------------------------------ Begin Drag and Drop target--------------------------------------------------------*/
	if (ImGui::BeginDragDropTarget()) {
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("MY_DND")) {
			int i = *(int*)payload->Data; dnd[i].Reset();
		}
		ImGui::EndDragDropTarget();
	}
	/*------------------------------------------ End Drag and Drop target----------------------------------------------------------*/

	ImGui::SameLine();
	/*------------------------------------------ Begin Subplots--------------------------------------------------------------------*/
	if (ImPlot::BeginSubplots("##ItemSharing", rows, cols, ImVec2(-1, -1), flags))
	{
		for (int i = 0; i < rows * cols; ++i) {
			if (ImPlot::BeginPlot("")) {
				for (int k = 0; k < dnd_size; k++)
				{
					if (dnd[k].Plt == 1 && dnd[k].Data.size() > 0 && dnd[k].SubPlotId == i) {
						ImPlot::SetAxis(dnd[k].Yax);
						ImPlot::SetNextLineStyle(dnd[k].Color);
						ImPlot::PlotLine(dnd[k].SignalName.c_str(), &dnd[k].DataVec2[0].x, &dnd[k].DataVec2[0].y, dnd[k].DataVec2.size(), 0, 0, 2 * sizeof(float));
						// allow legend item labels to be DND sources
						if (ImPlot::BeginDragDropSourceItem(dnd[k].SignalName.c_str())) {
							ImGui::SetDragDropPayload("MY_DND", &k, sizeof(int));
							ImPlot::ItemIcon(dnd[k].Color); ImGui::SameLine();
							ImGui::TextUnformatted(dnd[k].SignalName.c_str());
							ImPlot::EndDragDropSource();
						}
					}
				}
				// allow the main plot area to be a DND target
				if (ImPlot::BeginDragDropTargetPlot()) {
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("MY_DND")) {
						int Z = *(int*)payload->Data;
						dnd[Z].Plt = 1;
						dnd[Z].SubPlotId = i;
						dnd[Z].Yax = ImAxis_Y1;
					}
					ImPlot::EndDragDropTarget();
				}
				ImPlot::EndPlot();
			}
		}
		ImPlot::EndSubplots();
	}
	/*------------------------------------------ End Subplots----------------------------------------------------------------------*/

	/*------------------------------------------ Begin Top menu for config------------------------------------------------*/
	/*Menu for changing amount of plots*/
	if (ImGui::BeginMenuBar()) {
		if (ImGui::BeginMenu("Config")) {
			ImGui::MenuItem("Change Row/Columns", NULL, &show_rows_cols);
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}
	if (show_rows_cols)
	{
		ImGui::Begin("Configure Rows & Columns", &show_rows_cols);

		ImGui::InputInt("Rows", &rows);
		ImGui::InputInt("Columns", &cols);
		if (ImGui::Button("Done"))
		{
			show_rows_cols = false;
		}
		ImGui::End();
	}
	/*------------------------------------------ End Top menu for config--------------------------------------------------*/

	ImGui::End();
}

/***

Getters Setters

***/

void DataClientLayer::SetShowBrakeData(bool setval){m_ShowBrakeData = setval;}

void DataClientLayer::SetDriverInputsStatus(bool setval){m_ShowDriverInputStatus = setval;}

void DataClientLayer::SetLoadRunAndShowMultiSignalPlot(bool setval){ m_LoadRunAndShowMultiSignalPlot = setval;}

void DataClientLayer::SetPositionPlot(bool setval){m_ShowPositionPlot = setval;}

void DataClientLayer::SetShiftLight(bool setval){m_ShowShiftLight = setval;}

void DataClientLayer::SetMultiSignalPlotLive(bool setval) { m_ShowMultiSignalPlotLive = setval;};