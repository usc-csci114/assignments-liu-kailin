#include <iostream>
#include <cmath>
#include <string>

using namespace std;

int main() {
    double x1, y1, x2, y2, x3, y3;
    double area=0;
    string input; 
    bool go= true;

    while (go){
        cout << "Enter the 3 points of the triangle where each x and y coordinate is separated by whitespace." << endl;
        cout << "You may enter all on one line or on separate lines." << endl;
        cout << "if you would like to terminate the program, please write 'quit' " << endl;

        //check if user wants to quit
        cin >> input;
        if (input=="quit"){
            cout << "User chose to terminate program." << endl;
            go =false;
            break;
        }
        else{
            x1= stod(input);
        }

        // clear previous errors for future loops
        cin.clear();

        //get remaining input
        cin >> y1 >> x2 >> y2 >> x3 >> y3; 

        // check if parsing is successful
        if (cin.fail()){
            cout << "Invalid input. Please try again." << endl;
        }
        else{
            area = (0.5)*abs(x1*(y2-y3) + x2*(y3-y1) + x3*(y1-y2));
            cout << "The area of the triangle is: " << area << endl;
            go = false;
            break;
        }
    }

    return 0;
}