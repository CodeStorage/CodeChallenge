#include "PhilipsDevice.h"

#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>

#include "json.hpp"


PhilipsDevice::PhilipsDevice(std::string IP, int portNum):
m_IP(IP), m_portNum(portNum)
{

}

PhilipsDevice::~PhilipsDevice()
{

}

bool PhilipsDevice::Initialize()
{


    return true;
}

bool PhilipsDevice::SendGETMethod(std::string URL, nlohmann::json &jresp)
{
    nlohmann::json tempJresp;
    PhilipsDevice::SendMessage(URL, tempJresp);
    if(tempJresp.empty())
    {
        std::cerr << "GET Request Failed" << std::endl;
        return false;
    }
    
    jresp = tempJresp;
    return true;
}

bool PhilipsDevice::SendPUTMethod(std::string URL, nlohmann::json &jresp)
{
    nlohmann::json tempJresp;
    PhilipsDevice::SendMessage(URL, tempJresp);
    if(tempJresp.empty())
    {
        std::cerr << "PUT Request Failed" << std::endl;
        return false;
    }
    
    jresp = tempJresp;
    return true;
}

//I copied and modified this code from LinHttpHandler a file I found online
bool PhilipsDevice::SendMessage(std::string URL, nlohmann::json &jsonResult)
{
	// create socket
	int socketFD = socket(AF_INET, SOCK_STREAM, 0);

	if (socketFD < 0)
	{
		std::cerr << "Failed to open socket\n";
		throw(std::runtime_error("Failed to open socket"));
	}

	// lookup ip address
	hostent	*server;
	server = gethostbyname(m_IP.c_str());
	if (server == NULL)
	{
		std::cerr << "Failed to find host with address " << m_IP << "\n";
		throw(std::runtime_error("Failed to find host"));
	}

	// fill in the structure
	sockaddr_in server_addr;
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(m_portNum);
	memcpy(&server_addr.sin_addr.s_addr, server->h_addr, server->h_length);

	// connect the socket
	if (connect(socketFD, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
	{
		std::cerr << "Failed to connect socket\n";
		throw(std::runtime_error("Failed to connect socket"));
	}

	// send the request

        //std::cout << "URL : \n" << URL << std::endl;
	size_t total = URL.length();
	ssize_t sent = 0;
	do
	{
		ssize_t bytes = write(socketFD, URL.c_str() + sent, total - sent);
		if (bytes < 0)
		{
			std::cerr << "Failed to write message to socket\n";
			throw(std::runtime_error("Failed to write message to socket"));
		}
		if (bytes == 0)
		{
			break;
		}
		if (bytes)
		{
			sent += bytes;
		}
	} while (sent < total);

	// receive the response
	std::string response;
	total = sizeof(response) - 1;
	int received = 0;
	char buffer[1024] = {};
	do
	{
		ssize_t bytes = read(socketFD, buffer, 1023);
		if (bytes < 0)
		{
			std::cerr << "Failed to read response from socket: " << errno << std::endl;
			throw(std::runtime_error("Failed to read response from socket"));
		}
		if (bytes == 0)
		{
			break;
		}
		if (bytes)
		{
			received += bytes;
			response.append(buffer, bytes);
                        //std::cout << response.c_str() << std::endl;
                        //break;
		}
	} while (true);

	if (received == total)
	{
		std::cerr << "Failed to store complete response from socket\n";
		throw(std::runtime_error("Failed to store complete response from socket"));
	}

        close(socketFD);
        if(response.size() > 0)
        {
            //this section parses the JSON out of the HTTP message
            size_t index = response.find("[{");
            if(index >= response.size())
                index = response.find("{\"");
            if(index < response.size())
            {
                std::string substr = response.substr(index,(response.size() - index));
                //std::cout << "substr: \n" << substr.c_str() << std::endl;
                jsonResult = nlohmann::json::parse(substr);
            }
        }

	return true;
}

