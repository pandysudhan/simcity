# SimCity Simulation

This project simulates the growth of a city over time, including residential, commercial, and industrial zones, and their interactions with pollution.

## Building the Project

1. Make sure you have a C++ compiler (g++) installed
2. Compile the project using g++:
   ```bash
   g++ -std=c++11 -o simcity main.cpp Cell.cpp Grid.cpp Simulation.cpp ResidentialZone.cpp CommercialZone.cpp IndustrialZone.cpp
   ```

## Running the Simulation

1. Prepare your configuration file (e.g., `config.txt`) with the following format:
   ```
   region1.csv
   50
   5
   ```
   Where:
   - First line: name of the region layout file
   - Second line: maximum number of time steps
   - Third line: refresh rate for displaying the region state

2. Prepare your region layout file (e.g., `region1.csv`) in CSV format using these symbols:
   - R: Residential zone
   - I: Industrial zone
   - C: Commercial zone
   - -: Road
   - T: Powerline
   - #: Powerline over road
   - P: Power plant
   - (space): Empty cell

3. Run the simulation:
   ```bash
   ./simcity
   ```

4. When prompted, enter the name of your configuration file.

5. The simulation will run and display:
   - Initial region state
   - Periodic updates based on the refresh rate
   - Final statistics
   - Area analysis based on user-input coordinates

## Example Region Layout
```
 , ,T,#,T,T,T, 
I,I,I,-,C,C,T, 
I,I,I,-,C,C,T,P
I,I,I,-,C,C,T, 
-,-,-,-,-,-,#,-
 , ,-,R,R,R,T, 
 , ,-,R,R,R, , 
 , ,-,R,R,R, , 