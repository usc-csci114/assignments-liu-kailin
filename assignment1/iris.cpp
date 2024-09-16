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
            inputStats(setosa, temp_sL,  temp_sW, temp_pL, temp_pW);
        }
        else if (species == versicolor.species){
            inputStats(versicolor, temp_sL,  temp_sW, temp_pL, temp_pW);
        }
        else if (species == virginica.species){
            inputStats(virginica, temp_sL,  temp_sW, temp_pL, temp_pW);
        }
    }
    ifile.close();

    findIrisStats(setosa);
    findIrisStats(versicolor);
    findIrisStats(virginica);


    // print table 
    // header
    cout << setw(30) << left << "Iris Data" << endl;
    cout << string(100, '-') << endl;
    cout << setw(10) << left << "|" 
        << "|" << setw(20) << left << "Sepal Length" 
        << "|" << setw(20) << left << "Sepal Width" 
        << "|" << setw(20) << left << "Petal Length" 
        << "|" << setw(20) << left << "Petal Width" 
        << "|" << endl;
    cout << string(100, '-') << endl;

    // each species
    cout <<  "|" << setw(10) << "setosa" 
        << "|" << setw(20) << setosa.stats_sL.min << "," << setosa.stats_sL.max << "," 
                           << setosa.stats_sL.mean << ","<< setosa.stats_sL.stdev  
        << "|" << setw(20) << setosa.stats_sW.min << "," << setosa.stats_sW.max << "," 
                           << setosa.stats_sW.mean << ","<< setosa.stats_sW.stdev  
        << "|" << setw(20) << setosa.stats_pL.min << "," << setosa.stats_pL.max << "," 
                           << setosa.stats_pL.mean << ","<< setosa.stats_pL.stdev 
        << "|" << setw(20) << setosa.stats_pW.min << "," << setosa.stats_pW.max << "," 
                           << setosa.stats_pW.mean << ","<< setosa.stats_pW.stdev  
        << "|" << endl;
    
    // not done


    return 0;

}


// helper functions

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

string statsSTRING(){

}


Stats findGenStats(const vector<double>& vec){
    Stats temp;
    temp.min = findMIN(vec);
    temp.max = findMAX(vec);
    temp.mean = findMEAN(vec);
    temp.stdev = findSD(vec);
    return temp;
}


void findIrisStats(Flower& iristype) {
    iristype.stats_sL = findGenStats(iristype.sL);
    iristype.stats_sW = findGenStats(iristype.sW);
    iristype.stats_pL = findGenStats(iristype.pL);
    iristype.stats_pW = findGenStats(iristype.pW);
}

void inputStats(Flower& flower, double temp_sL, double temp_sW, double temp_pL, double temp_pW){
    flower.sL.push_back(temp_sL);
    flower.sW.push_back(temp_sW);
    flower.pL.push_back(temp_pL);
    flower.pW.push_back(temp_pW);
}