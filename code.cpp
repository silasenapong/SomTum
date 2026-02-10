#include <iostream>
#include <cmath>
using namespace std;

int main() {
    double pm25[200];
    int count = 0;
    int n;

    cout << "Enter number of PM2.5 data: ";
    cin >> n;

    for (int i = 0; i < n; i++) {
        cout << "Enter PM2.5 value \n" << i + 1 << ": ";
        cin >> pm25[i];
        count++;
    }

    double min = pm25[0];
    double max = pm25[0];
    double sum = 0;

    for (int i = 0; i < count; i++) {
        if (pm25[i] < min)
            min = pm25[i];

        if (pm25[i] > max)
            max = pm25[i];

        sum += pm25[i];
    }

    double average = sum / count;

    double variance = 0;
    for (int i = 0; i < count; i++) {
        variance += pow(pm25[i] - average, 2);
    }
    variance = variance / count;

    double stdDev = sqrt(variance);

    cout << "PM2.5 Analysis Result\n";
    cout << "Total data : " << count << endl;
    cout << "Average PM2.5 : " << average << endl;
    cout << "Minimum PM2.5 : " << min << endl;
    cout << "Maximum PM2.5 : " << max << endl;
    cout << "Standard Deviation : " << stdDev << endl;

    return 0;
}
