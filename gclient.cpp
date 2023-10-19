/*
 gclient.cpp
 04/18/2023
 Daniel Nguyen
 The program will open up read/write pipes and will display a open failure if it is less
 than OK. Create a do while loop that will start the game and continue the game if the word
 is not guessed
 */

// Include in the input and output streams to read in input and display output
#include <string>
#include <iostream>
#include <fstream>
#include <stdint.h>

// Include in the statements to use the write/read pipes functions
#include <stdlib.h>
#include <string.h>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <fcntl.h>

#include <stdexcept>
#include <sstream>

// Include in the LineInfo.h file to catch exceptions and throw domain errors
#include "LineInfo.h"

using namespace std;

// Create a global const int for OK, READ_MAX_LEN, and the number of letter guesses
const int READ_MAX_LEN = 100;
const int NO_OF_LETTER_GUESS_MAX = 12;
const int OK = 0;

int main(int argc, char* argv[]) {

    
    // Create a try
    try {
        
        // If the argument is not equal to 1 than throw a domain error
        if (argc != 1)
            throw domain_error(LineInfo("Usage: ./gclient ", __FILE__, __LINE__));

        // Create a string variable that will hold
        string srd_cwr_req_np_str = "./srd_cwr_req_np";
        mkfifo(srd_cwr_req_np_str.c_str(), 0600);

        
        // Open the read function
        int srd_cwr_req_np_fd = open(srd_cwr_req_np_str.c_str(), O_WRONLY);
        
        // If the s - read is less than OK than display a open failure
        if (srd_cwr_req_np_fd < OK)
            throw domain_error(LineInfo("open FAILURE", __FILE__, __LINE__));

        
        // Get the pid and update the file name in swr_crd_np_str
        string swr_crd_np_str = "./srd_crd_np-";
        int i = getpid(); string s; stringstream out; out << i; s = out.str();
        swr_crd_np_str += s;
        
        // If the write function is less than OK than throw a domain error
        if (write(srd_cwr_req_np_fd, swr_crd_np_str.c_str(), swr_crd_np_str.size() + 1) < OK)
            throw domain_error(LineInfo("write FAILURE", __FILE__, __LINE__));

        // Create a fifo with a 0600 in the argument
        mkfifo(swr_crd_np_str.c_str(), 0600);

        // If the s - write is less than OK than throw a domain error
        int swr_crd_np_fd = open(swr_crd_np_str.c_str(), O_RDONLY);
        if (swr_crd_np_fd < OK)
            throw domain_error(LineInfo("open FAILURE", __FILE__, __LINE__));

        char clientno_ary[READ_MAX_LEN] = { 0 };
        
        // If the read pipe is less than OK than throw a domain error
        if (read(swr_crd_np_fd, clientno_ary, READ_MAX_LEN) < OK)
            throw domain_error(LineInfo("read FAILURE", __FILE__, __LINE__));
        string clientno_str(clientno_ary);

        // Create a char array with a READ_MAX_LEN as the argument
        char randomWordAry[READ_MAX_LEN] = { 0 };
        
        // If the read piepe is less than OK than throw a domain error
        if(read(swr_crd_np_fd, randomWordAry, READ_MAX_LEN) < OK)
            throw domain_error(LineInfo("read FAILURE", __FILE__, __LINE__));
        string randomword_str(randomWordAry);

        
        // If the read is less than OK than throw a domain error
        char srd_cwr_np_ary[READ_MAX_LEN] = { 0 };
        if (read(swr_crd_np_fd, srd_cwr_np_ary, READ_MAX_LEN) < OK)
            throw domain_error(LineInfo("read FAILURE", __FILE__, __LINE__));

        // Create a string object with the s -read array as the argument
        string srd_cwr_np_str(srd_cwr_np_ary);
        mkfifo(srd_cwr_np_str.c_str(), 0600);

        // Create an int variable and open it
        int srd_cwr_np_fd = open(srd_cwr_np_str.c_str(), O_WRONLY);
        
        // If the s - read is less than OK than throw a domain error
        if(srd_cwr_np_fd < OK)
            throw domain_error(LineInfo("read FAILURE", __FILE__, __LINE__));

        // Create a variable that will hold the number of tries
        int noOfTries = 0;

        // Display the game starting and the number of guesses the user has left
        cout << endl << endl << "Game Start" << endl << endl <<
            "You have " << NO_OF_LETTER_GUESS_MAX << " letter guesses to win" << endl << endl;

        
        // Create a do while loop that will "play the game".
        do {
            char guessWordAry[READ_MAX_LEN] = { 0 };
            
            // If the read pipe is less than OK than throw a domain error
            if(read(swr_crd_np_fd, guessWordAry, READ_MAX_LEN) < OK)
                throw domain_error(LineInfo("read FAILURE", __FILE__, __LINE__));

            // Create a string guessword and assign the guessword array as the argument
            string guessword_str(guessWordAry);

   
            // If the guess word is equal to the random word than display that the user won
            if (guessword_str == randomword_str) {
                cout << endl << guessword_str << endl;
                cout << endl << endl << "You Win!" << endl << endl;
                break;
            }

            // If the number of tries is greater than the 12 guesses than display
            // that the user is out of tries
            if (noOfTries > NO_OF_LETTER_GUESS_MAX) {
                cout << endl << endl << "Out of tries : " << noOfTries - 1 << " allowed" << endl <<
                    "The word is: " << randomword_str << endl << endl;
                break;
            }

            // If not any of those above, than display the client number and the number of tries
            cout << endl;
            cout << "Client : " << clientno_str << endl <<
                "No of tries: " << noOfTries << endl <<
                "(Guess) Enter a letter in the word : " << guessword_str << endl;
            cout << endl;

            
            // Create a blank char and cin the guess
            char letterGuess_char = ' ';
            cin >> letterGuess_char;
            string letterGuess_str = " ";
            letterGuess_str[0] = letterGuess_char;

            // If the write is less than OK than throw a domain error
            if (write(srd_cwr_np_fd, letterGuess_str.c_str(), letterGuess_str.size()+1) < OK)
                throw domain_error(LineInfo("write FAILURE", __FILE__, __LINE__));

            // Increment the number of tries
            noOfTries++;

        } while (true);

        // Close the pipes
        close(srd_cwr_req_np_fd);
        close(swr_crd_np_fd);
        close(srd_cwr_np_fd);
        
        
        // Unlink the strings and pipes
        unlink(srd_cwr_req_np_str.c_str());
        unlink(swr_crd_np_str.c_str());
        unlink(srd_cwr_np_str.c_str());
        


    }
    
    // If there is an exception than catch it and display a exit failure
    catch (exception& e) {
        cout << e.what() << endl;
        exit(EXIT_FAILURE);
    }

    // If not then exit with success
    exit(EXIT_SUCCESS);
}
