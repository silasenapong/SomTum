#include <iostream>
#include <fstream>
#include <iomanip>
#include "SendToBackEnd.h"
using namespace std;


void sendToBackend(const string& outputPath,
                   const vector<AirRecord>& records,
                   const AnalysisResult& result)
{
    ofstream out(outputPath);

    if (!out.is_open()) {
        cerr << "[SendToBackend] ERROR: Cannot write to file: "
             << outputPath << "\n";
        return;
    }

    out << fixed << setprecision(2);
    out << "{\n";

    out << "  \"summary\": {\n";
    out << "    \"total_records\": "     << records.size()               << ",\n";
    out << "    \"mean_pm25\": "         << result.mean                  << ",\n";
    out << "    \"max_pm25\": "          << result.maxVal                << ",\n";
    out << "    \"min_pm25\": "          << result.minVal                << ",\n";
    out << "    \"median_pm25\": "       << result.median                << ",\n";
    out << "    \"std_dev\": "           << result.stdDev                << ",\n";
    out << "    \"trend\": \""           << result.trend                 << "\",\n";
    out << "    \"anomaly_count\": "     << result.anomalyIndices.size() << ",\n";
    out << "    \"anomaly_threshold\": " << result.anomalyThreshold      << "\n";
    out << "  },\n";

    out << "  \"records\": [\n";

    for (int i = 0; i < (int)records.size(); i++) {
        const AirRecord& r = records[i];

        bool isAnomaly = false;
        for (int idx : result.anomalyIndices)
            if (idx == i) { isAnomaly = true; break; }

        int    aqi    = (i < (int)result.aqiValues.size())
                        ? result.aqiValues[i]     : -1;
        string aqiCat = (i < (int)result.aqiCategories.size())
                        ? result.aqiCategories[i] : "Unknown";

        out << "    {\n";
        out << "      \"date\": \""         << r.date        << "\",\n";  // YYYY/MM/DD
        out << "      \"time\": \""         << r.time        << "\",\n";  // HH:MM:SS
        out << "      \"temperature\": "    << r.temperature << ",\n";
        out << "      \"humidity\": "       << r.humidity    << ",\n";
        out << "      \"pm25\": "           << r.pm25        << ",\n";
        out << "      \"aqi\": "            << aqi           << ",\n";
        out << "      \"aqi_category\": \"" << aqiCat        << "\",\n";
        out << "      \"is_anomaly\": "     << (isAnomaly ? "true" : "false") << "\n";
        out << "    }";
        if (i < (int)records.size() - 1) out << ",";
        out << "\n";
    }

    out << "  ]\n";
    out << "}\n";

    out.close();

    cout << "[SendToBackend] Done — results written to: " << outputPath << "\n";
}
//pp