#include <iostream>
#include <fstream>
#include <cmath>
using namespace std;

int main() {

    ifstream file("../data/data.txt");
    if (!file) {
    cout << "File not found!" << endl;
    return 0;
}
    string line;

    string date[2000];
    string timeData[2000];
    double temperature[2000];
    double humidity[2000];
    double pm25[2000];

    int count = 0;

    while (getline(file, line)) {

        int po;

        po = line.find(",");
        date[count] = line.substr(0, po);
        line.erase(0, po + 1);

        po = line.find(",");
        timeData[count] = line.substr(0, po);
        line.erase(0, po + 1);

        po = line.find(",");
        temperature[count] = stod(line.substr(0, po));
        line.erase(0, po + 1);

        po = line.find(",");
        humidity[count] = stod(line.substr(0, po));
        line.erase(0, po + 1);

        pm25[count] = stod(line);

        count++;
    }

    file.close();

    double sum = 0;
    for (int i = 0; i < count; i++) {
        sum += pm25[i];
    }

    double mean = sum / count;

    double maxVal = pm25[0];
    double minVal = pm25[0];

    for (int i = 0; i < count; i++) {
        if (pm25[i] > maxVal)
            maxVal = pm25[i];

        if (pm25[i] < minVal)
            minVal = pm25[i];
    }
    
    cout << "Total Data = " << count << endl;
    cout << "Mean PM2.5 = " << mean << endl;
    cout << "Max PM2.5 = " << maxVal << endl;
    cout << "Min PM2.5 = " << minVal << endl;

    return 0;
}
