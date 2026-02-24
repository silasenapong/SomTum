#include <iostream>
#include <fstream>
#include <string>
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

    double firstHalfSum = 0;
    double secondHalfSum = 0;

    int half = count / 2;

    for (int i = 0; i < half; i++) {
        firstHalfSum += pm25[i];
    }

    for (int i = half; i < count; i++) {
        secondHalfSum += pm25[i];
    }

    double firstAvg = firstHalfSum / half;
    double secondAvg = secondHalfSum / (count - half);

    cout << "Total Data = " << count << endl;
    cout << "First Half Average = " << firstAvg << endl;
    cout << "Second Half Average = " << secondAvg << endl;

    if (secondAvg > firstAvg) {
        cout << "Trend: PM2.5 is Increasing" << endl;
    }
    else if (secondAvg < firstAvg) {
        cout << "Trend: PM2.5 is Decreasing" << endl;
    }
    else {
        cout << "Trend: PM2.5 is Stable" << endl;
    }

    return 0;
}