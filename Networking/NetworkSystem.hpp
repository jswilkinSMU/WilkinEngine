#pragma once
#include <cstdint>
#include <deque>
#include <vector>
#include <string>
// -----------------------------------------------------------------------------
struct NetworkSystemConfig
{
	int m_numClients = 8;
	std::string m_port = "3100";
	std::string m_address = "127.0.0.1";
	int m_sendBufferSize = 2048;
	int m_recvBufferSize = 2048;
};
// -----------------------------------------------------------------------------
enum NetworkState
{
	NETWORKSTATE_UNINITIALIZED = -1,
	NETWORKSTATE_IDLE,
	NETWORKSTATE_SERVER_LISTENING,
	NETWORKSTATE_CLIENT_CONNECTING,
	NETWORKSTATE_CLIENT_CONNECTED,
	NETWORKSTATE_DISCONNECTING,
	NUM_NETWORK_STATES
};
// -----------------------------------------------------------------------------
class NetworkSystem
{
public:
	NetworkSystem(NetworkSystemConfig const& config);
	~NetworkSystem();
	void Startup();
	void Shutdown();
	void BeginFrame();
	void EndFrame();

	void StartServer();
	void StartClient();
	void Disconnect(uint64_t client);
	bool IsConnected() const;  
	bool IsServer() const;

	void CheckIncomingConnections();
	void CheckServerClientCommunication();
	void CheckClientConnection();
	void CheckClientCommunication();
	void SendStringToAll(std::string const& text);
	void ProcessIncomingMessages();

	NetworkSystemConfig   GetNetworkConfig() const;
	uint64_t			  GetMyClientSocket() const;
	std::vector<uint64_t> GetExternalClients() const;
private:
	NetworkSystemConfig m_config;
	NetworkState m_currentNetworkState = NETWORKSTATE_UNINITIALIZED;

	// If I am the client
	uint64_t m_myClientSocket; 
	std::deque<unsigned char> m_incomingData; // pop front to receive
	std::deque<unsigned char> m_outgoingData; // push back to send FIFO

	// If I am the server
	uint64_t m_myServerListenSocket;
	std::vector<uint64_t> m_externalClientSockets;
 };
// -----------------------------------------------------------------------------