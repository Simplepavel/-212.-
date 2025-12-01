#ifndef RATING_H
#define RATING_H

class Stats{
    private:
        int wins;
        int losses;
        int draws;
        int games;
    public:
        Stats():wins(0), losses(0), draws(0), games(0){}
        void recordWin(){
             wins++;
             games++;
        }
        void recordLoss(){
             losses++; 
             games++; 
        }
        void recordDraw(){
             draws++; 
             games++; 
        }
        int getWins()const {
             return wins;
        }
        int getLosses() const {
             return losses; 
        }
        int getDraws() const {
             return draws;
        }
        int getGames() const {
             return games;
        }
};

class Rating {
    private:
        int rating;
        Stats stats;
    public:
        Rating(int initial = 1200):rating(initial) {}
        int getRating() const {
            return rating;
        }
        const Stats& getStats() const {
             return stats; 
        }
        void updateRating(int opponentRating, double score);
        const char* getRank() const;
};

class Matchmaker{
public:
    bool arePlayersCompatible(int r1, int r2, int maxdiff = 150) const;
};



#endif