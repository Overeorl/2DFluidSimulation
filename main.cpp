#include <cmath>
#include <SFML/Graphics.hpp>

//-------------------------//
//Thoughts
//Flow direction ----->

//-------------------------//

int main(){
    std::pair<float,float> WindowSize = {1200,1200};
    sf::RenderWindow window(sf::VideoMode(WindowSize.first,WindowSize.second), "2D Sim");

    std::pair<float,float> gridSize = {240,0};
    gridSize.second=gridSize.first*(WindowSize.second/WindowSize.first);

    //testShape
    sf::CircleShape testShape(250.f);
    testShape.setFillColor(sf::Color::White);
    testShape.setPointCount(48);
    testShape.setPosition((WindowSize.first/2)-testShape.getRadius(),(WindowSize.second/2)-testShape.getRadius());

    while (window.isOpen()){
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            };
        };

        //add here later
    
        window.clear();
        window.draw(testShape);
        window.display();
    };
};