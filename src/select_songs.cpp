#include <cstddef>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "../vendor/csv.hpp"

int main()
{
    csv::CSVReader reader("data/raw/chordonomicon_v2.csv");
    std::ofstream output("data/processed/candidate_songs.csv");

    if (!output)
    {
        std::cerr << "Could not open the output file.\n";
        return 1;
    }

    auto writer = csv::make_csv_writer(output);

    writer << std::vector<std::string>{
        "song_id", "artist_id", "genre", "spotify_song_id", "chords"
    };

    std::size_t selected = 0;

    for (csv::CSVRow& row : reader)
    {
        std::string genre = row["main_genre"].get<std::string>();

        if (genre != "rock" && genre != "jazz" && genre != "electronic")
        {
            continue;
        }

        writer << std::vector<std::string>{
            row["id"].get<std::string>(),
            row["artist_id"].get<std::string>(),
            genre,
            row["spotify_song_id"].get<std::string>(),
            row["chords"].get<std::string>()
        };

        ++selected;
    }

    std::cout << "Selected rows: " << selected << '\n';

    return 0;
}
