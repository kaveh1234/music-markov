#include <cstddef>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <string>
#include "../vendor/csv.hpp"

int main()
{
    csv::CSVFormat format;
    format.delimiter('\t');

    csv::CSVReader reader("data/processed/training_chords.tsv", format);

    std::map<std::string, std::map<std::string, std::size_t>> counts;
    std::map<std::string, std::size_t> totals;

    for (csv::CSVRow& row : reader)
    {
        std::string genre = row["genre"].get<std::string>();
        std::string chord = row["chord"].get<std::string>();

        ++counts[genre][chord];
        ++totals[genre];
    }

    std::ofstream output("results/order0_raw.tsv");

    if (!output)
    {
        std::cerr << "Could not open the output file.\n";
        return 1;
    }

    output << "genre\tchord\tcount\tprobability\n";
    output << std::setprecision(10);

    for (const auto& genre_entry : counts)
    {
        const std::string& genre = genre_entry.first;

        for (const auto& chord_entry : genre_entry.second)
        {
            double probability =
                static_cast<double>(chord_entry.second) / totals[genre];

            output << genre << '\t'
                   << chord_entry.first << '\t'
                   << chord_entry.second << '\t'
                   << probability << '\n';
        }

        std::cout << genre << ": "
                  << totals[genre] << " chord tokens, "
                  << genre_entry.second.size() << " distinct labels\n";
    }

    return 0;
}
