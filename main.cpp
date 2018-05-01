#include <iostream>
#include <utility>      // std::pair, std::make_pair
#include <cstdio>
#include <fstream>
#include <cstring>
#include <stdlib.h>
#include <string>
#include <algorithm>
#include <bits/c++io.h>
#include <algorithm>
#include <sstream>
#include <cmath>
#include <math.h>
#include <iomanip>
#include <time.h>
#include <fstream>      // std::ofstream


using namespace std;

double Qtab[6][6][2][6];    // The Q table first 3 index represent the state. the last index represents the action
int blocks[6][6];           // the blocks: if there are any blocks at those coordinates then blocks[x][y]=num of blocks it has otherwise 0
int policy = 0;             // policy number
int experiment = 0;         // Experiment number
double alpha;                  // learning rate
double gamma = 0.3;         //discount rate
int steps;                  // The number of steps for which the algorithm will run
int NORTH = 0, SOUTH = 0, EAST = 0, WEST = 0;
int bankAccount =0;         //bank account of the agent
int reward;
ofstream ofs("output.txt");

int term = 0;
int first = 0;
int drop = 0;

struct space
{
    int i;  // x coordinate of agent
    int j;  // y coordinate of GENT
    int x;  // agent caries a block
}sSpace;

void initQtab()
{
    for(int i = 1; i < 6; ++i)
        for(int j = 1; j < 6; ++j)
            for(int k = 0; k<2; ++k)
                for(int l = 0; l<6; ++l)
                    Qtab[i][j][k][l] = 0.0;
}

void initBlocks()
{
    for(int i = 1; i < 6; ++i)
        for(int j = 1; j < 6; ++j)
        {
            blocks[i][j] = 0;
        }

    blocks[1][1] = 5;
    blocks[3][3] = 5;
    blocks[5][5] = 5;
}

bool isDropOff()
{
    if(experiment < 5 || term < 2 || experiment == 6)
    {
        if((sSpace.i == 5 && sSpace.j == 1) && blocks[5][1] < 5)
            return true;
        else if((sSpace.i == 5 && sSpace.j == 3) && blocks[5][3] < 5)
            return true;
        else if((sSpace.i == 2 && sSpace.j == 5) && blocks[2][5] < 5)
            return true;
        else
            return false;
    }
    else
    {
        if((sSpace.i == 1 && sSpace.j == 1) && blocks[1][1] < 5)
            return true;
        else if((sSpace.i == 3 && sSpace.j == 3) && blocks[3][3] < 5)
            return true;
        else if((sSpace.i == 5 && sSpace.j == 5) && blocks[5][5] < 5)
            return true;
        else
            return false;
    }
}

bool isPickUp()
{
    if(experiment < 5 || term < 2)
    {
        if((sSpace.i == 1 && sSpace.j == 1) && blocks[1][1] > 0)
            return true;
        else if((sSpace.i == 3 && sSpace.j == 3) && blocks[3][3] > 0)
            return true;
        else if((sSpace.i == 5 && sSpace.j == 5) && blocks[5][5] > 0)
            return true;
        else
            return false;
    }
    else if (experiment == 5)
    {
        if((sSpace.i == 5 && sSpace.j == 1) && blocks[5][1] > 0)
            return true;
        else if((sSpace.i == 5 && sSpace.j == 3) && blocks[5][3] > 0)
            return true;
        else if((sSpace.i == 2 && sSpace.j == 5) && blocks[2][5] > 0)
            return true;
        else
            return false;
    }
    else if (experiment == 6)
    {
        if((sSpace.i == 2 && sSpace.j == 2) && blocks[2][2] > 0)
            return true;
        else if((sSpace.i == 4 && sSpace.j == 4) && blocks[4][4] > 0)
            return true;
        else if((sSpace.i == 1 && sSpace.j == 5) && blocks[1][5] > 0)
            return true;
        else
            return false;
    }
    else
        return false;

}

void setPolicy(int x)
{
    policy = x;
}

void setExperiment(int x)
{
    experiment = x;

    if(experiment == 1)
    {
        setPolicy(1);
        alpha = 0.3;
        steps = 10000;
    }
    else if(experiment == 2)
    {
        setPolicy(2);
        alpha = 0.3;
        steps = 10000;
    }
    else if(experiment == 3)
    {
        setPolicy(3);
        alpha = 0.3;
        steps = 10000;
    }
    else if(experiment == 4)
    {
        setPolicy(3);
        alpha = 0.5;
        steps = 10000;
    }
    else if(experiment == 5)
    {
        setPolicy(3);
        alpha = 0.5;
        steps = 10000;
    }
    else if(experiment == 6)
    {
        setPolicy(2);
        alpha = 0.5;
        steps = 10000;
    }
}

void initAgent()
{
    sSpace.i = 1;
    sSpace.j = 5;
    sSpace.x = 0;
    bankAccount = 0;
    reward = 0;
}

/*
Operators:
North valid:1 invalid:0
South valid:1 invalid:0
East  valid:1 invalid:0
West  valid:1 invalid:0
*/
void resetAllOperators()
{
    EAST = 0;
    WEST = 0;
    SOUTH = 0;
    NORTH = 0;
}

void checkValidOperators()
{
    resetAllOperators();
    if(sSpace.i == 1 && sSpace.j == 5)
    {
        SOUTH = 1;
        WEST = 1;
    }
    else if(sSpace.i == 5 && sSpace.j == 5)
    {
        NORTH = 1;
        WEST = 1;
    }
    else if(sSpace.i == 5 && sSpace.j == 1)
    {
        NORTH = 1;
        EAST = 1;
    }
    else if(sSpace.i == 1 && sSpace.j == 1)
    {
        SOUTH = 1;
        EAST = 1;
    }
    else if(sSpace.j == 5)
    {
        NORTH = 1;
        SOUTH = 1;
        WEST = 1;
    }
    else if(sSpace.i == 5)
    {
        NORTH = 1;
        EAST = 1;
        WEST = 1;
    }
    else if(sSpace.j == 1)
    {
        NORTH = 1;
        SOUTH = 1;
        EAST = 1;
    }
    else if(sSpace.i == 1)
    {
        EAST = 1;
        SOUTH = 1;
        WEST = 1;
    }
    else
    {
        NORTH = 1;
        SOUTH = 1;
        EAST = 1;
        WEST = 1;
    }

}

int numValidOperators()
{
    int counter = 0;
    if (NORTH == 1)
        counter++;
    if (SOUTH == 1)
        counter++;
    if (EAST == 1)
        counter++;
    if (WEST == 1)
        counter++;

    return counter;
}

void moveNorth()
{
    sSpace.i = sSpace.i - 1;
    bankAccount--;
    reward--;
}

void moveSouth()
{
    sSpace.i = sSpace.i + 1;
    bankAccount--;
    reward--;
}

void moveEast()
{
    sSpace.j = sSpace.j + 1;
    bankAccount--;
    reward--;
}

void moveWest()
{
    sSpace.j = sSpace.j - 1;
    bankAccount--;
    reward--;
}

void pickUp()
{
    sSpace.x++;
    blocks[sSpace.i][sSpace.j]--;
    bankAccount+=13;
    reward+=13;
}

void dropOff()
{
    sSpace.x--;
    blocks[sSpace.i][sSpace.j]++;
    bankAccount+=13;
    reward+=13;
}

void pickRandomOperator()
{
    //srand (time(NULL));
    int rand_num;
    int num = numValidOperators();
    rand_num = rand() % num + 1;

    int counter = 0;
    if(NORTH == 1)
    {
        counter++;
        if (counter == rand_num)
        {
            //ofs << "moving the agent North" << endl;
            Qtab[sSpace.i][sSpace.j][sSpace.x][0] = (1 - alpha)*Qtab[sSpace.i][sSpace.j][sSpace.x][0] +
            alpha* (-1 + gamma * fmax(Qtab[sSpace.i-1][sSpace.j][sSpace.x][0],
                                     fmax(Qtab[sSpace.i-1][sSpace.j][sSpace.x][1],
                                     fmax(Qtab[sSpace.i-1][sSpace.j][sSpace.x][2],
                                     fmax(Qtab[sSpace.i-1][sSpace.j][sSpace.x][3],
                                     fmax(Qtab[sSpace.i-1][sSpace.j][sSpace.x][4],
                                     Qtab[sSpace.i-1][sSpace.j][sSpace.x][5]))))));
            moveNorth();
            return;
        }
    }
    if(SOUTH == 1)
    {
        counter++;
        if (counter == rand_num)
        {
            //ofs << "moving the agent South" << endl;
            Qtab[sSpace.i][sSpace.j][sSpace.x][1] = (1 - alpha)*Qtab[sSpace.i][sSpace.j][sSpace.x][1] +
            alpha* (-1 + gamma * fmax(Qtab[sSpace.i+1][sSpace.j][sSpace.x][0],
                                     fmax(Qtab[sSpace.i+1][sSpace.j][sSpace.x][1],
                                     fmax(Qtab[sSpace.i+1][sSpace.j][sSpace.x][2],
                                     fmax(Qtab[sSpace.i+1][sSpace.j][sSpace.x][3],
                                     fmax(Qtab[sSpace.i+1][sSpace.j][sSpace.x][4],
                                     Qtab[sSpace.i+1][sSpace.j][sSpace.x][5]))))));
            moveSouth();
            return;
        }
    }
    if(EAST == 1)
    {
        counter++;
        if (counter == rand_num)
        {
            //ofs << "moving the agent East" << endl;
            Qtab[sSpace.i][sSpace.j][sSpace.x][2] = (1 - alpha)*Qtab[sSpace.i][sSpace.j][sSpace.x][2] +
            alpha* (-1 + gamma * fmax(Qtab[sSpace.i][sSpace.j+1][sSpace.x][0],
                                     fmax(Qtab[sSpace.i][sSpace.j+1][sSpace.x][1],
                                     fmax(Qtab[sSpace.i][sSpace.j+1][sSpace.x][2],
                                     fmax(Qtab[sSpace.i][sSpace.j+1][sSpace.x][3],
                                     fmax(Qtab[sSpace.i][sSpace.j+1][sSpace.x][4],
                                     Qtab[sSpace.i][sSpace.j+1][sSpace.x][5]))))));
            moveEast();
            return;
        }
    }
    if(WEST == 1)
    {

        counter++;
        if (counter == rand_num)
        {
            //ofs << "moving the agent West" << endl;
            Qtab[sSpace.i][sSpace.j][sSpace.x][3] = (1 - alpha)*Qtab[sSpace.i][sSpace.j][sSpace.x][3] +
            alpha* (-1 + gamma * fmax(Qtab[sSpace.i][sSpace.j-1][sSpace.x][0],
                                     fmax(Qtab[sSpace.i][sSpace.j-1][sSpace.x][1],
                                     fmax(Qtab[sSpace.i][sSpace.j-1][sSpace.x][2],
                                     fmax(Qtab[sSpace.i][sSpace.j-1][sSpace.x][3],
                                     fmax(Qtab[sSpace.i][sSpace.j-1][sSpace.x][4],
                                     Qtab[sSpace.i][sSpace.j-1][sSpace.x][5]))))));
            moveWest();
            return;
        }
    }
}

void pickMaxOperator()
{
    int num = numValidOperators();
    double max_val;
    if (num == 4)
    {
        max_val = fmax(Qtab[sSpace.i][sSpace.j][sSpace.x][0],
                                     fmax(Qtab[sSpace.i][sSpace.j][sSpace.x][1],
                                     fmax(Qtab[sSpace.i][sSpace.j][sSpace.x][2],
                                     (Qtab[sSpace.i][sSpace.j][sSpace.x][3]))));
    }
    else if (num == 3)
    {
        if (NORTH == 1 && SOUTH == 1 && EAST == 1)
        {
            max_val = fmax(Qtab[sSpace.i][sSpace.j][sSpace.x][0],
                                     fmax(Qtab[sSpace.i][sSpace.j][sSpace.x][1],
                                     (Qtab[sSpace.i][sSpace.j][sSpace.x][2])));
        }
        else if (NORTH == 1 && SOUTH == 1 && WEST == 1)
        {
            max_val = fmax(Qtab[sSpace.i][sSpace.j][sSpace.x][0],
                                     fmax(Qtab[sSpace.i][sSpace.j][sSpace.x][1],
                                     (Qtab[sSpace.i][sSpace.j][sSpace.x][3])));
        }
        else if (NORTH == 1 && EAST == 1 && WEST == 1)
        {
            max_val = fmax(Qtab[sSpace.i][sSpace.j][sSpace.x][0],
                                     fmax(Qtab[sSpace.i][sSpace.j][sSpace.x][2],
                                     (Qtab[sSpace.i][sSpace.j][sSpace.x][3])));
        }
        else if (SOUTH == 1 && EAST == 1 && WEST == 1)
        {
            max_val = fmax(Qtab[sSpace.i][sSpace.j][sSpace.x][1],
                                     fmax(Qtab[sSpace.i][sSpace.j][sSpace.x][2],
                                     (Qtab[sSpace.i][sSpace.j][sSpace.x][3])));
        }
    }
    else if (num == 2)
    {
        if(NORTH == 1 && EAST == 1)
        {
            max_val = fmax(Qtab[sSpace.i][sSpace.j][sSpace.x][0],
                                     (Qtab[sSpace.i][sSpace.j][sSpace.x][2]));
        }
        else if (NORTH == 1 && WEST == 1)
        {
            max_val = fmax(Qtab[sSpace.i][sSpace.j][sSpace.x][0],
                                     (Qtab[sSpace.i][sSpace.j][sSpace.x][3]));
        }
        else if(SOUTH == 1 && EAST == 1)
        {
            max_val = fmax(Qtab[sSpace.i][sSpace.j][sSpace.x][1],
                                     (Qtab[sSpace.i][sSpace.j][sSpace.x][2]));
        }
        else if (SOUTH == 1 && WEST == 1)
        {
            max_val = fmax(Qtab[sSpace.i][sSpace.j][sSpace.x][1],
                                     (Qtab[sSpace.i][sSpace.j][sSpace.x][3]));
        }
    }
    if (max_val != Qtab[sSpace.i][sSpace.j][sSpace.x][0])
    {
        NORTH = 0;
    }
    if (max_val != Qtab[sSpace.i][sSpace.j][sSpace.x][1])
    {
        SOUTH = 0;
    }
    if (max_val != Qtab[sSpace.i][sSpace.j][sSpace.x][2])
    {
        EAST = 0;
    }
    if (max_val != Qtab[sSpace.i][sSpace.j][sSpace.x][3])
    {
        WEST = 0;
    }
    pickRandomOperator();
}

void pickOperator()
{
    //srand (time(NULL));
    int pick;
    pick = rand() % 100;

    if (policy == 2)
        if (pick < 65)
        {
            pickMaxOperator();
            return;
        }
    if (policy == 3)
        if (pick < 90)
        {
            pickMaxOperator();
            return;
        }

    pickRandomOperator();

}

void printQtable()
{
    ofs << "State   NORTH   SOUTH   EAST   WEST   PICKUP   DROPOFF" << endl;
    ofs << setprecision(2) << fixed;
    for(int k = 0; k<2; ++k)
    {
        for(int i = 1; i < 6; ++i)
        {
            for(int j = 1; j < 6; ++j)
            {
                //for(int k = 0; k<2; ++k)
                //{
                ofs << "(" << i << "," << j << "," << k << ")  ";
                //for(int l = 0; l<6; ++l)
                //{
                    ofs << Qtab[i][j][k][0] << "   ";
                    ofs << Qtab[i][j][k][1] << "   ";
                    ofs << Qtab[i][j][k][3] << "   ";
                    ofs << Qtab[i][j][k][2] << "   ";
                    ofs << Qtab[i][j][k][4] << "   ";
                    ofs << Qtab[i][j][k][5];
                //}
                ofs << endl;
                //}
            }
            //ofs << endl;
        }
        //ofs << endl;
    }
    ofs << "Agent's bank account: " << bankAccount << endl << endl;
    ofs << "step number\t\tReward\t\tBlocks Delivered" << endl;
}

void beginExperiment()
{
    // run for number of steps specified
    // check which operators are valid
    //select operator as per the policy
    // update Q table at every step

    term = 0;
    first = 0;
    drop = 0;
    ofs << "step number\t\tReward\t\tBlocks Delivered\t\tBank Acoount" << endl;
    while(steps--)
    {
        if((10000-steps)%50 == 0)
        {
            //ofs << setprecision(3) << fixed;
            ofs << 10000-steps;
            ofs << "\t\t\t" << reward;
            ofs << "\t\t\t" << drop;
            ofs << "\t\t\t" << bankAccount << endl;
            reward = 0;
        }
        if(10000-steps == 101) //This will print the Q-table after first 100 steps
        {
            ofs << "Q table after 100 steps:" << endl;
            printQtable();
        }
        if((blocks[5][1] == 5 || blocks[5][3] == 5 || blocks[2][5] == 5) && first == 0)  //This will print the Q-table when the first drop-off location is filled
        {
            ofs << "first drop-off location is filled. Q table:" << endl;
            first++;
            printQtable();
        }
        if((blocks[1][1] == 0 && blocks[3][3] == 0 && blocks[5][5] == 0 && blocks[5][1] == 5 && blocks[5][3] == 5 && blocks[2][5] == 5) ||
           (experiment == 5 && term > 1 && blocks[1][1] == 5 && blocks[3][3] == 5 && blocks[5][5] == 5 && blocks[5][1] == 0 && blocks[5][3] == 0 && blocks[2][5] == 0))
        {
            term++;
            ofs << "step number " << 10000-steps << endl;
            ofs << "total blocks delivered are " << drop << endl;
            drop = 0;
            ofs << "TERMINATION STATE REACHED!! Q table:" << endl;
            printQtable();
            if(experiment < 5 || term < 2)
            {
                initBlocks();
                initAgent();
            }
            else if (experiment == 5)
            {
                initBlocks();
                blocks[1][1] = 0;
                blocks[3][3] = 0;
                blocks[5][5] = 0;
                blocks[5][1] = 5;
                blocks[5][3] = 5;
                blocks[2][5] = 5;
                initAgent();
            }
            else if (experiment == 6)
            {
                initBlocks();
                blocks[1][1] = 0;
                blocks[3][3] = 0;
                blocks[5][5] = 0;
                blocks[2][2] = 5;
                blocks[4][4] = 5;
                blocks[1][5] = 5;
                initAgent();
            }


            if(term == 4 && experiment > 1)
                break;
        }

        //ofs << isPickUp() << endl;
        if(sSpace.x == 0 && isPickUp())
        {
            //ofs << "step number " << 10000-steps << endl;
            //ofs << "Pick up occurs" << endl;
            //pick it up
            //take the rewardofsofsofsofs
            Qtab[sSpace.i][sSpace.j][sSpace.x][4] = (1 - alpha)*Qtab[sSpace.i][sSpace.j][sSpace.x][4] +
            alpha* (13 + gamma * fmax(Qtab[sSpace.i][sSpace.j][1][0],
                                     fmax(Qtab[sSpace.i][sSpace.j][1][1],
                                     fmax(Qtab[sSpace.i][sSpace.j][1][2],
                                     fmax(Qtab[sSpace.i][sSpace.j][1][3],
                                     fmax(Qtab[sSpace.i][sSpace.j][1][4],
                                     Qtab[sSpace.i][sSpace.j][1][5]))))));
            pickUp();
            continue;
        }
        else if(sSpace.x == 1 && isDropOff())
        {
            //ofs << "step number " << 10000-steps << endl;
            //ofs << "Drop off occurs" << endl;
            //Drop it off
            //take the reward
            drop++;
            Qtab[sSpace.i][sSpace.j][sSpace.x][5] = (1 - alpha)*Qtab[sSpace.i][sSpace.j][sSpace.x][5] +
            alpha* (13 + gamma * fmax(Qtab[sSpace.i][sSpace.j][0][0],
                                     fmax(Qtab[sSpace.i][sSpace.j][0][1],
                                     fmax(Qtab[sSpace.i][sSpace.j][0][2],
                                     fmax(Qtab[sSpace.i][sSpace.j][0][3],
                                     fmax(Qtab[sSpace.i][sSpace.j][0][4],
                                     Qtab[sSpace.i][sSpace.j][0][5]))))));
            dropOff();
            continue;
        }
        if((experiment == 1) || ((10000 - steps) < 100) )
        {
            checkValidOperators();
            pickRandomOperator(); //excluding pick up and drop off
        }
        else if (experiment >= 2)
        {
            checkValidOperators();
            pickOperator();
        }
    }




}

int main()
{
    //srand (time(NULL));
    //ofs << "Random seed is:" << time(NULL) << endl;
    initQtab();
    initBlocks();
    initAgent();
    int seed;

    // select experiment 1/2/3/4/5/6
    for(int z = 1; z < 7; z++)
    {
        for(int y = 1; y<3; y++)
        {
            if (y == 1)
                seed = 1479441548;
            else
                seed = 1479423512;
            srand (seed);
            ofs << "Experiment number " << z << "       Run #" << y << "     Seed is " << seed << endl;
            initQtab();
            initBlocks();
            initAgent();
            setExperiment(z);
            beginExperiment();
        }
    }

    //printQtable();

    return 0;
}
