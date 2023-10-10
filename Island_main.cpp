/*
Name: Raghav Sunderasan
Date: 4/2/2023
Description: Prodecrual Island Generator
Usage: Create an Island using Particle Roll Algorithm, enter values it will print a raw island, normailzed island, and polished island
*/
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <cmath>
#include <vector>
#include <fstream>
#include <iomanip>

using namespace std;


//  print island 
void printIsland(const vector<vector<char>> &island) {
    for (const auto &row : island) {
        for (char c : row) {
            cout << c;
        }
        cout << endl;
    }
}


// save island to a file (fix, does not work)
void saveIslandToFile(const vector<vector<char>> &island, const string &filename) {
    ofstream outFile(filename);


    for (const auto &row : island) {
        for (char c : row) {
            outFile << c;
        }
        outFile << endl;
    }


    outFile.close();
}


// generaate random float between 0-1
double randomFloat() {
    return static_cast<double>(rand()) / (static_cast<double>(RAND_MAX) + 1);
}


//  check if a move is valid
bool isValidMove(int x, int y, int width, int height) {
    return x >= 0 && x < width && y >= 0 && y < height;
}


// MAIN FUNCTION
int main(int argc, char *argv[]) {
    int seed = time(0);


    // command line 
    for (int i = 1; i < argc; i++) {
        if (string(argv[i]) == "-s" && i + 1 < argc) {
            seed = stoi(argv[++i]);
        }
    }


    srand(seed); // 


    // declare inputs
    int width, height;
    int centerX, centerY, radius;
    int numParticles, maxLife, waterLine;
    // user inputs
    cout << "Welcome to Raghav Sunderasan's CSE240 island generator!" << endl;
    cout << "Enter grid width: ";
    cin >> width;
    cout << "Enter grid height: ";
    cin >> height;
    cout << "Enter drop-zone x-coordinate (0 - " << width - 1 << "): ";
    cin >> centerX;
    cout << "Enter drop-zone y-coordinate (0 - " << height - 1 << "): ";
    cin >> centerY;
    cout << "Enter drop-zone radius (minimum 2): ";
    cin >> radius;
    cout << "Enter number of particles to drop: ";
    cin >> numParticles;
    cout << "Enter max life of particles: ";
    cin >> maxLife;
    cout << "Enter value for waterline (40-200): ";
    cin >> waterLine;


    // Initialize the island grid
    vector<vector<int>> values(height, vector<int>(width, 0));


    // Particle Roll Algorithm
    for (int p = 0; p < numParticles; p++) {
        int parLife = maxLife;
        double r = radius * sqrt(randomFloat());
        double theta = randomFloat() * 2 * M_PI;
        int x = static_cast<int>(centerX + r * cos(theta));
        int y = static_cast<int>(centerY + r * sin(theta));
     // fixed

        while (parLife > 0 && isValidMove(x, y, width, height)) {
            values[y][x]++;


            // Generate potential moves
            vector<pair<int, int>> moves;
            for (int dx = -1; dx <= 1; dx++) {
                for (int dy = -1; dy <= 1; dy++) {
                    if (dx != 0 || dy != 0) {
                        int newX = x + dx;
                        int newY = y + dy;
                        if (isValidMove(newX, newY, width, height) && values[newY][newX] <= values[y][x]) {
                        moves.emplace_back(newX, newY);
                    }
                }
            }
        }


        // Choose a random valid move
        if (!moves.empty()) {
            int idx = rand() % moves.size();
            x = moves[idx].first;
            y = moves[idx].second;
        } else {
            break;
        }


        parLife--;
    }
}


// Find maximum value in the grid
int maxValue = 0;
for (const auto &row : values) {
    for (int val : row) {
        maxValue = max(maxValue, val);
    }
}


// Normalize and polished island
vector<vector<char>> polishedIsland(height, vector<char>(width));
double landZone = 255 - waterLine;
for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
        int normValue = static_cast<int>(static_cast<double>(values[y][x]) / maxValue * 255);


        if (normValue < 0.5 * waterLine) {
            polishedIsland[y][x] = '#';  // Deep water 50%
        } else if (normValue <= waterLine) {
            polishedIsland[y][x] = '~';  // Shallow water 50%
        } else if (normValue < waterLine + 0.15 * landZone) {
            polishedIsland[y][x] = '.';  // Coast/beach 15%
        } else if (normValue < waterLine + 0.4 * landZone) {
            polishedIsland[y][x] = '-';  // Plains/grass 40%
        } else if (normValue < waterLine + 0.8 * landZone) {
            polishedIsland[y][x] = '*';  // Forests 80%
        } else {
            polishedIsland[y][x] = '^';  // Mountains
        }
    }
}


// Output raw island (should print the write side correctly)
cout << "Raw island:" << endl;
for (const auto &row : values) {
    for (int val : row) {
        cout << setw(3) << val << " ";
    }
    cout << endl;
}
// Output Normalized island
cout << "Normalized:" << endl;
for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
        int normValue = static_cast<int>(static_cast<double>(values[y][x]) / maxValue * 255);
        cout << setw(3) << normValue << " ";
    }
    cout << endl;
}

// Output Final island
cout << "Final Island:" << endl;
printIsland(polishedIsland);


// Save polished island to a file (fixed)
saveIslandToFile(polishedIsland, "island.txt");


return 0;
}
