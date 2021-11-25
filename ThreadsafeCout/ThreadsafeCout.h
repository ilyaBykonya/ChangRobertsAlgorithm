#pragma once
#include <iostream>
#include <sstream>
using namespace std;

struct ThreadsafeCout: public stringstream
{
    ~ThreadsafeCout()
    {
        cout << rdbuf();
    }
};
