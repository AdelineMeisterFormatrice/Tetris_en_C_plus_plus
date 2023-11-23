#ifndef DATABASE_H
#define DATABASE_H

#include "../header/sqlite3.h"
#include <string>
#include <vector>
#include <score.h>


class DataBase
{
    public:
        DataBase();
        virtual ~DataBase();
        ///Methode pour ouvrir la base de données
        void openDatabase();
        ///Methode pour fermer la base de données
        void closeDataBase();
        ///Methode pour executer les requetes
        bool executeQuery(std::string query);

        ///Methode pour inserer les information du joueur en BDD
        bool insertScoreJoueur(std::string player_name, int score_nb, int nbTotalLignes, int niveau);
        /// Vecteur pour stocker les 10 meilleurs scores de la BDD et les afficher dans la fenetres scoreView
        std::vector<Score*>* afficherHighScore();



    private:
        sqlite3 * db;
        std::string dbfile;
};

#endif // DATABASE_H
