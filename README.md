"# mod.ui" 

A configurable desktop application meant to communicate with any MODBUS-capable device over the Serial Port. Features:
 - built using the Qt Framework
 - takes as input a configuration file that contains a list of pertinent parameters that need to be scanned from device via MODBUS; it will then automatically generate its own custom layout so as to display the data in the organizational manner suggested by the config file
 - will automatically scan all available unreserved serial ports (or the user can manually specify a port) to search for a MODBUS-capable device; it will then continuously read and update all of the parameters; also supports writing to the device and buttons for reading/writing to boolean parameters on the device

How the Code Works:
main.cpp is the entry point, which instantiates the QML Engine; the entire backend is handled by the backend class found in the backend.h/backend.cpp files. It creates child classes to handle:
 - Communication: the SerComm class maintains and uses the serial port held by the software. The ModbusModule class handles the MODBUS protocol
 - Data Handling: the MotorParameter class holds a single MODBUS parameter; ParameterBox class is responsible for holding all the MotorParameters (they are organized by whether they are contiguous in the MODBUS address space) data (along with helper methods to parse the data and format MODBUS commands); the ParamWarehouse class is responsible for reading the config file and dynamically generating ParameterBox instances to contain all of the specified Parameters. 
 - Front-end Customization: The PageTemplate class generates the QML Code necessary to render all the views in the front end; the ParamWarehouse class feeds the parsed config file data to the PageTemplate class so that pages for different sets of parameters can be created
 - The ParamWarehouse is also responsible for reacting to changes in the parameters (either read from the device or entered by the user) and updating them on the front-end (or in the device with a write command, if specified by the user).
 

