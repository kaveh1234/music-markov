#include <algorithm>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <map>
#include <random>
#include <set>
#include <string>
#include <vector>
#include "../vendor/csv.hpp"

int main()
{
    csv::CSVReader reader("data/processed/sample_songs.csv");

    std::vector<std::vector<std::string>> songs;
    std::set<std::string> artist_ids;

    for (csv::CSVRow& row : reader)
    {
        std::string artist = row["artist_id"].get<std::string>();

        artist_ids.insert(artist);

        songs.push_back({
            row["song_id"].get<std::string>(),
            artist,
            row["genre"].get<std::string>(),
            row["spotify_song_id"].get<std::string>(),
            row["chords"].get<std::string>()
        });
    }

    std::vector<std::string> artists(artist_ids.begin(), artist_ids.end());

    std::mt19937 generator(2026);
    std::shuffle(artists.begin(), artists.end(), generator);

    std::size_t training_count = artists.size() * 4 / 5;
    std::set<std::string> training_artists;

    for (std::size_t i = 0; i < training_count; ++i)
    {
        training_artists.insert(artists[i]);
    }

    std::ofstream training_output("data/processed/training_songs.csv");
    std::ofstream validation_output("data/processed/validation_songs.csv");

    if (!training_output || !validation_output)
    {
        std::cerr << "Could not open the output files.\n";
        return 1;
    }

    auto training_writer = csv::make_csv_writer(training_output);
    auto validation_writer = csv::make_csv_writer(validation_output);

    std::vector<std::string> header{
        "song_id", "artist_id", "genre", "spotify_song_id", "chords"
    };

    training_writer << header;
    validation_writer << header;

    std::map<std::string, std::size_t> training_counts;
    std::map<std::string, std::size_t> validation_counts;

    for (const auto& song : songs)
    {
        if (training_artists.count(song[1]) > 0)
        {
            training_writer << song;
            ++training_counts[song[2]];
        }
        else
        {
            validation_writer << song;
            ++validation_counts[song[2]];
        }
    }

    for (const auto& entry : training_counts)
    {
        std::cout << entry.first << ": "
                  << entry.second << " training, "
                  << validation_counts[entry.first] << " validation\n";
    }

    return 0;
}
