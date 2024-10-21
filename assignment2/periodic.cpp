#include <iostream>
#include <string>
#include <utility>

#include<fstream>
#include<sstream>
#include<vector>

#include <utility>
#include <cctype>

using namespace std;

struct Element {
	Element() :
	symbol(),name(),number(0),atomic_mass(0.0),type() {};
	
	string symbol; //e.g "H", "He"
	string name; //e.g "Hydrogen", "Helium"
	int number; //e.g 1, 2
	double atomic_mass; //e.g. 1.008, 4.0026
	string phase; //phase at room temp e.g gas, liquid, solid
	string type; //e.g. "metalliod, "transitional metals"
};

struct compclass {
  bool operator() ( pair<string, int> a, pair<string, int> b) { return (a.first < b.first);}
} comp;

//helper functions
size_t index_find(vector< pair<string, int> > index, string key) {
	int start = 0, end = index.size() - 1;

    while (start <= end) {
        int middle = start + (end - start) / 2;

		// check if key matches .first (symbol or name)
        if (index[middle].first == key) {
            return index[middle].second;  // Return the atomic number (the index in the elements vector)
        }
		// if key is larger than .first (treat like index), search in upper half
        else if (index[middle].first < key) {
            start = middle + 1;
        } 
		// if key is smaller, search in lower half;
		else {
            end = middle - 1;
        }
    }

    return -1;  // Return -1 if element not found
}


// display element info
void printData(const Element& found) {
    cout << "Element #" << found.number << " = " << found.name << " (" << found.symbol << ")" << endl;
    cout << "Atomic Mass = " << found.atomic_mass << endl;
    cout << "Phase = " << found.phase << endl;
    cout << "Type = " << found.type << endl;
}

int main(int argc, char* argv[])
{

	// CSV file intake portion 

	ifstream ifile("elements.csv"); // read in file
	vector<Element> elements(119);	// elements vector 
	vector< pair<string, int> > name_index; // pair by name
	vector< pair<string, int> > symbol_index; // pair by symbol

	if(ifile.fail()){
		cout << "Error reading file" << endl;
		return 1;
	}

	string line;
	getline(ifile, line); // skips header
	
	while (getline(ifile,line)){ // read in info
		stringstream ss(line);
        Element element;

        ss >> element.number;
		ss.ignore();  
		ss >> element.name;
		ss.ignore(); 
		ss >> element.symbol;
		ss.ignore();  
		ss >> element.atomic_mass;
		ss.ignore(); 
		ss >> element.phase;
		//last one needs to be read in using getline bc could have spaces
		string info;
		if (getline(ss, info, ',')) element.type = info;

        //add information to corresponding vectors
		elements[element.number] = element; // match index to number
		name_index.push_back(make_pair(element.name, element.number));
        symbol_index.push_back(make_pair(element.symbol, element.number));
	}
	ifile.close();

	// sort using the given function
	sort(name_index.begin(), name_index.end(), comp);
    sort(symbol_index.begin(), symbol_index.end(), comp);



	// user interaction portion 


	string user_input;

	cout << "Search by entering the atomic number (ex. 2), symbol (ex. He), or element name (ex. Helium)" << endl;
	cout << "Type 'quit' to exit" << endl;
	cin >> user_input;

	// runs as long as user doesn't enter quit
	while(user_input != "quit"){
		
		// check if number --> display all data
		if (isdigit(user_input[0])) {
			int user_number = stoi(user_input);

			if (user_number >= 1 && user_number <= 118){
				printData(elements[user_number]);
			}
			else {
				cout << "Invalid atomic number. Number must be between 1-118." << endl;
			}
		}
		// check if input is a string length 1-2 --> symbol pair search
		else if (user_input.length() == 1 || user_input.length() == 2) {
			size_t pair_index = index_find(symbol_index, user_input);

			if (pair_index == -1){
				cout << "Element symbol not found." << endl;
			}
			else {
				printData(elements[pair_index]);
			}
		}
		// check if input is a string length > 2 --> name pair search
		else if (user_input.length() > 2) {
			size_t pair_index = index_find(symbol_index, user_input);

			if (pair_index == -1){
				cout << "Element name not found." << endl;
			}
			else {
				printData(elements[pair_index]);
			}
		}
		// other input types invalid
		else {
			cout << "Invalid input. Please try again." << endl;
		}

		cout << "Search by entering an atomic number, element symbol, or element name" << endl;
		cout << "Type 'quit' to exit" << endl;
		cin >> user_input;
	}

	return 0;

}


