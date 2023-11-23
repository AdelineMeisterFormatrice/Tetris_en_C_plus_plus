#include <TextBox.hpp>

#include <SFML/Window/Event.hpp>

#include <iostream>


GUI::InputTextBox::InputTextBox(sf::Vector2f position, const char *_label,
                  const char *font_filepath,
                  sf::RenderWindow& _window) : label(_label),
                                               window(_window)
{
    if(!this->font.loadFromFile(font_filepath))
        std::cout << "Can not load font" << std::endl;
    this->text.setFont(this->font);
    this->text.setCharacterSize(20);
    this->text.setColor(sf::Color::Red);
    this->text.setStyle(sf::Text::Bold | sf::Text::Underlined);
    this->text.setString(this->text.getString() + " : ");
    this->label = this->label + " : ";
    this->text.setString(this->label);
}

void
GUI::InputTextBox::draw() {
    //this->window.clear();
    this->window.draw(this->text);
    this->window.display();
}

std::string
GUI::InputTextBox::getAnswer() {
    sf::Event event;
    bool done = false;
    while(!done) {
        while(window.pollEvent(event)) {
            this->draw();
            if(event.type == sf::Event::TextEntered) {
                if(event.key.code == 13 && this->answer.size() > 0)     // Return pressed
                    done = true;
                else if(event.key.code == 8 && this->answer.size() > 0) // Backspace pressed
                    this->answer.pop_back();
                else                                                    // Other characters
                    this->answer.push_back((char)event.text.unicode);
                this->text.setString(this->label + sf::String(this->answer));
            }
        }
    }
    return this->answer;
}
