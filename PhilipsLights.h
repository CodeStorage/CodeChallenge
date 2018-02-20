#include "PhilipsDevice.h"
#include <string.h>
#include "json.hpp"

class PhilipsLights : public PhilipsDevice
{
public:
    PhilipsLights(std::string IP, int portNum);
    ~PhilipsLights();
    bool Initialize();
    bool GetLightState(int lightNum, std::string &state);
    bool ChangeLightState(int lightNum, int bri, int hue, bool onOff, std::string &state);
    bool GetStateChange(std::string &state);

private:  
    std::vector<nlohmann::json> lightStates;

    struct LightState {
        std::string on;
        int bri;
        int hue;
        int sat;
        int xy[2];
        int ct;
        std::string alert;
        std::string effect;
        std::string colormode;
        std::string reachable;

    };

};

