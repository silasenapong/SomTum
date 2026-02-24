#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
using namespace std;

int main() {

    ifstream file("../data/data.txt");

    string line;

    string date[2000];
    string timeData[2000];
    double temperature[2000];
    double humidity[2000];
    double pm25[2000];

    int count = 0;

    while (getline(file, line)) {

        int pos;

        pos = line.find(",");
        date[count] = line.substr(0, pos);
        line.erase(0, pos + 1);

        pos = line.find(",");
        timeData[count] = line.substr(0, pos);
        line.erase(0, pos + 1);

        pos = line.find(",");
        temperature[count] = stod(line.substr(0, pos));
        line.erase(0, pos + 1);

        pos = line.find(",");
        humidity[count] = stod(line.substr(0, pos));
        line.erase(0, pos + 1);

        pm25[count] = stod(line);

        count++;
    }

    file.close();

    double sum = 0;

    for (int i = 0; i < count; i++) {
        sum += pm25[i];
    }

    double mean = sum / count;

    double varianceSum = 0;

    for (int i = 0; i < count; i++) {
        varianceSum += pow(pm25[i] - mean, 2);
    }

    double variance = varianceSum / count;

    double stdDev = sqrt(variance);

    double threshold = mean + (2 * stdDev);

    cout << "Mean = " << mean << endl;
    cout << "Standard Deviation = " << stdDev << endl;
    cout << "Anomaly Threshold = " << threshold << endl;
    cout << endl;

    cout << "Anomaly Data:" << endl;

    for (int i = 0; i < count; i++) {

        if (pm25[i] > threshold) {
            cout << date[i] << " "
                 << timeData[i]
                 << " PM2.5 = "
                 << pm25[i]
                 << " (Anomaly)" << endl;
        }
    }
    return 0;
}