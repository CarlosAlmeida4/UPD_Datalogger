#pragma once
#include "Walnut/Layer.h"
#include "Walnut/Networking/Client.h"

#include "Walnut/UI/Console.h"


#include <set>
#include <filesystem>

#include "ImPlot/implot.h"
#include "src/EASportsWRC/EASportsWRC.h"


class DataClientLayer : public Walnut::Layer
{
public:
	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnUIRender() override;

	void SetShowBrakeData(bool setval);
	void SetDriverInputsStatus(bool setval);
	void SetLoadRunAndShowMultiSignalPlot(bool setval);
	void SetPositionPlot(bool setval);
	void SetShiftLight(bool setval);
	void SetMultiSignalPlotLive(bool setval);

private:
	
	//Show the driver current driver inputs
	void DriverInputsStatus();
	// show the current stage time and distance
	void StageStatus();
	// Show the recent Brake related Data
	void BrakeData();
	// Store run modal
	void StoreRunModal();
	//Plot multiple signals
	void LoadRunAndMultiSignalPlot();
	//Plot vehicle position
	void VehiclePosition();
	// Shift light with RPM
	void ShiftLight();
	// Load stored Run TODO
	void LoadRunModal();
	// Live window for all signals
	void MultiSignalPlotLive();

public:
	struct DragAndDropItem {
		int              SubPlotId;
		int              Plt;
		ImAxis           Yax;
		EAtelemetryfloat_t Data;
		ImVector<ImVec2> DataVec2;
		ImVec4           Color;
		std::string SignalName;
		void Reset() { Plt = 0; Yax = ImAxis_Y1; int SubPlotId = -1; }
	};

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

private:

	


	std::unique_ptr<Walnut::Client> m_Client;
	Walnut::UI::Console m_Console{ "Chat" };
	std::string m_ServerIP;
	std::string m_ServerPort;


	Walnut::Buffer m_ScratchBuffer;

	float m_ColorBuffer[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

	std::string m_Username;
	uint32_t m_Color = 0xffffffff;

	bool m_StoreRunModalOpen = false;
	bool m_LoadRunModalOpen = false;
	bool m_ShowSuccessfulConnectionMessage = false;
	std::string m_StoragePath;
	std::string m_StorageFileName;

	bool m_ShowBrakeData = false;
	bool m_ShowDriverInputStatus = false;
	bool m_LoadRunAndShowMultiSignalPlot = false;
	bool m_ShowPositionPlot = false;
	bool m_ShowShiftLight = false;
	bool m_LoadRunModalRequest = false;
	bool m_ShowMultiSignalPlotLive = false;

};
