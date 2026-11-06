data <- read.delim("results/order0_validation.tsv")

actual <- data$actual_genre
predicted <- data$predicted_genre

confusion <- table(actual, predicted)
print(confusion)

correct <- actual == predicted
accuracy <- sum(correct) / length(correct)
print(100 * accuracy)

genres <- c("electronic", "jazz", "rock")
genre_accuracy <- numeric(3)

for (i in 1:3) {
    selected <- actual == genres[i]
    genre_accuracy[i] <- sum(correct[selected]) / sum(selected)
}

names(genre_accuracy) <- genres
print(100 * genre_accuracy)
