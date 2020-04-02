#include "stdafx.h"

TransferManager::TransferManager()
{
	
}

// (New Thread) add new Client and start communicating with him
void TransferManager::newClient(SOCKET clientSocket)
{
	// create new client's data
	ClientData* newClient = new ClientData(clientSocket);

	// insert this data to vector in transfer manager
	clientsData.push_back(newClient);

	// call new thread to communicate with given client
	this->clientsThreads.push_back(thread(&TransferManager::communicate, this, newClient));
}

void TransferManager::addGame(GameEngine* pGame)
{
	if(this->pGame==NULL)
		this->pGame = pGame;
	else
	{
		throw exception("duplicating game");
	}
}

TransferManager::~TransferManager()
{
}

// function called bu separate thread
// communicate with your client
void TransferManager::communicate(ClientData* data)
{
	int iSendResult;
	char recvbuf[DEFAULT_BUFLEN];
	char sendbuf[DEFAULT_BUFLEN] = { "Hello, server here!" };
	int iResult;

	cout << "communicating with client!" << endl;
	std::chrono::time_point<std::chrono::system_clock> current = std::chrono::system_clock::now();
	while (true)
	{
		iResult = recv(data->getSocket(), recvbuf, DEFAULT_BUFLEN, 0);
		if (iResult > 0) {
			current = std::chrono::system_clock::now();
			cout << "Bytes received: " << iResult << endl;
			cout << recvbuf;

			iSendResult = send(data->getSocket(), sendbuf, iResult, 0);
			if (iSendResult == SOCKET_ERROR) {
				cout << "send failed with error: " << WSAGetLastError() << endl;
				closesocket(data->getSocket());
				WSACleanup();
				return;
			}
			cout << "Bytes sent: " << iSendResult << endl;
			double*dataToSent = this->pGame->serialize();
			//TODO sen data
			int a = 0;
		}
		else if (iResult == 0)
		{	
			std::chrono::time_point<std::chrono::system_clock> now;
			now = std::chrono::system_clock::now();

			std::chrono::duration<double> elapsed_seconds = now - current;
			if(elapsed_seconds.count()<TIMEOUT)
				continue;
			// TODO: deleting player from game, delete all his data, threads etc
			break;
		}

		
		else {
			cout << "recv failed with error: " << WSAGetLastError() << endl;
			break;
		}
	}
	closesocket(data->getSocket());
	WSACleanup();
}

// check if every player is ready to play and if there is at least one player
bool TransferManager::readyToPlay()
{
	// check number of connected players
	if (this->clientsData.empty())
	{
		return false;
	}

	// check if all players are ready
	for (ClientData* client : this->clientsData)
	{
		if (client->getReady() == false)
		{
			return false;
		}
	}

	return true;
}

// get pointer to client's data vector
vector<ClientData*>* TransferManager::getClientsData()
{
	return &(this->clientsData);
}