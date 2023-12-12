#pragma once
#include "Walnut/Layer.h"
#include "Walnut/Networking/Client.h"

#include "Walnut/UI/Console.h"


#include <set>
#include <filesystem>


class DataClientLayer : public Walnut::Layer
{
public:
	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnUIRender() override;

	bool IsConnected() const;
	void OnDisconnectButton();
private:
	// UI
	void UI_ConnectionModal();
	void UI_ClientList();
	void ConnectButton();

	// Server event callbacks
	void OnConnected();
	void OnDisconnected();
	void OnDataReceived(const Walnut::Buffer buffer);
	void SendChatMessage(std::string_view message);
	
	//Output Windows
	void DriverInputs();
	void StageStatus();

private:
	void SaveConnectionDetails(const std::filesystem::path& filepath);
	bool LoadConnectionDetails(const std::filesystem::path& filepath);
private:
	std::unique_ptr<Walnut::Client> m_Client;
	Walnut::UI::Console m_Console{ "Chat" };
	std::string m_ServerIP;
	std::string m_ServerPort;
	std::filesystem::path m_ConnectionDetailsFilePath = "ConnectionDetails.yaml";

	Walnut::Buffer m_ScratchBuffer;

	float m_ColorBuffer[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

	std::string m_Username;
	uint32_t m_Color = 0xffffffff;

	bool m_ConnectionModalOpen = false;
	bool m_ShowSuccessfulConnectionMessage = false;
};
