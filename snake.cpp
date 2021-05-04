#include <iostream>
#include <string>
#include <stdio.h>
#include <fstream>
#include <thread>
#include <sys/select.h>
#include <termios.h>
#include <stropts.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <cstdlib>
#include <ctime>
//#include <conio.h>

using namespace std;

ofstream out("result.txt");

int _kbhit() {
    static const int STDIN = 0;
    static bool initialized = false;

    if (! initialized) {
        // Use termios to turn off line buffering
        termios term;
        tcgetattr(STDIN, &term);
        term.c_lflag &= ~ICANON;
        tcsetattr(STDIN, TCSANOW, &term);
        setbuf(stdin, NULL);
        initialized = true;
    }

    int bytesWaiting;
    ioctl(STDIN, FIONREAD, &bytesWaiting);
    return bytesWaiting;
}

struct Parte{
    int x=0;
    int y=0;
};

int getch(void)
{
	struct termios oldt, newt;
	int ch;

	tcgetattr(STDIN_FILENO, &oldt);
	newt = oldt;
	newt.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);
	ch = getchar();
	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

	return ch;
}

int plansa_de_joc[37][37];
int points=0;
int save_x;  // save_x memorises the last apple eaten on x coordonates
int save_y;  // save_y memorises the last apple eaten on y coordonates
int head_x;  // head_x memorises the previous position of the head of the snake on x coordonates
int head_y;  // head_y memorises the previous position of the head of the snake on y coordonates
int tour=0;  // tour memorises how much points to add to the snake after eating
bool increment=false;
Parte parte[35*35+1];
bool play=true;

int main()
{
    for(int i=0;i<=36;i++)
    {
        plansa_de_joc[0][i]=9;
        plansa_de_joc[36][i]=9;
    }
    for(int i=0;i<=36;i++)
    {
        plansa_de_joc[i][0]=9;
        plansa_de_joc[i][36]=9;
    }
    srand(time(0));
    parte[0].x=rand()%34+1;
    parte[0].y=rand()%34+1;
    int pozitie_y=rand()%34+1;
    int pozitie_x=rand()%34+1;
    bool right=false;
    bool left=false;
    bool up=false;
    bool down=false;
    while(play)
    {
        // clear the map
        for(int i=1;i<=35;i++)
        {
            for(int j=1;j<=35;j++)
            {
                plansa_de_joc[i][j]=0;
            }
        }
        // redraw the updated positions of the segments of the snake
        for(int i=0;i<=points;i++)
        {
            plansa_de_joc[parte[i].y][parte[i].x]=-5;
        }
        plansa_de_joc[pozitie_y][pozitie_x]=1;
        // write the map
        for (int i=0;i<=36;i++)
        {
            for (int j=0;j<=36;j++)
            {
                if(plansa_de_joc[i][j]==0)
                {
                    cout<<"  ";
                }
                else if(plansa_de_joc[i][j]==-5)
                {
                    cout<<"o ";
                }
                else if(plansa_de_joc[i][j]==1)
                {
                    cout <<"* ";
                }
                else if(plansa_de_joc[i][j]==9)
                {
                    cout<<"# ";
                }
            }
            cout<<endl;
        }
        cout<<"Press p for pause"<<endl;
        cout<<"Press q to quit the game"<<endl;
        cout<<"Score: "<<points<<endl;
        cout<<"tour: "<<tour<<endl;
        //system("stty raw");
        if(_kbhit())
        {
            switch (getch())
            {
                case 'a':
                    if(points==0)
                    {
                        right=false;
                        left=true;
                        up=false;
                        down=false;
                    }
                    else if(right==false)
                    {
                        right=false;
                        left=true;
                        up=false;
                        down=false;
                    }
                    break;
                case 's':
                    if(points==0)
                    {
                        right=false;
                        left=false;
                        up=false;
                        down=true;
                    }
                    else if(up==false)
                    {
                        right=false;
                        left=false;
                        up=false;
                        down=true;
                    }
                    break;
                case 'd':
                    if(points==0)
                    {
                        right=true;
                        left=false;
                        up=false;
                        down=false;
                    }
                    else if(left==false)
                    {
                        right=true;
                        left=false;
                        up=false;
                        down=false;
                    }
                    break;
                case 'w':
                    if(points==0)
                    {
                        right=false;
                        left=false;
                        up=true;
                        down=false;
                    }
                    else if(down==false)
                    {
                        right=false;
                        left=false;
                        up=true;
                        down=false;
                    }
                    break;
                case 'q':
                    play=false;
                    break;
                case 'p':
                    //system("stty cooked");
                    //system("clear");
                    cout<<"PRESS ANY KEY TO CONTINUE PLAYING"<<endl;
                    //system("stty raw");
                    char restart=getchar();
                    break;
            }
        }
        if(right)
        {
            plansa_de_joc[parte[0].y][parte[0].x]=0;
            if(plansa_de_joc[parte[0].y][parte[0].x+1]==9 || plansa_de_joc[parte[0].y][parte[0].x+1]==-5)
            {
                play=false;
            }
            if(play && plansa_de_joc[parte[0].y][parte[0].x+1]==1)
            {
                save_x=parte[0].x+1;
                save_y=parte[0].y;
                tour++;
                pozitie_y=rand()%34+1;
                pozitie_x=rand()%34+1;
            }
            head_x=parte[0].x;
            head_y=parte[0].y;
            parte[0].x++;
            plansa_de_joc[parte[0].y][parte[0].x]=-5;
            // follow algorithm
            for(int i=1;i<=points;i++)
            {
                int aux1=parte[i].x;
                int aux2=parte[i].y;
                parte[i].x=head_x;
                parte[i].y=head_y;
                head_x=aux1;
                head_y=aux2;
            }
            if(increment)
            {
                if(tour!=0)
                {
                    points++;
                    parte[points].x=save_x;
                    parte[points].y=save_y;
                    tour--;
                }
                if(tour==0)
                {
                    increment=false;
                }
            }
            if(parte[points].x==save_x && parte[points].y==save_y)
            {
                increment=true;
            }
        }
        else if(left)
        {
            plansa_de_joc[parte[0].y][parte[0].x]=0;
            if(plansa_de_joc[parte[0].y][parte[0].x-1]==9 || plansa_de_joc[parte[0].y][parte[0].x-1]==-5)
            {
                play=false;
            }
            if(play && plansa_de_joc[parte[0].y][parte[0].x-1]==1)
            {
                save_x=parte[0].x-1;
                save_y=parte[0].y;
                tour++;
                pozitie_y=rand()%34+1;
                pozitie_x=rand()%34+1;
            }
            head_x=parte[0].x;
            head_y=parte[0].y;
            parte[0].x--;
            plansa_de_joc[parte[0].y][parte[0].x]=-5;
            // follow algorithm
            for(int i=1;i<=points;i++)
            {
                int aux1=parte[i].x;
                int aux2=parte[i].y;
                parte[i].x=head_x;
                parte[i].y=head_y;
                head_x=aux1;
                head_y=aux2;
            }
            if(increment)
            {
                if(tour!=0)
                {
                    points++;
                    parte[points].x=save_x;
                    parte[points].y=save_y;
                    tour--;
                }
                if(tour==0)
                {
                    increment=false;
                }
            }
            if(parte[points].x==save_x && parte[points].y==save_y)
            {
                increment=true;
            }
        }
        else if(up)
        {
            plansa_de_joc[parte[0].y][parte[0].x]=0;
            if(plansa_de_joc[parte[0].y-1][parte[0].x]==9 || plansa_de_joc[parte[0].y-1][parte[0].x]==-5)
            {
                play=false;
            }
            if(play && plansa_de_joc[parte[0].y-1][parte[0].x]==1)
            {
                save_x=parte[0].x;
                save_y=parte[0].y-1;
                tour++;
                pozitie_y=rand()%34+1;
                pozitie_x=rand()%34+1;
            }
            head_x=parte[0].x;
            head_y=parte[0].y;
            parte[0].y--;
            plansa_de_joc[parte[0].y][parte[0].x]=-5;
            // follow algorithm
            for(int i=1;i<=points;i++)
            {
                int aux1=parte[i].x;
                int aux2=parte[i].y;
                parte[i].x=head_x;
                parte[i].y=head_y;
                head_x=aux1;
                head_y=aux2;
            }
            if(increment)
            {
                if(tour!=0)
                {
                    points++;
                    parte[points].x=save_x;
                    parte[points].y=save_y;
                    tour--;
                }
                if(tour==0)
                {
                    increment=false;
                }
            }
            if(parte[points].x==save_x && parte[points].y==save_y)
            {
                increment=true;
            }
        }
        else if(down)
        {
            plansa_de_joc[parte[0].y][parte[0].x]=0;
            if(plansa_de_joc[parte[0].y+1][parte[0].x]==9 || plansa_de_joc[parte[0].y+1][parte[0].x]==-5)
            {
                play=false;
            }
            if(play && plansa_de_joc[parte[0].y+1][parte[0].x]==1)
            {
                save_x=parte[0].x;
                save_y=parte[0].y+1;
                tour++;
                pozitie_y=rand()%34+1;
                pozitie_x=rand()%34+1;
            }
            head_x=parte[0].x;
            head_y=parte[0].y;
            parte[0].y++;
            plansa_de_joc[parte[0].y][parte[0].x]=-5;
            // follow algorithm
            for(int i=1;i<=points;i++)
            {
                int aux1=parte[i].x;
                int aux2=parte[i].y;
                parte[i].x=head_x;
                parte[i].y=head_y;
                head_x=aux1;
                head_y=aux2;
            }
            if(increment)
            {
                if(tour!=0)
                {
                    points++;
                    parte[points].x=save_x;
                    parte[points].y=save_y;
                    tour--;
                }
                if(tour==0)
                {
                    increment=false;
                }
            }
            if(parte[points].x==save_x && parte[points].y==save_y)
            {
                increment=true;
            }
        }
        // the speed of the snake (if nr grows, the speed drops!)
        int nr=0;
        while(nr<=30000000)
        {
            nr++;
        }
        //system("clear");
        //system("stty cooked");
    }
    cout<<"GAME OVER!";
    return 0;
}
