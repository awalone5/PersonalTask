#ifndef PI
#define PI 3.14159265358979323846
#endif
#include <iostream>
#include <cmath>
#include <math.h>
#include <windows.h>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
using namespace std;
int calculateDistance(double, double, double, double);

struct COORDS {
    double lat;
    double lon;
};
void printAllCoords(vector<COORDS>);
void deleteCoord(vector<COORDS>&, string);
void addNewCoord(vector<COORDS>&, string);

int main() {
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);
    string path = "C:\\Users\\admin\\source\\repos\\PersonalTask\\Data\\Coordinates.txt";
    ifstream inputFile(path);
    if (!inputFile.is_open())
    {
        cout << "Error opening file" << endl;
        return 0;
    }

    vector<COORDS> coordinates;
    string line;

    while (getline(inputFile, line))
    {
        size_t comma_pos = line.find(',');
        if (comma_pos != string::npos && comma_pos + 1 < line.length()) {
            string lat_str = line.substr(0, comma_pos);
            string lon_str = line.substr(comma_pos + 1);

            try {
                double lat = stod(lat_str);
                double lon = stod(lon_str);
                COORDS coords = { lat, lon };
                coordinates.push_back(coords);
            }
            catch (const invalid_argument& e) {
                cout << e.what() << endl;
            }
        }
    }
    printAllCoords(coordinates);
    cout << "How to use this program:" << endl << "To see all coordinates enter: 1" << endl << "To add new coordinate enter: 2" << endl << "To delete coordinate enter: 3" << endl << "To calculate distance enter: 4" << endl << "If you want to exite enter: 5" << endl;
    bool isExit = false;
    do
    {
        int choose;
        string buffer;
        getline(cin, buffer);
        choose = stoi(buffer);
        string slat, slon;
        double lat = 0.0, lon = 0.0;
        switch (choose)
        {
        case 1:
            printAllCoords(coordinates);
            break;
        case 2:
            addNewCoord(coordinates, path);
            break;
        case 3:
            deleteCoord(coordinates, path);
            break;
        case 4:
            cout << "Enter coordinates" << endl;
            getline(cin, slat);
            getline(cin, slon);
            lat = stod(slat), lon = stod(slon);
            for (COORDS coordinate : coordinates)
            {
                cout << calculateDistance(lat, lon, coordinate.lat, coordinate.lon) << " km" << endl;
            }
            break;
        case 5:
            isExit = true;
            break;
        default:
            cerr << "Err. Unknown choose" << endl;
            break;
        }
    } while (!isExit);
}

int calculateDistance(double lat1, double lon1, double lat2, double lon2) {
    double R = 6371;
    lat1 *= PI / 180.0;
    lon1 *= PI / 180.0;
    lat2 *= PI / 180.0;
    lon2 *= PI / 180.0;

    double deltaLon = lon2 - lon1;
    double deltaLat = lat2 - lat1;

    double a = pow(sin(deltaLat / 2), 2) + cos(lat1) * cos(lat2) * pow(sin(deltaLon / 2), 2);
    double c = 2 * atan2(sqrt(a), sqrt(1 - a));

    return round(R * c);
}

void printAllCoords(vector<COORDS> coordinates) {
    int id = 1;
    for (COORDS coordinate : coordinates)
    {
        cout << id << ": " << coordinate.lat << ", " << coordinate.lon << endl;
        id++;
    }
}

void addNewCoord(vector<COORDS>& coordinates, string path) {
    COORDS newCoords;
    cout << "Enter new coordinates (up to 4 symbols after a dot)" << endl;
    string lat, lon;
    getline(cin, lat);
    getline(cin, lon);

    double dlat = stod(lat), dlon = stod(lon);

    newCoords.lat = dlat;
    newCoords.lon = dlon;

    coordinates.push_back(newCoords);

    fstream outputFile(path);
    if (!outputFile.is_open()) {
        cerr << "Error opening file for writing!" << endl;
    }

    for (auto& coords : coordinates) {
        outputFile << coords.lat << ", " << coords.lon << endl;
    }

    outputFile.close();
    cout << "New coordinates added successful" << endl;
}

void deleteCoord(vector<COORDS>& coordinates, string file) {
    cout << "Enter id of coordinate which you want dalete" << endl;
    string sid;
    getline(cin, sid);
    int id = stoi(sid);
    int currId = 1;

    auto it = remove_if(coordinates.begin(), coordinates.end(), [id, &currId](COORDS& coord) {
        if (id == currId)
        {
            cout << coord.lat << ", " << coord.lon << endl;
            return coord.lat;
        }
        currId++;
    });

    if (it != coordinates.end()) {
        coordinates.erase(it, coordinates.end());
        cout << "Entry with id: " << id << " has been deleted." << endl;
    }
    else {
        cout << "No entries found for id: " << id << endl;
    }

    ofstream outputFile(file);
    if (!outputFile.is_open()) {
        cerr << "Error opening file for writing!" << endl;
    }

    for (auto& coords : coordinates) {
        outputFile << coords.lat << ", " << coords.lon << endl;
    }

    outputFile.close();
}