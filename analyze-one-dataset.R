# Sanity checking:
# * Check the number of correct responses - 80%
# * Intensity vs. correct responses - inverse U shape.
# * Hotspots in absolute angle
# * Learning curve
# * Effect as a function of delta between the angle and the probe.
library(dplyr)
library(ggplot2)

df <- read.csv("marisse-undirected-flat-disconnected.csv")

summary(df)

ggplot(df, aes(x = trialNum, y = answerCorrect)) + 
  stat_smooth(method = "gam") +
  geom_jitter(width = 1, height=.05)

ggplot(df, aes(x = delta, y = answerSame)) + 
  stat_summary()

ggplot(df, aes(x = delta, y = answerCorrect)) + 
  stat_summary()

ggplot(df, aes(x = center, y = answerCorrect)) + 
  stat_summary()

ggplot(df, aes(x = strength, y = answerCorrect)) + 
  stat_summary()

ggplot(df, aes(x = strength, y = answerCorrect, color=as.factor(delta))) + 
    geom_line(stat="summary") + geom_point(stat="summary")

ggplot(df, aes(x = delta, y = answerSame, color=as.factor(strength))) + 
  geom_line(stat="summary") + geom_point(stat="summary")
