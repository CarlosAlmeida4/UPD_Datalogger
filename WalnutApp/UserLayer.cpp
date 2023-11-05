#include "UserLayer.h"
#include "ServerPacket.h"

#include "Walnut/Application.h"
#include "Walnut/UI/UI.h"
#include "Walnut/Serialization/BufferStream.h"
#include "Walnut/Networking/NetworkingUtils.h"
#include "Walnut/Utils/StringUtils.h"

#include "misc/cpp/imgui_stdlib.h"

#include <yaml-cpp/yaml.h>

#include <iostream>
#include <fstream>

void ClientLayer::OnUIRender()
{
	UI_ConnectionModal();

	//m_Console.OnUIRender();
	//UI_ClientList();
}

void ClientLayer::UI_ConnectionModal()
{
	if (!m_ConnectionModalOpen && m_Client->GetConnectionStatus() != Walnut::Client::ConnectionStatus::Connected)
	{
		ImGui::OpenPopup("Connect to server");
	}

	m_ConnectionModalOpen = ImGui::BeginPopupModal("Connect to server", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	if (m_ConnectionModalOpen)
	{
		ImGui::Text("Your Name");
		ImGui::InputText("##username", &m_Username);

		ImGui::Text("Pick a color");
		ImGui::SameLine();
		ImGui::ColorEdit4("##color", m_ColorBuffer);

		ImGui::Text("Server Address");
		ImGui::InputText("##address", &m_ServerIP);
		ImGui::SameLine();
		if (ImGui::Button("Connect"))
		{
			m_Color = IM_COL32(m_ColorBuffer[0] * 255.0f, m_ColorBuffer[1] * 255.0f, m_ColorBuffer[2] * 255.0f, m_ColorBuffer[3] * 255.0f);

			if (Walnut::Utils::IsValidIPAddress(m_ServerIP))
			{
				m_Client->ConnectToServer(m_ServerIP);
			}
			else
			{
				// Try resolve domain name
				auto ipTokens = Walnut::Utils::SplitString(m_ServerIP, ':'); // [0] == hostname, [1] (optional) == port
				std::string serverIP = Walnut::Utils::ResolveDomainName(ipTokens[0]);
				if (ipTokens.size() != 2)
					serverIP = fmt::format("{}:{}", serverIP, 8192); // Add default port if hostname doesn't contain port
				else
					serverIP = fmt::format("{}:{}", serverIP, ipTokens[1]); // Add specified port

				m_Client->ConnectToServer(serverIP);
			}

		}

		if (Walnut::UI::ButtonCentered("Quit"))
			Walnut::Application::Get().Close();

		if (m_Client->GetConnectionStatus() == Walnut::Client::ConnectionStatus::Connected)
		{
			// Send username
			Walnut::BufferStreamWriter stream(m_ScratchBuffer);
			stream.WriteRaw<PacketType>(PacketType::ClientConnectionRequest);
			stream.WriteRaw<uint32_t>(m_Color); // Color
			stream.WriteString(m_Username); // Username

			m_Client->SendBuffer(stream.GetBuffer());

			SaveConnectionDetails(m_ConnectionDetailsFilePath);

			// Wait for response
			ImGui::CloseCurrentPopup();
		}
		else if (m_Client->GetConnectionStatus() == Walnut::Client::ConnectionStatus::FailedToConnect)
		{
			ImGui::TextColored(ImVec4(0.9f, 0.2f, 0.1f, 1.0f), "Connection failed.");
			const auto& debugMessage = m_Client->GetConnectionDebugMessage();
			if (!debugMessage.empty())
				ImGui::TextColored(ImVec4(0.9f, 0.2f, 0.1f, 1.0f), debugMessage.c_str());
		}
		else if (m_Client->GetConnectionStatus() == Walnut::Client::ConnectionStatus::Connecting)
		{
			ImGui::TextColored(ImVec4(0.8f, 0.8f, 0.8f, 1.0f), "Connecting...");
		}

		ImGui::EndPopup();
	}
}