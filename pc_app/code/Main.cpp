#include <iostream>
#include "ReceiveData.h"
#include "SendToBackEnd.h"
using namespace std;

// ─────────────────────────────────────────────
//  main.cpp  —  Data Analysis Layer entry point
//
//  Input format : YYYY/MM/DD,HH:MM:SS,TEMP,HUMID,PM2.5
//  Report window: midnight TODAY → 17:00:00 NEXT DAY
//
//  Flow:
//    Step 1 — receiveDataFromInput()   YOUR PART
//    Step 2 — filterByTimeWindow()     YOUR PART
//    Step 3 — teammates' algorithms fill AnalysisReport
//    Step 4 — sendToBackend()          YOUR PART
// ─────────────────────────────────────────────

int main(int argc, char* argv[]) {

    string inputFile  = (argc > 1) ? argv[1] : "../data/data.txt";
    string outputFile = (argc > 2) ? argv[2] : "../data/analysis_output.json";
    string todayDate  = (argc > 3) ? argv[3] : "2026/03/01";  // YYYY/MM/DD

    cout << "========================================\n";
    cout << "  Data Analysis Module\n";
    cout << "  Report window: " << todayDate << " 00:00:00\n";
    cout << "             --> next day 17:00:00\n";
    cout << "========================================\n\n";

    // ── STEP 1 : YOUR PART ───────────────────
    // Read all valid records from the data file
    cout << "[Step 1] Receiving data from Data Input...\n";
    vector<AirRecord> allRecords = receiveDataFromInput(inputFile);

    if (allRecords.empty()) {
        cerr << "[main] No valid data received. Exiting.\n";
        return 1;
    }

    // ── STEP 2 : YOUR PART ───────────────────
    // Filter to report window: midnight today → 5pm next day
    cout << "[Step 2] Filtering to report time window...\n";
    vector<AirRecord> windowRecords = filterByTimeWindow(allRecords, todayDate);

    if (windowRecords.empty()) {
        cerr << "[main] No records in time window. Exiting.\n";
        return 1;
    }

    // ── STEP 3 : TEAMMATES' PART ─────────────
    // Pass windowRecords to teammates' algorithm modules.
    // They calculate all fields and fill AnalysisReport.
    //
    // Once integrated it will look like:
    //   AnalysisReport report = buildReport(windowRecords, allRecords, todayDate);
    //
    // Placeholder until teammates connect their modules:
    AnalysisReport report;
    report.todayDate    = "01/03/2026";
    report.currentHour  = 12;
    // ... teammates fill the rest

    // ── STEP 4 : YOUR PART ───────────────────
    // Send completed report to Backend
    cout << "[Step 4] Sending analysis report to Backend...\n";
    sendToBackend(outputFile, report);

    cout << "\n[main] All steps complete.\n";
    return 0;
}