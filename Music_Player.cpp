#include <iostream>   
#include <windows.h>     // For Windows API functions (like file handling and playing sound)
#include <vector>        // For using the vector container (dynamic array)
#include <string>      

using namespace std;    

// Function to get all .wav files from the current folder
vector<string> getWAVFiles(const string& folder) {
    vector<string> files;                          // This will store the list of .wav file paths
    string searchPath = folder + "\\*.wav";        // Set the search pattern to look for .wav files

    WIN32_FIND_DATAA findData;                     // Structure to store info about a file found
    HANDLE hFind = FindFirstFileA(searchPath.c_str(), &findData);  // Start searching for files

    if (hFind != INVALID_HANDLE_VALUE) {           // If at least one file is found
        do {
            string fullPath = folder + "\\" + findData.cFileName;  // Make the full file path
            files.push_back(fullPath);             // Add the file to the list
        } while (FindNextFileA(hFind, &findData)); // Continue finding next file
        FindClose(hFind);                          // Close the search handle when done
    }

    return files;                                  // Return the list of .wav files
}

// Function to print MCI error messages (if any)
void printMCIErr(MCIERROR err, const string& msg) {
    char errorText[256];                                           // Buffer to hold error text
    mciGetErrorStringA(err, errorText, sizeof(errorText));        // Get error message from error code
    cout << msg << " error: " << errorText << endl;               // Print the error message
}

// Function to play a selected WAV file
void playWAV(const string& filePath) {
    cout << "\nTrying to play: " << filePath << endl;             // Show which file is being played

    // Create the MCI command to open the file
    string commandOpen = "open \"" + filePath + "\" alias audio"; 
    MCIERROR err = mciSendStringA(commandOpen.c_str(), NULL, 0, NULL); // Send command to open file
    if (err != 0) {                    // If there was an error opening the file
        printMCIErr(err, "Open");      // Show the error
        return;                        // Stop the function
    }

    // Command to play the file and wait until it's done
    err = mciSendStringA("play audio wait", NULL, 0, NULL);       
    if (err != 0) {                    // If there's an error playing the file
        printMCIErr(err, "Play");      // Show the error
    }

    // Command to close the file after playing
    mciSendStringA("close audio", NULL, 0, NULL);
}

int main() {
    vector<string> wavFiles = getWAVFiles(".");    // Get all .wav files in the current directory

    if (wavFiles.empty()) {                        // If no files were found
        cout << "No WAV files found in the current folder.\n";  // Show a message
        return 0;                                   // Exit the program
    }

    // List the available .wav files
    cout << "Available WAV files:\n";
    for (size_t i = 0; i < wavFiles.size(); ++i) {
        cout << i << ": " << wavFiles[i] << endl;  // Print each file with a number
    }

    int choice;                                     // Variable to store user's choice
    cout << "\nEnter the number of the file to play: ";
    cin >> choice;                                  // Get user input

    // Check if the choice is valid
    if (choice < 0 || choice >= static_cast<int>(wavFiles.size())) {
        cout << "Invalid choice.\n";                // Show error message
        return 0;                                   // Exit program
    }

    playWAV(wavFiles[choice]);                      // Play the selected file

    cout << "\nDone! Press Enter to exit...";       
    cin.ignore();                                   // Ignore leftover newline from previous input
    cin.get();                                      // Wait for user to press Enter
    return 0;                                       // Exit program
}


// g++ play_song.cpp -std=c++14 -lwinmm -o mp3player
