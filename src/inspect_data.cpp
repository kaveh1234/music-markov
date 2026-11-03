#include <iostream>
#include <string>
#include "../vendor/csv.hpp"

int main()
{
    csv::CSVReader reader("data/raw/chordonomicon_v2.csv");

    for (csv::CSVRow& row : reader)
    {
        std::cout << row["main_genre"].get<std::string>() << '\n';
        break;
    }

    return 0;
}
