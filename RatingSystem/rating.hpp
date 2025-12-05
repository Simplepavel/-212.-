#ifndef RATING_H
#define RATING_H


class Rating
{
private:
     int rating;

public:
     Rating(int initial = 1200) : rating(initial) {}
     int getRating() const
     {
          return rating;
     }
     void updateRating(int opponentRating, double score);
     const char *getRank() const;
};

class Matchmaker
{
public:
     bool arePlayersCompatible(int r1, int r2, int maxdiff = 150) const;
};

#endif