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

struct Passenger{
  std::string destination;
  unsigned int arrivalTime;
  double waitTime;
  };


class Station
{

private:
    std::random_device rd;
    std::mt19937 gen;
    //Struct;
public:
    enum struct stationType { local, metro, major };
    enum struct stationState { open, partial, closed };
    enum struct stationStats {mean, max, minimum, average, variance,median};
    //Distributions
    std::poisson_distribution<int> poisson_dist;
    std::normal_distribution<double> normal_dist;
    std::uniform_real_distribution<double> uniform_dist;
    std::beta_distribution<double> beta_dist;
private:

    std::string name; // none
    std::pair<unsigned int, unsigned int> position; //none
    //event logger with async
    std::vector<int> currentPassengerCount; //not yet
    int sizeStation; // done
    double passengerArrivalRate; // done

    int capacity;
    std::vector<Passenger> passengers;
    std::vector<int> surgeTimeSchedule = {7, 8, 9, 16, 17, 18};//hours
    std::map<stationStats, double> stationStatsValues;
    bool isPeakHour;

public:
    //Initialization of Parameters
    int getPassengerArrivalRate(stationType type) {
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
                    return std::max(0.0, normal_dist(gen) * sqrt(50) + 270); //u= 270, var =50
                case stationType::metro:
                    return std::max(0.0, normal_dist(gen) * sqrt(200) + 2700); //u= 2700, var =200
                case stationType::major:
                    return std::max(0.0, normal_dist(gen) * sqrt(300) + 3300); //u= 3300, var =300
            }
        }
        return 0;
    }

    int sizeStation(stationType type) {
        switch (type) {
            case stationType::local:
                return 2;
            case stationType::metro:
                return 3;
            case stationType::major:
                return 6;
        }
    }

    int getStationCapacity(stationType type) {
        switch (type) {
            case stationType::local:
                return 200;
            case stationType::metro:
                return 600;
            case stationType::major:
                return 1200;  //people each station can hold
        }
    }


    Station() = default;













    //Getters
    std::string getStationName(){
    return name; }
};


#endif //STATION_H
