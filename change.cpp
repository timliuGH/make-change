#include <climits>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

int *change(int amt, std::vector<int> coin);

int main()
{
    // Open input file
    std::ifstream iFile("amount.txt");

    // Open output file and append contents to EOF
    std::fstream oFile("change.txt", std::fstream::out | std::fstream::app);

    // Check if file opened correctly
    if (!iFile) {
        std::cout << "Error opening input file\n";
        return 1;
    }
    else {
        // Read from file one line at a time until EOF
        std::string str;
        while (getline(iFile, str)) {
            // Output string to output file "change.txt"
            oFile << str << '\n';

            // Convert string to numeric
            std::istringstream istr(str);

            // Add values to vector of denominations
            std::vector<int> denoms;
            int num;
            while (istr >> num) {
                denoms.push_back(num);
            }

            // Get amount from input file
            getline(iFile, str);

            // Output string to output file "change.txt"
            oFile << str << '\n';

            // Convert string to numeric
            std::istringstream istrAmount(str);
            int amount;
            istrAmount >> amount;

            // Get number of each coin needed to make change with least
            // number of coins
            int *coinCount = change(amount, denoms);
            int totalCoins = 0;

            // Iterate over each coin denomination
            for (int i = 0; i < denoms.size(); ++i) {
                // Output number of each denomination
                oFile << coinCount[i] << " ";

                // Sum number of each coin used
                totalCoins += coinCount[i];
            }

            // Output total number of coins used
            oFile << '\n' << totalCoins << '\n';

            delete [] coinCount;
        }
    }

    // Close the files
    iFile.close();
    oFile.close();

    return 0;
}

int *change(int amt, std::vector<int> coin)
{
    // Store number of coins needed to make change for each integer 
    // subtotal from 1 to amt
    int *C = new int[amt + 1];

    // Store index of last coin used for each subtotal to retrieve solution
    int *last = new int[amt + 1];

    // Zero coins are needed to make a subtotal of 0
    C[0] = 0;
    last[0] = INT_MIN;

    // Initialize remainder of C array as well as last-coin array
    for (int i = 1; i <= amt; ++i) {
        C[i] = INT_MAX;
        last[i] = INT_MIN;
    }

    // Iterate over all coin denominations 
    int N = coin.size();
    for (int n = 0; n < N; ++n) {
        // Iterate over all integer subtotal possibilities
        for (int i = 1; i <= amt; ++i) {
            // Check if current coin value is less than current subtotal
            if (coin[n] <= i) {
                // Check if adding coin is less than previous coin count
                if (1 + C[i - coin[n]] < C[i]) {
                    // Increment coin count if necessary
                    C[i] = 1 + C[i - coin[n]];

                    // Update latest coin added to last-coin array
                    last[i] = n;
                }
            }
        }
    }

    // Store number of each coin used for solution
    int *result = new int[N];

    // Zero-out result array 
    for (int i = 0; i < N; ++i) {
        result[i] = 0;
    }

    // Subtract latest coins added from amt until reach 0
    while (amt > 0) {
        // Find last coin added
        int coinIndex = last[amt];

        // Find value of last coin added
        int coinValue = coin[coinIndex];

        // Increment coin count
        result[coinIndex]++;

        // Update amt to find index of previous coin used
        amt -= coinValue;
    }

    // Free allocated memory
    delete [] C;
    delete [] last;

    return result;
}
