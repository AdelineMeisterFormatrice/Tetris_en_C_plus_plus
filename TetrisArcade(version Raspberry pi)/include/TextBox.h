#ifndef TEXTBOX_INCLUDED
#define TEXTBOX_INCLUDED

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/String.hpp>
#include <SFML/System/Vector2.hpp>


namespace GUI
{


    class InputTextBox
    {

        public:
            InputTextBox(sf::Vector2f position, const char *text,
                         const char *font_filepath, sf::RenderWindow& window);

            std::string getAnswer();
            void draw();

        private:
            std::string answer;
            sf::Font font;
            sf::String label;
            sf::Text text;
            sf::RenderWindow& window;
    };


}


#endif
