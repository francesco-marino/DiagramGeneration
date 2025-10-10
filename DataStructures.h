#pragma once

#include <vector>

using namespace std;

template <typename T>
vector<vector<T>> cartesian_product(const vector<vector<T>>& input);

vector<vector<int>> generate_sequences(int length, int sum=2);

bool is_connected(const std::vector<std::vector<int>>& adj) ;

vector<int> select_by_condition(const vector<vector<int>>& sequences, const vector<int> contInd, const vector<int> contValue);


// Generate the Cartesian product of a vector of vectors
template <typename T>
vector<vector<T>> cartesian_product(const vector<vector<T>>& input) {
    vector<vector<T>> result;

    // Handle empty input case
    if (input.empty()) {
        return result;
    }

    // Start with one empty combination
    result.push_back({});

    for (const auto& pool : input) {
        std::vector<std::vector<T>> new_result;
        for (const auto& combination : result) {
            for (const auto& item : pool) {
                std::vector<T> new_combination = combination;
                new_combination.push_back(item);
                new_result.push_back(std::move(new_combination));
            }
        }
        result = std::move(new_result);
    }

    return result;
}


