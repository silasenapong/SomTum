#pragma once

#include <string>
#include <vector>
#include "DataTypes.h"
using namespace std;

// ─────────────────────────────────────────────
//  SendToBackend.h
//  Sends AnalysisReport to Backend layer
// ─────────────────────────────────────────────

void sendToBackend(const string& outputPath,
                   const AnalysisReport& report);