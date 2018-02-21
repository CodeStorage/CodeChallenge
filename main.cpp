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
#include "PhilipsLights.h"

using namespace std;

int main()
{
    int portNum = 8080; // NOTE that the port number is same for both client and server
    string ip = "192.168.0.2";
    nlohmann::json lightState;
    unsigned int lightNum, lightBri, lightHue, lightOnOff;
    bool cont = true;
    int input;
    PhilipsLights lights(ip, portNum);
    lights.Initialize();
    
    lights.GetLightState(1, lightState);
    cout << "lightState 1: \n" << lightState.dump(4) << endl;
    lightState.clear();
    lights.GetLightState(2, lightState);
    cout << "lightState 2: \n" << lightState.dump(4) << endl;

	do
	{ 
        lightState.clear();
        cout << endl;
        cout << "1. GetLightState" << endl;
        cout << "2. ChangeLightState" << endl;
        cout << "3. GetLights" << endl;
        cout << "5. Exit" << endl;
        cout << "Enter Choice: ";
        cin >> input;
        switch ( input ) {
            case 1 : 
                cout << "GetLightState Enter Light Number: ";
                cin >> lightNum;
                lights.GetLightState(lightNum, lightState);
                cout << "lightState: \n" << lightState.dump(4) << endl;
            break;
            case 2 : 
                cout << "ChangeLightState Enter Light Number: ";
                cin >> lightNum;
                cout << "Enter bri% (1-100): ";
                cin >> lightBri;
                cout << "Enter hue (0-65535): ";
                cin >> lightHue;
                cout << "Enter onOff (0=OFF,1=ON): ";
                cin >> lightOnOff;
                
                lights.ChangeLightState(lightNum, lightBri, lightHue, lightOnOff, lightState);
                cout << "Modified lightState: \n" << lightState.dump(4) << endl;
            break;
            case 3 : 
                lights.GetLights(lightState);
                cout << "GetLights: \n" << lightState.dump(4) << endl;
            break;
            case 5 : 
                cont = false;
            break;
            default : 
                cout << "Invalid Choice" << endl;
            break;
        }

	} while (cont == true);

    return 0;
} 


 
