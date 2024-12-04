#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cmath>
#include <matplot/matplot.h>

using namespace matplot;
using namespace std;

// Class for each site type
class Site {
public:
    string sitetype;
    vector<double> Standardized_Moon;
    vector<double> total;

    Site(string site) : sitetype(site) {}
};


// Function to parse CSV and load data
void parseCSV(const string& filePath, Site& URBAN, Site& RURAL) {
    ifstream file(filePath);
    if (!file.is_open()) {
        cout << "Error opening file!" << endl;
        return;
    }

    string line;
    vector<string> headers;
    bool isFirstLine = true;
    int moonIndex = -1, siteIndex = -1, totalIndex = -1;

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
                if (row[i] == "Standardized Moon") 
                    moonIndex = i; // find Standardized Moon column
                else if (row[i] == "Location")
                    siteIndex = i; // find site column
                else if (row[i] == "Total")
                    totalIndex = i; // find total bug count column
            }
            isFirstLine = false; // change so it doesn't run a second time
        }

        // Skip rows with missing or invalid data
        if (row[moonIndex].empty() || row[totalIndex].empty())
            continue;

        // Add data to the corresponding site
        if (row[siteIndex] == "BG") {
            URBAN.Standardized_Moon.push_back(stod(row[moonIndex]));
            URBAN.total.push_back(stod(row[totalIndex]));
        } 
        else if (row[siteIndex] == "LK" || row[siteIndex] == "Stunt") {
            RURAL.Standardized_Moon.push_back(stod(row[moonIndex]));
            RURAL.total.push_back(stod(row[totalIndex]));
        }
    }
    file.close();
}

// manually calculate linear regression
pair<double, double> calculateLinearRegression(const vector<double>& x, const vector<double>& y) {
    double x_mean = 0, y_mean = 0;

    // calculate mean
    for (size_t i = 0; i < x.size(); i++) {
        x_mean += x[i];
        y_mean += y[i];
    }
    x_mean /= x.size();
    y_mean /= y.size();

    // calculate numerator and denominator
    double numerator = 0, denominator = 0;
    for (size_t i = 0; i < x.size(); ++i) {
        numerator += (x[i] - x_mean) * (y[i] - y_mean);
        denominator += pow(x[i] - x_mean, 2);
    }
    
    // final calculations
    double slope = numerator / denominator;
    double intercept = y_mean - slope * x_mean;

    return {slope, intercept};
}

int main() {
    // File path
    string filePath = "bug-attraction-goodney.csv"; 

    // Create classes
    Site urban("Urban");
    Site rural("Rural");

    // Parse data
    parseCSV(filePath, urban, rural);

    // Calculate regression lines
    pair<double, double> urbanRegression = calculateLinearRegression(urban.Standardized_Moon, urban.total);
    pair<double, double> ruralRegression = calculateLinearRegression(rural.Standardized_Moon, rural.total);

    // Prep regression line values
    vector<double> urbanFit;
    for (double x : urban.Standardized_Moon) {
        urbanFit.push_back(urbanRegression.first * x + urbanRegression.second);
    }

    vector<double> ruralFit;
    for (double x : rural.Standardized_Moon) {
        ruralFit.push_back(ruralRegression.first * x + ruralRegression.second);
    }


    // plot BG
    auto fig1 = figure(true);

    plot(urban.Standardized_Moon, urban.total, "rx");
    hold(on);
    // regression line
    plot(urban.Standardized_Moon, urbanFit, "r-");
    //matplot::legend({"Data Points", "Regression Line"});
    // labels
    xlabel("Standardized Moon");
    ylabel("Total Bugs");
    title("Urban Site (BG)");

    show();
    hold(on);
    //save("urban-BG.png");


    // plot LK and Stunt

    // scatter plot using blue xs
    plot(rural.Standardized_Moon, rural.total, "bx");
    hold(on);
    // regression line
    plot(rural.Standardized_Moon, ruralFit, "b-");  // Plot regression line
    matplot::legend({"Data Points Urban", "Regression Line Urban", "Data Points Rural", "Regression Line Rural"});
    // labels
    xlabel("Standardized Moon");
    ylabel("Total Bugs");
    title("Rural Sites (LK & Stunt)");
    show();
    save("combined-scatter.png");

    return 0;
}

// clang++ -Wall -std=c++11 bugs-task2-fixed.cpp -o task2 -g

// clang++ -g bugs-task2-fixed.cpp -o task2fixed -std=c++17 -I/opt/homebrew/include -ldlib -lmatplot -lblas -L/opt/homebrew/lib
