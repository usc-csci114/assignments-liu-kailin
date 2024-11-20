#include <iostream>
#include <cmath>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

using namespace std;

int main(){

    vector<string> iris = {"Iris-setosa","Iris-versicolor","Iris-virginica"};

    string setosa = "Iris-setosa";
    vector<double> s_sL, s_sW, s_pL, s_pW; 
    double s_min, s_max, s_mean, s_std;
    

    string versicolor = "Iris-versicolor";
    vector<double> v_sL, v_sW, v_pL, v_pW; 
    double ve_min, ve_max, ve_mean, ve_std;

    string virginica = "Iris-virginica";
    vector<double> vi_sL, vi_sW, vi_pL, vi_pW; 
    double vi_min, vi_max, vi_mean, vi_std;


    ifstream ifile("iris.txt");

    if(ifile.fail()){
        cout << "Couldn't return file" << endl;
        return 1;
    }

    string line;
    double s_length, s_width, p_length, p_width;
    string species; 
    while(getline(ifile, line)){
        stringstream ss(line); // for parsing the line

        // get information off the line
        getline(ss, s_length, ',');
        getline(ss, s_width, ',');
        getline(ss, p_length, ',');
        getline(ss, p_width, ',');
        getline(ss, species, ',');

        //depending on which species, add information to the vector
        if(species==setosa){
            s_sL.pushback(s_length);
            s_sW.pushback(s_width);
            s_pL.pushback(p_length);
            s_pW.pushback(p_width);
        }
        else if(species==versicolor){
            ve_sL.pushback(s_length);
            ve_sW.pushback(s_width);
            ve_pL.pushback(p_length);
            ve_pW.pushback(p_width);
        }
        else if(species==virginica){
            vi_sL.pushback(s_length);
            vi_sW.pushback(s_width);
            vi_pL.pushback(p_length);
            vi_pW.pushback(p_width);
        }   
    }

    ifile.close();


    return 0;

}

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
    double mean = findMean(vec);
    double diff_sums=0;
    for (int i=0; i<vec.size(); i++){
        double diff = vec[i]-mean;
        diff_sums += diff*diff;
    }
    return sqrt(diff_sums/vec.size());
}

vector<double> makeVECTOR(){

}
