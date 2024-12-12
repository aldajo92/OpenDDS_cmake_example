#include "Subscriber.h"
#include <exception> // For std::exception
#include <iostream>  // For std::cerr and std::endl

int main(int argc, char *argv[])
{
    try
    {
        run_subscriber();
    }
    catch (const std::exception &e)
    {
        // Log the exception message to stderr
        std::cerr << "Exception: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
