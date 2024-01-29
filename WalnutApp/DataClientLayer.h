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


private:
	
	//Show the driver current driver inputs
	void DriverInputsStatus();
	// show the current stage time and distance
	void StageStatus();
	// Show the recent Brake related Data
	void BrakeData();
	// Store run modal
	void StoreRunModal();

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
	bool m_ShowSuccessfulConnectionMessage = false;
	std::string m_StoragePath;
	std::string m_StorageFileName;
	
};
