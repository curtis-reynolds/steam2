#ifndef FILE_WRITER_H
#define FILE_WRITER_H

#include <string>
#include <iostream>
#include <fstream>
#include <iomanip>
#include "User.h"

class FileWriter
{
public:
    // Constructor that takes a filename as a parameter
    FileWriter(const std::string &filename) : filename(filename){};

    // Function to open the file in append mode and return whether the operation was successful
    bool openFile()
    {
        // Attempt to open the file in append mode
        fileStream.open(filename, std::ios::app);
        return fileStream.is_open();
    }

    // Function to close the file if it is open
    void closeFile()
    {
        if (fileStream.is_open())
        {
            fileStream.close();
        }
    }

    // Function to remove the existing END line from the file
    void removeEndLine(int length)
    {
        if (fileStream.is_open())
        {
            // Close the file
            fileStream.close();

            // Reopen the file in read mode
            std::ifstream inFile(filename);
            if (!inFile.is_open())
            {
                std::cerr << "Error: Unable to open the file for reading." << std::endl;
                return;
            }

            // Check if file is empty then skip removing END
            if (inFile.peek() == std::ifstream::traits_type::eof())
            {
                inFile.close();
                // Reopen the file in append mode
                fileStream.open(filename, std::ios::app);
                return;
            }

            // Create a temporary file to store all lines except END line
            std::ofstream tempFile("temp.txt");
            if (!tempFile.is_open())
            {
                std::cerr << "Error: Unable to create temporary file." << std::endl;
                inFile.close(); // Close the input file
                return;
            }

            std::string line;
            std::string endLine = "END";
            endLine.resize(length, '_');

            // Read and store all lines except the last two
            while (std::getline(inFile, line))
            {
                if (line != endLine)
                {
                    tempFile << line << "\n";
                }
            }

            // Close the input file and temporary file
            inFile.close();
            tempFile.close();

            // Remove the original file
            std::remove(filename.c_str());

            // Rename the temporary file to the original file
            std::rename("temp.txt", filename.c_str());

            // Reopen the file in append mode
            fileStream.open(filename, std::ios::app);
        }
        else
        {
            std::cerr << "Error: Unable to write to the file. Make sure it is open.writeend" << std::endl;
        }
    }

    // Function to add the END line back to the file
    void writeEndLine(int length)
    {
        if (fileStream.is_open())
        {
            // Close the file
            fileStream.close();

            // Reopen the file in append mode
            fileStream.open(filename, std::ios::app);

            std::string endLine = "END";
            endLine.resize(length, '_');

            // Write END line at the end of file
            fileStream << std::setw(length) << std::left << endLine
                       << "\n";

            // Reopen the file in append mode
            fileStream.open(filename, std::ios::app);
        }
        else
        {
            std::cerr << "Error: Unable to write to the file. Make sure it is open.removeend" << std::endl;
        }
    }

protected:
    // Member variable to store the filename
    std::string filename;

    // Output file stream object to handle file operations
    std::ofstream fileStream;
};

#endif
