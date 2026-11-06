#include <cstddef>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include "../vendor/csv.hpp"

int main()
{
    csv::CSVFormat format;
    format.delimiter('\t');

    csv::CSVReader reader("data/processed/training_chords.tsv", format);

    std::map<std::string, std::map<std::string, std::size_t>> counts;
    std::map<std::string, std::size_t> totals;
    std::set<std::string> vocabulary{"<UNK>"};

    for (csv::CSVRow& row : reader)
    {
        std::string genre = row["genre"].get<std::string>();
        std::string chord = row["chord"].get<std::string>();

        ++counts[genre][chord];
        ++totals[genre];
        vocabulary.insert(chord);
    }

    const std::size_t category_count = vocabulary.size();

    std::ofstream output("results/order0_smoothed.tsv");

    if (!output)
    {
        std::cerr << "Could not open the output file.\n";
        return 1;
    }

    output << "genre\tchord\tcount\tprobability\n";
    output << std::setprecision(17);

    for (const auto& genre_entry : counts)
    {
        const std::string& genre = genre_entry.first;

        for (const std::string& chord : vocabulary)
        {
            std::size_t count = counts[genre][chord];

            double probability =
                (static_cast<double>(count) + 1.0) /
                (static_cast<double>(totals[genre]) + category_count);

            output << genre << '\t'
                   << chord << '\t'
                   << count << '\t'
                   << probability << '\n';
        }

        std::cout << genre << ": "
                  << totals[genre] << " training chord tokens\n";
    }

    std::cout << "Shared vocabulary: "
              << category_count << " categories, including <UNK>\n";

    return 0;
}
