library(tidyverse)
library(vroom)
library(gridExtra) # The reliable alternative

# 1. Load Data
df <- vroom("../output/market_simulation_results.csv")

# 2. Market Profile Plot
price_dist_plot <- df %>%
  tail(100000) %>%
  ggplot(aes(x = Price)) +
  geom_histogram(fill = "steelblue", bins = 50, color = "white") +
  theme_minimal() +
  labs(title = "Market Profile: Price Distribution", x = "Price", y = "Frequency")

# 3. Trend Plot
trend_plot <- df %>%
  tail(100000) %>%
  mutate(index = seq_along(Price)) %>%
  mutate(SMA = stats::filter(Price, rep(1/500, 500), sides = 1)) %>%
  ggplot(aes(x = index)) +
  geom_line(aes(y = Price), color = "gray80", alpha = 0.3) +
  geom_line(aes(y = SMA), color = "firebrick", linewidth = 1) +
  theme_minimal() +
  labs(title = "Market Trend: 500-Period SMA", x = "Trade Sequence", y = "Price")

# 4. COMBINE BOTH (The "Master Dashboard")
# This replaces the price_dist_plot / trend_plot command
grid.arrange(price_dist_plot, trend_plot, ncol = 1)