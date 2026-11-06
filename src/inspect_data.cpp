#include <cstddef>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include "../vendor/csv.hpp"

int main()
{
    csv::CSVReader reader("data/raw/chordonomicon_v2.csv");

    std::ofstream output("data/processed/first_song.tsv");

    if (!output)
    {
        std::cerr << "Could not open the output file.\n";
        return 1;
    }

    output << "song_id\tartist_id\tgenre\tposition\tsection\tchord\n";

    for (csv::CSVRow& row : reader)
    {
        std::string genre = row["main_genre"].get<std::string>();

        if (genre != "rock")
        {
            continue;
        }

        std::string song_id = row["id"].get<std::string>();
        std::string artist_id = row["artist_id"].get<std::string>();
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

            output << song_id << '\t'
                   << artist_id << '\t'
                   << genre << '\t'
                   << position << '\t'
                   << section << '\t'
                   << token << '\n';
        }

        break;
    }

    std::cout << "Saved data/processed/first_song.tsv\n";

    return 0;
}
