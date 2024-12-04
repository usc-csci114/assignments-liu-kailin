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

// Function to parse CSV and populate order data
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

        // Parse each line into individual cells
        while (getline(ss, cell, ',')) {
            row.push_back(cell);
        }

        // Process the header row to determine column indices
        if (isFirstLine) {
            for (size_t i = 0; i < row.size(); i++) {
                if (row[i] == "Light Type") {
                    lightIndex = i;
                } else if (row[i] == "Diptera") { // Assume "Diptera" is the first insect order column
                    startIndex = i;
                } else if (row[i] == "Total") {
                    endIndex = i;
                }
            }

            if (lightIndex == -1 || startIndex == -1 || endIndex == -1) {
                cout << "Error: Missing required columns in the CSV file." << endl;
                return;
            }

            // Populate order names based on header row
            for (int i = startIndex; i < endIndex; i++) {
                orderNames.push_back(row[i]);
            }

            isFirstLine = false; // Mark header as processed
            continue;
        }

        // Process each data row
        string lightType = row[lightIndex];
        for (int i = startIndex; i < endIndex; i++) {
            string order = orderNames[i - startIndex];
            double count = stod(row[i]);

            // Determine the index of the light type
            int innerIndex = -1;
            for (size_t j = 0; j < lightTypes.size(); j++) {
                if (lightTypes[j] == lightType) {
                    innerIndex = j;
                    break;
                }
            }

            // Add the count to the order data
            if (innerIndex != -1) {
                if (orderData.find(order) == orderData.end()) {
                    orderData[order] = vector<double>(lightTypes.size(), 0.0);
                }
                orderData[order][innerIndex] += count;
            }
        }
    }

    file.close();
}

int main() {
    string filePath = "bug-attraction-goodney.csv"; // CSV file path

    // Vectors to store light types, order names, and order data
    vector<string> lightTypes = {"A", "B", "C", "LED", "CFL", "No"}; // Light type names
    vector<string> orderNames; // Insect order names
    map<string, vector<double>> orderData; // Map to store counts for each order by light type

    // Parse data
    parseCSV(filePath, orderData, lightTypes, orderNames);


    // Step 1: Find top 4 most common insect orders overall

    priority_queue<pair<double, string>> pq; // Priority queue to track the top orders

    for (const auto &entry : orderData) { // Loop through all orders
        double totalCount = 0.0;
        for (double count : entry.second) { // Sum counts for all light types for the order
            totalCount += count;
        }
        pq.push({totalCount, entry.first}); // Add the order and total count to the priority queue
    }

    vector<string> topOrders; // Vector to store the top 4 orders

    for (int i = 0; i < 4 && !pq.empty(); ++i) { // Extract the top 4 orders
        topOrders.push_back(pq.top().second); // Save the order name
        pq.pop(); // Remove the processed order from the queue
    }


    // Step 2: Get counts for the top 4 orders by light type

    vector<vector<double>> groupedData(4, vector<double>(6, 0.0)); // Store counts for each top order
    for (size_t i = 0; i < 4; i++) {
        const string &order = topOrders[i];
        groupedData[i] = orderData[order]; // Assign the counts for each light type to the grouped data
    }


    // Step 3: Prep x-axis positions for grouped bar chart

    size_t numOrders = 4; // Number of top orders
    size_t numLightTypes = 6; // Number of light types
    double groupWidth = 0.8 ; // Total width for each group of bars
    double barWidth = 0.1; // Width of each bar in a group
    double spacing = 2.0; // Spacing between groups


    // full discretion: for the offset I had to ask one of my CS friends
    // thus the logic is not fully original, BUT they explained and we worked together 

    vector<double> baseX; // Base positions for each light type group

    for (size_t i = 0; i < numLightTypes; i++) {
        baseX.push_back(i * spacing + 1); // Increment position with spacing
    }

    vector<vector<double>> barX(numOrders, vector<double>(numLightTypes)); // X positions for each bar

    for (size_t i = 0; i < numOrders; i++) { // Loop through each top order
        for (size_t j = 0; j < numLightTypes; j++) { // Loop through each light type
            // Calculate bar position with offsets
            barX[i][j] = baseX[j] - (groupWidth / 2) + (i + 0.8) * barWidth;
        }
    }


    // Step 4: Plot bar chart

    for (size_t i = 0; i < numOrders; i++) { // Plot each set of bars for the top 4 orders
        bar(barX[i], groupedData[i]); 
        hold(on);
    }

    // Adjust plot
    xticks(baseX); // Set x-ticks to match light type positions
    xticklabels(lightTypes); 
    title("Total Counts by Light Type and Top 4 Insect Orders"); 
    /*string topOrdersSubtitle = "Top 4 Orders: " + topOrders[0] + ", " + topOrders[1] + ", " + topOrders[2] + ", " + topOrders[3];
    text(0.5, 0.9, topOrdersSubtitle, 
        {{"horizontalalignment", "center"}, {"verticalalignment", "top"}, {"transform", "axes"}}); */
    xlabel("Light Type"); 
    ylabel("Total Count");
    matplot::legend(topOrders); 

    show();
    save("barplot.png");

    return 0;
}

// clang++ -g bugs-task3-fixed.cpp -o task3fixed -std=c++17 -I/opt/homebrew/include -ldlib -lmatplot -lblas -L/opt/homebrew/lib

