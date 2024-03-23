#ifndef _ASYNCO_FS_
#define _ASYNCO_FS_

#include "asynco.hpp"


#include <fstream>
#include <iostream>

using namespace std;

namespace marcelb {

/**
 * Asynchronous file reading
*/
template<typename Callback>
void asynco_read(string path, Callback&& callback) {
    asynco( [&path, callback] () {
        string content;
        try {
            string line;
            ifstream file (path);
            if (file.is_open()) {
                line.clear();
                while ( getline (file,line) ) {
                    content += line + "\n";
                }
                file.close();
            }

            else {
                throw runtime_error("Unable to open file"); 
            }

            callback(content, nullptr);
        } catch(exception& error) {
            callback(content, &error);
        }
    });
}

/**
 * Asynchronous file writing
*/
template<typename Callback>
void asynco_write(string path, string content, Callback&& callback) {
    asynco( [&path, &content, callback] () {
        try {
            ofstream file (path);
            if (file.is_open()) {
                file << content;
                file.close();
            }
            else {
                throw runtime_error("Unable to open file"); 
            }

            callback(nullptr);
        } catch(exception& error) {
            callback(&error);
        }
    });
}

}

#endif