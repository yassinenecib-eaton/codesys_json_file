// Project1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

void headerJson()
{
    std::cout << "{" << std::endl;
    std::cout << "  \"points\":[" << std::endl;
}

void footerJson()
{
    std::cout << " ]" << std::endl;
    std::cout << "}" << std::endl;
}


// for string delimiter
std::vector<std::string> split(const std::string& s, char delim) 
{
    std::vector<std::string> result;
    std::stringstream ss(s);
    std::string item;

    while (getline(ss, item, delim)) {
        result.push_back(item);
    }

    return result;
}

int main()
{
    std::string line;
    std::ifstream myfile("example.txt");

    if (myfile.is_open())
    {
        headerJson();

        while (std::getline(myfile, line))
        // std::getline(myfile, line);
        {
            std::cout << "  {" << std::endl;
            if (line == "")
            {
                std::cout << "empty line" << std::endl;
            }
            else
            {
                int i = 0;
               // std::cout << line << std::endl;
                std::vector<std::string> word = split(line, ' ');

                int l = word.size()-1;
                std::cout << "  \"name\":" + word[0] + "," << std::endl;

                std::cout << "  \"addr\":" + word[l] + "," << std::endl;
              
                if (word[l - 2].compare("16")==0)
                {
                    std::cout << "  \"size\": 1" << "," << std::endl;
                }

                else if (word[l - 2].compare("32")==0)
                {
                    std::cout << "  \"size\": 2" << "," << std::endl;
                }

                if (word[1].compare("Signed")==0)
                {
                    std::cout << "  \"type\": 1" << "," << std::endl;
                }
                else if (word[1].compare("Unsigned")==0)
                {
                    std::cout << "  \"type\": 2" << "," << std::endl;
                }
                std::cout << "  }\n" << std::endl;
            }
        }
        footerJson();
    }

    else
    {
        perror("The following error occurred");
    }
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
