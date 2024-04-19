
#include <iostream>
#include <math.h>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
using namespace std;

int predictionCount = 0;
int mispredictions;
int predictions;
int bitCount;
int counterValue;
int initCounterValue;
int totalValueCount;
int hybridGshareTableSize;
int hybridBimodalTableSize;
// int hybridGshareTable[];
// int hybridBimodalTable[];
string hybridGBHR = "";
string fileName;
vector<string> branches;
vector<string> taken;

int fileReading(string fileName)
{
    fstream newfile;
    newfile.open(fileName, ios::in);
    if (newfile.is_open())
    {
        string tp;
        while (getline(newfile, tp))
        {
            int n = tp.length();
            branches.push_back(tp.substr(0, n - 2));
            taken.push_back(tp.substr(n - 1));
        }
    }
    return 0;
}

string convertHexToBin(string hexbin)
{
    long int i = 0;
    string s = "";
    while (hexbin[i])
    {

        switch (hexbin[i])
        {
        case '0':
            s = s + "0000";
            break;
        case '1':
            s = s + "0001";
            break;
        case '2':
            s = s + "0010";
            break;
        case '3':
            s = s + "0011";
            break;
        case '4':
            s = s + "0100";
            break;
        case '5':
            s = s + "0101";
            break;
        case '6':
            s = s + "0110";
            break;
        case '7':
            s = s + "0111";
            break;
        case '8':
            s = s + "1000";
            break;
        case '9':
            s = s + "1001";
            break;
        case 'A':
        case 'a':
            s = s + "1010";
            break;
        case 'B':
        case 'b':
            s = s + "1011";
            break;
        case 'C':
        case 'c':
            s = s + "1100";
            break;
        case 'D':
        case 'd':
            s = s + "1101";
            break;
        case 'E':
        case 'e':
            s = s + "1110";
            break;
        case 'F':
        case 'f':
            s = s + "1111";
            break;
        default:
            cout << "\nInvalid hexadecimal digit "
                 << hexbin[i];
        }
        i++;
    }
    return s;
}

int binaryToDecimal(int n)
{
    int num = n;

    // Initializing base value to 1, i.e 2^0
    int base = 1;
    int dec_value = 0;

    int temp = num;
    while (temp)
    {
        int last_digit = temp % 10;
        temp = temp / 10;

        dec_value += last_digit * base;

        base = base * 2;
    }
    // cout << dec_value << endl;
    return dec_value;
}

string retrieveIndex(string bits, int pccount)
{
    int n = bits.length();
    return bits.substr(n - pccount - 2, pccount);
}

// Smith N Bit Predictor Simulator
int smithPredictor(int bitCount, string fileName)
{
    fileReading(fileName);
    int predictions = branches.size();
    int mispredictions;
    totalValueCount = pow(2, bitCount);
    initCounterValue = pow(2, bitCount - 1);
    counterValue = initCounterValue;
    for (int i = 0; i < taken.size(); i++)
    {
        if (taken[i] == "t")
        {
            if (counterValue >= initCounterValue)
            {
                predictionCount++;
                counterValue == totalValueCount - 1 ? counterValue : counterValue++;
            }
            else
            {
                counterValue == totalValueCount - 1 ? counterValue : counterValue++;
            }
        }
        else
        {
            if (counterValue < initCounterValue)
            {
                predictionCount++;
                counterValue == 0 ? counterValue : counterValue--;
            }
            else
            {
                counterValue == 0 ? counterValue : counterValue--;
            }
        }
    }
    mispredictions = taken.size() - predictionCount;
    float mispredictionRate = (float)mispredictions/predictions;
    cout << "Number of predictions: " << predictions <<endl;
    cout << "Number of mispredictions:  " << mispredictions << endl;
    cout << "misprediction rate:  " << fixed << setprecision(2) << mispredictionRate*100 << "%"<<endl;
    cout << "FINAL COUNTER CONTENT: " << counterValue << endl;
    return predictionCount;
}

// Bimodal Predictor Simulator
int bimodalPredictor(int bitCount, string fileName)
{
    fileReading(fileName);
    int index;
    predictionCount = 0;
    totalValueCount = pow(2, bitCount);
    int bimodalArray[totalValueCount];
    for (int i = 0; i < totalValueCount; i++)
    {
        bimodalArray[i] = 4;
    }
    for (int j = 0; j < branches.size(); j++)
    {
        string pc = convertHexToBin(branches[j]);
        index = stoi(retrieveIndex(pc, bitCount), 0, 2);
        if (taken[j] == "t")
        {
            if (bimodalArray[index] >= 4)
            {
                predictionCount++;
            }
            bimodalArray[index] = (bimodalArray[index] == 7) ? bimodalArray[index] : bimodalArray[index] + 1;
        }
        else
        {
            if (bimodalArray[index] < 4)
            {
                predictionCount++;
            }
            bimodalArray[index] = (bimodalArray[index] == 0) ? bimodalArray[index] : bimodalArray[index] - 1;
        }
    }
    predictions = branches.size();
    mispredictions = taken.size() - predictionCount;
    float mispredictionRate = (float)mispredictions/predictions;
    cout << "Number of predictions: " << predictions <<endl;
    cout << "Number of mispredictions:  " << mispredictions << endl;
    cout << "misprediction rate:  " << fixed << setprecision(2) << mispredictionRate*100 << "%"<<endl;
    cout << "FINAL" << " " << "BIMODAL CONTENTS"<< endl;
    for (int i =0 ; i< totalValueCount; i++) {
        cout << i << "        "<< bimodalArray[i]<<endl;
    }

    return 0;
}

// GShare Predictor Simulator
int gsharePredictor(int pcBits, int n, string fileName)
{
    fileReading(fileName);
    totalValueCount = pow(2, pcBits);
    int gShareArray[totalValueCount];
    for (int i = 0; i < totalValueCount; i++)
    {
        gShareArray[i] = 4;
    }
    string gbhr = "";
    for (int i = 0; i < n; i++)
    {
        gbhr = gbhr + "0";
    }

    for (int j = 0; j < branches.size(); j++)
    {
        string pc = convertHexToBin(branches[j]);
        string pcAddr = pc.substr(pc.size() - (2 + pcBits), pcBits);
        string naddr = pcAddr.substr(pcAddr.size() - n, n);
        int pcAddrSize = pcAddr.size() - (naddr.size() + 2);
        string xored = "";
        // nAddr XOR gbhr
        for (int i = 0; i < naddr.size(); i++)
        {
            if (naddr[i] == gbhr[i])
                xored += "0";
            else
                xored += "1";
        }
        int index = stoi(pcAddr.substr(0, pcAddr.size() - n) + xored, 0, 2);

        // Bitwise Shift Right
        if (taken[j] == "t")
        {
            gbhr.insert(0, "1");
            gbhr = gbhr.substr(0, gbhr.size() - 1);
        }
        else
        {
            gbhr.insert(0, "0");
            gbhr = gbhr.substr(0, gbhr.size() - 1);
        }

        if (taken[j] == "t")
        {
            if (gShareArray[index] >= 4)
            {
                predictionCount++;
            }
            gShareArray[index] = (gShareArray[index] == 7) ? gShareArray[index] : gShareArray[index] + 1;
        }
        else
        {
            if (gShareArray[index] < 4)
            {
                predictionCount++;
            }
            gShareArray[index] = (gShareArray[index] == 0) ? gShareArray[index] : gShareArray[index] - 1;
        }

        // string pcAddr2 = pcAddr.substr(0,pcAddrSize);
        // string finalString = pcAddr2 + gbhr;
        // // cout << finalString << endl;
        // int index = binaryToDecimal(stoi(finalString));
        // bimodalPredictor(pcBits, fileName, true, index);
    }
    predictions = branches.size();
    mispredictions = taken.size() - predictionCount;
    float mispredictionRate = (float)mispredictions/predictions;
    cout << "Number of predictions: " << predictions <<endl;
    cout << "Number of mispredictions:  " << mispredictions << endl;
    cout << "misprediction rate:  " << fixed << setprecision(2) << mispredictionRate*100 << "%"<<endl;
    cout << "FINAL" << " " << "GSHARE CONTENTS"<< endl;
    for (int i =0 ; i< totalValueCount; i++) {
        cout << i << "        "<< gShareArray[i]<<endl;
    }
    return 0;
}

int hybridGsharePredictor(string predictorType, int hybridGshareTable[], int i, int M1, int N)
{
    string pc = convertHexToBin(branches[i]);
    string pcAddr = pc.substr(pc.size() - (2 + M1), M1);
    string naddr = pcAddr.substr(pcAddr.size() - N, N);
    int pcAddrSize = pcAddr.size() - (naddr.size() + 2);
    string xored = "";
    // nAddr XOR hybridGBHR
    for (int j = 0; j < naddr.size(); j++)
    {
        if (naddr[j] == hybridGBHR[j])
            xored += "0";
        else
            xored += "1";
    }
    int index = stoi(pcAddr.substr(0, pcAddr.size() - N) + xored, 0, 2);

    if (taken[i] == "t")
    {

        if (hybridGshareTable[index] >= 4 && predictorType == "gshare")
        {
            predictionCount++;
        }
        if (hybridGshareTable[index] >= 4)
        {
            return 1;
        }
        if (predictorType == "gshare")
        {
            if (hybridGshareTable[index] == 7)
            {
                hybridGshareTable[index] = hybridGshareTable[index];
            }
            else
            {
                hybridGshareTable[index] = hybridGshareTable[index] + 1;
            }
        }
        // hybridGshareTable[index] = (hybridGshareTable[index] == 7) ? hybridGshareTable[index] : hybridGshareTable[index] + 1;
    }
    else
    {

        if (hybridGshareTable[index] < 4 && predictorType == "gshare")
        {
            predictionCount++;
        }
        if (hybridGshareTable[index] < 4)
        {
            return 1;
        }
        if (predictorType == "gshare")
        {
            if (hybridGshareTable[index] == 0)
            {
                hybridGshareTable[index] = hybridGshareTable[index];
            }
            else
            {
                hybridGshareTable[index] = hybridGshareTable[index] - 1;
            }
        }
        //  if (predictorType == "gshare")
        //     hybridGshareTable[index] = (hybridGshareTable[index] == 0) ? hybridGshareTable[index] : hybridGshareTable[index] - 1;
    }

    return 0;
}

int hybridBimodalPredictor(string predictorType, int hybridBimodalTable[hybridGshareTableSize], int i, int M2)
{
    string pc = convertHexToBin(branches[i]);
    int index = stoi(retrieveIndex(pc, M2), 0, 2);
    if (taken[i] == "t")
    {
        if (hybridBimodalTable[index] >= 4 && predictorType == "bimodal")
        {
            predictionCount++;
        }
        if (hybridBimodalTable[index] >= 4)
        {
            return 1;
        }
        if (predictorType == "bimodal")
            hybridBimodalTable[index] = (hybridBimodalTable[index] == 7) ? hybridBimodalTable[index] : hybridBimodalTable[index] + 1;
    }
    else
    {
        if (hybridBimodalTable[index] < 4 && predictorType == "bimodal")
        {
            predictionCount++;
        }
        if (hybridBimodalTable[index] < 4)
        {
            return 1;
        }
        if (predictorType == "bimodal")
            hybridBimodalTable[index] = (hybridBimodalTable[index] == 0) ? hybridBimodalTable[index] : hybridBimodalTable[index] - 1;
    }
    return 0;
}

// Hybrid Predictor Simulator
int hybridPredictor(int K, int M1, int N, int M2, string fileName)
{
    string predictorType;
    fileReading(fileName);
    totalValueCount = pow(2, K);

    // Chooser Table Creation
    int chooserTable[totalValueCount];
    for (int i = 0; i < totalValueCount; i++)
    {
        chooserTable[i] = 1;
    }

    // GShare Table Creation
    hybridGshareTableSize = pow(2, M1);
    int hybridGshareTable[hybridGshareTableSize];
    for (int i = 0; i < hybridGshareTableSize; i++)
    {
        hybridGshareTable[i] = 4;
    }

    // Bimodal Table Creation
    hybridBimodalTableSize = pow(2, M2);
    int hybridBimodalTable[hybridBimodalTableSize];
    for (int i = 0; i < hybridBimodalTableSize; i++)
    {
        hybridBimodalTable[i] = 4;
    }

    // hybridGBHR initialization
    for (int i = 0; i < N; i++)
    {

        hybridGBHR = hybridGBHR + "0";
    }

    for (int i = 0; i < branches.size(); i++)
    {
        string pc = convertHexToBin(branches[i]);
        int index = stoi(pc.substr(pc.size() - (K + 2), K), 0, 2);
        if (chooserTable[index] >= 2)
        {
            predictorType = "gshare";
        }
        else
        {
            predictorType = "bimodal";
        }
        // int gshare = hybridGsharePredictor(predictorType, hybridGshareTable, i, M1, N);
        // int bimodal = hybridBimodalPredictor(predictorType, hybridBimodalTable, i, M2);
        int gshare = 0;
        int bimodal = 0;
        string pcAddr = pc.substr(pc.size() - (2 + M1), M1);
        string naddr = pcAddr.substr(pcAddr.size() - N, N);
        int pcAddrSize = pcAddr.size() - (naddr.size() + 2);
        string xored = "";
        // nAddr XOR hybridGBHR
        for (int j = 0; j < naddr.size(); j++)
        {
            if (naddr[j] == hybridGBHR[j])
                xored += "0";
            else
                xored += "1";
        }
        int gshareIndex = stoi(pcAddr.substr(0, pcAddr.size() - N) + xored, 0, 2);

        if (taken[i] == "t")
        {

            if (hybridGshareTable[gshareIndex] >= 4 && predictorType == "gshare")
                predictionCount++;
            if (hybridGshareTable[gshareIndex] >= 4)
                gshare = 1;
            if (predictorType == "gshare")
                hybridGshareTable[gshareIndex] = (hybridGshareTable[gshareIndex] == 7) ? hybridGshareTable[gshareIndex] : hybridGshareTable[gshareIndex] + 1;
        }
        else if (taken[i] == "n")
        {

            if (hybridGshareTable[gshareIndex] < 4 && predictorType == "gshare")
                predictionCount++;
            if (hybridGshareTable[gshareIndex] < 4)
                gshare = 1;
            if (predictorType == "gshare")
                hybridGshareTable[gshareIndex] = (hybridGshareTable[gshareIndex] == 0) ? hybridGshareTable[gshareIndex] : hybridGshareTable[gshareIndex] - 1;
        }
        else
            gshare = 0;

        int bimodalIndex = stoi(retrieveIndex(pc, M2), 0, 2);
        if (taken[i] == "t")
        {
            if (hybridBimodalTable[bimodalIndex] >= 4 && predictorType == "bimodal")
                predictionCount++;
            if (hybridBimodalTable[bimodalIndex] >= 4)
                bimodal = 1;
            if (predictorType == "bimodal")
                hybridBimodalTable[bimodalIndex] = (hybridBimodalTable[bimodalIndex] == 7) ? hybridBimodalTable[bimodalIndex] : hybridBimodalTable[bimodalIndex] + 1;
        }
        else if (taken[i] == "n")
        {
            if (hybridBimodalTable[bimodalIndex] < 4 && predictorType == "bimodal")
                predictionCount++;
            if (hybridBimodalTable[bimodalIndex] < 4)
                bimodal = 1;
            if (predictorType == "bimodal")
                hybridBimodalTable[bimodalIndex] = (hybridBimodalTable[bimodalIndex] == 0) ? hybridBimodalTable[bimodalIndex] : hybridBimodalTable[bimodalIndex] - 1;
        }
        else
            bimodal = 0;

        if (taken[i] == "t")
        {
            hybridGBHR.insert(0, "1");
            hybridGBHR = hybridGBHR.substr(0, hybridGBHR.size() - 1);
        }
        else
        {
            hybridGBHR.insert(0, "0");
            hybridGBHR = hybridGBHR.substr(0, hybridGBHR.size() - 1);
        }
        if (gshare > bimodal)
            chooserTable[index] = (chooserTable[index] == 3) ? chooserTable[index] : chooserTable[index] + 1;
        if (gshare < bimodal)
            chooserTable[index] = (chooserTable[index] == 0) ? chooserTable[index] : chooserTable[index] - 1;
    }
    predictions = branches.size();
    mispredictions = taken.size() - predictionCount;
    float mispredictionRate = (float)mispredictions/predictions;
    cout << "Number of predictions: " << predictions <<endl;
    cout << "Number of mispredictions:  " << mispredictions << endl;
    cout << "misprediction rate:  " << fixed << setprecision(2) << mispredictionRate*100 << "%"<<endl;
    cout << "FINAL " <<"CHOOSER TABLE CONTENTS"<< endl;
    for (int i = 0; i < totalValueCount; i++)
    {
        cout << i << "        " << chooserTable[i] << endl;
    }
    cout << "FINAL " << "GSHARE CONTENTS"<< endl;
    for (int i =0 ; i< hybridGshareTableSize; i++) {
        cout << i << "        "<< hybridGshareTable[i]<<endl;
    }
    cout << "FINAL BIMODAL CONTENTS"<< endl;
    for (int i =0 ; i< hybridBimodalTableSize; i++) {
        cout << i << "        "<< hybridBimodalTable[i]<<endl;
    }
    return 0;
}

int main(int argc, char *argv[])
{
    string branchPredictorType = argv[1];
    bitCount = atoi(argv[2]);
    fileName = argv[3];
    if (branchPredictorType == "smith")
    {
        smithPredictor(atoi(argv[2]), argv[3]);
    }
    else if (branchPredictorType == "gshare")
    {
        gsharePredictor(atoi(argv[2]), atoi(argv[3]), argv[4]);
    }
    else if (branchPredictorType == "hybrid")
    {
        hybridPredictor(atoi(argv[2]), atoi(argv[3]), atoi(argv[4]), atoi(argv[5]), argv[6]);
    }
    else if (branchPredictorType == "bimodal")
    {
        bimodalPredictor(atoi(argv[2]), argv[3]);
    }
    return 0;
}