#include <bits/stdc++.h>
using namespace std;

class FlightDetails {
public:
    string flightCode;
    int startCity;
    int endCity;
    int cost;
    int startTime, endTime;

    FlightDetails(string code, int srcCity, int destCity, int price, int depTime, int arrTime) {
        flightCode = code;
        startCity = srcCity;
        endCity = destCity;
        cost = price;
        startTime = depTime;
        endTime = arrTime;
    }

    FlightDetails() {}
};

class JourneyState {
public:
    int accumulatedCost;
    int currentCity;
    int currentTime;
    vector<FlightDetails> itinerary;

    JourneyState(int totalCost, int city, int time) {
        accumulatedCost = totalCost;
        currentCity = city;
        currentTime = time;
    }

    JourneyState() {}
};

class CompareCost {
public:
    bool operator()(const JourneyState& state1, const JourneyState& state2) {
        return state1.accumulatedCost > state2.accumulatedCost;
    }
};

int convertToMinutes(int hhmmTime) {
    int minutes = hhmmTime % 100;
    int hours = hhmmTime / 100;
    return hours * 60 + minutes;
}

int findCheapestJourney(vector<vector<FlightDetails>> &flightNetwork, int originCity, int destinationCity, int earliestDeparture, int latestArrival) {
    priority_queue<JourneyState, vector<JourneyState>, CompareCost> minHeap;
    map<string, FlightDetails> visitedFlights;
    JourneyState initialState(0, originCity, earliestDeparture - 30);
    minHeap.push(initialState);

    while (!minHeap.empty()) {
        JourneyState currentState = minHeap.top();
        minHeap.pop();

        int currentCost = currentState.accumulatedCost;
        int city = currentState.currentCity;
        int time = currentState.currentTime;

        if (city == destinationCity && time <= latestArrival) {
            return currentCost;
        }

        int earliestPossibleDeparture = time + 30;
        for (FlightDetails &flight : flightNetwork[city]) {
            if (flight.startTime >= earliestPossibleDeparture && flight.endTime <= latestArrival && visitedFlights.find(flight.flightCode) == visitedFlights.end()) {
                JourneyState nextState(currentCost + flight.cost, flight.endCity, flight.endTime);
                nextState.itinerary = currentState.itinerary;
                nextState.itinerary.push_back(flight);
                minHeap.push(nextState);
                visitedFlights[flight.flightCode] = flight;
            }
        }
    }

    return -1;
}

int main() {
    int numCities;
    cout << "Enter the number of cities: ";
    cin >> numCities;

    vector<vector<FlightDetails>> flightNetwork(numCities + 1);

    int numFlights;
    cout << "Enter the number of flights: ";
    cin >> numFlights;

    cout << "Enter flight details (Departure City, Arrival City, Departure Time (HHMM), Arrival Time (HHMM), Flight Number, Price):\n";
    while (numFlights--) {
        int depCity, arrCity, depTime, arrTime, price;
        string flightNo;
        cin >> depCity >> arrCity >> depTime >> arrTime >> flightNo >> price;
        depTime = convertToMinutes(depTime);
        arrTime = convertToMinutes(arrTime);
        flightNetwork[depCity].emplace_back(flightNo, depCity, arrCity, price, depTime, arrTime);
    }

    int numQueries;
    cout << "Enter the number of queries: ";
    cin >> numQueries;

    cout << "Enter each query (Source City, Destination City, Earliest Departure Time (HHMM), Latest Arrival Time (HHMM)):\n";
    while (numQueries--) {
        int source, destination, depTime, arrTime;
        cin >> source >> destination >> depTime >> arrTime;
        depTime = convertToMinutes(depTime);
        arrTime = convertToMinutes(arrTime);
        int result = findCheapestJourney(flightNetwork, source, destination, depTime, arrTime);
        if (result == -1) {
            cout << "No valid path found\n";
        } else {
            cout << "Cheapest journey cost: " << result << "\n";
        }
    }

    return 0;
}
