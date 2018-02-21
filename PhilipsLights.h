#include "PhilipsDevice.h"
#include <string.h>
#include "json.hpp"

class PhilipsLights : public PhilipsDevice
{
public:
    PhilipsLights(std::string IP, int portNum);
    ~PhilipsLights();
    bool Initialize();
    bool GetLightState(unsigned int lightNum, nlohmann::json &state);
    bool GetLights(nlohmann::json &lights);
    bool ChangeLightState(unsigned int lightNum, unsigned int bri, unsigned int hue, unsigned int onOff, nlohmann::json &state);

private:  
    nlohmann::json m_lightStates;

};

