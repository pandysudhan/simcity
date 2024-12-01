# SimCity Simulation

This project simulates the growth of a city over time, including residential, commercial, and industrial zones, their interactions with pollution, and an advanced road network system.

## Features

### Basic Features (Project 1)
- Zone management (Residential, Commercial, Industrial)
- Population growth simulation
- Power distribution
- Pollution tracking
- Area analysis

### Road Network Features (Project 2)
- Two types of roads:
  - Two-Lane Roads ('-'): Lower capacity (100 units)
  - Highways ('#'): Higher capacity (300 units)
- Traffic Management:
  - Congestion detection and avoidance
  - Real-time traffic monitoring
  - Best path calculation considering traffic levels
- Connectivity:
  - Automatic verification of road network connectivity
  - Zone access requirements for growth
  - Path finding between any two points

## Building the Project

1. Make sure you have a C++ compiler (g++) installed
2. Compile the project using g++:
   ```bash
   g++ -std=c++11 -o simcity main.cpp Cell.cpp Grid.cpp Simulation.cpp ResidentialZone.cpp CommercialZone.cpp IndustrialZone.cpp Road.cpp RoadNetwork.cpp
   ```

## Running the Simulation

1. Prepare your configuration file (e.g., `config.txt`) with the following format:
   ```
   Region Layout:region1.csv
   Time Limit:20
   Refresh Rate:1
   ```
   Where:
   - Region Layout: specifies the name of the region layout file
   - Time Limit: maximum number of time steps for the simulation
   - Refresh Rate: how often the region state should be displayed (1 means every time step)

2. Prepare your region layout file (e.g., `region1.csv`) in CSV format using these symbols:
   - R: Residential zone
   - I: Industrial zone
   - C: Commercial zone
   - -: Two-lane road
   - #: Highway
   - T: Powerline
   - P: Power plant
   - (space): Empty cell

3. Run the simulation:
   ```bash
   ./simcity
   ```

4. When prompted, enter the name of your configuration file.

## Traffic System

The simulation includes a sophisticated traffic management system:

1. Road Capacity:
   - Two-lane roads can handle up to 100 units of traffic
   - Highways can handle up to 300 units of traffic

2. Congestion:
   - Roads become congested when traffic exceeds 80% of capacity
   - Congested roads are highlighted in the display
   - The simulation automatically routes traffic to avoid congested areas

3. Connectivity:
   - The system ensures all zones are connected by roads
   - Zones require road access to grow
   - The simulation finds optimal paths between locations based on:
     - Distance
     - Current traffic levels
     - Road capacity

4. Traffic Simulation:
   - Traffic levels are updated each time step
   - Random fluctuations simulate peak and off-peak periods
   - Congestion affects zone growth and development

## Zone Growth Requirements

For a zone to grow, it must have:
1. Adequate power supply
2. Sufficient adjacent population
3. Road access (new in Project 2)
4. Available workers/goods (for Industrial/Commercial)

## Display Information

The simulation displays:
1. Grid State:
   - Zone types (R, I, C)
   - Road types (-, #)
   - Power infrastructure (P, T)
   - Population levels

2. Traffic Information:
   - Road network connectivity status
   - Congested road locations
   - Current traffic levels

3. Statistics:
   - Population by zone type
   - Pollution levels
   - Area analysis

## Example Region Layout
```
R,R,-,#,#,#,C,C
R,R,-,-,-,#,C,C
-,-,-,I,I,#,C,C
#,#,#,I,I,#,-,-
-,-,#,I,I,#,-,P
R,R,#,-,-,#,C,C
R,R,-,-,-,-,C,C
```

This layout demonstrates:
- Mixed road types (highways and two-lane roads)
- Connected zones (residential, industrial, commercial)
- Power infrastructure
- Strategic traffic flow management

## Implementation Details

1. Road Types:
   ```cpp
   enum class RoadType {
       TWO_LANE,    // Capacity: 100
       HIGHWAY      // Capacity: 300
   };
   ```

2. Traffic Management:
   ```cpp
   void updateCongestionStatus() {
       // Road is congested if traffic > 80% capacity
       isCongested = (currentTraffic > capacity * 0.8);
   }
   ```

3. Path Finding:
   - Uses A* algorithm with congestion weighting
   - Considers both distance and traffic levels
   - Automatically routes around congested areas

4. Connectivity:
   - Uses Breadth-First Search to verify network connectivity
   - Ensures all zones have access to the road network
   - Checks connectivity before allowing zone growth