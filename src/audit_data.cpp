#include <cstddef>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include "../vendor/csv.hpp"

int main()
{
    csv::CSVReader reader("data/processed/candidate_songs.csv");

    std::map<std::string, std::size_t> rows;
    std::map<std::string, std::size_t> missing_ids;
    std::map<std::string, std::set<std::string>> songs;
    std::map<std::string, std::set<std::string>> artists;

    for (csv::CSVRow& row : reader)
    {
        std::string genre = row["genre"].get<std::string>();
        std::string song_id = row["spotify_song_id"].get<std::string>();
        std::string artist_id = row["artist_id"].get<std::string>();

        ++rows[genre];

        if (song_id.empty() || artist_id.empty())
        {
            ++missing_ids[genre];
            continue;
        }

        songs[genre].insert(song_id);
        artists[genre].insert(artist_id);
    }

    for (const auto& entry : rows)
    {
        const std::string& genre = entry.first;

        std::cout << "\nGenre: " << genre << '\n';
        std::cout << "Rows: " << entry.second << '\n';
        std::cout << "Rows missing an ID: " << missing_ids[genre] << '\n';
        std::cout << "Distinct songs with both IDs: "
                  << songs[genre].size() << '\n';
        std::cout << "Distinct artists among those songs: "
                  << artists[genre].size() << '\n';
    }

    return 0;
}
