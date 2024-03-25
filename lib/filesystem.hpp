#ifndef _ASYNCO_FS_
#define _ASYNCO_FS_

#include "asynco.hpp"


#include <fstream>
#include <iostream>

using namespace std;
using namespace marcelb;
using namespace asynco;

namespace marcelb {
namespace asynco {
namespace fs {

/**
 * Asynchronous file reading with callback after read complete
*/
template<typename Callback>
void read(string path, Callback&& callback) {
    async( [&path, callback] () {
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
 * Asynchronous file reading
*/
future<string> read(string path) {
    return async( [&path] () {
        string content;
        string line;
        ifstream file (path);
        if (file.is_open()) {
            line.clear();
            while ( getline (file,line) ) {
                content += line + "\n";
            }
            file.close();
            return content;
        }

        else {
            throw runtime_error("Unable to open file"); 
        }
    });
}

/**
 * Asynchronous file writing with callback after write complete
*/
template<typename Callback>
void write(string path, string content, Callback&& callback) {
    async( [&path, &content, callback] () {
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


/**
 * Asynchronous file writing with callback after write complete
*/
future<void> write(string path, string content) {
    return async( [&path, &content] () {
        ofstream file (path);
        if (file.is_open()) {
            file << content;
            file.close();
            return;
        }
        else {
            throw runtime_error("Unable to open file"); 
        }
    });
}

}
}
}

#endif