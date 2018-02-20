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
    std::string lightState;
    int lightNum, lightBri, lightHue, tempOnOff;
    bool lightOnOff;
    bool cont = true;
    int input;
    PhilipsLights lights(ip, portNum);

    lights.GetLightState(1, lightState);
    cout << "lightState 1: \n" << lightState << endl;

    lights.GetLightState(2, lightState);
    cout << "lightState 2: \n" << lightState << endl;

	do
	{ 
        lightState.clear();
        cout << endl;
        cout << "1. GetLightState" << endl;
        cout << "2. ChangeLightState" << endl;
        cout << "5. Exit" << endl;
        cout << "Enter Choice: ";
        cin >> input;
        switch ( input ) {
            case 1 : 
                cout << "GetLightState Enter Light Number: ";
                cin >> lightNum;
                lights.GetLightState(lightNum, lightState);
                cout << "lightState: \n" << lightState << endl;
            break;
            case 2 : 
                cout << "ChangeLightState Enter Light Number: ";
                cin >> lightNum;
                cout << "ChangeLightState Enter bri% (1-100): ";
                cin >> lightBri;
                cout << "ChangeLightState Enter hue (0-65535): ";
                cin >> lightHue;
                cout << "ChangeLightState Enter onOff (0=OFF,1=ON): ";
                cin >> tempOnOff;
                lightOnOff = ((tempOnOff > 0) ? true : false);
                lights.ChangeLightState(lightNum, lightBri, lightHue, lightOnOff, lightState);
                cout << "Modified lightState: \n" << lightState << endl;
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


 
