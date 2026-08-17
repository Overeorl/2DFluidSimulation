#include <cmath>
#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>

//-------------------------//
//Thoughts
//Flow direction ----->

//-------------------------//
std::vector<float> eqFormula(std::vector<float>,float,std::vector<std::pair<float,float>>,std::pair<float,float>);
float dotCalc(std::pair<float,float> num1,std::pair<float,float> num2);

int main(){
    std::pair<float,float> WindowSize = {1200,1200};//x,y
    sf::RenderWindow window(sf::VideoMode(WindowSize.first,WindowSize.second), "2D Sim");

    std::pair<float,float> gridSize = {240,0};//x,y
    gridSize.second=gridSize.first*(WindowSize.second/WindowSize.first);

    //testShape
    sf::CircleShape testShape(250.f);
    testShape.setFillColor(sf::Color::White);
    testShape.setPointCount(48);
    testShape.setPosition((WindowSize.first/2)-testShape.getRadius(),(WindowSize.second/2)-testShape.getRadius());

    //Initialisation
    std::vector<std::pair<float,float>> directions(9);
    directions={{0,0},{1,0},{1,1},{0,1},{-1,1},{-1,0},{-1,-1},{0,-1},{1,-1}};
    std::vector<float> weights(9);
    weights={4.0/9,1.0/9,1.0/36,1.0/9,1.0/36,1.0/9,1.0/36,1.0/9,1.0/36};
    

    std::vector<float> cell(9);
    //Cell Initialisation
    float density = 1;
    std::pair<float,float> velocity = {0.1,0};
    cell = eqFormula(weights,density,directions,velocity);


    std::vector<std::vector<float>> row(gridSize.second, cell);//y
    std::vector<std::vector<std::vector<float>>> grid(gridSize.first, row);//x ----- (x,y,cell)

    



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

std::vector<float> eqFormula(std::vector<float> weights,
                                          float density,
            std::vector<std::pair<float,float>> directions,
                         std::pair<float,float> velocity){
    std::vector<float> f_eq(9);
    for (int i=0;i<=8;i++){
        f_eq[i] = weights[i] * density * (1+3*dotCalc(directions[i],velocity)+4.5*(dotCalc(directions[i],velocity))*(dotCalc(directions[i],velocity))-1.5*(dotCalc(velocity,velocity)));
    };
    return f_eq;
};

float dotCalc(std::pair<float,float> mat1,std::pair<float,float> mat2){
    return ((mat1.first*mat2.first)+(mat1.second*mat2.second));
};