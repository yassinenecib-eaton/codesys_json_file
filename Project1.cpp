// Project1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

void headerJson(std::ofstream& myfile)
{
    std::cout << "{" << std::endl;
    myfile << "{" << std::endl;
    std::cout << "  \"points\":[" << std::endl;
    myfile << "  \"points\":[" << std::endl;

}

void footerJson(std::ofstream& myfile)
{
    std::cout << " ]" << std::endl;
    myfile << " ]" << std::endl;
    std::cout << "}" << std::endl;
    myfile << "}" << std::endl;
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

int defineSize(std::string& aString)
{
    try
    {
        if (aString.compare("16") == 0)
        {
            return 1;
        }

        else if (aString.compare("32") == 0)
        {
            return 2;
        }

        else
        {
            throw(aString);
        }
    }

     catch (std::string error)
     {
         std::cout << "Size value out of range: " + error << std::endl;
     }
}

 int defineType(std::string& aString)
 {
     try
     {
         //Fourth field is "type"
         if (aString.find("Signed") != std::string::npos)
         {
             return 1;
         }
         else if (aString.find("Unsigned") != std::string::npos)
         {
             return 2;
         }

         else if (aString.find("Real (always 32 bits)") != std::string::npos)
         {
             return 3;
         }

         else
         {
             throw(aString);
         }
    }
    
    catch(std::string error)
    {
        std::cout << "Type value out of range: " + error << std::endl;
    }
}

int main()
{
    std::string line;
    std::ifstream myfile("example.txt");

    if (myfile.is_open())
    {
        //Create output file
        std::ofstream myJsonfile;
        myJsonfile.open("resultFile.json");

        headerJson(myJsonfile);

        while (std::getline(myfile, line))
        // std::getline(myfile, line);
        {
            std::cout << "  {" << std::endl;
            myJsonfile << "  {" << std::endl;
            if (line == "")
            {
                std::cout << "empty line" << std::endl;
            }
            else
            {
                int i = 0;
               //std::cout << line << std::endl;
                std::vector<std::string> word = split(line, '\t');
                //for (auto i : word) std::cout << i << std::endl;


                int l = word.size()-1;
                //First field is "name"
                std::cout << "  \"name\":\"" + word[0] + "\"," << std::endl;
                myJsonfile << "  \"name\":\"" + word[0] + "\"," << std::endl;

                //Second field is "addr"
                std::cout << "  \"addr\":" + word[l-1] + "," << std::endl;
                myJsonfile << "  \"addr\":" + word[l-1] + "," << std::endl;

                //Third field is "size"
                unsigned int  a_size = defineSize(word[l - 2]);
                std::cout << "  \"size\":" << a_size << "," << std::endl;
                myJsonfile << "  \"size\":" << a_size << "," << std::endl;
               

                //Fourth field is "type"
                unsigned int a_type = defineType(word[1]);
                std::cout << "  \"size\":" << a_type << "," << std::endl;
                myJsonfile << "  \"size\":" << a_type << "," << std::endl;
  
                //Last field is "value"
                std::cout << "  \"value\":" + word[l] << std::endl;
                myJsonfile << "  \"value\":" + word[l] << std::endl;

                std::cout << "  },\n" << std::endl;
                myJsonfile << "  },\n" << std::endl;
            }
        }
        footerJson(myJsonfile);
        myJsonfile.close();
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
