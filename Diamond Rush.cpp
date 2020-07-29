/*
iGraphics Group project
Section A2,Group 5
1805032 & 1805048
*/
# include "iGraphics.h"
#include<windows.h>
#include<bits/stdc++.h>
#include<stdio.h>
#include<stdlib.h>
#define wall 1
#define stone 3
#define grass 4
#define diamond 5
#define fire 6
#define fire_nob 7
#define snake 8
#define finish 9
#define torch 10

#define manu 'm'
#define game 'g'
#define gameover 'o'
#define win 'w'
#define End 'e'
#define Halloffame 'h'
#define Info 'i'


int index_r=10, index_c=0;
//index_r and index_c is the initial point from where the screen will show BMP images
int x=1,y=18;
//x and y is the position of the sprite
int SpriteStatus=1;
int DIAMOND_EARNED=0;
int TOTAL_DIAMOND=33;

char GameState='m';
char firey[3][20]={"Fire1.bmp","Fire2.bmp","Fire3.bmp"};

int render=0;
char diamondbmp[3][20]={"Diamond.bmp","Diamond2.bmp","Diamond3.bmp"};
char Torchbmp[3][20]={"torch1.bmp","torch2.bmp","torch3.bmp"};

int FireRange=0,l;
int GameOver=0;
int StoneOnTop=0;
int music=1;
int cnt=0;
int col1=7,col2=28,col3=40,z=-1,p=1,q=1;
int Grid[22][51];

char Player_name[20],temp_str[20],str[20];
int Str_len=0;

int i;
typedef struct{
    char Name[20]="";
    int Time=0.0;
}Score;
Score ScoreArr[6],temp;

char* StringShow();
void MoveRight();
void MoveLeft();
void MoveUp();
void MoveDown();
void Fall();
void SideFall();
void iShowGame();
void LoadGame();
void NewGame();
void GameOverWindow();
void snakeyfakey1();
void snakeyShaky2();
void snakeymazy3();
void addfire();
void DeathByStoneCheck();
void SaveGame();
void HallOfFame();
void timekeep();
void UpdateHallOfFame();
void iShowInfo();
void UpdateRender(){
    render=(render+1)%3;
}

void iDraw()
{
	iClear();

	if(GameState==manu){
        iShowBMP(0,0,"Main Manu.bmp");
        iShowBMP2(870,570,"Help.bmp",0);
        if(music==1)iShowBMP2(930,570,"MusicOn.bmp",0);
        else if(music==0)iShowBMP2(930,570,"MusicOff.bmp",0);
	}
	else if(GameState==game)iShowGame();
    else if(GameState==gameover)iShowBMP(0,0,"GameOver.bmp");
    else if(GameState==End)iShowBMP(0,0,"Win.bmp");
    else if(GameState==win){
        iShowBMP(0,0,"Win_name.bmp");
        iText(330,150,Player_name,GLUT_BITMAP_TIMES_ROMAN_24);
    }

    else if(GameState==Halloffame)HallOfFame();
    else if(GameState==Info)iShowInfo();
    if(GameOver==1)cnt++;
    if(cnt==3)GameOverWindow();
}

void iMouseMove(int mx, int my)
{

}

void iMouse(int button, int state, int mx, int my)
{
	if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
       // printf("%d %d\n",mx,my);
        if(mx>=600 && mx<=890 &&my>=25 && my<=112 && GameState==manu)exit(0);
        else if(mx>=600 && mx<=890 && my>=125 && my<=212 && GameState==manu){
            //user clicked hall of fame
            GameState=Halloffame;
            FILE *fp3=fopen("HallOfFame.txt","r");
            for(i=0;i<5;i++){
                fscanf(fp3,"%s %d",&ScoreArr[i].Name,&ScoreArr[i].Time);
            }
            fclose(fp3);
            for(i=0;i<5;i++){
                //printf("%s %d\n",ScoreArr[i].Name,ScoreArr[i].Time);
            }

        }
        else if(mx>=600 && mx<=850 && my>=224 && my<=310 && GameState==manu){
            //user clicked load game
            LoadGame();
            PlaySound(0,0,0);
            GameState=game;
        }
        else if(mx>=600 && mx<=890 && my>=324 && my<=415 && GameState==manu){
            //user clicked new game
            NewGame();
            PlaySound(0,0,0);
            GameState=game;
        }
        else if(mx>=930 && mx<=980 && my>=570 && my<=620 && GameState==manu){
            //user clicked music change button.
            if(music==1){
                music=0;
                PlaySound(0,0,0);
            }
            else{
                music=1;
                PlaySound("playMenu.wav",NULL,SND_LOOP | SND_ASYNC);
            }
        }
        else if(mx>=870 && mx<=920 && my>=570 && my<=620 && GameState==manu)GameState=Info;
        else if(mx>=300 && mx<=600 && my>=100 && my<=200 && GameState==gameover){
            GameState=manu;
            music=1;
            PlaySound("playMenu.wav",NULL,SND_LOOP | SND_ASYNC);
        }
        else if(mx>=300 && my<=600 && my>=100 && my<=200 && GameState==End){
            GameState=manu;
        }
        else if(mx>=10 && mx<=60 && my>=545 && my<=595 && GameState==Halloffame){
            GameState=manu;
        }
        else if(mx>=10 && mx<=60 && my>=545 && my<=595 && GameState==Info){
            GameState=manu;
        }
	}
	if(button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN){
	}
}

void iKeyboard(unsigned char key)
{
    int i;
	if(GameState == win)
	{
        if(key == '\r')
		{
			GameState = Halloffame;
			strcpy(Player_name, temp_str);
			for(i = 0; i < Str_len; i++)temp_str[i] = 0;
			strcpy(ScoreArr[5].Name,Player_name);
			for(i = 0; i < Str_len; i++)Player_name[i] = 0;
			Str_len = 0;
			UpdateHallOfFame();
		}
		else
		{
			temp_str[Str_len] = key;

			Str_len++;
			strcpy(Player_name, temp_str);
		}
	}
    if(key == 's'){

        SaveGame();
    }
    if(key == 'm')GameState=manu;
}

void iSpecialKeyboard(unsigned char key)
{

	if(key == GLUT_KEY_END){
		exit(0);
	}
	if(key == GLUT_KEY_LEFT){
        if(SpriteStatus!=-1 && Grid[y-1][x]!=0)SpriteStatus=-1;
		else if(x>=1 && Grid[y][x-1]==0)MoveLeft();
		else if(Grid[y][x-1]==fire)MoveLeft();
		else if(Grid[y][x-1]==snake){
            MoveLeft();
            GameOver=1;
		}
		else if(Grid[y][x-1]==snake)MoveLeft();
        else if(x>=1 && Grid[y][x-1]==diamond){
            if(music==1)PlaySound("playDiamond.wav",NULL,SND_ASYNC);
            MoveLeft();
            Grid[y][x]=0;
            DIAMOND_EARNED++;
        }
        else if(x>=1 && Grid[y][x-1]==grass){
            if(music==1)PlaySound("playGrass.wav",NULL,SND_ASYNC);
            MoveLeft();
            Grid[y][x]=0;
        }
        else if(x>=1 && Grid[y][x-1]==stone && Grid[y][x-2]==0){
            Grid[y][x-1]=0;
            Grid[y][x-2]=stone;
            if(music==1)PlaySound("playStone.wav",NULL,SND_ASYNC);
            MoveLeft();
        }
	}
	if(key == GLUT_KEY_RIGHT){
	    if(SpriteStatus!=1 && Grid[y-1][x]!=0)SpriteStatus=1;
		else if(x<49 && Grid[y][x+1]==0)MoveRight();
		else if(Grid[y][x+1]==fire)MoveRight();
		else if(Grid[y][x+1]==snake){
            MoveRight();
            GameOver=1;
		}
		else if(Grid[y][x+1]==finish)MoveRight();
		else if(Grid[y][x+1]==snake)MoveRight();
		else if(x<49 && Grid[y][x+1]==diamond){
            if(music==1)PlaySound("playDiamond.wav",NULL,SND_ASYNC);
            MoveRight();
            Grid[y][x]=0;
            DIAMOND_EARNED++;
		}
		else if(x<49 && Grid[y][x+1]==grass){
            if(music==1)PlaySound("playGrass.wav",NULL,SND_ASYNC);
            MoveRight();
            Grid[y][x]=0;
		}
		else if(x<49 && Grid[y][x+1]==stone && Grid[y][x+2]==0){
            Grid[y][x+1]=0;
            Grid[y][x+2]=stone;
            if(music==1)PlaySound("playStone.wav",NULL,SND_ASYNC);
            MoveRight();
		}
	}
	if(key == GLUT_KEY_UP){
		if(y<=20 && Grid[y+1][x]==0)MoveUp();
		else if(Grid[y+1][x]==fire)MoveUp();
		else if(Grid[y+1][x]==snake)MoveUp();
		else if(Grid[y+1][x]==snake){
            MoveUp();
            GameOver=1;
		}
		else if(y<=20 && Grid[y+1][x]==diamond){
            if(music==1)PlaySound("playDiamond.wav",NULL,SND_ASYNC);
            MoveUp();
            Grid[y][x]=0;
            DIAMOND_EARNED++;
		}
		else if(y<=20 && Grid[y+1][x]==grass){
            if(music==1)PlaySound("playGrass.wav",NULL,SND_ASYNC);
            MoveUp();
            Grid[y][x]=0;
		}
	}
	if(key == GLUT_KEY_DOWN){
		if(y>0 && Grid[y-1][x]==0)MoveDown();
		else if(Grid[y-1][x]==fire)MoveDown();
		else if(Grid[y-1][x]==snake)MoveDown();
		else if(Grid[y-1][x]==snake){
            MoveDown();
            GameOver=1;
		}
		else if(y>0 && Grid[y-1][x]==diamond){
            if(music==1)PlaySound("playDiamond.wav",NULL,SND_ASYNC);
            MoveDown();
            Grid[y][x]=0;
            DIAMOND_EARNED++;
		}
		else if(y>0 && Grid[y-1][x]==grass){
            if(music==1)PlaySound("playGrass.wav",NULL,SND_ASYNC);
            MoveDown();
            Grid[y][x]=0;
		}
	}
}

int main()
{
    NewGame();
    FILE *fp3=fopen("HallOfFame.txt","r");
    for(i=0;i<5;i++){
        fscanf(fp3,"%s\t%d\t%.02f\n",&ScoreArr[i].Name,&ScoreArr[i].Time);
    }
    fclose(fp3);
    iSetTimer(500,DeathByStoneCheck);
    iSetTimer(400,addfire);
    iSetTimer(500,SideFall);
    iSetTimer(1000,snakeyfakey1);
    iSetTimer(500,snakeyShaky2);
    iSetTimer(700,snakeymazy3);
    iSetTimer(1000,timekeep);
    iSetTimer(800,UpdateRender);
    if(music==1)PlaySound("playMenu.wav",NULL,SND_LOOP | SND_ASYNC);
	iInitialize(1000,620, "Diamond Rush");

	return 0;
}

char* StringShow(){
    char *ans=(char *)malloc(80*sizeof(char));
    char q[20]="Diamond- ";
    char p[20];
    strcpy(ans,q);
    *p=(DIAMOND_EARNED/10+'0');
    *(p+1)='\0';
    strcat(ans,p);
    *p=(DIAMOND_EARNED%10+'0');
    *(p+1)='\0';
    strcat(ans,p);
    return ans;
}
void MoveRight()
{
    x++;
    if(x>=10 && x<=40)index_c++;
    if(x>50)x=50;
}

void MoveLeft(){
    x--;
    if(x<=40){
        index_c--;
        if(index_c<0)index_c=0;
    }
}
void MoveDown(){
    y--;
    if(y<15)index_r--;
    if(index_r<0)index_r=0;
}

void MoveUp(){
    y++;
    if(y>=5)index_r++;
    if(index_r>10)index_r=10;
}

void Fall(){
    for(int i=1;i<22;i++){
        for(int j=0;j<51;j++){
            if(Grid[i][j]==stone && Grid[i-1][j]==0 && !(i-1==y && j==x)){
                Grid[i][j]=0;
                Grid[i-1][j]=stone;
                if(music==1)PlaySound("playStone.wav",NULL,SND_ASYNC);
                if(i-2==y && j==x){
                    GameOver=1;
                }
                break;
            }
            if(Grid[i][j]==diamond && Grid[i-1][j]==0 && !(i-1==y && j==x)){
                Grid[i][j]=0;
                Grid[i-1][j]=diamond;
                break;
            }
        }
    }
}

void SideFall(){
    for(int i=1;i<22;i++){
        for(int j=0;j<51;j++){
            //stone would go sidewards
            if(Grid[i][j]==stone && (Grid[i-1][j]==stone || Grid[i-1][j]==diamond) && Grid[i][j-1]==0 && !(i==y && j-1==x) && Grid[i-1][j-1]==0 && !(i-1==y && j-1==x)){
                Grid[i][j]=0;
                Grid[i-1][j-1]=stone;
                if(music==1)PlaySound("playStone.wav",NULL,SND_ASYNC);
                if(i-2==y && j-1==x)GameOver=1;
                break;
            }
            if(Grid[i][j]==stone && (Grid[i-1][j]==stone || Grid[i-1][j]==diamond) && Grid[i][j+1]==0 && !(i==y && j+1==x) && Grid[i-1][j+1]==0 && !(i-1==y && j+1==x)){
                Grid[i][j]=0;
                Grid[i-1][j+1]=stone;
                if(music==1)PlaySound("playStone.wav",NULL,SND_ASYNC);
                if(i-2==y && j+1==x)GameOver=1;
                break;
            }
        }
    }
}

void iShowGame(){
    if(GameOver==1)iText(100,605,"GAMEOVER",GLUT_BITMAP_TIMES_ROMAN_10);
    for(int i=0;i<12;i++){
        for(int j=0;j<20;j++){
            if(Grid[index_r+i][index_c+j]==0)iShowBMP(j*50,i*50,"walkway.bmp");
            else if(Grid[index_r+i][index_c+j]==wall)iShowBMP(j*50,i*50,"Wall.bmp");
            else if(Grid[index_r+i][index_c+j]==2)iShowBMP(j*50,i*50,"Wall2.bmp");
            else if(Grid[index_r+i][index_c+j]==stone)iShowBMP(j*50,i*50,"Stone.bmp");
            else if(Grid[index_r+i][index_c+j]==grass)iShowBMP(j*50,i*50,"Grass.bmp");
            else if(Grid[index_r+i][index_c+j]==torch){
                iShowBMP(j*50,(i+1)*50,"Wall2.bmp");
                iShowBMP(j*50,i*50,"Wall2.bmp");
                iShowBMP2(j*50,i*50,Torchbmp[render],0);
            }
            else if(Grid[index_r+i][index_c+j]==diamond)iShowBMP(j*50,i*50,diamondbmp[render]);
            else if(Grid[index_r+i][index_c+j]==fire_nob){
                iShowBMP(j*50,i*50,"Lion.bmp");
                for(l=0;l<FireRange;l++){
                    iShowBMP((j+l+1)*50,i*50,firey[l]);
                    if(index_c+j+l+1==x && index_r+i==y)GameOver=1;
                }
                for(l=FireRange;l<=3;l++)iShowBMP((j+l+1)*50,i*50,"walkway.bmp");
            }
            else if(Grid[index_r+i][index_c+j]==snake)iShowBMP(j*50,i*50,"Snake.bmp");
            else if(Grid[index_r+i][index_c+j]==finish){
                iShowBMP(j*50,i*50,"end.bmp");
                if(index_c+j==x && index_r+i==y){
                    if(DIAMOND_EARNED==TOTAL_DIAMOND)GameState=win;
                    else GameState=End;
                }
            }
        }
    }
    if(!(Grid[y-1][x]==wall || Grid[y-1][x]==diamond || Grid[y-1][x]==stone || Grid[y-1][x]==grass) )iShowBMP((x-index_c)*50,(y-index_r)*50,"Sprite_up.bmp");
    else if(SpriteStatus==1)iShowBMP((x-index_c)*50,(y-index_r)*50,"Sprite_right.bmp");
    else if(SpriteStatus==-1)iShowBMP((x-index_c)*50,(y-index_r)*50,"Sprite_left.bmp");
    iText(0,605,StringShow(),GLUT_BITMAP_TIMES_ROMAN_10);
    Fall();
}

void LoadGame(){
    int u,v,w;
    FILE *q;
    q = fopen("SAVEDGAME.txt","r");
    if(q==NULL){
        printf("ERROR");
    }
    while(1){
        fscanf(q,"%d %d %d",&u,&v,&w);
        if(u==0 && v==0 && w==0)break;
        Grid[u][v]=w;
    }
    fscanf(q,"%d %d",&x,&y);
    fscanf(q,"%d %d",&index_r,&index_c);
    fscanf(q,"%d",&DIAMOND_EARNED);
    fscanf(q,"%d",&ScoreArr[5].Time);
    GameOver=0;
    cnt=0;
    fclose(q);

}

void NewGame(){
    index_r=10;
    index_c=0;
    x=1;
    y=18;
    int u,v,w;
    GameOver=0;
    DIAMOND_EARNED=0;
    ScoreArr[5].Time=0;
    cnt=0;
    FILE *p;
    p = fopen("INPUT.txt","r");
    while(1){
        fscanf(p,"%d %d %d",&u,&v,&w);
        if(u==0 && v==0 && w==0)break;
        Grid[u][v]=w;
    }
    fclose(p);
}

void GameOverWindow(){
    Sleep(1000);
    GameState=gameover;
}


void snakeyfakey1()
{
    if(Grid[col1+z][26]==0){
        Grid[col1][26]=0;
        col1+=z;
        Grid[col1][26]=snake;
    }
    else z*=(-1);
    if(x==26 && y==col1)GameOver=1;
}
void snakeyShaky2()
{
    if(Grid[8][col2+p]==0){
        Grid[8][col2]=0;
        col2+=p;
        Grid[8][col2]=snake;
    }
    else p*=(-1);
    if(x==col2 && y==8)GameOver=1;
}
void snakeymazy3()
{
    if(Grid[6][col3+q]==0){
        Grid[6][col3]=0;
        col3+=q;
        Grid[6][col3]=snake;
    }
    else q*=(-1);
    if(x==col3 && y==6)GameOver=1;
}

void addfire(){
    FireRange+=1;
    if(FireRange==4)FireRange=0;
}

void DeathByStoneCheck(){
    if(Grid[y+1][x]==stone && StoneOnTop<6){
        StoneOnTop+=1;
    }
    else if(Grid[y+1][x]==stone && StoneOnTop==6){
        GameOver=1;
    }
    else if(Grid[y+1][x]!=stone)StoneOnTop=0;
}


void SaveGame(){
    FILE *p;
    p=fopen("SAVEDGAME.txt","w");
    int i,j;
    for(i=0;i<22;i++){
        for(j=0;j<51;j++){
            if(Grid[i][j]!=snake)fprintf(p,"%d %d %d\n",i,j,Grid[i][j]);
            else fprintf(p,"%d %d %d\n",i,j,0);
        }
    }
    fprintf(p,"%d %d %d\n",0,0,0);
    fprintf(p,"%d %d\n",x,y);
    fprintf(p,"%d %d\n",index_r,index_c);
    fprintf(p,"%d\n",DIAMOND_EARNED);
    fprintf(p,"%d\n",ScoreArr[5].Time);
    fclose(p);
}

void HallOfFame(){
    iShowBMP(0,0,"Halloffamewindow.bmp");
    iShowBMP(10,545,"BackButton.bmp");
    char showScore[5][100];
    iSetColor(255,255,0);
    iText(260,300,"Rank",GLUT_BITMAP_TIMES_ROMAN_24);
    int ind=250;
    for(i=0;i<5;i++){
        sprintf(showScore[i],"0%d",i+1);
        iText(260,ind,showScore[i],GLUT_BITMAP_TIMES_ROMAN_24);
        ind-=50;
    }
    iText(320,300,"Name",GLUT_BITMAP_TIMES_ROMAN_24);
    ind=250;
    for(i=0;i<5;i++){
        sprintf(showScore[i],"%s",ScoreArr[i].Name);
        iText(320,ind,showScore[i],GLUT_BITMAP_TIMES_ROMAN_24);
        ind-=50;
    }
    iText(600,300,"Time",GLUT_BITMAP_TIMES_ROMAN_24);
    ind=250;
    for(i=0;i<5;i++){
        sprintf(showScore[i],"%d seconds",ScoreArr[i].Time);
        iText(600,ind,showScore[i],GLUT_BITMAP_TIMES_ROMAN_24);
        ind-=50;
    }
}

void timekeep(){
    if(GameState==game){
        ScoreArr[5].Time++;
        //printf("%d\n",ScoreArr[5].Time);
    }
}

void UpdateHallOfFame(){
    int i,j,min_index;
    for(i=0;i<6;i++){
        min_index=i;
        for(j=i+1;j<6;j++){
            if(ScoreArr[j].Time<ScoreArr[min_index].Time)min_index=j;
        }
        temp=ScoreArr[i];
        ScoreArr[i]=ScoreArr[min_index];
        ScoreArr[min_index]=temp;
    }
    FILE *p;
    p=fopen("HallOfFame.txt","w");
    for(i=0;i<5;i++){
        fprintf(p,"%s \t %d\n",ScoreArr[i].Name,ScoreArr[i].Time);
    }
}


void iShowInfo(){
    iShowBMP(0,0,"Info.bmp");
    iShowBMP2(10,545,"BackButton.bmp",0);
    //iShowBMP2(100,150,"instructions.bmp",0);
    iShowBMP2(100,100,"Info2.bmp",0);
}
