#include <cmath>
#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>

//-------------------------//
//Thoughts
//Flow direction ----->

//-------------------------//

//Can change
int PARTICLES = 100;
float DENSITY = 1;
std::pair<float,float> startVelocity = {0.1,0};


bool started = false;

//Declatation
void streaming(std::vector<std::vector<std::vector<float>>>&);
std::pair<float,float> velocityCalc(std::vector<float>);
std::vector<float> eqFormula(std::pair<float,float> velocity);
float dotCalc(std::pair<float,float> num1,std::pair<float,float> num2);
std::pair<int,int> gridPosCalculator(int,int);
void updateLeft(std::pair<float,float>,std::vector<std::vector<std::vector<float>>>&);
void updateGrid(std::vector<std::vector<std::vector<float>>>&);

std::pair<int,int> WindowSize = {1200,1200};//x,y
std::pair<int,int> gridSize = {240,0};//x,y

std::vector<std::pair<float,float>> directions = {{0,0},{1,0},{1,1},{0,1},{-1,1},{-1,0},{-1,-1},{0,-1},{1,-1}};
    std::vector<float> weights = {4.0/9,1.0/9,1.0/36,1.0/9,1.0/36,1.0/9,1.0/36,1.0/9,1.0/36};


int main(){
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
            };

        if (event.type==sf::Event::KeyPressed){
            if (event.key.code == sf::Keyboard::Key::Space){
                if (!started){
                    started = true;
                }
                else{
                    updateLeft({0,0},grid);
                    started = false;
                };
            };
        };
    };

        //add here later
        if (started){updateLeft(startVelocity,grid);};


        window.clear();
        window.draw(testShape);
        window.display();
    };
};

std::vector<float> eqFormula(std::pair<float,float> velocity){
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

void updateGrid(std::vector<std::vector<std::vector<float>>>& OriginalGrid){
//if started dont do first row
//if stopped do 0 and treat left wall as a collision - dissipate from right until pressure is at normal levels 1 atm

int startx = 0;
std::pair<float,float> newVelocity={0,0};
    if (started){
        startx = 1;
    };
    for (int x=startx;x<gridSize.first;x++){
        for (int y=0;y<gridSize.second;y++){
            newVelocity=velocityCalc(OriginalGrid[x][y]);
            OriginalGrid[x][y]=eqFormula(newVelocity);
        };
    };
    streaming(OriginalGrid);
};

void streaming(std::vector<std::vector<std::vector<float>>>& OriginalGrid){
    std::vector<std::vector<std::vector<float>>> CopyGrid=OriginalGrid;
    for (int x=0;x<gridSize.first;x++){
        for (int y=0;y<gridSize.second;y++){
            if (x==0){
                OriginalGrid[x][y][1]=CopyGrid[x][y][5];//left wall hit
            }else{
                OriginalGrid[x][y][1]=CopyGrid[x-1][y][1];
            };

            if (x==0 && y==0){
                OriginalGrid[x][y][2]=CopyGrid[x][y][6];//left-top corner hit
            }else if (x==0){
                OriginalGrid[x][y][2]=CopyGrid[x][y][4];//left wall hit
            }else if (y==0){
                OriginalGrid[x][y][2]=CopyGrid[x][y][8];//top wall hit
            }else{
                OriginalGrid[x][y][2]=CopyGrid[x-1][y-1][2];
            };
            
            if (y==0){
                OriginalGrid[x][y][3]=CopyGrid[x][y][7];//top wall hit
            }else{
                OriginalGrid[x][y][3]=CopyGrid[x][y-1][3];
            };
            
            if (y==0){
                OriginalGrid[x][y][4]=CopyGrid[x][y][6];//top wall hit
            }else if (x==gridSize.first-1){
                OriginalGrid[x][y][4]=weights[4]; //right hole hit
            }else{
                OriginalGrid[x][y][4]=CopyGrid[x+1][y-1][4];
            };

            if (x==gridSize.first-1){
                OriginalGrid[x][y][5]=weights[5]; //right hole hit
            }else{
            OriginalGrid[x][y][5]=CopyGrid[x+1][y][5];
            };
            
            if (y==gridSize.second-1){
                OriginalGrid[x][y][6]=CopyGrid[x][y][4]; //bottom wall hit
            }else if (x==gridSize.first-1){
                OriginalGrid[x][y][6]=weights[6]; //right hole hit
            }else{
                OriginalGrid[x][y][6]=CopyGrid[x+1][y+1][6];
            };
            
            if (y==gridSize.second-1){
                OriginalGrid[x][y][7]=CopyGrid[x][y][3]; //bottom wall hit
            }else{
                OriginalGrid[x][y][7]=CopyGrid[x][y+1][7];
            };
            
            if (x==0 && y==gridSize.second-1){
                OriginalGrid[x][y][8]=CopyGrid[x][y][4];//left-bottom corner hit
            }else if (x==0){
                OriginalGrid[x][y][8]=CopyGrid[x][y][6];//left wall hit
            }else if (y==gridSize.second-1){
                OriginalGrid[x][y][8]=CopyGrid[x][y][2]; //bottom wall hit
            }else{
                OriginalGrid[x][y][8]=CopyGrid[x-1][y+1][8];
            };
        };
    };
};

std::pair<float,float> velocityCalc(std::vector<float> cell){
    //add velocity calculator here.
    std::pair<float,float> velocity;
    float sum =0;
    float sumX =0;
    float sumY =0;

    for(int i=0; i<=8;i++){
        sum+=cell[i];
        sumX+=directions[i].first*cell[i];
        sumY+=directions[i].second*cell[i];
    };

    velocity.first=(1/sum) * (sumX);
    velocity.second=(1/sum) * (sumY); 

    return velocity;
};