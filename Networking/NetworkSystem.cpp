#include "Engine/Networking/NetworkSystem.hpp"
#include "Engine/Core/EngineCommon.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <WinSock2.h>
#include <WS2TCPIP.h>
#pragma comment(lib, "Ws2_32.lib")

#include "Engine/Core/DevConsole.hpp"

NetworkSystem::NetworkSystem(NetworkSystemConfig const& config)
	:m_config(config),
	 m_myClientSocket(INVALID_SOCKET),
	 m_myServerListenSocket(INVALID_SOCKET)
{
}

NetworkSystem::~NetworkSystem()
{
}

void NetworkSystem::Startup()
{
	m_currentNetworkState = NETWORKSTATE_UNINITIALIZED;

	WSADATA data;
	int errorCode = WSAStartup(MAKEWORD(2, 2), &data);

	if (errorCode != 0)
	{
		ERROR_AND_DIE(Stringf("WSAStartup failed with error %d", errorCode));
	}

	g_theDevConsole->AddLine(DevConsole::INFO_MINOR, "NetworkSystem Started");
	m_currentNetworkState = NETWORKSTATE_IDLE;
}

void NetworkSystem::Shutdown()
{
	// Disconnect each client
	for (int clientIndex = 0; clientIndex < static_cast<int>(m_externalClientSockets.size()); ++clientIndex)
	{
		uint64_t client = m_externalClientSockets[clientIndex];
		Disconnect(client);
	}

	// Disconnect my client socket
	Disconnect(m_myClientSocket);
	
	// Disconnect my server listen socket
	Disconnect(m_myServerListenSocket);

	// Call Cleanup and return state to uninitialized
	int errorCode = WSACleanup();
	if (errorCode != 0)
	{
		ERROR_RECOVERABLE(Stringf("WSACleanup failed with error %d", WSAGetLastError()));
	}
	m_currentNetworkState = NETWORKSTATE_UNINITIALIZED;
}

void NetworkSystem::BeginFrame()
{
	switch (m_currentNetworkState)
	{
		case NETWORKSTATE_SERVER_LISTENING:
		{
			CheckIncomingConnections();
			CheckServerClientCommunication();
			break;
		}
		case NETWORKSTATE_CLIENT_CONNECTING:
		{
			CheckClientConnection();
			break;
		}
		case NETWORKSTATE_CLIENT_CONNECTED:
		{
			CheckClientCommunication();
			break;
		}
		default:
		{
			break;
		}
	}
}

void NetworkSystem::EndFrame()
{
}

void NetworkSystem::StartServer()
{
	// Create a server listen socket
	m_myServerListenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	// Set listen socket to non-blocking
	unsigned long blockingMode = 1;
	ioctlsocket(static_cast<SOCKET>(m_myServerListenSocket), FIONBIO, &blockingMode);

	// Bind my server listen socket to an incoming port
	uint32_t myIPAddressU32 = INADDR_ANY;
	uint16_t myListenPortU16 = static_cast<unsigned short>(atoi(m_config.m_port.c_str()));
	sockaddr_in addr;
	addr.sin_family = AF_INET;										
	addr.sin_addr.S_un.S_addr = htonl(myIPAddressU32);				
	addr.sin_port = htons(myListenPortU16);

	int result = bind(static_cast<SOCKET>(m_myServerListenSocket), (sockaddr*)&addr, (int)sizeof(addr));
	if (result == SOCKET_ERROR)
	{
		ERROR_AND_DIE(Stringf("Bind failed with error: %d", WSAGetLastError()));
	}

	// Listen for new incoming connections on this socket
	result = listen(static_cast<SOCKET>(m_myServerListenSocket), SOMAXCONN);
	if (result == SOCKET_ERROR)
	{
		ERROR_AND_DIE(Stringf("Listen failed with error: %d", WSAGetLastError()));
	}

	// Change network state to server listening
	m_currentNetworkState = NETWORKSTATE_SERVER_LISTENING;
	g_theDevConsole->AddLine(DevConsole::INPUT_INSERTION_POINT, "Network Server started and listening for clients");
}

void NetworkSystem::StartClient()
{
	// Create a client socket
	m_myClientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	// Set client socket to non-blocking
	unsigned long blockingMode = 1;
	ioctlsocket(static_cast<SOCKET>(m_myClientSocket), FIONBIO, &blockingMode);

	// Begin connecting to server
	IN_ADDR addr;												
	int result = inet_pton(AF_INET, m_config.m_address.c_str(), &addr);
	if (result != 1)
	{
		ERROR_AND_DIE(Stringf("inet_pton failed for address '%s'. Error code: %d", m_config.m_address.c_str(), WSAGetLastError()));
	}
	uint32_t serverIPAddressU32 = ntohl(addr.S_un.S_addr);			
	uint16_t serverPortU16 = static_cast<unsigned short>(atoi(m_config.m_port.c_str()));

	// Get socket in address and connect
	sockaddr_in sockIn_addr;											
	sockIn_addr.sin_family = AF_INET;
	sockIn_addr.sin_addr.S_un.S_addr = htonl(serverIPAddressU32);
	sockIn_addr.sin_port = htons(serverPortU16);
	connect(static_cast<SOCKET>(m_myClientSocket), (sockaddr*)&sockIn_addr, (int)sizeof(sockIn_addr));

	// Change NetworkState to connecting
	m_currentNetworkState = NETWORKSTATE_CLIENT_CONNECTING;
}

void NetworkSystem::Disconnect(uint64_t client)
{
	if (client != INVALID_SOCKET)
	{
		closesocket(static_cast<SOCKET>(client));
		client = INVALID_SOCKET;
	}
	m_currentNetworkState = NetworkState::NETWORKSTATE_DISCONNECTING;
}

bool NetworkSystem::IsConnected() const
{
	return m_currentNetworkState == NetworkState::NETWORKSTATE_CLIENT_CONNECTED || m_currentNetworkState == NetworkState::NETWORKSTATE_SERVER_LISTENING;
}

bool NetworkSystem::IsServer() const
{
	return m_myServerListenSocket;
}

void NetworkSystem::CheckIncomingConnections()
{
	SOCKET newClientSocket = accept(static_cast<SOCKET>(m_myServerListenSocket), NULL, NULL);
	if (newClientSocket != INVALID_SOCKET)
	{
		unsigned long blockingMode = 1;
		ioctlsocket(newClientSocket, FIONBIO, &blockingMode);
		m_externalClientSockets.push_back(newClientSocket);
	}
}

void NetworkSystem::CheckServerClientCommunication()
{
	char receiveBuffer[2048];

	for (int clientIndex = 0; clientIndex < static_cast<int>(m_externalClientSockets.size()); ++clientIndex)
	{
		SOCKET clientSocket = static_cast<SOCKET>(m_externalClientSockets[clientIndex]);

		while (!m_outgoingData.empty())
		{
			unsigned char byte = m_outgoingData.front();
			send(clientSocket, reinterpret_cast<char*>(&byte), 1, 0);
			m_outgoingData.pop_front();
		}

		int bytesReceived = recv(clientSocket, receiveBuffer, sizeof(receiveBuffer), 0);
		if (bytesReceived > 0)
		{
			m_incomingData.insert(m_incomingData.end(), receiveBuffer, receiveBuffer + bytesReceived);
		}
		else if (bytesReceived == 0 || (bytesReceived == SOCKET_ERROR && WSAGetLastError() != WSAEWOULDBLOCK))
		{
			closesocket(clientSocket);
			m_externalClientSockets.erase(m_externalClientSockets.begin() + clientIndex);
			clientIndex -= 1;
		}
	}
}

void NetworkSystem::CheckClientConnection()
{
	fd_set writeSockets;
	fd_set exceptSockets;

	FD_ZERO(&writeSockets);
	FD_ZERO(&exceptSockets);
	FD_SET(static_cast<SOCKET>(m_myClientSocket), &writeSockets);
	FD_SET(static_cast<SOCKET>(m_myClientSocket), &exceptSockets);

	timeval waitTime = { 0, 0 };
	int result = select(0, nullptr, &writeSockets, &exceptSockets, &waitTime);

	if (result == SOCKET_ERROR)
	{
		// TODO: Add a retry or fail case
		return;
	}

	if (FD_ISSET(m_myClientSocket, &exceptSockets))
	{
		Disconnect(m_myClientSocket);
		m_currentNetworkState = NETWORKSTATE_IDLE;
		return;
	}

	if (FD_ISSET(m_myClientSocket, &writeSockets))
	{
		m_currentNetworkState = NETWORKSTATE_CLIENT_CONNECTED;
	}
}

void NetworkSystem::CheckClientCommunication()
{
	while (!m_outgoingData.empty())
	{
		unsigned char byte = m_outgoingData.front();
		send(static_cast<SOCKET>(m_myClientSocket), reinterpret_cast<char*>(&byte), 1, 0);
		m_outgoingData.pop_front();
	}

	char receiveBuffer[2048];
	int bytesReceived = recv(static_cast<SOCKET>(m_myClientSocket), receiveBuffer, sizeof(receiveBuffer), 0);
	if (bytesReceived > 0)
	{
		m_incomingData.insert(m_incomingData.end(), receiveBuffer, receiveBuffer + bytesReceived);
	}
	else if (bytesReceived == 0 || (bytesReceived == SOCKET_ERROR && WSAGetLastError() != WSAEWOULDBLOCK))
	{
		Disconnect(m_myClientSocket);
		m_currentNetworkState = NETWORKSTATE_IDLE;
	}
}

void NetworkSystem::SendStringToAll(std::string const& text)
{
	// Sending the string
	for (int characterIndex = 0; characterIndex < static_cast<int>(text.size()); ++characterIndex)
	{
		char character = text[characterIndex];
		m_outgoingData.push_back(static_cast<unsigned char>(character));
	}

	// Ending the string
	m_outgoingData.push_back('\0');
}

void NetworkSystem::ProcessIncomingMessages()
{
	while (true) 
	{
		auto found = std::find(m_incomingData.begin(), m_incomingData.end(), '\0');
		if (found == m_incomingData.end()) 
		{
			break;
		}

		std::string commandString(m_incomingData.begin(), found);
		m_incomingData.erase(m_incomingData.begin(), found + 1);

		std::string remoteCmd = "RemoteCmd cmd=" + commandString;
		g_theDevConsole->Execute(remoteCmd);
	}
}

NetworkSystemConfig NetworkSystem::GetNetworkConfig() const
{
	return m_config;
}

uint64_t NetworkSystem::GetMyClientSocket() const
{
	return m_myClientSocket;
}

std::vector<uint64_t> NetworkSystem::GetExternalClients() const
{
	return m_externalClientSockets;
}
