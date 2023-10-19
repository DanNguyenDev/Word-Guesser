/*
 gserver.cpp

 This program will act as a server for the client code and use a child pipe to read/write
 data. If any arguments are missing, or if the pipes fail to open, etc than throw a domain
 error. Program will read in the word/data and "store" the data in an array.
 
 */

// Include in the statements to receive input and output

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

// Include in these statements to handle pipe read/write functions

#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>


// Include in these statements to use vector, numeric, and algorithmic functions
#include <vector>
#include <iterator>
#include <numeric>
#include <algorithm>

#include "LineInfo.h"

using namespace std;


// Create the global constants READ_MAX_LEN, OK, CHILD_PID, etc
const int READ_MAX_LEN = 100;
const int OK = 0;
const int CHILD_PID = 0;
const int FORK_ERROR = 0;

int main(int argc, char* argv[]) {
    
    try {
        
        // If the argument is not equal to 2 than display that the arguments is missing
        if(argc != 2)
            throw domain_error(LineInfo("Argument missing ", __FILE__, __LINE__));
        
        
        // Create a vector of string type
        vector<string> vectorStrings;
        
        // Open the file and read in the value
        ifstream FileStringStream(argv[1]);
        
        // If the file fails to open than display that there was an error opening the file
        if(FileStringStream.fail()) {
            stringstream s;
            s << "Error opening file Random Numbers File " << argv[1] << endl;
            throw domain_error(LineInfo(s.str(), __FILE__, __LINE__));
        }
        
        // Send the file name to the istream iterator
        istream_iterator<string> inputIt(FileStringStream);
        
        // Copy over the values from the file
        copy(inputIt, istream_iterator<string>(), back_inserter(vectorStrings));
        
        // Create variables of int and unsigned long type to store vector string size and the
        // current client count
        unsigned long NoOfElements = vectorStrings.size();
        static int clientcount = 0;
        
        // Assign the string value with the file string
        string srd_cwr_req_np_str = "./srd_cwr_req_np";
        mkfifo(srd_cwr_req_np_str.c_str(), 0600);
        
        
        // Open the s-read string
        int srd_cwr_req_np_fd = open(srd_cwr_req_np_str.c_str(), O_RDONLY);
        
        // If it is less than OK (0) than display that it failed to open
        if(srd_cwr_req_np_fd < OK)
            throw domain_error(LineInfo("open FAILURE", __FILE__, __LINE__));
        
        char swr_crd_np_ary[READ_MAX_LEN] = {0};
        
        // If it is less than OK (0) than display that it failed to open
        if(read(srd_cwr_req_np_fd, swr_crd_np_ary, READ_MAX_LEN) < OK)
            throw domain_error(LineInfo("read FAILURE", __FILE__,__LINE__));
        
        // Create a string with the s- write array inside the argument
        string swr_crd_np_str(swr_crd_np_ary);
        
        // Close and unlink any strings and int variables
        close(srd_cwr_req_np_fd);
        unlink(srd_cwr_req_np_str.c_str());
        
        // Make a fifo and asign the s- write and 0600 as the arguments
        mkfifo(swr_crd_np_str.c_str(), 0600);
        
        int swr_crd_np_fd = open(swr_crd_np_str.c_str(), O_WRONLY);
        if(swr_crd_np_fd < OK)
            throw domain_error(LineInfo("open FAILURE", __FILE__, __LINE__));
        
        time_t t;
        srand((unsigned)time(&t));
        int randomIndexChoice = (rand() % NoOfElements);
        string randomword_str = vectorStrings[randomIndexChoice];
        string guessword_str(randomword_str.length(), '-');
        
        // Increment the client count
        clientcount++;
        
        // Get the fork number by assigning it to forkpid
        pid_t forkpid = fork();
        
        
        // If it is less than 0 than display that the fork failed and throw a domain error
        if(forkpid < 0) {
            stringstream s;
            s << "fork failed" << endl;
            throw domain_error(LineInfo(s.str(), __FILE__, __LINE__));
        }
        
        // If the forkpid is equal to the CHILD_PID
        // than display out the stringstream
        if(forkpid == CHILD_PID) {
            stringstream ss;
            ss << clientcount;
            string str = ss.str();
            string clientcount_str = ss.str();
            
            // If it is less than OK than throw a domain error and display that the
            // write failure failed
            if(write(swr_crd_np_fd, clientcount_str.c_str(), clientcount_str.size() + 1) < OK)
                throw domain_error(LineInfo("write FAILURE", __FILE__, __LINE__));
            
            // Have it sleep for seconds
            sleep(3);
            
            // If the write is less than ok than throw a domain error
            if(write(swr_crd_np_fd, randomword_str.c_str(), (randomword_str.size() + 1)) < OK)
                throw domain_error(LineInfo("write FAILURE", __FILE__, __LINE__));
            
            
            string srd_cwr_np_str = "./srd_cwr_np-";
            
            // Assign i with the getpid
            int i = getpid();
            string s;
            stringstream out;
            out << i;
            s = out.str();
            srd_cwr_np_str += s;
            
            // If the write is less than OK than throw a domain error
            if(write(swr_crd_np_fd, srd_cwr_np_str.c_str(), srd_cwr_np_str.size() + 1) < OK)
                throw domain_error(LineInfo("write FAILURE", __FILE__, __LINE__));
            
            // Create a mkfifo
            mkfifo(srd_cwr_np_str.c_str(), 0600);
            
            // Assign the int value with the s-read and read only in the arguments
            int srd_cwr_np_fd = open(srd_cwr_np_str.c_str(), O_RDONLY);
            
            // If the s-read is less than ok display that the open failed to open
            if(srd_cwr_np_fd < OK)
                throw domain_error(LineInfo("open FAILURE", __FILE__, __LINE__));
            
            
            // Create a do while loop that will read in the values and assign it to the values
            do {
                
                // If the write is less than OK than throw a domain error
                if(write(swr_crd_np_fd, guessword_str.c_str(), (guessword_str.size() + 1)) < OK)
                    throw domain_error(LineInfo("write FAILURE", __FILE__, __LINE__));
                
                // If the read is less than OK than throw a domain error
                char guessletter_ary[READ_MAX_LEN] = {0};
                if(read(srd_cwr_np_fd, guessletter_ary, READ_MAX_LEN) < OK)
                    throw domain_error(LineInfo("read FAILURE", __FILE__, __LINE__));
                
                // Create a for loop that will assign the values
                for(int i = 0; i < randomword_str.length(); i++)
                    if(randomword_str[i] == guessletter_ary[0])
                        guessword_str[i] = guessletter_ary[0];
              
                // While it is true
            } while(true);
            
           
        }
        
        // Exit with the success line
        exit(EXIT_SUCCESS);
        
    }
    
    // If there is an exception than catch and display a message to enter the key once or twice
    catch(exception& e) {
    cout << e.what() << endl << endl;
    

    cout << "Press the enter key once or twice to leave..." << endl << endl;
    

    exit(EXIT_FAILURE);
}
    
    exit(EXIT_SUCCESS);
    
}

