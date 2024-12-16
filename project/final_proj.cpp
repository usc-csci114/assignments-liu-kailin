#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <string>
#include <algorithm>
#include <matplot/matplot.h>
#include <random>

using namespace matplot;
using namespace std;

// Helper Functions (test parsing)

void printMapKeys(const map<string, vector<double>> &countryData) {
    cout << "Keys (countries) in the map:" << endl;
    for (const auto &pair : countryData) {
        cout << pair.first << endl; 
    }
}

void printCountryData(const map<string, vector<double>> &countryData, vector<string> &columnNames, const string &country) {
    // Check if country exists in the map
    auto it = countryData.find(country);
    if (it != countryData.end()) {
        // Access vector of values for the country
        const vector<double> &values = it->second;
        
        // Print all values
        cout << "Data for " << country << ":" << endl;
        int i =0;
        for (double value : values) {
            cout << "[" << i << "]" << columnNames[i] << ": " << value << endl;
            i++;
        }
        cout << endl;
    } 
    else {
        cout << "Country '" << country << "' not found in data." << endl;
    }
}



// Parse CSV, populate data, and dynamically find column indices
void parseCSV(const string &filePath, map<string, vector<double>> &countryData, 
              vector<string> &countryNames, vector<string> &columnNames, map<string, int> &columnIndices) {
  
    ifstream ifile(filePath);
    
    if (!ifile.is_open()) {
        cout << "Error opening file!" << endl;
        return;
    }
    
    string line;
    bool isFirstLine = true;
    
    vector<int> animalIndices, vegetableIndices; // Indices for sums later
    int avoid_NK =0, avoid_SK = 0; // Row indices with special cases
    
    getline(ifile, line); // Skip first line (headers, hard coded in main)
    
    // Find indices for sum later
    for (size_t i = 0; i < columnNames.size(); i++) { 
        
        // Identify indices for columns needed for sum
        /*if(columnNames[i] == "Obesity"){
        undernourish_idx=i+1;
        cout << "FOUND UNDERNOURISH AT " << undernourish_idx << endl;
        cout << endl;
        }*/
        if (columnNames[i] == "Animal Products" || columnNames[i] == "Meat") {
            //cout << "found animal index at " << i << endl;
            animalIndices.push_back(i); // Adjust to match vector index
        }
        if (columnNames[i] == "Vegetables" || columnNames[i] == "Vegetal Products") {
            //cout << "found veggie index at " << i << endl;
            vegetableIndices.push_back(i);
        }
    }

    // Find indices of special cases (country name includes comma)
    for (size_t i = 0; i < countryNames.size(); i++) { 
        if (countryNames[i]== "Korea, North") {
            avoid_NK = i;
            //cout << avoid_NK << endl;
        }
        if (countryNames[i]== "Korea, South"){
            avoid_SK = i;
            //cout << avoid_SK << endl;
        }
    }
    
    // Add calculated columns to column names
    columnNames.push_back("Animal + Meat");
    columnNames.push_back("Vegetables + Vegetal");
    
    // Reassign/add corresponding column indices
    columnIndices["Animal + Meat"] = columnNames.size() - 2;
    columnIndices["Vegetables + Vegetal"] = columnNames.size() - 1;
    
    /* for (size_t i = 0; i < columnNames.size(); i++) { 
        cout << columnNames[i] << endl;
    }
    cout << endl; */
        
        
        
    // Parsing data
    int rownum=1;
    while (getline(ifile, line)) {
        stringstream ss(line);
        string cell;
        vector<string> row;
        
        // Parse each line into cells
        while (getline(ss, cell, ',')) {
        row.push_back(cell);
        }
        
        // Process data rows
        
        string country = countryNames[rownum-1]; // First column is the country name
        //cout << "Country: " << country << endl;
        
        vector<double> values;
        
        // Process all columns except the first and last 
        for (size_t i = 1; i < row.size() - 1; i++) {
            //cout << "got to " << columnNames[i-1] << " value "<< row[i] << endl;

            if (rownum == avoid_NK || rownum == avoid_SK){
                i++; // Increment once because comma will make second half of name a numer
            }

            try {
                values.push_back(stod(row[i])); // Attempt to convert the cell value to a double
            } catch (const std::invalid_argument &) {
                values.push_back(-1.0); // If conversion fails, use -1.0 as the default
            }
        //}
        
        }
        
        // Calculate Animal Products + Meat and Vegetables + Vegetal Products sums
        double animalSum = 0.0, vegetableSum = 0.0;
        for (int idx : animalIndices) {
            animalSum += values[idx];
        }
        for (int idx : vegetableIndices) {
            vegetableSum += values[idx];
        }
        values.push_back(animalSum); // Append calculated sums to the data
        values.push_back(vegetableSum); // Animal first then veggie
        
        // Add completed vector to map
        countryData[country] = values;

        rownum++;
    }
    
    //cout << "Successfully parsed" << endl;
    //printMapKeys(countryData);
    //printCountryData(countryData, columnNames, "Kuwait");
    
    ifile.close();
}


// Subset for top 5 and bottom 5 metric rate countries
void subsetCountries(const map<string, vector<double>> &countryData, const vector<string> &countryNames,
                     const map<string, int> &columnIndices, vector<string> &topCountries, vector<string> &bottomCountries, 
                     string metric) {
  
  priority_queue<pair<double, string>> maxHeap; // Max-heap to store the top 5 countries
  priority_queue<pair<double, string>, vector<pair<double, string>>, greater<pair<double, string>>> minHeap; // Min-heap to store the bottom 5 countries
  
  int metricIndex = columnIndices.at(metric); // Get index of the "metric" column
  
  // Loop through each country in the list of country names
  for (const auto &country : countryNames) {
    // Get metric rate using the index
    double metricRate = countryData.at(country)[metricIndex];
    
    // Push the country and its metric rate into both heaps if metric rate is valid
    if (metricRate != -1 && country != "Kiribati" && country != "Korea, North" && country != "Korea, South") { // exclude special cases
      maxHeap.push({metricRate, country});  // For max-heap (highest metric rate)
      minHeap.push({metricRate, country});  // For min-heap (lowest metric rate)
    }
    
    // If the size of the max-heap exceeds 5, remove the smallest (because it's max-heap, smallest value comes first)
        if (maxHeap.size() > 5) {
            maxHeap.pop(); 
        }

        // If the size of the min-heap exceeds 5, remove the largest (because it's min-heap, largest value comes first)
    if (minHeap.size() > 5) {
      minHeap.pop();
    }
  }
  
  // Pop top (biggest) 5 from min-heap
  while (!minHeap.empty()) {
    topCountries.push_back(minHeap.top().second); // Store the country name
    minHeap.pop(); // Remove top element
  }
  
  // Reverse to get lowest metric rate first
  reverse(topCountries.begin(), topCountries.end());
  cout << "Top 5: ";
  for (const auto &country : topCountries) {
    cout << country << ", ";
  }
  cout << endl;
  
  // Pop top (lowest) 5 countries from  max-heap
  while (!maxHeap.empty()) {
    bottomCountries.push_back(maxHeap.top().second); // Store the country name
    maxHeap.pop(); // Remove top element
  }
  
  // Reverse to get highest metric rate first
  reverse(bottomCountries.begin(), bottomCountries.end());
  cout << "Bottom 5: ";
  for (const auto &country : bottomCountries) {
    cout << country << ", ";
  }
  cout << endl;
  
}



// Create simple bar plots
void makeBP(const map<string, vector<double>> &countryData, vector<string> &countryNames, const vector<string> &columnNames, 
            const map<string, int> &columnIndices, vector<string> &topCountries, vector<string> &bottomCountries) {
  
    vector<double> ticks = {1.0, 2.0, 3.0, 4.0, 5.0};
    
    // BY OBESITY
    vector<double> topRates, bottomRates;
    for (const string &country : topCountries) {
        topRates.push_back(countryData.at(country)[columnIndices.at("Obesity")]);
    }
    for (const string &country : bottomCountries) {
        bottomRates.push_back(countryData.at(country)[columnIndices.at("Obesity")]);
    }

    for (size_t i=0; i < topRates.size(); i++){
        cout << topCountries[i] << ": " << topRates[i] << endl;
    }

    for (size_t i=0; i < bottomRates.size(); i++){
        cout << bottomCountries[i] << ": " << bottomRates[i] << endl;
    }


    // Barplot for top 5 countries
    matplot::bar(ticks, topRates);
    ylabel("Obesity Rate (%)");
    xticklabels(topCountries);
    title("Countries with Highest Obesity Rates");
    show();
    save("BP-Top5.png");
    
    auto fig = figure();
    // Barplot for bottom 5 countries
    matplot::bar(ticks, bottomRates);
    ylabel("obesity Rate (%)");
    xticklabels(bottomCountries);
    title("Countries with Lowest Obesity Rates");
    show();
    save("BP-Bottom5.png");


     // BY COVID-19 CASES
    fig = figure();    
    
    /*vector<double> caseRates;
    for (const string &country : topCountries) {
        caseRates.push_back(countryData.at(country)[columnIndices.at("Confirmed")]);
    }
    for (const string &country : bottomCountries) {
        caseRates.push_back(countryData.at(country)[columnIndices.at("Confirmed")]);
    }
    
    cout << caseRates.size() << endl;
    double tick =0.0;
    for (int i=0; i < caseRates.size(); i++){
        ticks[i]= tick;
        tick += 1.0;
        cout << tick << " ";
    }
    cout << endl;

    ticks = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0};


    vector<string> tenCountries = topCountries;
    tenCountries.insert(tenCountries.end(), bottomCountries.begin(), bottomCountries.end());
    cout << "got here" << endl;
    cout << tenCountries.size() << endl;
    for (size_t i=0; i<tenCountries.size(); i++){
        cout << tenCountries[i] << endl;
    }
    matplot::bar(ticks, caseRates);
    cout << "got here" << endl;
    ylabel("Covid-19 Case Rate");
    xticklabels(tenCountries);
    title("Covid-19 Case Rate by Country");
    show();
    save("BP-Cases.png");*/ 

    vector<double> topCaseRates, bottomCaseRates;
    for (const string &country : topCountries) {
        topCaseRates.push_back(countryData.at(country)[columnIndices.at("Confirmed")]);
    }
    for (const string &country : bottomCountries) {
        bottomCaseRates.push_back(countryData.at(country)[columnIndices.at("Confirmed")]);
    }

    for (size_t i=0; i < topCaseRates.size(); i++){
        cout << topCountries[i] << ": " << topCaseRates[i] << endl;
    }

    for (size_t i=0; i < bottomCaseRates.size(); i++){
        cout << bottomCountries[i] << ": " << bottomCaseRates[i] << endl;
    }

    // Barplot for top 5 countries
    matplot::bar(ticks, topCaseRates);
    ylabel("Confirmed Cases Rate (%)");
    xticklabels(topCountries);
    title("Case Rates of Top 5 Obesity Rate Countries");
    show();
    save("BP-Top5-Cases.png");
    
    fig = figure();
    // Barplot for bottom 5 countries
    matplot::bar(ticks, bottomCaseRates);
    ylabel("Confirmed Cases Rate (%)");
    xticklabels(bottomCountries);
    title("Case Rates of Bottom 5 Obesity Rate Countries");
    show();
    save("BP-Bottom5-Cases.png");
    

}



// Create grouped bar charts
void makeGroupedBarCharts(const map<string, vector<double>> &countryData, vector<string> &topCountries,
                          vector<string> &bottomCountries, const map<string, int> &columnIndices, string metric) {

    
    // Food categories to plot
    vector<string> foodGroups = {"Fish, Seafood", "Animal + Meat", "Vegetables + Vegetal", "Cereals - Excluding Beer", "Sugar & Sweeteners"};
    
    // Fill in the bar data for the top countries (for display in terminal)
    vector<vector<double>> topBarData_DISPLAY;
    for (const string &country : topCountries) {
        topBarData_DISPLAY.push_back({
            countryData.at(country)[columnIndices.at("Fish, Seafood")],
            countryData.at(country)[columnIndices.at("Animal + Meat")],
            countryData.at(country)[columnIndices.at("Vegetables + Vegetal")],
            countryData.at(country)[columnIndices.at("Cereals - Excluding Beer")],
            countryData.at(country)[columnIndices.at("Sugar & Sweeteners")]
        });
    }
    // Fill in the bar data for the top countries (for grouped bar graph)
    vector<vector<double>> topBarData_GRAPH;
    for (const string &group : foodGroups) {
        topBarData_GRAPH.push_back({
            countryData.at(topCountries[0])[columnIndices.at(group)],
            countryData.at(topCountries[1])[columnIndices.at(group)],
            countryData.at(topCountries[2])[columnIndices.at(group)],
            countryData.at(topCountries[3])[columnIndices.at(group)],
            countryData.at(topCountries[4])[columnIndices.at(group)]
        });
    }

    // Print out info about food groups by country
    cout << "TOP COUNTRIES by " << metric << endl;
    for (size_t i=0; i < 5; i++){
        cout << topCountries[i] << ": " << endl;
        for (size_t j=0; j < 5; j++){
            cout << foodGroups[j] << ": " << topBarData_DISPLAY[i][j]<< ", ";
        }
        cout << endl;
    } 
    
    // Fill in the bar data for the bottom countries (for display in terminal)
    vector<vector<double>> bottomBarData_DISPLAY;
    for (const string &country : bottomCountries) {
        bottomBarData_DISPLAY.push_back({
            countryData.at(country)[columnIndices.at("Fish, Seafood")],
            countryData.at(country)[columnIndices.at("Animal + Meat")],
            countryData.at(country)[columnIndices.at("Vegetables + Vegetal")],
            countryData.at(country)[columnIndices.at("Cereals - Excluding Beer")],
            countryData.at(country)[columnIndices.at("Sugar & Sweeteners")]
        });
    }
    // Fill in the bar data for the bottom countries (for grouped bar graph) 
    vector<vector<double>> bottomBarData_GRAPH;
    for (const string &group : foodGroups) {
        bottomBarData_GRAPH.push_back({
            countryData.at(bottomCountries[0])[columnIndices.at(group)],
            countryData.at(bottomCountries[1])[columnIndices.at(group)],
            countryData.at(bottomCountries[2])[columnIndices.at(group)],
            countryData.at(bottomCountries[3])[columnIndices.at(group)],
            countryData.at(bottomCountries[4])[columnIndices.at(group)]
        });
    }

    // Print out info about food groups by country
    cout << "BOTTOM COUNTRIES by " << metric << endl;
    for (size_t i=0; i < 5; i++){
        cout << bottomCountries[i] << ": " << endl;
        for (size_t j=0; j < 5; j++){
            cout << foodGroups[j] << ": " << bottomBarData_DISPLAY[i][j]<< ", ";
        }
        cout << endl;
    }

    
  // GRAPHING 

    vector<double> xValues = {1.0, 2.0, 3.0, 4.0, 5.0};

    // Plot the bottom countries' bar data (stacked/grouped)
    auto fig = figure();
    auto b1 = bar(xValues, topBarData_GRAPH);


    // Adjust plot for top countries
    xticklabels(topCountries); // Set x-tick labels to the country names
    title("Countries with Highest Rates");
    xlabel("Countries");
    ylabel("Supply (kcal/person/day)");
    //matplot::legend(foodGroups); // Legend for food groups
    show();
    string filelabel = "GBP-TopCountries-" + metric + ".png";
    save(filelabel);

    // Plot the bottom countries' bar data (stacked/grouped)
    fig = figure();
    auto b2 = bar(xValues, bottomBarData_GRAPH);

    // Adjust plot for bottom countries
    xticklabels(bottomCountries); // Set x-tick labels to the country names
    title("Countries with Lowest Rates");
    xlabel("Countries");
    ylabel("Supply (kcal/person/day)");
    //matplot::legend(foodGroups); // Legend for food groups
    show();
    filelabel = "GBP-BottomCountries-" + metric +".png";
    save(filelabel);

}


// Linear Regression helper function
pair<double, double> calculateLinearRegression(const vector<double> &x, const vector<double> &y) {
    double x_mean = 0, y_mean = 0;

    // Calculate means
    for (size_t i = 0; i < x.size(); ++i) {
        x_mean += x[i];
        y_mean += y[i];
    }
    x_mean /= x.size();
    y_mean /= y.size();

    // Calculate numerator and denominator for slope
    double numerator = 0, denominator = 0;
    for (size_t i = 0; i < x.size(); ++i) {
        numerator += (x[i] - x_mean) * (y[i] - y_mean);
        denominator += pow(x[i] - x_mean, 2);
    }

    // Calculate slope and intercept
    double slope = numerator / denominator;
    double intercept = y_mean - slope * x_mean;

    return {slope, intercept};
}

// Make Scatter Plot
void makeScatterPlots(const map<string, vector<double>> &countryData, const vector<string> &countryNames,
                      const vector<string> &foodGroups,const map<string, int> &columnIndices) {
    // Define COVID-19 metrics to analyze
    vector<string> metrics = {"Confirmed", "Deaths", "Recovered", "Active"};

    for (const auto &metric : metrics) { // Outmost loop for disease metric
        for (const auto &foodGroup : foodGroups) { // For each disease metric, make 1 plot for each food group
            vector<double> xValues, yValues;

            for (const auto &country : countryNames) { // Process each country
                try {
                    // Get food group and metric data
                    double x = countryData.at(country).at(columnIndices.at(foodGroup));
                    double y = countryData.at(country).at(columnIndices.at(metric));

                    // Omit invalid data points (e.g., missing or default values)
                    if (x == -1 || y == -1 || 
                        foodGroup == "Animal + Meat" && x > 100 || foodGroup == "Vegetables + Vegetal" && x < 5
                        || foodGroup == "Fish, Seafood" && x > 4) {
                        throw runtime_error("Invalid data");
                    }
                    xValues.push_back(x);
                    yValues.push_back(y);
                } catch (const exception &e) {
                    // Log the issue and skip this country
                    cout << "Skipping country " << country << " due to data issue: " << e.what() << endl;
                    continue;
                }
            }

            // Ensure there's enough valid data for regression
            if (xValues.size() < 10) {
                cout << "Not enough valid data for " << foodGroup << " vs " << metric << endl;
                continue;
            }

            // Calculate regression line
            pair<double, double> regression = calculateLinearRegression(xValues, yValues);
            vector<double> regressionLine;

            for (double x : xValues) {
                regressionLine.push_back(regression.first * x + regression.second);
            }

            // Plot scatter and regression line
            auto fig = figure(true);
            plot(xValues, yValues, "o"); // Scatter plot
            hold(on);
            plot(xValues, regressionLine, "-"); // Regression line
            xlabel(foodGroup + " (Supply)");
            ylabel(metric + " (Cases)");
            title(foodGroup + " vs " + metric + " (All Countries)");

            // Save the figure
            save("SP_" + foodGroup + "_v_" + metric + ".png");
            show();
        }
    }
}



// --------------------------------------------------------------------------------------------------------------------


// Main 
int main() {
    string filePath = "Food_Supply_kcal_Data.csv";
  
    map<string, vector<double>> countryData;
    // Had to hard code this bc of comma separated titles
    vector<string> countryNames = {"Afghanistan", "Albania", "Algeria", "Angola", "Antigua and Barbuda", "Argentina", "Armenia", "Australia", "Austria", "Azerbaijan", 
        "Bahamas", "Bangladesh", "Barbados", "Belarus", "Belgium", "Belize", "Benin", "Bolivia", "Bosnia and Herzegovina", "Botswana", 
        "Brazil", "Bulgaria", "Burkina Faso", "Cabo Verde", "Cambodia", "Cameroon", "Canada", "Central African Republic", "Chad", "Chile", 
        "China", "Colombia", "Congo", "Costa Rica", "Cote d'Ivoire", "Croatia", "Cuba", "Cyprus", "Czechia", "Denmark", "Djibouti", 
        "Dominica", "Dominican Republic", "Ecuador", "Egypt", "El Salvador", "Estonia", "Eswatini", "Ethiopia", "Fiji", "Finland", 
        "France", "French Polynesia", "Gabon", "Gambia", "Georgia", "Germany", "Ghana", "Greece", "Grenada", "Guatemala", "Guinea", 
        "Guinea-Bissau", "Guyana", "Haiti", "Honduras", "Hungary", "Iceland", "India", "Indonesia", "Iran (Islamic Republic of)", 
        "Iraq", "Ireland", "Israel", "Italy", "Jamaica", "Japan", "Jordan", "Kazakhstan", "Kenya", "Kiribati", "Korea, North", 
        "Korea, South", "Kuwait", "Kyrgyzstan", "Lao People's Democratic Republic", "Latvia", "Lebanon", "Lesotho", "Liberia", 
        "Lithuania", "Luxembourg", "Madagascar", "Malawi", "Malaysia", "Maldives", "Mali", "Malta", "Mauritania", "Mauritius", 
        "Mexico", "Mongolia", "Montenegro", "Morocco", "Mozambique", "Myanmar", "Namibia", "Nepal", "Netherlands", "New Caledonia", 
        "New Zealand", "Nicaragua", "Niger", "Nigeria", "North Macedonia", "Norway", "Oman", "Pakistan", "Panama", "Paraguay", "Peru", 
        "Philippines", "Poland", "Portugal", "Republic of Moldova", "Romania", "Russian Federation", "Rwanda", "Saint Kitts and Nevis", 
        "Saint Lucia", "Saint Vincent and the Grenadines", "Samoa", "Sao Tome and Principe", "Saudi Arabia", "Senegal", "Serbia", 
        "Sierra Leone", "Slovakia", "Slovenia", "Solomon Islands", "South Africa", "Spain", "Sri Lanka", "Sudan", "Suriname", "Sweden", 
        "Switzerland", "Taiwan*", "Tajikistan", "Thailand", "Timor-Leste", "Togo", "Trinidad and Tobago", "Tunisia", "Turkey", 
        "Turkmenistan", "Uganda", "Ukraine", "United Arab Emirates", "United Kingdom", "United Republic of Tanzania", 
        "United States of America", "Uruguay", "Uzbekistan", "Vanuatu", "Venezuela (Bolivarian Republic of)", "Vietnam", "Yemen", 
        "Zambia", "Zimbabwe"};
    vector<string> columnNames = { "Alcoholic Beverages", "Animal Products", "Animal fats", "Aquatic Products, Other",
        "Cereals - Excluding Beer","Eggs", "Fish, Seafood", "Fruits - Excluding Wine", "Meat", "Milk - Excluding Butter", 	
        "Miscellaneous","Offals", "Oilcrops", "Pulses", "Spices", "Starchy Roots", "Stimulants", "Sugar Crops", 
        "Sugar & Sweeteners", "Treenuts", "Vegetal Products", "Vegetable Oils", "Vegetables", "Obesity", "Undernourished",
        "Confirmed", "Deaths", "Recovered", "Active", "Population"};
    vector<string> foodGroups = {"Fish, Seafood", "Animal + Meat", "Vegetables + Vegetal", "Cereals - Excluding Beer", "Sugar & Sweeteners"};
    
    map<string, int> columnIndices;

    // Hard code/ populate columnIndices
    for (size_t i = 0; i < columnNames.size(); ++i) {
        columnIndices[columnNames[i]] = i; // Map column name to its index
    }
    
    // Parse CSV
    parseCSV(filePath, countryData, countryNames, columnNames, columnIndices);

    
    // Subset countries by highest and lowest obesity rate
    vector<string> topCountries, bottomCountries;
    subsetCountries(countryData, countryNames, columnIndices, topCountries, bottomCountries, "Obesity");
    
    // Make Bar Plot
    makeBP(countryData, countryNames, columnNames, columnIndices, topCountries, bottomCountries);

    // Subset countries by highest and lowest COVID rate
    vector<string> topCOVID, bottomCOVID;
    subsetCountries(countryData, countryNames, columnIndices, topCOVID, bottomCOVID, "Confirmed");
    

    // Make Grouped Bar Charts
    makeGroupedBarCharts(countryData, topCountries, bottomCountries, columnIndices, "Obesity");
    makeGroupedBarCharts(countryData, topCOVID, bottomCOVID, columnIndices, "Confirmed");
    
    // Make Scatter Plots
    makeScatterPlots(countryData, countryNames, foodGroups, columnIndices);


    return 0;
}

// Compilation line
// clang++ -g final_proj.cpp -o finalproj -std=c++17 -I/opt/homebrew/include -ldlib -lmatplot -lblas -L/opt/homebrew/lib