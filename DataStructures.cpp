#include <functional>
#include <iostream>
#include <vector>

using std::vector;
using std::cout;
using std::endl;

#include "DataStructures.h"


vector<vector<int>> generate_sequences(int length, int sum/*=2*/) {

    vector<vector<int>> sequences;
    
    // Generate sequences of the specified length, where sum of elements is equal to 'sum'
    if (length <= 0) return sequences;
    if (length == 1) {
        sequences.push_back({sum});
        return sequences;
    }

    // Define array of zeros
    vector<int> current(length, 0);

    std::function<void(int, int)> backtrack = [&](int pos, int remaining) {
        if (pos == length - 1) {
            current[pos] = remaining;
            sequences.push_back(current);
            return;
        }
        for (int i = 0; i <= remaining; ++i) {
            current[pos] = i;
            backtrack(pos + 1, remaining - i);
        }
    };

    backtrack(0, sum);
    return sequences;
}


vector<int> select_by_condition(const vector<vector<int>>& sequences, const vector<int> contInd, const vector<int> contValue) {
    vector<int> indecesFiletered;

    for (int ind = 0; ind < static_cast<int>(sequences.size()); ++ind) {
        const auto& seq = sequences[ind];
        bool match = true;
        for (size_t i = 0; i < contInd.size(); ++i) {
            if (contInd[i] < 0 || contInd[i] >= static_cast<int>(seq.size()) || seq[contInd[i]] != contValue[i]) {
                match = false;
                break;
            }
        }
        if (match) {
            indecesFiletered.push_back(ind);
        }
    }

    return indecesFiletered;
}

