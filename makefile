all:main.cpp
	g++ -o main main.cpp PhilipsLights.* PhilipsDevice.* json.hpp

clean:
	rm main
