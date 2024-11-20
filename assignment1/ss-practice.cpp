#include <iostream>
#include <string>
#include <sstream>

using namespace std;

int main(int argc, char* argv[])
{
    string input;
    cin >> input;
    stringstream ss(input);

    string token;

    int i=1;
    while( ss>> token){
        cout << "Token" << i++ << ":" << token << endl;
    }
    
}