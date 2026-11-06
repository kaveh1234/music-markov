#include <cstddef>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>
#include "../vendor/csv.hpp"

int main()
{
    csv::CSVReader reader("data/processed/candidate_songs.csv");

    std::map<std::string, std::vector<std::string>> songs;
    std::set<std::string> conflicting_ids;

    for (csv::CSVRow& row : reader)
    {
        std::string id = row["song_id"].get<std::string>();
        std::string artist = row["artist_id"].get<std::string>();
        std::string genre = row["genre"].get<std::string>();
        std::string spotify_id = row["spotify_song_id"].get<std::string>();
        std::string chords = row["chords"].get<std::string>();

        if (spotify_id.empty() || artist.empty() || chords.empty())
        {
            continue;
        }

        if (songs.count(spotify_id) > 0)
        {
            if (songs[spotify_id][1] != artist ||
                songs[spotify_id][2] != genre)
            {
                conflicting_ids.insert(spotify_id);
            }

            continue;
        }

        songs[spotify_id] = {id, artist, genre, spotify_id, chords};
    }

    std::ofstream output("data/processed/clean_songs.csv");

    if (!output)
    {
        std::cerr << "Could not open the output file.\n";
        return 1;
    }

    auto writer = csv::make_csv_writer(output);

    writer << std::vector<std::string>{
        "song_id", "artist_id", "genre", "spotify_song_id", "chords"
    };

    std::map<std::string, std::size_t> counts;

    for (const auto& entry : songs)
    {
        if (conflicting_ids.count(entry.first) > 0)
        {
            continue;
        }

        writer << entry.second;
        ++counts[entry.second[2]];
    }

    for (const auto& entry : counts)
    {
        std::cout << entry.first << ": " << entry.second << '\n';
    }

    std::cout << "Conflicting song IDs excluded: "
              << conflicting_ids.size() << '\n';

    return 0;
}
