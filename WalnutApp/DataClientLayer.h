#pragma once
#include "Walnut/Layer.h"
#include "Walnut/Networking/Client.h"

#include "Walnut/UI/Console.h"


#include <set>
#include <filesystem>

#include "ImPlot/implot.h"


class DataClientLayer : public Walnut::Layer
{
public:
	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnUIRender() override;

	void SetShowBrakeData(bool setval);
	void SetDriverInputsStatus(bool setval);
	void SetMultiSignalPlot(bool setval);
	void SetPositionPlot(bool setval);
	void SetShiftLight(bool setval);

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
	void MultiSignalPlot();
	//Plot vehicle position
	void VehiclePosition();
	// Shift light with RPM
	void ShiftLight();
	// Load stored Run TODO
	void LoadRunModal();

public:


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
	bool m_ShowMultiSignalPlot = false;
	bool m_ShowPositionPlot = false;
	bool m_ShowShiftLight = false;
	bool m_LoadRunModalRequest = false;
};
