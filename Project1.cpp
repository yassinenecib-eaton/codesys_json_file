// codesysReadHandleJson.cpp : Defines the entry point for the application.
//

using namespace std;

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>

//Globa variable
std::string line;
std::ifstream myfile("test.tsv");

void headerJsonPt(std::ofstream& myfile)
{
    //std::cout << "{" << std::endl;
    //myfile << "{" << std::endl;
    std::cout << "  \"points\": [" << std::endl;
    myfile << "  \"points\": [" << std::endl;
}

void headerJsonSeq(std::ofstream& myfile)
{
    std::cout << "{" << std::endl;
    myfile << "{" << std::endl;
    std::cout << "\"seq\":[" << std::endl;
    myfile << "\"seq\":[" << std::endl;
}

void footerJson(std::ofstream& myfile)
{
    std::cout << "  }" << std::endl;
    myfile << "  }" << std::endl;
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

    while (getline(ss, item, delim)) 
    {
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
    return -1;
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

    catch (std::string error)
    {
        std::cout << "Type value out of range: " + error << std::endl;
    }
    return -1;
}

void sortNumbers(std::vector <std::string> a_vector, std::vector<int>& a_vectorInteger)
{
    for (int i = 0; i < a_vector.size(); i++)
    {
        a_vectorInteger.push_back(std::stoi(a_vector[i]));
    }
    std::sort(a_vectorInteger.begin(), a_vectorInteger.end());
}

void buildJson(int i, int pos, vector<int> a_vectoSorted, vector<std::string> a_vector, vector<int> listSizeInteger, std::ofstream& a_Jsonfile)
{
    int k = 0;
    int j = i - pos;
    int startAddr = a_vectoSorted[j];
    int sumSize = 0;

    //Search the size corresponding size in the vector not sorted
    for (j = i - pos; j <= i + 1; j++)
    {
        for (k = 0; k < a_vectoSorted.size(); k++)
        {
            if (stoi(a_vector[k]) == a_vectoSorted[j])
            {
                break;
            }
        }
        sumSize += listSizeInteger[k];
    }

    //We display the first address/size of the numerical sequence of a node
    std::cout << "   \"addr\":" << startAddr << "," << std::endl;
    a_Jsonfile << "   \"addr\":" << startAddr << "," << std::endl;
    std::cout << "   \"size\":" << sumSize << std::endl;
    a_Jsonfile << "   \"size\":" << sumSize << std::endl;
}

void handleComma(bool* a_comma, std::ofstream& a_Jsonfile, bool seqOrNot)
{
    if (seqOrNot)
    {
        if (*a_comma)
        {
            std::cout << "  }," << std::endl;
            a_Jsonfile << "  }," << std::endl;
            *a_comma = false;
        }
    }
    else
    {
        if (*a_comma)
        {
            std::cout << "," << std::endl;
            a_Jsonfile << "," << std::endl;
            std::cout << std::endl;
            a_Jsonfile << std::endl;
        }
    }
}
unsigned int searchSequence(std::vector <std::string> a_vector, std::vector<int> listTypeInteger, std::ofstream& a_Jsonfile)
{
    int pos = 0;
    bool addComma = false;
    vector <int> a_vectorSorted;

    //Sort address from the smaller to the bigger
    sortNumbers(a_vector, a_vectorSorted);
    
    //Look for a numerical sequence between 2 numbers
    unsigned int step = a_vectorSorted[1] - a_vectorSorted[0];
   // std::cout << "\"seq\": [" << std::endl;

    //Search this squence through the vector
    for (int i = 0; i < a_vector.size(); i++)
    {
        if (a_vectorSorted[i + 1] + step == a_vectorSorted[i + 2])
        {
            //As long as we find a sequence we increment position
            pos++;
        }

        else
        {
            if (pos > 0)
            {
                //Dot not add comma at the last of the js node {},
                handleComma(&addComma, a_Jsonfile, false);
               
                std::cout << "  {" << std::endl;
                a_Jsonfile << "  {" << std::endl;
                buildJson(i, pos, a_vectorSorted, a_vector, listTypeInteger, a_Jsonfile);
                pos = 0;
                std::cout << "  }";
                a_Jsonfile << "  }";
                addComma = true;
            }
            //Look for a new numerical sequence
            if (i + 2 < a_vector.size())
            {
                step = a_vectorSorted[i + 2] - a_vectorSorted[i + 1];
            }
        }
    }
    std::cout << "\n]," << std::endl;
    a_Jsonfile << "\n]," << std::endl;
    return 0;
}


int main()
{
    std::vector<std::string> listAddr, listSize;
    std::vector<int> listSizeInteger;
    bool l_addComma = false;

    if (myfile.is_open())
    {
        //Create output file
        std::ofstream myJsonfile;
        myJsonfile.open("resultFile.json");

        //Display sequence
        while (std::getline(myfile, line))
        {
            std::vector<std::string> sequenceVector = split(line, '\t');
            size_t l = sequenceVector.size() - 1;
            listAddr.push_back(sequenceVector[l - 1]);
            listSizeInteger.push_back(defineSize(sequenceVector[l - 2]));
        }

        headerJsonSeq(myJsonfile);
        searchSequence(listAddr, listSizeInteger, myJsonfile);
        headerJsonPt(myJsonfile);


        myfile.clear();
        myfile.seekg(0);

        while (std::getline(myfile, line))
        {
            handleComma(&l_addComma, myJsonfile, true);

            std::cout << "  {" << std::endl;
            myJsonfile << "  {" << std::endl;
            if (line == "")
            {
                std::cout << "empty line" << std::endl;
            }
            else
            {
                std::vector<std::string> word = split(line, '\t');
                size_t l = word.size() - 1;

                
                //First field is "name"
                std::cout << "  \"name\":\"" + word[0] + "\"," << std::endl;
                myJsonfile << "  \"name\":\"" + word[0] + "\"," << std::endl;

                //Second field is "addr"
                std::cout << "  \"addr\":" + word[l - 1] + "," << std::endl;
                myJsonfile << "  \"addr\":" + word[l - 1] + "," << std::endl;

                //Third field is "size"
                unsigned int  a_size = defineSize(word[l - 2]);
                std::cout << "  \"size\":" << a_size << "," << std::endl;
                myJsonfile << "  \"size\":" << a_size << "," << std::endl;

                //Fourth field is "type"
                unsigned int a_type = defineType(word[1]);
                std::cout << "  \"type\":" << a_type << "," << std::endl;
                myJsonfile << "  \"type\":" << a_type << "," << std::endl;

                //Last field is "value"
                std::cout << "  \"value\":" + word[l] << std::endl;
                myJsonfile << "  \"value\":" + word[l] << std::endl;

               // std::cout << "  }\n" ;
                //myJsonfile << "  }\n" ;
                l_addComma = true;
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
