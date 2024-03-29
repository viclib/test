#include <stdio.h>

typedef struct pair_ { int x; int y; } pair;

pair f(int a, int b){
    int k = (b >> a << (a+1)) + (1<<a);
    pair p = {k - 1, k + (b&((1<<a)-1))};
    return p;
};

int main(){
    pair correct[4][16] = {{{0,1},{2,3},{4,5},{6,7},{8,9},{10,11},{12,13},{14,15},{16,17},{18,19},{20,21},{22,23},{24,25},{26,27},{28,29},{30,31}},{{1,2},{1,3},{5,6},{5,7},{9,10},{9,11},{13,14},{13,15},{17,18},{17,19},{21,22},{21,23},{25,26},{25,27},{29,30},{29,31}},{{3,4},{3,5},{3,6},{3,7},{11,12},{11,13},{11,14},{11,15},{19,20},{19,21},{19,22},{19,23},{27,28},{27,29},{27,30},{27,31}},{{7,8},{7,9},{7,10},{7,11},{7,12},{7,13},{7,14},{7,15},{23,24},{23,25},{23,26},{23,27},{23,28},{23,29},{23,30},{23,31}}};

    for (int i=0; i<4; ++i)
        for (int j=0; j<16; ++j)
            printf("f(%.2d,%.2d) = (%.2d,%.2d) | Correct: %d %d\n",
                i,
                j,
                f(i,j).x,
                f(i,j).y,
                f(i,j).x == correct[i][j].x,
                f(i,j).y == correct[i][j].y);

    return 0;
};

