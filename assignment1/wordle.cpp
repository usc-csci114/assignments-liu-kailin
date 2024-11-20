#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>


using namespace std;

// helper function
int compareGUESS(const string& guess, const vector<string>& words, size_t index){
    
    // set comparison word
    const string& actual_word = words[index];

    // set colors
    string green = "\033[92m";
	string yellow = "\033[93m";
	string coloroff = "\033[0m";

    int correct=0; // counts number of green/ same slot matches

    // Compare each character
    for (size_t i = 0; i < actual_word.length(); i++) { // loop through each character 

        bool found = false;

        if (guess[i] == actual_word[i]) { // does it match at same slot?
            cout << green << guess[i];
            found = true;
            correct++;
        }
        else {
            for (size_t j=0; j < actual_word.length(); j++){ // does it exist in the word
                if(guess[i]==actual_word[j]){
                    cout << yellow << guess[i];
                    found = true;
                    break;
                }
            }
            if(!found){ // it doesn't exist in the word
                cout << coloroff << guess[i];
            }
        }

    }
    cout << endl;

    return correct;
}



// main --------------------------------------------------------------------------------

int main(){

    //intake file
    ifstream ifile("miniwordlist.txt");

    if(ifile.fail()){
        cout << "Couldn't return file" << endl;
        return 1;
    }

    //take in each possible word
    string line;
    vector<string> words;
    while(ifile >>line){
        words.push_back(line);
    }
    ifile.close();


    // keep playing as long as there are more words to generate or user wants to continue
    
    size_t turn = 0;  // tracks number of turns
    bool again = true; // tracks if the user wants to conitnue
    
    while (turn < words.size() && again == true){
        
        // welcome statement
        cout << "Welcome to Wordle!" << endl;
        for (int i=0; i<6; i++){
            cout << "*****" << endl;
        }
        cout << "Enter your guess:" << endl;

        string guess;
        cin >> guess; 

        // check that the guess is valid
        if (guess.length() != 5){
            cout << "Guess does not have 5 characters, please try again:" << endl;
            cin >> guess;
        }

        // loop for 6 tries
        int correctGuesses=0; // store whether all characters match
        string black = "\033[0m"; // store color
        vector<string> past_guesses; // store past guesses
       
        for (int i=0; i<6; i++){
             cout << endl;
            //print out past guesses
            for (int k=0; k<i; k++){
                compareGUESS(past_guesses[k],words, turn);
            }
            // compare this guess
            correctGuesses = compareGUESS(guess, words, turn);
            // store this guess
            past_guesses.push_back(guess);
            // print out remaining empty tries
            for (int j=5-i; j>0; j--){ 
                cout << black << "*****" << endl;
            }

            // endgame
            if(correctGuesses==5){
                cout << "You have correctly guessed the wordle!" << endl;
                break;
            }
            if(correctGuesses!= 5 && i==5){
                cout << "You have reached the maximum number of turns!" << endl;
                break;
            }

            // new try
            cout << "Enter your guess" << endl;
            cin >> guess;
            if (guess.length() != 5){
                cout << "Guess does not have 5 characters, please try again:" << endl;
                cin >> guess;
            }

            

        }

        // prompt user to see if they want to continue
        string play;
        cout << "Would you like to play again? " << endl;
        cout << " Enter 'new' to start a new round or 'quit' to exit program " << endl;
        cin >> play;

        if(play=="new"){
            again=true;
        }
        else if (play=="quit"){
            again=false;
        }
        
        turn++; // increase turns
    }
    
    // exit statement if there are no more words to guess
    if(turn==words.size()){
        cout << "Maximum number of games reached. Thanks for playing!" << endl;
    }

    return 0; 
}

