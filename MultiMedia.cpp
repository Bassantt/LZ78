#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <unordered_map>

using namespace std;

void encode()
{
    ifstream inFile;
    inFile.open("enwik8"); //open the input file
    stringstream strStream;
    strStream << inFile.rdbuf();  //read the file
    string str = strStream.str(); //str holds the content of the file
    //////////////
    ofstream outencoded("encode", ios::binary);

    // //encode//
    unordered_map<string, int> dic;
    int lengthofdic = 1;
    //
    int i = 0;
    int next = 1;
    string current = string(1, str[0]);

    while (i < str.length())
    {
        string currentplusnext = current;
        currentplusnext.push_back(str[next]);
        auto result = dic.find(currentplusnext);
        if (result == dic.end())
        {
            if (dic.empty())
            {
                int zero = 0;
                outencoded.write(reinterpret_cast<char *>(&zero), 3);
                outencoded.write(reinterpret_cast<char *>(&current[0]), 1);
                dic.insert({current, lengthofdic});
            }
            else
            {
                string sub1 = currentplusnext.substr(0, currentplusnext.length() - 1);
                auto result = dic.find(sub1);
                if (result != dic.end())
                {
                    outencoded.write(reinterpret_cast<char *>(&result->second), 3);
                    outencoded.write(reinterpret_cast<char *>(&currentplusnext[currentplusnext.length() - 1]), 1);
                    dic.insert({currentplusnext, lengthofdic});
                    next++;
                }
                else
                {
                    int zero = 0;
                    outencoded.write(reinterpret_cast<char *>(&zero), 3);
                    outencoded.write(reinterpret_cast<char *>(&current[0]), 1);
                    dic.insert({current, lengthofdic});
                }
            }

            lengthofdic++;
            current = string(1, str[next]);
            i = next;
            next++;
        }
        else
        {
            current = currentplusnext;
            i = next;
            next++;
        }
    }
    outencoded.close();
}
void decode()
{
    ifstream readfiletodecode;
    ofstream decodedfile;

    decodedfile.open("decodedfile");
    readfiletodecode.open("encode", ios::in | ios::binary);
    readfiletodecode.seekg(0, ios::end);
    int size = (int)readfiletodecode.tellg();
    readfiletodecode.seekg(0, ios::beg);

    //////////////////////////////////////////////////////////////////////////////
    unordered_map<int, string> decodedic;
    int lengthofdic = 1;
    int reading;
    char charapppend;

    while (readfiletodecode.tellg() < size)
    {
        readfiletodecode.read(reinterpret_cast<char *>(&reading), 3);
        readfiletodecode.read(reinterpret_cast<char *>(&charapppend), 1);

        if (reading == 0)
        {
            decodedic.insert({lengthofdic, string(1, charapppend)});
            lengthofdic++;
            decodedfile.put(charapppend);
        }
        else
        {
            auto result = decodedic.find(reading);
            string entary = result->second;
            entary.push_back(charapppend);
            decodedic.insert({lengthofdic, entary});
            lengthofdic++;
            for (int i = 0; i < entary.length(); i++)
            {
                decodedfile.put(entary[i]);
            }
        }
    }
    //////////////////////////////////////////////////////////////////
    readfiletodecode.close();
    decodedfile.close();
}
int main()
{ //encode
    encode();
    //decode
    decode();

    return 0;
}