data <- read.delim("results/order0_validation.tsv")

actual <- data$actual_genre
predicted <- data$predicted_genre

confusion <- table(actual, predicted)
print(confusion)

number_songs <- length(actual)
number_correct <- 0

for (i in 1:number_songs) {
    if (actual[i] == predicted[i]) {
        number_correct <- number_correct + 1
    }
}

accuracy <- number_correct / number_songs
cat("\noverall accuracy:", 100 * accuracy, "%\n")

genres <- c("electronic", "jazz", "rock")

for (genre in genres) {
    genre_songs <- 0
    genre_correct <- 0

    for (i in 1:number_songs) {
        if (actual[i] == genre) {
            genre_songs <- genre_songs + 1

            if (actual[i] == predicted[i]) {
                genre_correct <- genre_correct + 1
            }
        }
    }

    accuracy <- genre_correct / genre_songs
    cat(genre, ":", 100 * accuracy, "%\n")
}
