#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <map>
#include <queue>
#include <algorithm>
#include <matplot/matplot.h>


using namespace matplot;
using namespace std;

// working on seg fault error

void parseCSV(const string &filePath, map<string, vector<double>> &orderData, vector<string> &lightTypes, vector<string> &orderNames) {
    
    ifstream file(filePath);

    if (!file.is_open()) {
        cout << "Error opening file!" << endl;
        return;
    }

    string line;
    bool isFirstLine = true;
    int lightIndex = -1, startIndex = -1, endIndex = -1;

    // Read the file
    while (getline(file, line)) {
        stringstream ss(line);
        string cell;
        vector<string> row;

        // Parse the row
        while (getline(ss, cell, ',')) {
            row.push_back(cell);
        }

        // Process headers to find the index of relevant columns
        if (isFirstLine) {
            for (size_t i = 0; i < row.size(); i++) {
                if (row[i] == "Light Type") {
                    lightIndex = i;
                } else if (row[i] == "Diptera") { // Assuming the first insect order is Diptera
                    startIndex = i;
                } else if (row[i] == "Total") {
                    endIndex = i;
                }
            }

            if (lightIndex == -1 || startIndex == -1 || endIndex == -1) {
                cout << "Error: Missing required columns in the CSV file." << endl;
                return;
            }

            // Populate light types and order names from headers
            for (int i = startIndex; i < endIndex; i++) {
                orderNames.push_back(row[i]);
            }
            for (size_t i = 0; i < lightIndex; i++) {
                lightTypes.push_back(row[i]);
            }

            isFirstLine = false; // Headers processed
            continue;
        }

        // Skip rows with missing data
        string lightType = row[lightIndex];

        // For each order, add the counts to the corresponding entry in the map
        for (int i = startIndex; i < endIndex; i++) {
            string order = orderNames[i - startIndex];
            double count = stod(row[i]);
            
            // Find the index within the map vector for the current light type using a for loop
            int innerIndex = -1;
            for (size_t i = 0; i < lightTypes.size(); ++i) {
                if (lightTypes[i] == lightType) {
                    innerIndex = i;
                    break;
                }
            }

            // If lightType was found (lightIndex is not -1), add the count to the corresponding order
            if (lightIndex != -1) {
                // Make the vector if first time
                if (orderData.find(order) == orderData.end()) {
                    orderData[order] = vector<double>(lightTypes.size(), 0.0);  // Initialize if not found
                }

                // Add the count to the correct index for the current light type
                orderData[order][innerIndex] += count;
            }
    }

    file.close();
    }
}

int main() {
     // File path
    string filePath = "bug-attraction-goodney.csv"; 

    // vectors store light types, order names, and the order data map (parallels for later)
    vector<string> lightTypes;  //store the names of the light types
    vector<string> orderNames;  // store the names of insect orders
    
    // Map of insect orders -> vector of counts by light type
    map<string, vector<double>> orderData;  

    // Parse data
    parseCSV(filePath, orderData, lightTypes, orderNames);

    // Find top 4
    /* vector<double> totalCounts; // tally total counts by order
    for (const auto &entry : orderData) {

        if (entry.second[i] > maxCount) {
            maxCount = entry.second[i];
            bestLightType = entry.first;
        } 
    } */

    // Priority queue (max-heap) to store orders with their total counts (lab 5)
    priority_queue<pair<double, string>> pq;

    // Calculate total counts for each order and push to priority queue
    for (const auto& entry : orderData) {
        double totalCount = 0.0;
        // sum counts across all light types
        for (double count : entry.second) {
            totalCount += count;
        }

        pq.push({totalCount, entry.first});
    }

    // Vector to store  top 4 orders
    vector<string> topOrders;

    // Extract top 4 orders from the priority queue
    for (int i = 0; i < 4 && !pq.empty(); ++i) {
        // Get the top order (highest count)
        auto top = pq.top();
        
        // Store the order in the vector
        topOrders.push_back(top.second);
        
        // Remove the top element from the priority queue
        pq.pop();
    }

    // PLOT

    // vectors for each of the top 4 orders
    vector<double> order1Counts(lightTypes.size(), 0.0);
    vector<double> order2Counts(lightTypes.size(), 0.0);
    vector<double> order3Counts(lightTypes.size(), 0.0);
    vector<double> order4Counts(lightTypes.size(), 0.0);

    // input data
    for (int i = 0; i < topOrders.size(); ++i) {
        const string& order = topOrders[i];
        const vector<double>& counts = orderData[order];
        
        if (i == 0) {
            order1Counts = counts;
        } 
        else if (i == 1) {
            order2Counts = counts;
        } 
        else if (i == 2) {
            order3Counts = counts;
        } 
        else if (i == 3) {
            order4Counts = counts;
        }
    }

    // Prepare x positions for each light type group
    vector<double> x(lightTypes.size());
    for (int i = 0; i < lightTypes.size(); ++i) {
        x[i] = i + 1;
    }

    // Plot grouped bar chart
    // Plot bars for each of the top 4 orders
    bar(x, order1Counts);
    for (auto& xi : x) {
        xi += 0.2;
    }  // Offset x for the next set of bars
    hold(on);
    
    bar(x, order2Counts);
    for (auto& xi : x) {
        xi += 0.2;
    }  // Offset x for the next set of bars
    hold(on);
    
    bar(x, order3Counts);
    for (auto& xi : x) {
        xi += 0.2;
    }  // Offset x for the next set of bars
    hold(on);
    
    bar(x, order4Counts);
    for (auto& xi : x) {
        xi += 0.2;
    }  // Offset x for the next set of bars
    hold(on);
    
    // Label the x-axis with the light types
    // xticks({1.0, 1.2, 1.4}, lightTypes);

    title("Total Counts by Light Type and Order");
    xlabel("Light Type");
    ylabel("Total Count");

    // Show the plot
    show();

    // save plot
    save("barplot.png");


    return 0;
}
   



// clang++ -g bugs-task3-INCOMPLETE.cpp -o task3fixed -std=c++17 -I/opt/homebrew/include -ldlib -lmatplot -lblas -L/opt/homebrew/lib
