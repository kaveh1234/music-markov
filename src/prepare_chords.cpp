#include <cstddef>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include "../vendor/csv.hpp"

int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        std::cerr << "Usage: prepare_chords INPUT.csv OUTPUT.tsv\n";
        return 1;
    }

    csv::CSVReader reader(argv[1]);
    std::ofstream output(argv[2]);

    if (!output)
    {
        std::cerr << "Could not open the output file.\n";
        return 1;
    }

    output << "song_id\tartist_id\tgenre\tposition\tsection\tchord\n";

    std::size_t song_count = 0;
    std::size_t chord_count = 0;

    for (csv::CSVRow& row : reader)
    {
        std::string id = row["song_id"].get<std::string>();
        std::string artist = row["artist_id"].get<std::string>();
        std::string genre = row["genre"].get<std::string>();
        std::string chords = row["chords"].get<std::string>();

        std::istringstream chord_stream(chords);
        std::string token;
        std::string section = "unknown";
        std::size_t position = 0;

        while (chord_stream >> token)
        {
            if (token.front() == '<' && token.back() == '>')
            {
                section = token.substr(1, token.size() - 2);
                continue;
            }

            ++position;
            ++chord_count;

            output << id << '\t'
                   << artist << '\t'
                   << genre << '\t'
                   << position << '\t'
                   << section << '\t'
                   << token << '\n';
        }

        ++song_count;
    }

    std::cout << "Processed " << song_count << " songs and "
              << chord_count << " chord tokens.\n";

    return 0;
}
