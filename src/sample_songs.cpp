#include <algorithm>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <map>
#include <random>
#include <string>
#include <vector>
#include "../vendor/csv.hpp"

int main()
{
    csv::CSVReader reader("data/processed/clean_songs.csv");

    std::map<std::string, std::vector<std::vector<std::string>>> groups;

    for (csv::CSVRow& row : reader)
    {
        std::string genre = row["genre"].get<std::string>();

        groups[genre].push_back({
            row["song_id"].get<std::string>(),
            row["artist_id"].get<std::string>(),
            genre,
            row["spotify_song_id"].get<std::string>(),
            row["chords"].get<std::string>()
        });
    }

    std::ofstream output("data/processed/sample_songs.csv");

    if (!output)
    {
        std::cerr << "Could not open the output file.\n";
        return 1;
    }

    auto writer = csv::make_csv_writer(output);

    writer << std::vector<std::string>{
        "song_id", "artist_id", "genre", "spotify_song_id", "chords"
    };

    std::mt19937 generator(42);

    for (auto& entry : groups)
    {
        auto& songs = entry.second;
        std::shuffle(songs.begin(), songs.end(), generator);

        std::map<std::string, std::size_t> artist_counts;
        std::size_t selected = 0;

        for (const auto& song : songs)
        {
            const std::string& artist = song[1];

            if (artist_counts[artist] >= 20)
            {
                continue;
            }

            writer << song;
            ++artist_counts[artist];
            ++selected;

            if (selected == 1000)
            {
                break;
            }
        }

        std::cout << entry.first << ": "
                  << selected << " songs from "
                  << artist_counts.size() << " artists\n";
    }

    return 0;
}
