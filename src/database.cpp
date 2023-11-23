#include "database.h"
#include <cstring>
#include <iostream>
#include <stdio.h>

DataBase::DataBase()
{
    //ctor
    dbfile = "./database.db";
}

DataBase::~DataBase()
{
    //dtor
}

void DataBase::openDatabase()
{
    sqlite3_initialize();
    sqlite3_open(dbfile.c_str(), &db);
}

void DataBase::closeDataBase()
{
    sqlite3_close(db);
    sqlite3_shutdown();
}

bool DataBase::executeQuery(std::string query)
{
    char * errmsg = 0;
    return sqlite3_exec(db, query.c_str(), NULL, 0, &errmsg);
}

bool DataBase::insertScoreJoueur(std::string player_name, int score_nb, int nbTotalLignes, int niveau)
{
    std::string query = "INSERT INTO highScore VALUES ('";
    query += player_name;
    query += "',";
    query += std::to_string(score_nb);
    query += ",";
    query += std::to_string(nbTotalLignes);
    query += ",";
    query += std::to_string(niveau);
    query += ")";



    return executeQuery(query);
}

std::vector<Score*>* DataBase::afficherHighScore()
{
    std::string query = "SELECT * FROM highScore  ORDER BY score_nb desc limit 10";
    std::vector<Score*>* highScores = new std::vector<Score*>;
    int i;

    sqlite3_stmt * stmt;
    sqlite3_prepare_v2(db, query.c_str(), strlen(query.c_str())+1, &stmt, NULL);

    do
    {
        i = sqlite3_step(stmt);
        if(i == SQLITE_ROW)
        {
            Score * highScore = new Score();
            highScores->push_back(highScore);

            highScore->player_name = (char*)sqlite3_column_text(stmt,0);
            highScore->score_nb = sqlite3_column_int(stmt,1);
            highScore->nbTotalLignes = sqlite3_column_int(stmt,2);
            highScore->niveau = sqlite3_column_int(stmt,3);
        }
    }
    while(i == SQLITE_ROW);

    return highScores;
}



