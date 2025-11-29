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


#endif