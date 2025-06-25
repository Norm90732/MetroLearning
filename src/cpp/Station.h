//Each station is initiliazed with values computed for a whole day.
//Using precomputed values stored in a vector that is traversed as the day increases.
#ifndef STATION_H
#define STATION_H

#include <string>
#include <vector>
#include <map>
#include <random>
#include <utility>
#include <chrono>
#include <algorithm>
#include <cmath>
#include <thread>

struct Passenger{
    std::string destination;
    unsigned int arrivalTime;
    double waitTime;
};

class Station
{
private:
    //Random Number Initiliazation
    std::random_device rd;
    std::mt19937 gen{rd()};

    //time
    std::chrono::steady_clock::time_point simulationStart;
    std::chrono::steady_clock::time_point lastUpdate;
    double timeScale = 3600.0;
    std::chrono::seconds simulatedElapsed{0};
    int currentSimulatedHour = 0;


public:
    //Start and Update Simulation
    void StartSimulation() {
        simulationStart = std::chrono::steady_clock::now();
        lastUpdate = simulationStart;
    }

    void updateSimulationTime() {
        auto now = std::chrono::steady_clock::now();
        auto realElapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastUpdate);


        auto simulatedIncrement = std::chrono::seconds(
            static_cast<long long>(realElapsed.count() * timeScale / 1000.0)
        );

        simulatedElapsed += simulatedIncrement;
        currentSimulatedHour = (simulatedElapsed.count() / 3600) % 24;
        lastUpdate = now;
        updatePeakHourStatus();

        if (hasHourChanged()) {
            passengerArrivalRate = calculatePassengerArrivalRate(stationType_);
        }
    }

    //Gets the Current Hour
    int getCurrentHour() const {
        return currentSimulatedHour;
    }
    //Checks if teh hour has changed
    bool hasHourChanged() {
        if (currentSimulatedHour != lastHour) {
            lastHour = currentSimulatedHour;
            return true;
        }
        return false;
    }
public:
    //Enum Struct
    enum struct stationType { local, metro, major };
    enum struct stationState { open, partial, closed };
    enum struct stationStats {mean, max, minimum, average, variance,median};
    //Distributions
    std::poisson_distribution<int> poisson_dist;
    std::normal_distribution<double> normal_dist;
    std::uniform_real_distribution<double> uniform_dist;
private:
    stationType stationType_;
    std::string name; // none
    std::pair<unsigned int, unsigned int> position; //none
    //event logger with async
    int sizeStation; // done
    double passengerArrivalRate; // done

    int capacity;
    int currentPassengerCount;
    std::vector<int> surgeTimeSchedule = {7, 8, 9, 16, 17, 18};//hours
    std::map<stationStats, double> stationStatsValues; //to do
    bool isPeakHour;
    int lastHour = -1;

private:
    //Initialization of Parameters
    int calculatePassengerArrivalRate(stationType type) {
        if (isPeakHour == true) {
            switch (type) {
                case stationType::local:
                    return std::max(0.0,normal_dist(gen) * std::sqrt(150) + 800); //u= 800, var =150
                case stationType::metro:
                    return std::max(0.0,normal_dist(gen) * std::sqrt(500) + 8000); //u= 8000, var =500
                case stationType::major:
                    return std::max(0.0,normal_dist(gen) * std::sqrt(1000) + 9800);  //u= 9800, var =1000
            }
        } else {
            switch (type) {
                case stationType::local:
                    return std::max(0.0, normal_dist(gen) * std::sqrt(50) + 270); //u= 270, var =50
                case stationType::metro:
                    return std::max(0.0, normal_dist(gen) * std::sqrt(200) + 2700); //u= 2700, var =200
                case stationType::major:
                    return std::max(0.0, normal_dist(gen) * std::sqrt(300) + 3300); //u= 3300, var =300
            }
        }
        return 0;
    }

    int calculateSizeStation(stationType type) const {
        switch (type) {
            case stationType::local:
                return 2;
            case stationType::metro:
                return 3;
            case stationType::major:
                return 6;
        }
        return 0;
    }

    int calculateStationCapacity(stationType type) const {
        switch (type) {
            case stationType::local:
                return 200;
            case stationType::metro:
                return 600;
            case stationType::major:
                return 1200;  //people each station can hold
        }
        return 0;
    }

    void updatePeakHourStatus() {
        int hour = getCurrentHour();
        isPeakHour = std::find(surgeTimeSchedule.begin(),surgeTimeSchedule.end(),hour) != surgeTimeSchedule.end();
    }
public:
    //Constructor
    Station(const std::string stationName, stationType type, std::pair<unsigned int,unsigned int> pos)
: gen(rd()), name(stationName), position(pos), stationType_(type), isPeakHour(false), lastHour(-1), currentPassengerCount(0) {

        sizeStation = calculateSizeStation(stationType_);
        capacity = calculateStationCapacity(stationType_);

        simulatedElapsed = std::chrono::seconds{0};
        currentSimulatedHour = 0;

        updatePeakHourStatus();
        passengerArrivalRate = calculatePassengerArrivalRate(stationType_);
    }
    //Getters
    std::string getStationName(){
        return name;
    }
    stationType getStationType() const {
        return stationType_;
    }

    int getPassengerArrivalRate() const {
        return static_cast<int>(passengerArrivalRate);
    }

    int getSizeStation() const {
        return calculateSizeStation(stationType_);
    }

    int getStationCapacity() const {
        return calculateStationCapacity(stationType_);
    }
    double getCurrentPassengerArrivalRate() const {
        return passengerArrivalRate;
    }
    int getCurrentSizeStation() const { return sizeStation; }
    int getCurrentCapacity() const { return capacity; }
    int getCurrentPassengerCount() const { return currentPassengerCount; }
    bool getIsPeakHour() const { return isPeakHour; }
};


#endif //STATION_H
