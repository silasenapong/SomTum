#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>

using namespace std;
// setting path และ URL ของFirebase
const string PROJECT_ID = "studio-215835202-3b08b"; 
const string COLLECTION = "datalog"; 
const string URL = "https://firestore.googleapis.com/v1/projects/" + PROJECT_ID + "/databases/(default)/documents/" + COLLECTION;

const string RAW_JSON_FILE = "temp_raw_data.json";
const string OUTPUT_TXT_FILE = "data_api.txt"; 

// ดึงข้อมูลดิบจาก Firebase
bool fetchFromFirebase() {
    cout << "-> [1/2] Fetching data from Firebase..." << endl;
    string command = "curl -s -X GET \"" + URL + "\" > " + RAW_JSON_FILE;
    int result = system(command.c_str());
    
    return (result == 0); 
}

// แปลงข้อมูล JSON เป็น .txt และลบไฟล์ขยะ
bool convertJsonToText() {
    cout << "-> [2/2] Converting and cleaning data..." << endl;
    
    ifstream inFile(RAW_JSON_FILE);
    ofstream outFile(OUTPUT_TXT_FILE);
    string line;
    string date = "", time = "", temp = "", humid = "", pm25 = "";

    if (!inFile.is_open() || !outFile.is_open()) {
        return false; // ถ้าเปิดไฟล์ไม่ได้
    }

    while(getline(inFile, line)) {
        
        // search Date
        if(line.find("\"date\"") != string::npos) {
            getline(inFile, line);
            size_t start = line.find_first_of("0123456789");
            if (start != string::npos) {
                size_t end = line.find("\"", start);
                date = line.substr(start, end - start);
            }
        }
        // search Time
        else if(line.find("\"time\"") != string::npos) {
            getline(inFile, line);
            size_t start = line.find_first_of("0123456789");
            if (start != string::npos) {
                size_t end = line.find("\"", start);
                time = line.substr(start, end - start);
            }
        }
        // search Temperature
        else if(line.find("\"temperature\"") != string::npos) {
            getline(inFile, line);
            size_t start = line.find_first_of("0123456789.-");
            if (start != string::npos) {
                size_t end = line.find_first_not_of("0123456789.-", start);
                temp = (end == string::npos) ? line.substr(start) : line.substr(start, end - start);
            }
        }
        // search Humidity
        else if(line.find("\"humidity\"") != string::npos) {
            getline(inFile, line);
            size_t start = line.find_first_of("0123456789.-");
            if (start != string::npos) {
                size_t end = line.find_first_not_of("0123456789.-", start);
                humid = (end == string::npos) ? line.substr(start) : line.substr(start, end - start);
            }
        }
        // search PM2.5
        else if(line.find("\"pm25\"") != string::npos) {
            getline(inFile, line);
            size_t start = line.find_first_of("0123456789.-");
            if (start != string::npos) {
                size_t end = line.find_first_not_of("0123456789.-", start);
                pm25 = (end == string::npos) ? line.substr(start) : line.substr(start, end - start);
            }
        }
        
        // เมื่อเจอ createTime แปลว่าจบ 1 ชุดข้อมูล ให้เขียนลงไฟล์ .txt
        if(line.find("\"createTime\"") != string::npos) {
            if(date != "") {
                outFile << date << "," << time << "," << temp << "," << humid << "," << pm25 << "\n";
                // ล้างค่าเก่าเพื่อเตรียมรับข้อมูลใหม่
                date = ""; time = ""; temp = ""; humid = ""; pm25 = "";
            }
        }
    }
    
    inFile.close();
    outFile.close();
    
    // ลบไฟล์ JSON ชั่วคราวที่อ่านค่าไปแล้วทิ้ง
    remove(RAW_JSON_FILE.c_str());
    
    return true; 
}

int main() {
    cout << "=========================================" << endl;
    cout << "  [Backend Support] Frontend Data Puller " << endl;
    cout << "=========================================" << endl;

    // เรียกใช้ฟังก์ชันดึงข้อมูล
    if (fetchFromFirebase()) {
        
        // ถ้ายิง Firebase สำเร็จ ให้แปลงข้อมูลและลบไฟล์ขยะ
        if (convertJsonToText()) {
            cout << "\n[Success] Data converted and saved to: " << OUTPUT_TXT_FILE << endl;
            cout << "-> Frontend can now read this text file!" << endl;
        } else {
            cout << "\n[Error] Cannot process or save the file." << endl;
        }
        
    } else {
        cout << "\n[Error] Failed to connect to Firebase." << endl;
    }

    cout << "=========================================" << endl;
    return 0;
}