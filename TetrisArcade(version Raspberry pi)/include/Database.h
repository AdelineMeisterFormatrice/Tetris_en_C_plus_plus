#ifndef DATABASE_H
#define DATABASE_H

#include </home/pi/damien/TetrisArcade/include/Sqlite3.h>
#include <string>
#include <vector>
#include </home/pi/damien/TetrisArcade/include/Score.h>


class DataBase
{
    public:
        DataBase();
        virtual ~DataBase();

        void openDatabase();
        void closeDataBase();
        bool executeQuery(std::string query);

        bool insertScoreJoueur(std::string player_name, int score_nb, int nbTotalLignes, int niveau);
        std::vector<Score*>* afficherHighScore();


    private:
        sqlite3 * db;
        std::string dbfile;
};

#endif // DATABASE_H
