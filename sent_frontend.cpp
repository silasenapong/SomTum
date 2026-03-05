//จริงๆ เอาข้อมูลใน foder completed_data ไปประมวลทำFrontend ต่อได้เลยนะ หรือ
//จะใช้code ด้านล่าที่จะดึงข้อมูลมาจาก Firebase ที่จะแปลงเป็น .txt ก็ได้เหมือนกัน

#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>

using namespace std;

int main() {
    // 1. ตั้งแหล่งที่จะดึงข้อมูล คือ datalog 
    string projectId = "studio-215835202-3b08b"; 
    string collection = "datalog"; 
    string url = "https://firestore.googleapis.com/v1/projects/" + projectId + "/databases/(default)/documents/" + collection;
    
    // 2. Set(Raw JSON) และไฟล์.txt เพื่อจะส่งไปให้ Frontend
    string rawFile = "temp_raw_data.json";
    string cleanTxtFile = "frontend_datalog.txt";

    cout << "=========================================" << endl;
    cout << "  [Backend Support] Fetching & Converting" << endl;
    cout << "=========================================" << endl;

    // 3. ใช้ curl ดึงข้อมูล
    string command = "curl -s -X GET \"" + url + "\" > " + rawFile;
    int result = system(command.c_str());
    
    if (result != 0) {
        cout << "[Error] Failed to connect to Firebase." << endl;
        return 1;
    }

    // 4. แปลงข้อมูลจาก JSON ให้กลายเป็น .txt 
    ifstream inFile(rawFile);
    ofstream outFile(cleanTxtFile);
    string line;
    string date = "", time = "", temp = "", humid = "", pm25 = "";

    if (inFile.is_open() && outFile.is_open()) {
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
            
            // เมื่อเจอ createTime แปลว่าจบ 1 set ให้เขียนลงไฟล์ .txt
            if(line.find("\"createTime\"") != string::npos) {
                if(date != "") {
                    // เขียนข้อมูลลงไฟล์ .txt 
                    outFile << date << "," << time << "," << temp << "," << humid << "," << pm25 << "\n";
                    
                    // ล้างค่าต่างๆเพื่อเตรียมรับข้อมูลชุดต่อไป
                    date = ""; time = ""; temp = ""; humid = ""; pm25 = "";
                }
            }
        }
        inFile.close();
        outFile.close();
        
        // ลบไฟล์ JSON ที่อ่านค่าไปแล้วทิ้งไป 
        remove(rawFile.c_str());

        cout << "[Success] Data converted and saved to: " << cleanTxtFile << endl;
        cout << "-> Frontend can now easily read this text file line by line!" << endl;
    } else {
        cout << "[Error] Cannot process the files." << endl;
    }

    return 0;
}