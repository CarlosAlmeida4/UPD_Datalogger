#pragma once

#include "Walnut/Layer.h"
#include "Walnut/Networking/Client.h"

#include "Walnut/UI/Console.h"

#include "UserInfo.h"

#include <set>
#include <filesystem>


class UserLayer : public Walnut::Layer
{
public:
	virtual void OnUIRender() override;
private:
	// UI
	void UI_ConnectionModal();
	void UI_ClientList();

	// Server event callbacks
	void OnConnected();
	void OnDisconnected();
	void OnDataReceived(const Walnut::Buffer buffer);

	void SendChatMessage(std::string_view message);

public:

private:
	std::unique_ptr<Walnut::Client> m_Client;
	Walnut::UI::Console m_Console{ "Chat" };
	std::string m_ServerIP;
	std::filesystem::path m_ConnectionDetailsFilePath = "ConnectionDetails.yaml";

	Walnut::Buffer m_ScratchBuffer;

	float m_ColorBuffer[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

	std::string m_Username;
	uint32_t m_Color = 0xffffffff;

	std::map<std::string, UserInfo> m_ConnectedClients;
	bool m_ConnectionModalOpen = false;
	bool m_ShowSuccessfulConnectionMessage = false;

};

