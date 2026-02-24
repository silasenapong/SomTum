#include <iostream>
#include <fstream>
using namespace std;

double AQI(double pm) {

    double aqi = 0;

    if (pm <= 12.0) {
        aqi = (pm / 12.0) * 50;
    }
    else if (pm <= 35.4) {
        aqi = 51 + (pm - 12.1) * (100 - 51) / (35.4 - 12.1);
    }
    else if (pm <= 55.4) {
        aqi = 101 + (pm - 35.5) * (150 - 101) / (55.4 - 35.5);
    }
    else if (pm <= 150.4) {
        aqi = 151 + (pm - 55.5) * (200 - 151) / (150.4 - 55.5);
    }
    else if (pm <= 250.4) {
        aqi = 201 + (pm - 150.5) * (300 - 201) / (250.4 - 150.5);
    }
    else {
        aqi = 301 + (pm - 250.5) * (500 - 301) / (500.4 - 250.5);
    }

    return aqi;
}

int main() {

    ifstream file("../data/data.txt");

    string line;
    int count = 0;

    while (getline(file, line)) {

        int pos;

        pos = line.find(",");
        line.erase(0, pos + 1);

        pos = line.find(",");
        line.erase(0, pos + 1);

        pos = line.find(",");
        line.erase(0, pos + 1);

        pos = line.find(",");
        line.erase(0, pos + 1);

        double pm = stod(line);

        double aqi = AQI(pm);

        cout << "PM2.5 = " << pm 
             << "  AQI = " << (int)aqi << endl;

        count++;
    }
    
    cout << "Total Data = " << count << endl;

    file.close();

    return 0;
}