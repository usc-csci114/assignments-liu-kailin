
// study purpose: understand what light attracts insects and why types are most affected
// 6 light trap types
// 1 urban location, 2 rural
// 16 times at urban, 8 times at rural

// data structure
    // row: 1 trap on one night
    // columns: light type, location, site, weather measures, insects

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <string>
#include <matplotlibcpp.h> // Include matplotlib-cpp for graphing

using namespace std;

// Helper Functions ---------------------------------------------------------------------------------------

// parse data, returns number of orders for use in main
int parseCSV(const string& filePath, map<string, vector<double>>& lightData) {
    ifstream file(filePath);
    if (!file.is_open()) {
        cerr << "Error opening file!" << endl;
        return;
    }

    string line;
    vector<string> headers;
    bool isFirstLine = true;
    int  lightIndex = -1, startIndex = -1, endIndex = -1;

    // Read the file
    while (getline(file, line)) {
        stringstream ss(line);
        string cell;
        vector<string> row;

        // Parse the row
        while (getline(ss, cell, ',')) {
            row.push_back(cell);
        }

        // Process headers to find what index info is at
        if (isFirstLine) {
            for (size_t i = 0; i < row.size(); i++) {
                if (row[i] == "Light Type") {
                    lightIndex = i; // find light type column
                }
                else if (row[i] == "Diptera"){
                   startIndex = i;
                }
                else if (row[i] == "Total") {
                    endIndex = i; // find total bug count column
                }
            }
            isFirstLine = false; // change so it doesn't run a second time
        }

        // Add data to the map
        string lightType = row[lightIndex];
        double
        

        // Accumulate values for each light type (lightData[lightType] = key / string)
        int order=0; // track the location  in the vector that correspondes with the order
        for (int i=startIndex; i< endIndex; i++) {
            double count = stod(row[i]); // convert count in that row to a double
            lightData[lightType].second[order] += count; // insert it to the corresponding slot in the vector
                                                            // ex: index/order 0 = diptera
            order++;
        }
        
    }
    file.close();
    
    return endIndex-startIndex+1; // return number of orders
}



// Main ---------------------------------------------------------------------------------------


int main() {
    // File path
    string filePath = "bug-attraction.csv";
    
    // Map to store light type and insect counts
    map<string, vector<double>> lightData;
    
// TASK 1: find which light type is most attractive for each species -----------------------------
    
    // parse data
    numorders = parseCSV(filePath, lightData); // returns the number of orders there are
    
    // compare light types
    double max = 0; // value to store counts for each light type by order
    // vector to store order names
    vector<string> orderNames = {Diptera, Hymenoptera, Hemiptera, Psocoptera,
                                Coleoptera, Collembola, Arachnid, Thysanura,
                                Isoptera, Lepidoptera, Neuroptera, Larave,
                                Orthoptera, Unident}
    // vector to store corresponding best light type
    vector<double> bestLight;
    // comparison loop
    for (int i = 0; i < numorders; i++){ // loop for the number of orders there are
        bool first = true; // boolean to see if it is the first light type
        for (const auto& entry : lightData) { // loop through each light type
            
            if (first){
                max = entry.second[i]; // set max as the first light type's order value
                bestLight[i] = entry.first; // store name of first light type
                first = false; // set as false so it doesn't repeat
            }
            
            if (entry.second[i] > max){ // compare for subsequent
                max = entry.second[i];
                bestlight[i] = entry.first; // store name of corresponding light type
            }
        }
    }
    
    // print
    cout << setw(30) << left << "Task 1" << endl; // headers
    cout << string(50, '-') << endl;
    cout << setw(12) << left << "|"
        << "|" << setw(20) << left << "Order"
        << "|" << setw(20) << left << "Best Light Type" << endl;
    
    for (size_t i=0; i < orderNames.size(); i++){
        cout << setw(12) << left << "|"
            << "|" << setw(20) << left << orderNames[i]; // print order
            << "|" << setw(20) << left << bestLight[i] << endl; // print corresponding best light type
        
    }
    

    return 0;
}
