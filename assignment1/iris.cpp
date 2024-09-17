#include <iostream>
#include <cmath>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <numeric>
#include <algorithm>

using namespace std;

struct Stats {
    double min, max, mean, stdev;
};

class Flower {
    public:
    string species;
    vector<double> sL, sW, pL, pW;
    Stats stats_sL, stats_sW, stats_pL, stats_pW;

    Flower(string iristype){
        species=iristype;
    };
};

//------------------------------------------------------------------------------------------------------------------------------------

// helper functions

// calculating statistics
double findMIN(const vector<double>& vec){
    return *min_element(vec.begin(), vec.end());
}

double findMAX(const vector<double>& vec){
    return *max_element(vec.begin(), vec.end());
}

double findMEAN(const vector<double>& vec){
    return accumulate(vec.begin(), vec.end(), 0.0)/vec.size();
}

double findSD(const vector<double>& vec){
    double mean = findMEAN(vec);
    double diff_sums=0;
    for (int i=0; i<vec.size(); i++){
        double diff = vec[i]-mean;
        diff_sums += diff*diff;
    }
    return sqrt(diff_sums/vec.size());
}


// add new intakes into the corresponding vector
void inputStats(Flower& flower, double temp_sL, double temp_sW, double temp_pL, double temp_pW){
    flower.sL.push_back(temp_sL);
    flower.sW.push_back(temp_sW);
    flower.pL.push_back(temp_pL);
    flower.pW.push_back(temp_pW); 
}

// function to run through the vector of each measurement
Stats findGenStats(const vector<double>& vec){
    Stats temp;
    temp.min = findMIN(vec);
    temp.max = findMAX(vec);
    temp.mean = findMEAN(vec);
    temp.stdev = findSD(vec);
    return temp;
}

// input the statistics of each measurement into the member Stats
void findIrisStats(Flower& iristype) {
    iristype.stats_sL = findGenStats(iristype.sL);
    iristype.stats_sW = findGenStats(iristype.sW);
    iristype.stats_pL = findGenStats(iristype.pL);
    iristype.stats_pW = findGenStats(iristype.pW);
}


//------------------------------------------------------------------------------------------------------------------------------------


int main(){

    // create classes for each iris
    Flower setosa("Iris-setosa");
    Flower versicolor("Iris-versicolor");
    Flower virginica("Iris-virginica");

    // read in file
    ifstream ifile("iris.txt");

    if(ifile.fail()){
        cout << "Couldn't return file" << endl;
        return 1;
    }

    //take in each row of info
    string line;
    double temp_sL, temp_sW, temp_pL, temp_pW;
    string species; 
    while(getline(ifile, line)){
        stringstream ss(line);

        ss >> temp_sL;
            ss.ignore();
        ss >> temp_sW;
            ss.ignore(); 
        ss >> temp_pL;
            ss.ignore(); 
        ss >> temp_pW;
            ss.ignore(); 
        ss >> species;     

        if (species == setosa.species){
            inputStats(setosa, temp_sL, temp_sW, temp_pL, temp_pW);
        }
        else if (species == versicolor.species){
            inputStats(versicolor, temp_sL, temp_sW, temp_pL, temp_pW);
        }
        else if (species == virginica.species){
            inputStats(virginica, temp_sL, temp_sW, temp_pL, temp_pW);
        }
    }
    ifile.close();

    findIrisStats(setosa);
    findIrisStats(versicolor);
    findIrisStats(virginica);


    // print table 
    // header
    cout << endl;
    cout << setw(30) << left << "Iris Data" << endl;
    cout << string(100, '-') << endl;
    cout << setw(12) << left << "|" 
        << "|" << setw(20) << left << "Sepal Length" 
        << "|" << setw(20) << left << "Sepal Width" 
        << "|" << setw(20) << left << "Petal Length" 
        << "|" << setw(20) << left << "Petal Width" 
        << "|" << endl;
    cout << string(100, '-') << endl;

    // each species
    cout <<  "|" << setw(11) << "setosa" 
        << "|" << setprecision(1) << fixed << setosa.stats_sL.min << "," 
                << setprecision(1) << fixed << setosa.stats_sL.max << "," 
                << setprecision(1) << fixed << setosa.stats_sL.mean << ","
                << setprecision(1) << fixed << setosa.stats_sL.stdev 
                << "     " 
        << "|" << setprecision(1) << fixed << setosa.stats_sW.min << "," 
                << setprecision(1) << fixed << setosa.stats_sW.max << "," 
                << setprecision(1) << fixed << setosa.stats_sW.mean << ","
                << setprecision(1) << fixed << setosa.stats_sW.stdev 
                << "     " 
        << "|" << setprecision(1) << fixed << setosa.stats_pL.min << "," 
                << setprecision(1) << fixed << setosa.stats_pL.max << "," 
                << setprecision(1) << fixed << setosa.stats_pL.mean << ","
                << setprecision(1) << fixed << setosa.stats_pL.stdev 
                << "     " 
        << "|" << setprecision(1) << fixed << setosa.stats_pW.min << "," 
                << setprecision(1) << fixed << setosa.stats_pW.max << "," 
                << setprecision(1) << fixed << setosa.stats_pW.mean << ","
                << setprecision(1) << fixed << setosa.stats_pW.stdev 
                << "     " << "|" << endl;

    cout <<  "|" << setw(11) << "versicolor" 
        << "|" << setprecision(1) << fixed << versicolor.stats_sL.min << "," 
                << setprecision(1) << fixed << versicolor.stats_sL.max << "," 
                << setprecision(1) << fixed << versicolor.stats_sL.mean << ","
                << setprecision(1) << fixed << versicolor.stats_sL.stdev 
                << "     " 
        << "|" << setprecision(1) << fixed << versicolor.stats_sW.min << "," 
                << setprecision(1) << fixed << versicolor.stats_sW.max << "," 
                << setprecision(1) << fixed << versicolor.stats_sW.mean << ","
                << setprecision(1) << fixed << versicolor.stats_sW.stdev 
                << "     " 
        << "|" << setprecision(1) << fixed << versicolor.stats_pL.min << "," 
                << setprecision(1) << fixed << versicolor.stats_pL.max << "," 
                << setprecision(1) << fixed << versicolor.stats_pL.mean << ","
                << setprecision(1) << fixed << versicolor.stats_pL.stdev 
                << "     " 
        << "|" << setprecision(1) << fixed << versicolor.stats_pW.min << "," 
                << setprecision(1) << fixed << versicolor.stats_pW.max << "," 
                << setprecision(1) << fixed << versicolor.stats_pW.mean << ","
                << setprecision(1) << fixed << versicolor.stats_pW.stdev 
                << "     " << "|" << endl;
    
    cout <<  "|" << setw(11) << "virginica" 
        << "|" << setprecision(1) << fixed << virginica.stats_sL.min << "," 
                << setprecision(1) << fixed << virginica.stats_sL.max << "," 
                << setprecision(1) << fixed << virginica.stats_sL.mean << ","
                << setprecision(1) << fixed << virginica.stats_sL.stdev 
                << "     " 
        << "|" << setprecision(1) << fixed << virginica.stats_sW.min << "," 
                << setprecision(1) << fixed << virginica.stats_sW.max << "," 
                << setprecision(1) << fixed << virginica.stats_sW.mean << ","
                << setprecision(1) << fixed << virginica.stats_sW.stdev 
                << "     " 
        << "|" << setprecision(1) << fixed << virginica.stats_pL.min << "," 
                << setprecision(1) << fixed << virginica.stats_pL.max << "," 
                << setprecision(1) << fixed << virginica.stats_pL.mean << ","
                << setprecision(1) << fixed << virginica.stats_pL.stdev 
                << "     " 
        << "|" << setprecision(1) << fixed << virginica.stats_pW.min << "," 
                << setprecision(1) << fixed << virginica.stats_pW.max << "," 
                << setprecision(1) << fixed << virginica.stats_pW.mean << ","
                << setprecision(1) << fixed << virginica.stats_pW.stdev 
                << "     " << "|" << endl;

    cout << string(100, '-') << endl;

    return 0;

}



