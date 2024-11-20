#include <iostream>
#include <vector>
# include <sstream>
#include <iomanip>
#include <cmath>
#include "system.h"

using namespace std;

int main(int argc, char* argv[])
{
    if (argc < 3) {
        cerr << "Usage: " << argv[0] << " n m [coefficients of the matrix]\n";
        return 1;
    }

    // Parse n and m and convert to integers
    size_t n = static_cast<size_t>(atoi(argv[1]));
    size_t m = static_cast<size_t>(atoi(argv[2]));

    // Check n and m
    if (n == 0 || m == 0) {
        cerr << "n and m must be greater than 0" << endl;
        return 1;
    }

    // 2D vector of size n rows and m+1 columns (the m unknowns plus the b column)
    vector<vector<double>> matrix(n, vector<double>(m + 1));

    // Ask the user to enter the system matrix row by row
    for (size_t i = 0; i < n; ++i) {
        bool validInput = false;
        while (!validInput) {
            cout << "Enter coefficients for row " << i + 1 << endl;
            cout << " Ex: " << m << " coefficients + 1 constant term " << endl;
            for (size_t j = 0; j < m + 1; ++j) {
                cin >> matrix[i][j];
            }

            // For each row, check to see if m + 1 doubles were read
            if (cin.fail()) {
                cin.clear(); // Clear flag
                cin.ignore(10, '\n'); // Discard invalid input
                cout << "Invalid input, please enter " << m << " coefficients + 1 constant term" << endl;
            } else {
                validInput = true;
            }
        }
    }

    // Instantiate the System object and call solve
    System system(n, m, matrix);
    system.solve();

    // Print the number of solutions
    int numSolutions = system.getNumSolutions();
    
    // 0 solutions
    if (numSolutions == 0) {
        cout << "System has no solutions" << endl;
    }
    // 1 solution
    else if (numSolutions == 1) {
        cout << "System has 1 unique solution" << endl;
        
        vector<double> solution = system.getSolution();
        for (size_t i = 0; i < solution.size(); ++i) {
            cout << "x" << i + 1 << " = "
                << fixed << setprecision(4) << solution[i] << "\n";
        }
    }
    // 2 solutions
    else if (numSolutions == 2) {
        cout << "System has infinitely many solutions" << endl;
        
        vector<double> solution = system.getSolution();
        for (size_t i = 0; i < solution.size(); ++i) {
            cout << "x" << i + 1 << " = "
                << fixed << setprecision(4) << solution[i] << "\n";
        }
    }

    return 0;

}
