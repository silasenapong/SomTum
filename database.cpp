#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <filesystem>
#include <thread>
#include <chrono>

using namespace std;
namespace fs = std::filesystem;

int main() {
    string folderPath = "./data_clean";
    string completedFolderPath = "./completed_data";
    string projectId = "studio-215835202-3b08b"; 
    string collection = "datalog"; 
    string url = "https://firestore.googleapis.com/v1/projects/" + projectId + "/databases/(default)/documents/" + collection;
    // สร้างโฟลเดอร์ completed_data
    if (!fs::exists(completedFolderPath)) {
        fs::create_directory(completedFolderPath);
    }

    cout << "=========================================" << endl;
    cout << "  Auto Data Pipeline Started!            " << endl;
    cout << "  Checking folder every 1 hour...        " << endl;
    cout << "  (Press Ctrl + C to stop the program)   " << endl;
    cout << "=========================================" << endl;

    while (true) {
        
        if (!fs::exists(folderPath)) {
            cout << "\nError: Folder 'data_clean' not found!" << endl;
        } else {
            int fileCount = 0; // ตัวแปรไว้นับว่ารอบนี้เจอไปกี่ไฟล์แล้ว
            // ลูปอ่านไฟล์ในfolder data_clean
            for (const auto& entry : fs::directory_iterator(folderPath)) {
                if (entry.is_regular_file()) {
                    fileCount++;
                    string filePath = entry.path().string();
                    string fileName = entry.path().filename().string();
                    
                    ifstream inputFile(filePath);
                    string line;
                    
                    if (inputFile.is_open()) {
                        getline(inputFile, line);
                        inputFile.close();
                        
                        stringstream ss(line);
                        string dateStr, timeStr, tempStr, humidStr, pm25Str;
                        
                        getline(ss, dateStr, ',');
                        getline(ss, timeStr, ',');
                        getline(ss, tempStr, ',');
                        getline(ss, humidStr, ',');
                        getline(ss, pm25Str, ',');
                        
                        while (!pm25Str.empty() && (pm25Str.back() == '\r' || pm25Str.back() == '\n')) {
                            pm25Str.pop_back();
                        }
                        
                        cout << "-> Uploading: [" << fileName << "] ... ";
                        
                        string jsonData = "{\\\"fields\\\": {"
                            "\\\"date\\\": {\\\"stringValue\\\": \\\"" + dateStr + "\\\"}, "
                            "\\\"time\\\": {\\\"stringValue\\\": \\\"" + timeStr + "\\\"}, "
                            "\\\"temperature\\\": {\\\"doubleValue\\\": " + tempStr + "}, "
                            "\\\"humidity\\\": {\\\"doubleValue\\\": " + humidStr + "}, "
                            "\\\"pm25\\\": {\\\"doubleValue\\\": " + pm25Str + "}"
                            "}}";
                        
                        string command = "curl -s -X POST -H \"Content-Type: application/json\" -d \"" + jsonData + "\" \"" + url + "\" > NUL";
                        system(command.c_str());
                        
                        string newFilePath = completedFolderPath + "/" + fileName;
                        if (fs::exists(newFilePath)) fs::remove(newFilePath);
                        fs::rename(filePath, newFilePath);
                        
                        cout << "Done & Moved!" << endl;
                    }
                }
            }
            // สรุปผลการทำงานของแต่ละรอบ(ผ่าน/ไม่ผ่าน)
            if (fileCount == 0) {
                cout << "No new files found in 'data_clean'." << endl;
            } else {
                cout << "Successfully processed " << fileCount << " files." << endl;
            }
        }

        cout << "\nWaiting for 1 hour before the next check..." << endl;
        std::this_thread::sleep_for(std::chrono::hours(1));
    }

    return 0;
}