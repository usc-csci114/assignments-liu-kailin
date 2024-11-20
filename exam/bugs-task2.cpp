
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
#include <string>
#include <matplotlibcpp.h> // Include matplotlib-cpp for graphing

using namespace std;

// Helper Functions

// class for each site type
class Site {
    public:
    string sitetype;
    vector<double> Standardized_Moon;
    vector<double> total;

    Site(string site){
        sitetype = site;
    };
};

// parse data, returns number of orders for use in main
void parseCSV(const string& filePath, Site& URBAN, Site& RURAL) {
    
    ifstream file(filePath);
    if (!file.is_open()) {
        cerr << "Error opening file!" << endl;
        return;
    }

    string line;
    vector<string> headers;
    bool isFirstLine = true;
    int  moonIndex = -1, siteIndex = -1, totalIndex = -1;

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
                if (row[i] == "Standardized Moon") {
                    moonIndex = i; // find Standardized Moon column
                }
                else if (row[i] == "Location"){
                   siteIndex = i; // find site column
                }
                else if (row[i] == "Total"){
                   totalIndex = i; // find total bug count column
                }
            }
            isFirstLine = false; // change so it doesn't run a second time
        }

        // Add data to the corresponding site
        
        if(row[siteIndex] == "BG"){
            URBAN.Standardized_Moon.push_back(row[moonIndex]);
            URBAN.Standardized_Moon.push_back(row[totalIndex]);
        }
        else if (row[siteIndex] == "LK" || row[siteIndex] == "Stunt"){
            RURAL.Standardized_Moon.push_back(row[moonIndex]);
            RURAL.Standardized_Moon.push_back(row[totalIndex]);
        }

    }
    file.close();
    
}



int main() {
    
    // File path
    string filePath = "bug-attraction.csv";
    
    // TASK 2: Scatter Plot ------------------------------------------------------------------------
    
    // create classes
    Site urban ("urban");
    Site rural ("rural");
    
    // parse data
    parseCSV(filePath, urban, rural);
    
    // plot BG
    plot(urban.Standardized_Moon, urban.total, "rx");
    hold(on);
    vector<double> trend_x = linspace(2.2, 4.0, 10);
    vector<double> trend_y = transform(trend_x, [](auto x) {return 240.5*x + 1043;});
    show();
    save("urban-BG.svg");
    
    // plot LK and Stunt
    plot(rural.Standardized_Moon, rural.total, "rx");
    hold(on);
    vector<double> trend_x = linspace(2.2, 4.0, 10);
    vector<double> trend_y = transform(trend_x, [](auto x) {return 240.5*x + 1043;});
    show();
    save("rural-LK-Stunt.svg");

    return 0;
}
