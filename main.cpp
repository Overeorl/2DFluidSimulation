#include <cmath>
#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>

//-------------------------//
//Thoughts
//Flow direction ----->

//-------------------------//

int PARTICLES = 100;
bool started = false;
float DENSITY = 1;
std::pair<float,float> startVelocity = {0.1,0};


std::pair<float,float> velocityCalc(std::vector<float>);
std::vector<float> eqFormula(std::vector<float>,float,std::vector<std::pair<float,float>>,std::pair<float,float>);
float dotCalc(std::pair<float,float> num1,std::pair<float,float> num2);
std::pair<int,int> gridPosCalculator(int,int);
void updateLeft(std::pair<float,float>,std::vector<std::vector<std::vector<float>>>&);
void updateGrid(std::vector<std::vector<std::vector<float>>>&);

std::pair<int,int> WindowSize = {1200,1200};//x,y
std::pair<int,int> gridSize = {240,0};//x,y

int main(){
    //Can change


    sf::RenderWindow window(sf::VideoMode(WindowSize.first,WindowSize.second), "2D Sim");
    
    gridSize.second=gridSize.first*(WindowSize.second/WindowSize.first);

    //testShape
    sf::CircleShape testShape(250.f);
    testShape.setFillColor(sf::Color::White);
    testShape.setPointCount(48);
    testShape.setPosition((WindowSize.first/2)-testShape.getRadius(),(WindowSize.second/2)-testShape.getRadius());

    //Initialisation
    std::vector<float> cell(9);
    cell={4.0/9,1.0/9,1.0/36,1.0/9,1.0/36,1.0/9,1.0/36,1.0/9,1.0/36};
    //Cell Initialisation
    std::vector<std::vector<float>> row(gridSize.second, cell);//y
    std::vector<std::vector<std::vector<float>>> grid(gridSize.first, row);//x ----- (x,y,cell)

    std::vector<std::pair<float,float>> particleLocations(PARTICLES);


    while (window.isOpen()){
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            if (event.key.code == sf::Keyboard::Key::Space){
                if (!started){
                    updateLeft(startVelocity,grid);
                }
                else if (started){
                    updateLeft({0,0},grid);
                };
            };
            };
        };

        //add here later


        window.clear();
        window.draw(testShape);
        window.display();
    };
};

std::vector<float> eqFormula(std::pair<float,float> velocity){

    std::vector<std::pair<float,float>> directions(9);
    directions={{0,0},{1,0},{1,1},{0,1},{-1,1},{-1,0},{-1,-1},{0,-1},{1,-1}};
    std::vector<float> weights(9);
    weights={4.0/9,1.0/9,1.0/36,1.0/9,1.0/36,1.0/9,1.0/36,1.0/9,1.0/36};


    std::vector<float> f_eq(9);
    for (int i=0;i<=8;i++){
        f_eq[i] = weights[i] * DENSITY * (1+3*dotCalc(directions[i],velocity)+4.5*(dotCalc(directions[i],velocity))*(dotCalc(directions[i],velocity))-1.5*(dotCalc(velocity,velocity)));
    };
    return f_eq;
};

float dotCalc(std::pair<float,float> mat1,std::pair<float,float> mat2){
    return ((mat1.first*mat2.first)+(mat1.second*mat2.second));
};

std::pair<int,int> gridPosCalculator(int Pixelx,int Pixely){
    return {trunc(Pixelx/(WindowSize.first/gridSize.first)),trunc(Pixely/(WindowSize.second/gridSize.second))};
};

void updateLeft(std::pair<float,float> NewVelocity,std::vector<std::vector<std::vector<float>>>& Grid){
    std::vector<float> replacingCell;
    replacingCell=eqFormula(NewVelocity);

    for (int i = 0; i<gridSize.second;i++){
        Grid[0][i]=replacingCell;
    };

    updateGrid(Grid);
};

void updateGrid(std::vector<std::vector<std::vector<float>>>& Grid){
//if started dont do first row
//if stopped do 0 and treat left wall as a collision - dissipate from right until pressure is at normal levels 1 atm
int startx = 0;
std::pair<float,float> newVelocity={0,0};
    if (started){
        startx = 1;
    };
    for (int x=startx;x<gridSize.first;x++){
        for (int y=startx;y<gridSize.second;y++){
            newVelocity=velocityCalc(Grid[x][y]);
            Grid[x][y]=eqFormula(newVelocity);
        };
    };

};
std::pair<float,float> velocityCalc(std::vector<float> cell){
    //add velocity calculator here.
    return {0,0};
};