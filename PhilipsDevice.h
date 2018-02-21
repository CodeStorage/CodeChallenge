#include <string.h>
#include "json.hpp"

class PhilipsDevice
{
public:
    PhilipsDevice(std::string IP, int portNum);
    ~PhilipsDevice();
    bool Initialize();
    bool SendGETMethod(std::string URL, nlohmann::json &jresp);
    bool SendPUTMethod(std::string URL, nlohmann::json &jresp);
    bool SendMessage(std::string msg, nlohmann::json &jsonResult);

private:  

    const std::string m_IP;
    const int m_portNum;

};

