#include <cmath>
#include <cstddef>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include "../vendor/csv.hpp"

struct Song
{
    std::string genre;
    std::size_t chord_count = 0;
    std::map<std::string, double> log_sums;
};

int main()
{
    const std::vector<std::string> genres{
        "electronic", "jazz", "rock"
    };

    csv::CSVFormat format;
    format.delimiter('\t');

    std::map<std::string, std::map<std::string, double>> probabilities;

    csv::CSVReader model_reader("results/order0_smoothed.tsv", format);

    for (csv::CSVRow& row : model_reader)
    {
        std::string genre = row["genre"].get<std::string>();
        std::string chord = row["chord"].get<std::string>();

        probabilities[genre][chord] = row["probability"].get<double>();
    }

    std::map<std::string, Song> songs;

    csv::CSVReader validation_reader(
        "data/processed/validation_chords.tsv", format
    );

    for (csv::CSVRow& row : validation_reader)
    {
        std::string id = row["song_id"].get<std::string>();
        std::string chord = row["chord"].get<std::string>();

        Song& song = songs[id];
        song.genre = row["genre"].get<std::string>();
        ++song.chord_count;

        for (const std::string& genre : genres)
        {
            const auto& model = probabilities.at(genre);
            auto found = model.find(chord);

            double probability = found == model.end()
                ? model.at("<UNK>")
                : found->second;

            song.log_sums[genre] += std::log(probability);
        }
    }

    std::ofstream output("results/order0_validation.tsv");

    if (!output)
    {
        std::cerr << "Could not open the output file.\n";
        return 1;
    }

    output << "song_id\tactual_genre\tchord_count"
           << "\telectronic_score\tjazz_score\trock_score"
           << "\tpredicted_genre\n";
    output << std::setprecision(17);

    std::size_t correct = 0;

    for (const auto& entry : songs)
    {
        const Song& song = entry.second;
        std::string predicted = genres.front();

        output << entry.first << '\t'
               << song.genre << '\t'
               << song.chord_count;

        for (const std::string& genre : genres)
        {
            double score = song.log_sums.at(genre) / song.chord_count;
            output << '\t' << score;

            if (song.log_sums.at(genre) > song.log_sums.at(predicted))
            {
                predicted = genre;
            }
        }

        output << '\t' << predicted << '\n';

        if (predicted == song.genre)
        {
            ++correct;
        }
    }

    std::cout << "Scored " << songs.size() << " validation songs.\n";
    std::cout << "Correct genre predictions: "
              << correct << " / " << songs.size() << '\n';

    return 0;
}
