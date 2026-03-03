#include <iostream>
#include "ReceiveData.h"
#include "SendToBackEnd.h"
using namespace std;


int main(int argc, char* argv[]) {

    string inputFile  = (argc > 1) ? argv[1] : "../data/data.txt";
    string outputFile = (argc > 2) ? argv[2] : "../data/analysis_output.json";

    cout << "========================================\n";
    cout << "  Data Analysis Module\n";
    cout << "========================================\n\n";

    cout << "[Step 1] Receiving data from Data Input...\n";
    vector<AirRecord> records = receiveDataFromInput(inputFile);

    if (records.empty()) {
        cerr << "[main] No valid data received. Exiting.\n";
        return 1;
    }
    AnalysisResult result;

    cout << "[Step 3] Sending analysis data to Backend...\n";
    sendToBackend(outputFile, records, result);

    cout << "\n[main] All steps complete.\n";
    return 0;
}
//p