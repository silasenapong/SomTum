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

// setting path และ URL สำหรับ Firebase
const string FOLDER_PATH = "./data_clean";
const string PROJECT_ID = "studio-215835202-3b08b"; 
const string COLLECTION = "datalog"; 
const string URL = "https://firestore.googleapis.com/v1/projects/" + PROJECT_ID + "/databases/(default)/documents/" + COLLECTION;

// ส่งข้อมูลไปยัง Firebase โดยcurl ผ่านเข้าระบบ
void uploadToFirebase(const string& dateStr, const string& timeStr, const string& tempStr, const string& humidStr, const string& pm25Str) {
    string jsonData = "{\\\"fields\\\": {"
        "\\\"date\\\": {\\\"stringValue\\\": \\\"" + dateStr + "\\\"}, "
        "\\\"time\\\": {\\\"stringValue\\\": \\\"" + timeStr + "\\\"}, "
        "\\\"temperature\\\": {\\\"doubleValue\\\": " + tempStr + "}, "
        "\\\"humidity\\\": {\\\"doubleValue\\\": " + humidStr + "}, "
        "\\\"pm25\\\": {\\\"doubleValue\\\": " + pm25Str + "}"
        "}}";
    
    string command = "curl -s -X POST -H \"Content-Type: application/json\" -d \"" + jsonData + "\" \"" + URL + "\" > NUL";
    system(command.c_str());
}

// จัดการไฟล์ (อ่าน, แยกตัวแปร, ส่งขึ้น Cloud, ลบไฟล์ทิ้ง)
void processFile(const fs::directory_entry& entry) {
    string filePath = entry.path().string();
    string fileName = entry.path().filename().string();
    
    ifstream inputFile(filePath);
    string line;
    
    if (inputFile.is_open()) {
        getline(inputFile, line);
        inputFile.close(); // ปิดไฟล์ก่อนลบ
        
        stringstream ss(line);
        string dateStr, timeStr, tempStr, humidStr, pm25Str;
        
        // ตัวแปร (วันที่, เวลา, อุณหภูมิ, ความชื้น, PM2.5)
        getline(ss, dateStr, ',');
        getline(ss, timeStr, ',');
        getline(ss, tempStr, ',');
        getline(ss, humidStr, ',');
        getline(ss, pm25Str, ',');
        
        while (!pm25Str.empty() && (pm25Str.back() == '\r' || pm25Str.back() == '\n')) {
            pm25Str.pop_back();
        }
        
        cout << "-> Uploading: [" << fileName << "] ... ";
        
        // ยิงขึ้น Firebase
        uploadToFirebase(dateStr, timeStr, tempStr, humidStr, pm25Str);
        
        // ลบไฟล์ทิ้งหลังจากอ่านและส่งข้อมูลเสร็จแล้ว
        fs::remove(filePath);
        
        cout << "Done & Deleted!" << endl;
    }
}

// สรุปผลการทำงานของแต่ละรอบ(ผ่าน/ไม่ผ่าน)
void processDirectory() {
    if (!fs::exists(FOLDER_PATH)) {
        cout << "\nError: Folder '" << FOLDER_PATH << "' not found!" << endl;
        return; 
    }

    int fileCount = 0;
    
    // ลูปอ่านfiles ในfolder data_clean
    for (const auto& entry : fs::directory_iterator(FOLDER_PATH)) {
        if (entry.is_regular_file()) {
            fileCount++;
            processFile(entry);
        }
    }
    
    // สรุปผล
    if (fileCount == 0) {
        cout << "No new files found in '" << FOLDER_PATH << "'." << endl;
    } else {
        cout << "Successfully processed and deleted " << fileCount << " files." << endl;
    }
}

int main() {
    cout << "=========================================" << endl;
    cout << "  Auto Data Pipeline Started!            " << endl;
    cout << "  Checking folder every 5 minutes...     " << endl;
    cout << "  (Press Ctrl + C to stop the program)   " << endl;
    cout << "=========================================" << endl;

    // ลูปทำงานตลอดเวลา
    while (true) {
        processDirectory(); // สั่งให้เริ่มเช็คfolder และ upload data
        
        cout << "\nWaiting for 5 minutes before the next check..." << endl;
        std::this_thread::sleep_for(std::chrono::minutes(5));
    }

    return 0;
}