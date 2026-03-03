#include <iostream>
#include <fstream>
#include <iomanip>
#include "SendToBackEnd.h"
using namespace std;

// ─────────────────────────────────────────────
//  SendToBackend.cpp
//  Task  : Send AnalysisReport to Backend layer
//          Output: JSON file
//
//  Field order matches ex_data.pdf spec exactly:
//  Today → Hourly → Weekly → Monthly → Yearly
// ─────────────────────────────────────────────

void sendToBackend(const string& outputPath,
                   const AnalysisReport& r)
{
    ofstream out(outputPath);

    if (!out.is_open()) {
        cerr << "[SendToBackend] ERROR: Cannot write to: " << outputPath << "\n";
        return;
    }

    out << fixed << setprecision(2);
    out << "{\n";

    // ── TODAY ─────────────────────────────────
    out << "  \"today\": {\n";
    out << "    \"date\": \""           << r.todayDate       << "\",\n";  // 01/03/2026
    out << "    \"current_hour\": "     << r.currentHour     << ",\n";   // 12
    out << "    \"current_pm25\": "     << r.currentPM25     << ",\n";   // 48.60
    out << "    \"max_pm25\": "         << r.todayMax         << ",\n";   // 76.40
    out << "    \"min_pm25\": "         << r.todayMin         << ",\n";   // 12.40
    out << "    \"time_of_max\": \""    << r.timeOfTodayMax   << "\"\n";  // 12:05:44
    out << "  },\n";

    // ── HOURLY (24 values, 0 after current hour) ──
    // midnight today → next midnight
    out << "  \"hourly_pm25\": [";
    for (int i = 0; i < 24; i++) {
        out << r.hourlyPM25[i];
        if (i < 23) out << ", ";
    }
    out << "],\n";

    // ── WEEKLY ────────────────────────────────
    out << "  \"weekly\": {\n";
    out << "    \"avg\": "              << r.weeklyAvg        << ",\n";   // 56.00
    out << "    \"max_date\": \""       << r.weekMaxDate      << "\",\n"; // 27/02
    out << "    \"max_val\": "          << r.weekMaxVal       << ",\n";   // 90.00 (*)
    out << "    \"min_date\": \""       << r.weekMinDate      << "\",\n"; // 25/02
    out << "    \"min_val\": "          << r.weekMinVal       << ",\n";   // 13.00
    out << "    \"daily_averages\": [\n";
    // 6 entries: yesterday → 6 days ago
    for (int i = 0; i < 6; i++) {
        out << "      { \"date\": \"" << r.dailyDate[i]
            << "\", \"avg\": "        << r.dailyAvg[i] << " }";
        if (i < 5) out << ",";
        out << "\n";
    }
    out << "    ]\n";
    out << "  },\n";

    // ── MONTHLY ───────────────────────────────
    out << "  \"monthly\": {\n";
    out << "    \"avg\": "              << r.monthlyAvg       << ",\n";   // 54.00
    out << "    \"max_date\": \""       << r.monthMaxDate     << "\",\n"; // 14/02/2026
    out << "    \"max_val\": "          << r.monthMaxVal      << ",\n";   // 98.00
    out << "    \"min_date\": \""       << r.monthMinDate     << "\",\n"; // 03/02/2026
    out << "    \"min_val\": "          << r.monthMinVal      << "\n";    // 5.00
    out << "  },\n";

    // ── YEARLY ────────────────────────────────
    out << "  \"yearly\": {\n";
    out << "    \"avg\": "                    << r.yearlyAvg           << ",\n"; // 54.00
    out << "    \"max_month\": "              << r.yearMaxMonth        << ",\n"; // 11
    out << "    \"max_month_val\": "          << r.yearMaxMonthVal     << ",\n"; // 87.00
    out << "    \"min_month\": "              << r.yearMinMonth        << ",\n"; // 7
    out << "    \"min_month_val\": "          << r.yearMinMonthVal     << ",\n"; // 14.00
    out << "    \"months_exceeding_37_5\": "  << r.monthsExceedingLimit << ",\n";// 100 (count)
    out << "    \"monthly_avgs\": [";
    // Jan–Dec (12 values), months after current = 0
    for (int i = 0; i < 12; i++) {
        out << r.monthlyAvgPerMonth[i];
        if (i < 11) out << ", ";
    }
    out << "]\n";
    out << "  }\n";

    out << "}\n";
    out.close();

    cout << "[SendToBackend] Done — report written to: " << outputPath << "\n";
}