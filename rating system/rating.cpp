#include "rating.h"
#include<cmath>

void Rating::updateRating(int opponentRating, double score) {
    const double K = 32;
    double exp = 1.0/(1.0 + pow(10.0, (opponentRating - rating) / 400.0));//ожидаемый результат
    int delta = (int)(K * (score - exp));
    rating += delta;
    if (score == 1){
        stats.recordWin();
    }
    else if (score == 0){
        stats.recordLoss();
    }
    else{
         stats.recordDraw();
    }
}
