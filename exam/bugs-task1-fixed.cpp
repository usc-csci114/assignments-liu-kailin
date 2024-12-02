#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <string>
#include <iomanip> 
#include <iostream>
#include <typeinfo>

using namespace std;


// Helper Functions ---------------------------------------------------------------------------------------

// parse data, returns the number of orders for use in main
void parseCSV(const string &filePath, map<string, vector<double>> &lightData, vector<string> &orderNames) {
    ifstream file(filePath);

    if (!file.is_open()) {
        cerr << "Error opening file!" << endl;
        return;
    }

    string line;
    bool isFirstLine = true;
    int lightIndex = -1, startIndex = -1, endIndex = -1;

    // Read the file
    //int rownum =0; 
    while (getline(file, line)) {

        //cout << "Processing row " << rownum << endl;
        //rownum++;

        stringstream ss(line);
        string cell;
        std::vector<std::string> row;
        string lt("Light Type");

        // Parse the row
        while (getline(ss, cell, ',')) {
            row.push_back(cell);
            //cout << "Cell is " << cell << endl;
        }
        
        /*for (size_t i = 0; i < row.size(); i++){
            cout << row[i] << "|" << endl ;
        }*/

        // Process headers to find the index of relevant columns
        if (isFirstLine) {
            //cout << "ran first line" << endl;
            for (size_t i = 0; i < row.size(); i++) {
                //cout << "running " << i << ":" << row[i] << endl;

                if (row[i] == lt) {
                    lightIndex = i;
                    //cout << "light found " << endl;
                } else if (row[i] == "Diptera") {
                    startIndex = i;
                } else if (row[i] == "Total") {
                    endIndex = i;
                }
            }

            if (lightIndex == -1 || startIndex == -1 || endIndex == -1) {
                cerr << "Error: Missing required columns in the CSV file." << endl;
                return;
            }

            // Populate order names from headers
            for (int i = startIndex; i < endIndex; i++) {
                orderNames.push_back(row[i]);
            }

            isFirstLine = false; // Headers processed
            continue;
        }

        // Skip rows with missing data
        if (!isFirstLine){
            // Add data to the map
            //cout << "ran past firstline" << endl; 
            string lightType = row[lightIndex];

            // intialize map if first time --> is there a better way to check this (the logic doesn't really click for me)
            if (lightData.find(lightType) == lightData.end()) { 
                lightData[lightType] = vector<double>(endIndex - startIndex, 0.0);
            }
            // add data to corresponding map pair
            for (int i = startIndex; i < endIndex; i++) {
                double count = stod(row[i]);
                lightData[lightType][i - startIndex] += count;
            }
        }

    }

    file.close();
}
// Main ---------------------------------------------------------------------------------------

int main() {
    // File path
    string filePath = "bug-attraction-goodney.csv";

    // Map to store light type and insect counts
    map<string, vector<double>> lightData;

    // Vector to store order names
    /*vector<string> orderNames ={"Diptera", "Hymenoptera", "Hemiptera", "Psocoptera",
                                "Coleoptera", "Collembola", "Arachnid", "Thysanura",
                                "Isoptera", "Lepidoptera", "Neuroptera", "Larave",
                                "Orthoptera", "Unident"};*/
    vector<string> orderNames;

    // Parse data
    parseCSV(filePath, lightData, orderNames);

    /*if (orderNames.empty() || lightData.empty()) {
        cerr << "Error: No data to process. Check the input file." << endl;
        return 1;
    }*/

    // Determine the most attractive light type for each order
    vector<string> bestLight(orderNames.size());
    
    for (size_t i = 0; i < orderNames.size(); i++) {
        //cout << "RUNNING " << orderNames[i] << endl;
        string bestLightType = "NA";
        double maxCount = 0;

        for (const auto& entry : lightData) {
            if (i >= entry.second.size()) {
                cerr << "Error: Inconsistent data size for light type " << entry.first << endl;
                return 1;
            }

           //cout << "Looking at Light Type " << entry.first << endl;
            
            if (entry.second[i] > maxCount) {
                maxCount = entry.second[i];
                 bestLightType = entry.first;
            }

        }

        bestLight[i] = bestLightType;
        // cout << endl;
        //cout << "For " << orderNames[i] << "Best light is: " << bestLight[i] << endl;
    }

    // Print the table
    cout << setw(30) << left << "Species" << "Best Light Type" << endl;
    cout << string(50, '-') << endl;
    for (size_t i = 0; i < orderNames.size(); i++) {
        cout << setw(30) << left << orderNames[i] << bestLight[i] << endl;
    }

    return 0;
}

// clang++ -Wall -std=c++11 bugs-task1-fixed.cpp -o task1 -g
