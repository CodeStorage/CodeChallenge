#include "PhilipsLights.h"

#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <string.h>

PhilipsLights::PhilipsLights(std::string IP, int portNum):
PhilipsDevice(IP, portNum)
{

}

PhilipsLights::~PhilipsLights()
{

}

bool PhilipsLights::Initialize()
{
return true;
}

bool PhilipsLights::GetLightState(int lightNum, std::string &state)
{
    nlohmann::json jresp;
    std::ostringstream stringStream;
    stringStream << "GET /api/newdeveloper/lights/" << lightNum << " HTTP/1.1\r\n";
    stringStream << "Connection: close\r\n\r\n";
    std::string URL = stringStream.str();
    //std::cout << "URL: " << URL << std::endl;
    if(!PhilipsDevice::SendGETMethod(URL, jresp))
    {
        return false;
    }

    state = jresp.dump(4);

    return true;
}

bool PhilipsLights::ChangeLightState(int lightNum, int bri, int hue, bool onOff, std::string &state)
{
    nlohmann::json jresp;
    nlohmann::json jpayload;
    jpayload["hue"] = hue;
    jpayload["on"] = onOff;
    jpayload["bri"] = bri * 254/100;
    std::string tempPayloadMsg = jpayload.dump(2);

    std::ostringstream stringStream;
    stringStream << "PUT /api/newdeveloper/lights/" << lightNum << "/state HTTP/1.1\r\n";
    stringStream << "Content-Type: application/json\r\n";
    stringStream << "Content-Length: " << tempPayloadMsg.size() << "\r\n";
    //stringStream << "Content-Length: 45\r\n";
    stringStream << "Connection: close\r\n\r\n";

    std::string URL = stringStream.str();
    URL = URL + tempPayloadMsg;
    //std::cout << "URL: \n" << URL << "\r\n\r\n";
    if(!PhilipsDevice::SendPOSTMethod(URL, jresp))
    {
        return false;
    }

    state = jresp.dump(4);

    return true;
}
