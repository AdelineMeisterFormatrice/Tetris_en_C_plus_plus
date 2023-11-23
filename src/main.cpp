#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <ctime>
#include <cstdlib>
#include <sstream>
#include <string>
#include <iostream>
#include <database.h>
#include <TextBox.hpp>
#include <vector>
#include <score.h>

#define FONT_PATH  "resources/font/D3Stonism.ttf"
#define TILES_PATH "resources/tiles.png"
#define LOGO_PATH  "resources/logoTetris.png"

const int hauteur = 20;
const int largeur = 10;

//////////// class map /////////////
int AireDeJeu[hauteur][largeur] = {0};

struct point
{
    int coordX,coordY;
}
pointA[4], pointB[4];

int figures[7][4] =
{
    1,3,5,7, // I
    2,4,5,7, // Z
    3,5,4,6, // S
    3,4,5,7, // T
    2,3,5,7, // L
    3,5,7,6, // J
    2,3,4,5, // O
};

int score_nb = 0;
int nbLignes = 0;
int nbTotalLignes = 0;
int niveau = 1;

///Methode pour eviter les colision, renvoie faux si une case de l'aire de jeu est ocuper par un block
///ou si on essaie de sortir de l'aire de jeu
///ou si on atteind le haut de l'aire de jeu et qu'on fait game over
bool check()
{
    for(int i=0; i<4; i++)
        if(pointA[i].coordX<0 || pointA[i].coordX>=largeur || pointA[i].coordY>=hauteur)
        {
            return false;
        }
        else if (AireDeJeu[pointA[i].coordY][pointA[i].coordX])
        {
            return false;
        }
    return true;
}

int main()
{
    srand(time(NULL));
    ///Creation de la fenetre
    sf::RenderWindow menuPrincipal(sf::VideoMode(800, 600), "Menu");
    ///Chargement des textures
    sf::Texture forme_t;
    sf::Texture logo_t;
    forme_t.loadFromFile(TILES_PATH);
    logo_t.loadFromFile(LOGO_PATH);
    sf::Font font;
    if(!font.loadFromFile(FONT_PATH))
        return EXIT_FAILURE;
    ///On applique les textures sur les sprites
    sf::Sprite forme_s(forme_t);
    sf::Sprite logo_s(logo_t);
    int positionX=0;
    bool rotate=false;
    int colorNum=1;
    float timer=0,delay=1;
    sf::Clock clock;
    while(menuPrincipal.isOpen())
    {
        menuPrincipal.clear(sf::Color::Black);
        logo_s.setPosition(350,0);
        menuPrincipal.draw(logo_s);
        sf::Text textlancerJeu;
        textlancerJeu.setFont(font);
        textlancerJeu.setCharacterSize(20);
        textlancerJeu.setString("Appuyer sur Entrer pour lancer le jeu");
        textlancerJeu.setFillColor(sf::Color::Red);
        textlancerJeu.setPosition(5,250);
        sf::Text textVoirScore;
        textVoirScore.setFont(font);
        textVoirScore.setCharacterSize(20);
        textVoirScore.setString("Appuyer sur Espace pour voir les scores");
        textVoirScore.setFillColor(sf::Color::Red);
        textVoirScore.setPosition(5,275);
        sf::Text textQuitter;
        textQuitter.setFont(font);
        textQuitter.setCharacterSize(20);
        textQuitter.setString("Appuyer sur Echap pour quitter le jeu");
        textQuitter.setFillColor(sf::Color::Red);
        textQuitter.setPosition(5,300);
        menuPrincipal.draw(textQuitter);
        menuPrincipal.draw(textVoirScore);
        menuPrincipal.draw(textlancerJeu);
        menuPrincipal.display();
        sf::Event event;
        while (menuPrincipal.pollEvent(event))
        {
            if(event.type == sf::Event::Closed || event.key.code == sf::Keyboard::Escape)
                menuPrincipal.close();
            if(event.key.code == sf::Keyboard::Space)
            {
                menuPrincipal.close();
                sf::RenderWindow scoreView(sf::VideoMode(800,600), "High Score");
                scoreView.clear(sf::Color::Black);
                logo_s.setPosition(350,0);
                scoreView.draw(logo_s);
                sf::Text textHighScore;

                DataBase bdd;
                bdd.openDatabase();
                std::vector<Score*>* highScores = bdd.afficherHighScore();
                int x=0;
                int y=0;
                for(int i = 0; i<highScores->size(); i++)
                {
                    std::cout << (*highScores)[i]->player_name << " " << (*highScores)[i]->score_nb << " " << (*highScores)[i]->nbTotalLignes << " " << (*highScores)[i]->niveau << std::endl;
                    textHighScore.setFont(font);
                    textHighScore.setCharacterSize(20);
                    textHighScore.setString("" + (*highScores)[i]->player_name + " " + std::to_string((*highScores)[i]->score_nb) + " " + std::to_string((*highScores)[i]->nbTotalLignes) + " " + std::to_string((*highScores)[i]->niveau));
                    textHighScore.setFillColor(sf::Color::Red);
                    textHighScore.move(x,y+30);
                    scoreView.draw(textHighScore);

                }

                bdd.closeDataBase();
                textHighScore.setFont(font);
                textHighScore.setCharacterSize(20);
                textHighScore.setString("Appuyer sur Echap pour quitter");
                textHighScore.setFillColor(sf::Color::Red);
                textHighScore.setPosition(5,400);
                scoreView.draw(textHighScore);
                scoreView.display();

                while(scoreView.isOpen())
                {
                    sf::Event eventClose;

                    while(scoreView.pollEvent(eventClose))
                    {
                        if(eventClose.type == sf::Event::Closed || eventClose.key.code == sf::Keyboard::Escape)
                        {
                            scoreView.close();
                        }

                    }
                }
            }
            if (event.key.code == sf::Keyboard::Return)
            {
                menuPrincipal.close();
                pointA[0].coordX = 0, pointA[0].coordY = 1;
                pointA[1].coordX = 1, pointA[1].coordY = 1;
                pointA[2].coordX = 1, pointA[2].coordY = 2;
                pointA[3].coordX = 1, pointA[3].coordY = 3;

                sf::RenderWindow jeuTetris(sf::VideoMode(1500, 950), "Tetris");

                sf::Music music;
                if(!music.openFromFile("resources/audio/music.ogg"))
                {
                    std::cout << "peut pas lire music" << std::endl;
                }

                music.play();

                music.setLoop(true);

                while (jeuTetris.isOpen())
                {

                    float time = clock.getElapsedTime().asSeconds();
                    clock.restart();
                    timer+=time;
                    sf::Event event;
                    while (jeuTetris.pollEvent(event))
                    {
                        ///Si on clique sur la croix de fermeture la fenetre se ferme
                        if (event.type == sf::Event::Closed)
                            jeuTetris.close();
                        ///Gestion des evenements clavier
                        if(event.type == sf::Event::KeyPressed)
                        {
                            ///Si on appuie sur la fleche haut la piece fait une rotation
                            if(event.key.code==sf::Keyboard::Up)
                            {
                                rotate=true;
                            }
                            ///Si on appuie sur la fleche gauche on deplace la piece d'une case vers la gauche
                            else if (event.key.code==sf::Keyboard::Left)
                            {
                                positionX=-1;
                            }
                            ///Si on appuie sur la fleche droite on deplace la piece d'une case vers la droite
                            else if (event.key.code==sf::Keyboard::Right)
                            {
                                positionX=1;
                            }
                        }
                        ///Si on appuie sur la fleche bas la piece descend plus vite
                        if(event.key.code==sf::Keyboard::Down)
                        {
                            delay=0.05;
                        }

                        ///Gestion des evenement du joystick pour la version raspberry pi ///
                        switch(event.type)
                        {
                        case sf::Event::Closed:
                            jeuTetris.close();
                            break;
                            ///On verifie qu'un joystick est connecter
                        case sf::Event::JoystickConnected:
                            std::cout << "Joystick" << event.joystickConnect.joystickId + 1 << "connetcter" << std::endl;
                            break;
                        case sf::Event::JoystickDisconnected:
                            std::cout << "joystick deconecter" << std::endl;
                            break;
                            ///On vérifie quel bouton est pressé
                        case sf::Event::JoystickButtonPressed:
                            std::cout << "joystick button " << event.joystickButton.button << "presser" << std::endl;
                            ///Si le bouton 4 est pressé on fait une rotation
                            if(event.joystickButton.button == 4)
                                rotate = true;
                            break;
                        case sf::Event::JoystickButtonReleased:
                            std::cout << "joystick button relacher" << std::endl;
                            break;
                            ///On vérifie la position du joystick
                        case sf::Event::JoystickMoved:
                            if(event.joystickMove.axis == sf::Joystick::Y)
                            {
                                std::cout << event.joystickMove.position << std::endl;
                                ///Si le joystick est a la position 100 on deplace la piéce d'une case vers la droite
                                if (event.joystickMove.position == 100)
                                    positionX=1;
                                    ///Si le joystick est à la position -100 on deplace la piéce d'une case vers la gauche
                                else if(event.joystickMove.position == -100)
                                    positionX=-1;
                            }
                            break;
                        }
                        ///Si on appuie sur le bouton 5 la piéce descend plus vite
                        if(event.joystickButton.button == 5)
                        {
                            delay = 0.05;
                        }
                    }
                    /// <- Deplacement -> ///
                    for(int i=0; i<4; i++)
                    {
                        pointB[i] = pointA[i];
                        pointA[i].coordX += positionX;
                    }
                    if(!check())
                    {
                        for(int i = 0; i < 4; i++)
                        {
                            pointA[i]=pointB[i];
                        }
                        std::cout << "deplacement impossible" << std::endl;
                    }
                    ///Rotation////
                    if(rotate)
                    {
                        point p = pointA[1]; ///centre de rotation
                        for(int i=0; i<4; i++)
                        {
                            int x = pointA[i].coordY - p.coordY;
                            int y = pointA[i].coordX - p.coordX;
                            pointA[i].coordX = p.coordX - x ;
                            pointA[i].coordY = p.coordY + y;
                        }
                        if(!check())
                        {
                            for(int i=0; i<4; i++)
                            {
                                pointA[i] = pointB[i];
                            }
                            std::cout << "rotation impossible" << std::endl;
                        }
                    }
                    ///Deplacement des pieces vers le bas //////
                    if(timer>delay)
                    {
                        for(int i = 0; i < 4; i++)
                        {
                            pointB[i] = pointA[i];
                            pointA[i].coordY += 1;
                        }
                        if(!check())
                        {
                            for(int i=0; i<4; i++)
                                AireDeJeu[pointB[i].coordY][pointB[i].coordX] = colorNum;
                            colorNum = 1+rand()%7;
                            int piece = rand()%7;
                            for(int i=0; i<4; i++)
                            {
                                pointA[i].coordX = figures[piece][i] % 2;
                                pointA[i].coordY = figures[piece][i] / 2;
                            }
                        }
                        timer=0;
                    }

                    ///Game Over///
                    if(!check())
                    {
                        std::cout << "Game Over" << std::endl;
                        jeuTetris.close();
                        sf::RenderWindow gameOver(sf::VideoMode(800,600), "Game Over");
                        sf::Text textGameOver;
                        textGameOver.setFont(font);
                        textGameOver.setCharacterSize(70);
                        textGameOver.setString("Game Over");
                        textGameOver.setFillColor(sf::Color::Red);
                        textGameOver.setPosition(210,40);
                        sf::Text textScore;
                        textScore.setFont(font);
                        textScore.setCharacterSize(20);
                        textScore.setString("Votre Score : " + std::to_string(score_nb));
                        textScore.setFillColor(sf::Color::Red);
                        textScore.setPosition(5,200);
                        sf::Text textLigne;
                        textLigne.setFont(font);
                        textLigne.setCharacterSize(20);
                        textLigne.setString("Vous avez fait " + std::to_string(nbTotalLignes) + " lignes ");
                        textLigne.setFillColor(sf::Color::Red);
                        textLigne.setPosition(5,230);
                        sf::Text textNiveau;
                        textNiveau.setFont(font);
                        textNiveau.setCharacterSize(20);
                        textNiveau.setString("Votre niveau : " + std::to_string(niveau));
                        textNiveau.setFillColor(sf::Color::Red);
                        textNiveau.setPosition(5,260);
                        std::string player_name;
                        GUI::InputTextBox name_box(sf::Vector2f(5,290), "Entrer votre nom", FONT_PATH, gameOver);
                        player_name = name_box.getAnswer();
                        gameOver.clear();
                        gameOver.draw(textGameOver);
                        gameOver.draw(textScore);
                        gameOver.draw(textLigne);
                        gameOver.draw(textNiveau);
                        DataBase bdd;
                        ///Insertion en base de données et création de la bdd si elle n'existe pas
                        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
                        {
                            bdd.openDatabase();
                            bdd.executeQuery("CREATE TABLE IF NOT EXISTS highScore ( player_name TEXT,  score_nb INT, nbTotalLignes INT,  niveau INT)");
                            bdd.insertScoreJoueur(player_name, score_nb,nbTotalLignes,niveau);
                            bdd.closeDataBase();
                        }
                        sf::Text textQuitter;
                        textQuitter.setFont(font);
                        textQuitter.setCharacterSize(20);
                        textQuitter.setString("Appuyer sur Echap pour quitter");
                        textQuitter.setFillColor(sf::Color::Red);
                        textQuitter.setPosition(5,290);
                        gameOver.draw(textQuitter);
                        gameOver.display();
                        while(gameOver.isOpen())
                        {
                            sf::Event eventClose;
                            while(gameOver.pollEvent(eventClose))
                            {
                                if(eventClose.type == sf::Event::Closed || eventClose.key.code == sf::Keyboard::Escape)
                                {
                                    gameOver.close();
                                }
                            }
                        }
                    }
                    ///On controle si une ligne est complete/////////
                    int k=hauteur-1;
                    for(int i=hauteur-1; i>0; i--)
                    {
                        int count=0;
                        for(int j=0; j<largeur; j++)
                        {
                            if (AireDeJeu[i][j])
                            {
                                count++;
                            }
                            AireDeJeu[k][j]=AireDeJeu[i][j];
                        }
                        if(count<largeur)
                        {
                            k--;
                        }
                        ///Gestion du score et des niveaux
                        if(count==largeur)
                        {
                            nbLignes++;
                            nbTotalLignes++;
                            std::cout << "Nombre de lignes : " << nbLignes  << std::endl;
                            switch(nbLignes)
                            {
                            case  1:
                                score_nb +=10;
                                std::cout << "Score = " << score_nb << std::endl;
                                break;
                            case  2:
                                score_nb += 50;
                                std::cout << "Score = " << score_nb << std::endl;
                                break;
                            case  3:
                                score_nb += 100;
                                std::cout << "Score = " << score_nb << std::endl;
                                break;
                            case  4:
                                score_nb += 500;
                                std::cout << "Score = " << score_nb << std::endl;
                                break;
                            default:
                                break;
                            }
                            if(nbTotalLignes >=20 && niveau == 1)
                            {
                                niveau++;
                                delay -= 0.2;
                                std::cout << delay << std::endl;
                            }
                            else if(nbTotalLignes >=40 && niveau == 2)
                            {
                                niveau++;
                                delay -= 0.2;
                                std::cout << delay << std::endl;
                            }
                            else if(nbTotalLignes >=60 && niveau == 3)
                            {
                                niveau++;
                                delay -= 0.3;
                                std::cout << delay << std::endl;
                            }
                        }
                    }
                    nbLignes = 0;
                    positionX=0;
                    rotate=false;
                    delay = 1;
                    ///dessin des différents éléments du jeu//////
                    ///On dessine le logo tetris
                    jeuTetris.clear(sf::Color::Black);
                    logo_s.setPosition(750,0);
                    jeuTetris.draw(logo_s);
                    ///On dessine la zone de jeu
                    sf::RectangleShape coteGauche(sf::Vector2f(5,640));
                    coteGauche.setPosition(622,260);
                    coteGauche.setFillColor(sf::Color::Red);
                    jeuTetris.draw(coteGauche);
                    sf::RectangleShape coteDroit(sf::Vector2f(5,640));
                    coteDroit.setPosition(947,260);
                    coteDroit.setFillColor(sf::Color::Red);
                    jeuTetris.draw(coteDroit);
                    sf::RectangleShape coteBas(sf::Vector2f(330,5));
                    coteBas.setPosition(622,900);
                    coteBas.setFillColor(sf::Color::Red);
                    jeuTetris.draw(coteBas);
                    ///On dessine la piece courante
                    for(int i=0; i<hauteur; i++)
                        for(int j=0; j<largeur; j++)
                        {
                            if(AireDeJeu[i][j]==0)
                            {
                                continue;

                            }
                            forme_s.setTextureRect(sf::IntRect(AireDeJeu[i][j]*32,0,32,32));
                            forme_s.setPosition(sf::Vector2f(j*32, i*32));
                            forme_s.move(sf::Vector2f(627,260));
                            jeuTetris.draw(forme_s);
                        }
                    for(int i=0; i<4; i++)
                    {
                        forme_s.setTextureRect(sf::IntRect(colorNum*32,0,32,32));
                        forme_s.setPosition(sf::Vector2f(pointA[i].coordX*32,pointA[i].coordY*32));
                        forme_s.move(sf::Vector2f(627,260));
                        jeuTetris.draw(forme_s);
                    }
                    ///On dessine le texte
                    sf::Text text;
                    text.setFont(font);
                    text.setCharacterSize(40);
                    text.setString("Score : " + std::to_string(score_nb));
                    text.setFillColor(sf::Color::Red);
                    text.setPosition(20,300);
                    jeuTetris.draw(text);
                    sf::Text tLignes;
                    tLignes.setFont(font);
                    tLignes.setCharacterSize(40);
                    tLignes.setString("Ligne : " + std::to_string(nbTotalLignes));
                    tLignes.setFillColor(sf::Color::Red);
                    tLignes.setPosition(20,350);
                    jeuTetris.draw(tLignes);
                    sf::Text tNiveau;
                    tNiveau.setFont(font);
                    tNiveau.setCharacterSize(40);
                    tNiveau.setString("Niveau : " + std::to_string(niveau));
                    tNiveau.setFillColor(sf::Color::Red);
                    tNiveau.setPosition(977,300);
                    jeuTetris.draw(tNiveau);
                    jeuTetris.display();
                }
            }
        }
    }
    return 0;
}
