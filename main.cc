#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

int main(int argc, const char **argv)
{
    if (argc < 2)
    {
        cout << "Please give the path to the input csv file" << endl;
        return 1;
    }
    string cursor;
    ifstream numCount(argv[1]);
    if (!numCount)
    {
        cerr << "Error: file could not be opened at " << argv[1] << endl;
        exit(1);
    }
    ifstream fileStream(argv[1]);
    int totalValueLines = -1;
    int lines = 0;
    int fields = 1;

    while (getline(numCount, cursor))
    {
        totalValueLines++;
    }
    numCount.close();
    int inputFieldIndex, outputFieldIndex;
    int inputLinesAdded = 0;
    int outputLinesAdded = 0;
    float X[totalValueLines];
    float Y[totalValueLines];

    while (getline(fileStream, cursor))
    {
        if (lines == 0)
        {
            // Checking how much fields there are in the csv file
            for (int i = 0; i < cursor.length(); i++)
            {
                if (cursor[i] == ',')
                {
                    fields++;
                }
            }
            // Validating numbers of field
            if (fields > 1)
            {
                // adding one comma to the last of the cursor for taking the last field in the next part of code

                cursor += ',';
            }
            else
            {
                cerr << "Input file should at least have 2 fields!" << endl;
                return 1;
            }
            // Making an array of field names with the length of fields
            string fieldNames[fields];

            // Adding the field names to the field names array
            int nextFieldStartingIndex = 0;
            int fieldsAdded = 0;
            for (int i = 0; i < cursor.length(); i++)
            {
                if (cursor[i] == ',')
                {
                    fieldNames[fieldsAdded] = cursor.substr((nextFieldStartingIndex), (i - nextFieldStartingIndex));
                    fieldsAdded++;
                    nextFieldStartingIndex = (i + 1);
                }
            }

            cout << fields << " fields found..." << endl;
            cout << "Fields are: " << endl;
            for (int i = 0; i < fields; i++)
            {
                cout << fieldNames[i] << endl;
            }
            cout << endl;

            cout << "Please select your input field" << endl;
            while (true)
            {
                string inputHandler = "";
                int inputLocal = 0;
                cout << "Please give the number of your input field." << endl;
                for (int i = 0; i < fields; i++)
                {
                    cout << (i + 1) << " - " << fieldNames[i] << endl;
                }
                cout << endl
                     << "1-" << fields << "> ";
                cin >> inputHandler;
                cout << endl;
                try
                {
                    inputLocal = stoi(inputHandler);

                    if (0 < inputLocal && inputLocal <= fields)
                    {
                        inputFieldIndex = (inputLocal - 1);
                        break;
                    }
                    else
                    {
                        cerr << "Please give the number in the given range!! \n"
                             << endl;
                    }
                }
                catch (exception _)
                {
                    cerr << "Please add a number!! \n"
                         << endl;
                }
            }

            if (fields == 2)
            {
                outputFieldIndex = inputFieldIndex == 1 ? 0 : 1;
                cout << "Input File have 2 fields and you have chosen " << fieldNames[inputFieldIndex]
                     << " as the input field. So " << fieldNames[outputFieldIndex]
                     << " is taken as the output field..." << endl;
            }
            else
            {
                string remainingFields[fields - 1];
                int index = 0;
                for (int i = 0; i < fields; i++)
                {
                    if (i != inputFieldIndex)
                    {
                        remainingFields[index] = fieldNames[i];
                        index++;
                    }
                }
                while (true)
                {

                    string inputHandler = "";
                    int inputLocal = 0;
                    cout << "Please give the number of your output field." << endl;
                    for (int i = 0; i < index; i++)
                    {
                        cout << (i + 1) << " - " << remainingFields[i] << endl;
                    }
                    cout << endl
                         << "1-" << index << "> ";
                    cin >> inputHandler;
                    cout << endl;
                    try
                    {
                        inputLocal = stoi(inputHandler);
                        if (0 < inputLocal && inputLocal <= (fields - 1))
                        {
                            outputFieldIndex = inputLocal > inputFieldIndex ? inputLocal : (inputLocal - 1);
                            break;
                        }
                        else
                        {
                            cerr << "Please give the number in the given range!! \n"
                                 << endl;
                        }
                    }
                    catch (exception _)
                    {
                        cerr << "Please add a number!! \n"
                             << endl;
                    }
                }
            }
            cout << "Input field is " << fieldNames[inputFieldIndex] << "." << endl;
            cout << "Output field is " << fieldNames[outputFieldIndex] << "." << endl;
            lines++;
        }
        else
        {
            if (lines <= totalValueLines && cursor.length() > 2)
            {
                string line = cursor + ',';
                int fieldIndex = -1;
                int nextFieldStartIndex = 0;
                for (int i = 0; i < line.length(); i++)
                {
                    if (line[i] == ',')
                    {
                        fieldIndex++;
                        try
                        {
                            if (fieldIndex == inputFieldIndex)
                            {
                                X[(lines - 1)] = stof(line.substr(nextFieldStartIndex, (i - nextFieldStartIndex)));
                            }
                            else if (fieldIndex == outputFieldIndex)
                            {
                                Y[(lines - 1)] = stof(line.substr(nextFieldStartIndex, (i - nextFieldStartIndex)));
                            }
                        }
                        catch (exception _)
                        {
                            cout << "Found non-number value in input or output field skipping it.." << endl;
                        }
                        nextFieldStartIndex = i + 1;
                    }
                }
                lines++;
            }
        }
    }

    fileStream.close();

    cout << "Started linear regression calculations... \n"
         << endl;

    double avgOfXY = 0;
    for (int i = 0; i < totalValueLines; i++)
    {
        avgOfXY += (X[i] * Y[i]);
    }
    avgOfXY /= totalValueLines;

    double avgOfX = 0;
    for (int i = 0; i < totalValueLines; i++)
    {
        avgOfX += X[i];
    }
    avgOfX /= totalValueLines;

    double avgOfY = 0;
    for (int i = 0; i < totalValueLines; i++)
    {
        avgOfY += Y[i];
    }

    avgOfY /= totalValueLines;

    double squareOfAvgOfX = avgOfX * avgOfX;
    double avgOfSquaresofX = 0;
    for (int i = 0; i < totalValueLines; i++)
    {
        avgOfSquaresofX += (X[i] * X[i]);
    }
    avgOfSquaresofX /= totalValueLines;

    float m = ((avgOfX * avgOfY) - avgOfXY) / (squareOfAvgOfX - avgOfSquaresofX);
    float c = (avgOfY - (m * avgOfX));

    while (true)
    {
        cout << "Give the value of dependant variable to get the prediction.. Press 'q' to quit" << endl;
        cout << "Dependant Variable> ";
        string input;
        cin >> input;
        cout << endl;
        if (input == "q")
        {
            break;
        }
        try
        {
            float x = stof(input);

            float y = ((m * x) + c);

            cout << "Result: " << y << endl;
        }
        catch (exception _)
        {
            cerr << "Please give a numeric value" << endl;
        }
    }

    return 0;
}
