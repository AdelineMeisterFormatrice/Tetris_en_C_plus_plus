#ifndef SCORE_H
#define SCORE_H
#include <string>


class Score
{
    public:
        Score();
        virtual ~Score();

        std::string player_name;
        int score_nb;
        int nbTotalLignes;
        int niveau;

    protected:

    private:
};

#endif // SCORE_H
