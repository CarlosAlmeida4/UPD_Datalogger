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

#include "UDPClient.h"

#include "ImPlot/implot.h"


// utility structure for realtime plot
struct RollingBuffer {
	float Span;
	ImVector<ImVec2> Data;
	RollingBuffer() {
		Span = 10.0f;
		Data.reserve(2000);
	}
	void AddPoint(float x, float y) {
		float xmod = fmodf(x, Span);
		if (!Data.empty() && xmod < Data.back().x)
			Data.shrink(0);
		Data.push_back(ImVec2(xmod, y));
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
	//ConnectButton();
	DriverInputsStatus();
	StageStatus();
	BrakeData();
	if (!lUDPClient.isRunning_b)
	{
		lUDPClient.startClient();
	}

}

void DataClientLayer::ConnectButton() {
	ImGui::Begin("Hello");
	bool ConnectionPressed = ImGui::Button("Button");
	ImGui::End();
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

	ImGui::End();
}

void DataClientLayer::BrakeData()
{
	
	ImGui::Begin("Brake Data");

	static RollingBuffer BrakePos;
	float current_time = 0;

	if (l_EASportsWRC.TelemetryData_v.size() != 0)
	{
		current_time = l_EASportsWRC.TelemetryData_v.back().current_time;
		float BrakePosition = l_EASportsWRC.TelemetryData_v.back().brake;
		
		BrakePos.AddPoint(current_time, BrakePosition);
	}

	static float history = 10.0f;
	ImGui::SliderFloat("History", &history, 1, 30, "%.1f s");
	BrakePos.Span = history;

	static ImPlotAxisFlags flags = ImPlotAxisFlags_NoTickLabels;
	/*
	if (ImPlot::BeginPlot(" Brake Data ", ImVec2(-1, 150)))
	{
		ImPlot::SetupAxes(NULL, NULL, flags, flags);
		ImPlot::SetupAxisLimits(ImAxis_X1, current_time - history, current_time, ImGuiCond_Always);
		ImPlot::SetupAxisLimits(ImAxis_Y1, 0, 1);
		ImPlot::SetNextFillStyle(IMPLOT_AUTO_COL, 0.5f);
		if (BrakePos.Data.Size != 0)
		{
			ImPlot::PlotLine("Brake Pedal", &BrakePos.Data[0].x, &BrakePos.Data[0].y, BrakePos.Data.size(), 0, 0, 2 * sizeof(float));
			std::cout << "Brake Position: " << BrakePos.Data[0].y << std::endl;
		}
		ImPlot::EndPlot();
	}
	*/
	if (ImPlot::BeginPlot(" Brake Data ", ImVec2(-1, 150)))
	{
		ImPlot::SetupAxes(NULL, NULL, flags, flags);
		ImPlot::SetupAxisLimits(ImAxis_X1, 0, history, ImGuiCond_Always);
		ImPlot::SetupAxisLimits(ImAxis_Y1, 0, 1);
		ImPlot::SetNextFillStyle(IMPLOT_AUTO_COL, 0.5f);
		if (BrakePos.Data.Size != 0)
		{
			ImPlot::PlotLine("Brake Pedal", &BrakePos.Data[0].x, &BrakePos.Data[0].y, BrakePos.Data.size(), 0, 0, 2 * sizeof(float));
			std::cout << "Brake Position: " << BrakePos.Data[0].y << std::endl;
		}
		ImPlot::EndPlot();
	}
	

	ImGui::End();
}

void DataClientLayer::OnDisconnectButton()
{
	m_Client->Disconnect();
}

void DataClientLayer::OnConnected()
{
	m_Console.ClearLog();
	// Welcome message sent in PacketType::ClientConnectionRequest response handling
}

void DataClientLayer::OnDisconnected()
{
	m_Console.AddItalicMessageWithColor(0xff8a8a8a, "Lost connection to server!");
}

void DataClientLayer::SaveConnectionDetails(const std::filesystem::path& filepath)
{
	YAML::Emitter out;
	{
		out << YAML::BeginMap; // Root
		out << YAML::Key << "ConnectionDetails" << YAML::Value;

		out << YAML::BeginMap;
		out << YAML::Key << "Username" << YAML::Value << m_Username;
		out << YAML::Key << "Color" << YAML::Value << m_Color;
		out << YAML::Key << "ServerIP" << YAML::Value << m_ServerIP;
		out << YAML::EndMap;

		out << YAML::EndMap; // Root
	}

	std::ofstream fout(filepath);
	fout << out.c_str();
}

bool DataClientLayer::LoadConnectionDetails(const std::filesystem::path& filepath)
{
	if (!std::filesystem::exists(filepath))
		return false;

	YAML::Node data;
	try
	{
		data = YAML::LoadFile(filepath.string());
	}
	catch (YAML::ParserException e)
	{
		std::cout << "[ERROR] Failed to load message history " << filepath << std::endl << e.what() << std::endl;
		return false;
	}

	auto rootNode = data["ConnectionDetails"];
	if (!rootNode)
		return false;

	m_Username = rootNode["Username"].as<std::string>();

	m_Color = rootNode["Color"].as<uint32_t>();
	ImVec4 color = ImColor(m_Color).Value;
	m_ColorBuffer[0] = color.x;
	m_ColorBuffer[1] = color.y;
	m_ColorBuffer[2] = color.z;
	m_ColorBuffer[3] = color.w;

	m_ServerIP = rootNode["ServerIP"].as<std::string>();

	return true;
}
