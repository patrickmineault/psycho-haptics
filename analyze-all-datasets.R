# Sanity checking:
# * Check the number of correct responses - 80%
# * Intensity vs. correct responses - inverse U shape.
# * Hotspots in absolute angle
# * Learning curve
# * Effect as a function of delta between the angle and the probe.

# Action items:
# * Find whether the 0 degree position dip is explainable by physiology
# * Think about adjusting position of motors accordingly
# * Think about adding more motors --
#     Make one more
# * OUR flat ring prototype 
# * Take out boost transistor circuitry (5V vs. 3V thing).
# * Start thinking about means of adherence
#     Tegaderm vs. EKG 3M tape

library(dplyr)
library(ggplot2)

csvs <- c("marisse-flat-directed.csv", 
          "marisse-flat-directed-disconnected.csv",
          "marisse-flat-undirected-disconnected.csv",
          "marisse-ortho-directed.csv",
          "marisse-ortho-directed-disconnected.csv",
          "marisse-ortho-undirected.csv",
          "marisse-paper-undirected.csv")

results <- lapply(csvs, function(name) {
  df <- read.csv(name)
  df <- df[1:256,]
  return(df)
})

big.df <- do.call(rbind, results)
big.df <- big.df %>% rename(condition = message)

big.df %>% group_by(condition) %>% 
  dplyr::summarize(p.correct = sum(answerCorrect) / n())

summary(big.df)

ggplot(big.df, aes(x= (condition), y=answerCorrect)) +
  stat_summary() + coord_flip()

ggplot(big.df, aes(x = trialNum, y = answerCorrect, color=condition, group=condition)) + 
  stat_smooth(method = "gam") +
  geom_jitter(width = 1, height=.05)

# Performance.
# Unsliced
ggplot(big.df, aes(x = delta, y = answerSame)) + 
  stat_summary() 

# Sliced by condition
ggplot(big.df, aes(x = delta, y = answerSame, color=condition)) + 
  stat_summary() + geom_line(stat="summary")

# Look at the S's for the undirecteds
ggplot(big.df %>% filter(condition %in% c("marisse_paper_undirected",
                                          "marisse_ortho_undirected",
                                          "marisse_flat_undirected_disconnected")),
       aes(x = delta, y = answerSame, color=condition)) + 
  stat_summary() + stat_smooth(alpha=.1, method="glm", method.args = list(family = "binomial"))


ggplot(big.df, aes(x = delta, y = answerCorrect)) + 
  stat_summary()

ggplot(big.df, aes(x = center, y = answerCorrect)) + 
  stat_summary()

ggplot(big.df %>% filter(condition != "marisse_paper_undirected"), 
       aes(x = center, y = answerCorrect)) + 
  stat_summary()

ggplot(big.df, 
       aes(x = center, y = answerCorrect, color=condition)) + 
  stat_summary()

ggplot(big.df, aes(x = strength/255*5, y = answerCorrect)) + 
  stat_summary()

ggplot(big.df, aes(x = strength, y = answerCorrect, color=condition)) + 
  stat_summary() + geom_line(stat="summary")


ggplot(big.df, aes(x = strength, y = answerCorrect, color=as.factor(delta))) + 
    geom_line(stat="summary") + geom_point(stat="summary")

ggplot(big.df, aes(x = delta, y = answerSame, color=as.factor(strength))) + 
  geom_line(stat="summary") + geom_point(stat="summary")
