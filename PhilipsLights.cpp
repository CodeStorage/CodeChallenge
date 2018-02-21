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
    nlohmann::json lightState;
    bool rs(true);

    PhilipsDevice::Initialize();
    //get all lights and store num lights
    //only lights 1,2

    //get state on all lights and store bri,hue,onOff
    rs = PhilipsLights::GetLightState(1, lightState);
    rs = rs && PhilipsLights::GetLightState(2, lightState);
    return rs;
}

bool PhilipsLights::GetLightState(unsigned int lightNum, nlohmann::json &state)
{
    nlohmann::json jresp;
    std::ostringstream stringStream;
    std::string tempLightNum = std::to_string(lightNum);
    stringStream << "GET /api/newdeveloper/lights/" << lightNum << " HTTP/1.1\r\n";
    stringStream << "Connection: close\r\n\r\n";
    std::string URL = stringStream.str();
    //std::cout << "URL: " << URL << std::endl;
    if(!PhilipsDevice::SendGETMethod(URL, jresp))
    {
        return false;
    }

    m_lightStates[tempLightNum]["bri"] = jresp["state"]["bri"];
    m_lightStates[tempLightNum]["hue"] = jresp["state"]["hue"];
    m_lightStates[tempLightNum]["on"] = jresp["state"]["on"];

    state["name"] = jresp["name"];
    state["hue"] = jresp["state"]["hue"];
    state["on"] = jresp["state"]["on"];
    state["brightness"] = jresp["state"]["bri"];

    return true;
}

bool PhilipsLights::GetLights(nlohmann::json &lights)
{
    nlohmann::json jresp;
    std::ostringstream stringStream;
    stringStream << "GET /api/newdeveloper/lights HTTP/1.1\r\n";
    stringStream << "Connection: close\r\n\r\n";
    std::string URL = stringStream.str();
    //std::cout << "URL: " << URL << std::endl;
    if(!PhilipsDevice::SendGETMethod(URL, jresp))
    {
        return false;
    }

    lights = jresp;

    return true;
}

bool PhilipsLights::ChangeLightState(unsigned int lightNum,unsigned int bri, unsigned int hue, unsigned int onOff, nlohmann::json &newState)
{
    nlohmann::json jresp;
    nlohmann::json tempJresp, jpayload;
    bool boolOnOff;
    std::string tempLightNum = std::to_string(lightNum);
    
    //parameter range checking
    if(hue > 65535)
    { 
        std::cerr << "hue out of range" << std::endl;
        return false;
    }
    if(bri > 100)
    { 
        std::cerr << "bri out of range" << std::endl;
        return false;
    }

    //Construct REST URL
    boolOnOff = ((onOff > 0) ? true : false);
    jpayload["hue"] = hue;
    jpayload["on"] = boolOnOff;
    jpayload["bri"] = bri * 254/100;
    std::string tempPayloadMsg = jpayload.dump(2);

    std::ostringstream stringStream;
    stringStream << "PUT /api/newdeveloper/lights/" << tempLightNum << "/state HTTP/1.1\r\n";
    stringStream << "Content-Type: application/json\r\n";
    stringStream << "Content-Length: " << tempPayloadMsg.size() << "\r\n";
    stringStream << "Connection: close\r\n\r\n";

    std::string URL = stringStream.str();
    URL = URL + tempPayloadMsg;
    //std::cout << "URL: \n" << URL << "\r\n\r\n";
    if(!PhilipsDevice::SendPUTMethod(URL, jresp))
    {
        return false;
    }
    
    //update private variables and construct json table of changed values
    stringStream.str(std::string()); //clear stream
    stringStream << "/lights/" << tempLightNum << "/state/bri";
    if(m_lightStates[tempLightNum]["bri"] != jresp[0]["success"][stringStream.str()])
    {
        newState["bri"] = jresp[0]["success"][stringStream.str()];
        m_lightStates[tempLightNum]["bri"] = jresp[0]["success"][stringStream.str()];
    }
    stringStream.str(std::string()); //clear stream
    stringStream << "/lights/" << tempLightNum << "/state/hue";
    if(m_lightStates[tempLightNum]["hue"] != jresp[1]["success"][stringStream.str()])
    {
        newState["hue"] = jresp[1]["success"][stringStream.str()];
        m_lightStates[tempLightNum]["hue"] = jresp[1]["success"][stringStream.str()];
    }
    stringStream.str(std::string()); //clear stream
    stringStream << "/lights/" << tempLightNum << "/state/on";
    if(m_lightStates[tempLightNum]["on"] != jresp[2]["success"][stringStream.str()])
    {
        newState["on"] = jresp[2]["success"][stringStream.str()];
        m_lightStates[tempLightNum]["on"] = jresp[2]["success"][stringStream.str()];
    }

    return true;
}
