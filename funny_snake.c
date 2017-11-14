// ==========================================
// проверка использования библиотеки  ncurses
// =========== игра змейка ==================
// ==========================================
// ==========================================
	
#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <signal.h>
#include <sys/time.h>
#include <errno.h>
#include <unistd.h>


//---------------------------------------------------------------
#define border_x_min (col_max-9*col_max/10)
#define border_x_max (col_max-2*col_max/10)
#define border_y_min (row_max-9*row_max/10)
#define border_y_max (row_max-2*row_max/10)



typedef struct _point
{
	int _x; // coord X
	int _y; // coord Y
	int _d; // moving direction of the point
} point;

typedef struct _unit
{
	point* cord;  
	point* tpa;
	int len;
	int num_tpa;
	
} Unit;


typedef enum _game_status {game_exit=0, game_menu, game_on, game_over} GameStatus;




//----------------------define global varablre -------------------

Unit *Snake, *Rabbit;
point *snake_body_frame;
WINDOW *MainMenu, *tuneMenu;


static int row,col;
static int row_max,col_max;
static int move_flag;
static int rabbitInFild;
static int calc;
static char str_BUF1[5],str_BUF2[5];
static int RabbitWasEaten; 
static GameStatus GST;
static int level;
//static int tuneMenuStatus;


//------------------ declaretion  handlers and functions -------------------------
void gti_1(int);
void snake_body_control();
void snake_move(int);
void rabbit_factory(void);
void rander(void);
void addNewElementInBackOfSnakeBody(point** Arr, int* len);
void addNewElementInBackOfArr(point** Arr,int* len, point source);
void delElementFromBackOfArr( point** Arr, int* len);
void delElementFromBeginOfArr (point** Arr, int* len);
void copy_body_frame(point * original,point** copy, int len);
void init_scr();
void CreateGameFild();
int InitUnits();
int DestroyUnits();
void gameMenuOpen();
void gameMenuClose();



//------------------ define  handlers ----------------------------------
void gti_1 (int signo)
{
	rabbit_factory();
	snake_move(move_flag);
	rander();		
		
}


void rabbit_factory(void)
{
if (GST==game_on)
 {
	RabbitWasEaten=0;
	if (!rabbitInFild)
	{
		Rabbit->cord->_x=border_x_min+rand()%((border_x_max-border_x_min)-1)+1;
		Rabbit->cord->_y=border_y_min+rand()%((border_y_max-border_y_min)-1)+1;
		rabbitInFild=1;
	}
	else 
	{
		 if ((Snake->cord[0]._y==Rabbit->cord->_y)&&(Snake->cord[0]._x==Rabbit->cord->_x))
			{
				RabbitWasEaten=1;
				rabbitInFild=0;
	 			calc++;
			}
	}
 }

}


void snake_body_control()
{
	if (RabbitWasEaten)
	{
		addNewElementInBackOfSnakeBody(&Snake->cord, &Snake->len);
		//RabbitWasEaten=0;
	}

}

void snake_move(int mv_flag)
{
 
 int i,j,turn_flag;


if (GST==game_on)
{
	snake_body_control();
	copy_body_frame(Snake->cord,&snake_body_frame,Snake->len);

	if (Snake->cord[0]._d!=mv_flag)
	{
		Snake->cord[0]._d=mv_flag;
		if ((Snake->len>1)&&((Snake->num_tpa)<=(Snake->len-1)))
			addNewElementInBackOfArr(&Snake->tpa,&Snake->num_tpa, Snake->cord[0]);
	}
 
 	for (i=0;i<Snake->len;i++)
 	{
		turn_flag=0;

		if ((Snake->cord[0]._x>border_x_min)&&(Snake->cord[0]._x<border_x_max)&&(Snake->cord[0]._y>border_y_min)&&(Snake->cord[0]._y<border_y_max))
		{
			if (Snake->num_tpa)
			{
				for (j=(Snake->num_tpa)-1;j>=0;j--)
				{
					if ((Snake->cord[i]._x==Snake->tpa[j]._x)&&(Snake->cord[i]._y==Snake->tpa[j]._y))
					{
						switch(Snake->tpa[j]._d)
						{
						case 1:
							Snake->cord[i]._x--;
							break;
						case 2:
							Snake->cord[i]._x++;
							break;
						case 3:
							Snake->cord[i]._y--;
							break;
						case 4:
							Snake->cord[i]._y++;
							break;
						default:break;
						}
					
					Snake->cord[i]._d=Snake->tpa[j]._d;
					if ((i==Snake->len-1)&&(j==0))
						delElementFromBeginOfArr(&Snake->tpa, &Snake->num_tpa);
					turn_flag=1;
					}
				}	
			}
		
			if (!turn_flag)
			{
				switch(Snake->cord[i]._d)
				{
				case 1:
					Snake->cord[i]._x--;
					break;
				case 2:
					Snake->cord[i]._x++;
					break;
				case 3:
					Snake->cord[i]._y--;
					break;
				case 4:
					Snake->cord[i]._y++;
					break;
				default:break;
				}
			}	
		}	
		else 
		  	 GST=game_over;   //----------- game over
 	}
 }

}


void rander (void)
{
	int i;

	if (GST==game_on)
	{
		if (rabbitInFild)
			mvaddch(Rabbit->cord->_y,Rabbit->cord->_x,'*');

		for(i=0;i<Snake->len;i++ )
		{ 
			mvaddch(snake_body_frame[i]._y,snake_body_frame[i]._x,' ');
			mvaddch(Snake->cord[i]._y,Snake->cord[i]._x,'@');
		}

		sprintf (str_BUF1,"%d",calc);
		mvaddstr(border_y_max+3,border_x_min,"Calc-");
		mvaddstr(border_y_max+3,border_x_min+7,str_BUF1);
		sprintf (str_BUF2,"%d",level);
		mvaddstr(border_y_max+4,border_x_min,"Level-");
		mvaddstr(border_y_max+4,border_x_min+7,str_BUF2);
	}

	if (GST==game_over)
	{
		mvaddstr(border_y_max/2,border_x_max/2-5,"G A M E   O V E R !!!!!");
	}
	wrefresh(stdscr);
}

void  addNewElementInBackOfSnakeBody ( point** Arr, int* len)
{
	int i;
	point *tVar1,*tVar2;

	(*len)=(*len)+1;

	
	tVar1=(point*)malloc(sizeof(point)*(*len));
	
	for (i=0;i<(*len-1);i++)
	{
                tVar1[i]._x=(*Arr)[i]._x;
		tVar1[i]._y=(*Arr)[i]._y;
		tVar1[i]._d=(*Arr)[i]._d;
	}
	
	switch(tVar1[*len-2]._d)
	{
	case 1:
		tVar1[*len-1]._x=(tVar1[*len-2]._x)+1;
		tVar1[*len-1]._y=tVar1[*len-2]._y;
		tVar1[*len-1]._d=tVar1[*len-2]._d;
		break;
	case 2:
		tVar1[*len-1]._x=(tVar1[*len-2]._x)-1;
		tVar1[*len-1]._y=tVar1[*len-2]._y;
		tVar1[*len-1]._d=tVar1[*len-2]._d;
		break;
	case 3:
		tVar1[*len-1]._x=tVar1[*len-2]._x;
		tVar1[*len-1]._y=(tVar1[*len-2]._y)+1;
		tVar1[*len-1]._d=tVar1[*len-2]._d;
		break;
	case 4:
		tVar1[*len-1]._x=tVar1[*len-2]._x;
		tVar1[*len-1]._y=(tVar1[*len-2]._y)-1;
		tVar1[*len-1]._d=tVar1[*len-2]._d;
		break;
	default:
		break;
	}

	tVar2=*Arr;
	*Arr=tVar1;
	tVar1=tVar2;	

	free(tVar1);
}

void  addNewElementInBackOfArr ( point** Arr, int* len, point source)
{
	int i;
	point *tVar1,*tVar2;

	(*len)=(*len)+1;

	
	tVar1=(point*)malloc(sizeof(point)*(*len));

	if (Arr==NULL)
	{
		tVar1->_x=source._x;
		tVar1->_y=source._y;
		tVar1->_d=source._d;
	}
	else
	{
		for (i=0;i<(*len-1);i++)
		{
                	tVar1[i]._x=(*Arr)[i]._x;
			tVar1[i]._y=(*Arr)[i]._y;
			tVar1[i]._d=(*Arr)[i]._d;
		}
	
		tVar1[*len-1]._x=source._x;
		tVar1[*len-1]._y=source._y;
		tVar1[*len-1]._d=source._d;
	}
	tVar2=*Arr;
	*Arr=tVar1;
	tVar1=tVar2;

	free(tVar1);	
}


void delElementFromBackOfArr( point** Arr, int* len)
{
	int i;
	point *tVar1,*tVar2;

	(*len)=(*len)-1;

	if (*len!=0)
	{
		tVar1=(point*)malloc(sizeof(point)*(*len));
	
		for (i=0;i<(*len-1);i++)
		{
        	        tVar1[i]._x=(*Arr)[i]._x;
			tVar1[i]._y=(*Arr)[i]._y;
			tVar1[i]._d=(*Arr)[i]._d;
		}


		tVar2=*Arr;
		*Arr=tVar1;
		tVar1=tVar2;	
	}
	else 
	{
		tVar1=*Arr;
		*Arr=NULL;
	}

	free(tVar1);
}

void delElementFromBeginOfArr (point** Arr, int* len)
{
	int i;
	point *tVar1,*tVar2;

	if (*len!=1)
	{
		tVar1=(point*)malloc(sizeof(point)*(*len-1));
	
		for (i=1;i<(*len);i++)
		{
        	        tVar1[i-1]._x=(*Arr)[i]._x;
			tVar1[i-1]._y=(*Arr)[i]._y;
			tVar1[i-1]._d=(*Arr)[i]._d;
		}

		tVar2=*Arr;
		*Arr=tVar1;
		tVar1=tVar2;	
	}
	else 
	{
		tVar1=*Arr;
		*Arr=NULL;
	}

	free(tVar1);

	(*len)=(*len)-1;
}



void copy_body_frame(point * original , point **copy, int len)
{

	int i;
	point *tVar1,*tVar2;

	tVar1=(point*)malloc(sizeof(point)*len);
	for(i=0;i<len;i++)
	{
		tVar1[i]._x=original[i]._x;
		tVar1[i]._y=original[i]._y;
		tVar1[i]._d=original[i]._d;
	}

	tVar2=*copy;
	*copy=tVar1;	
	tVar1=tVar2;
	
	free (tVar1);
}

void init_scr()
{

	//-------------init ncurses -----------------------------------
	initscr();
	start_color();
	nodelay(stdscr,TRUE);
	curs_set(0);
	keypad (stdscr, TRUE);
	noecho();
	init_pair (1,COLOR_WHITE,COLOR_BLUE);
	init_pair (2,COLOR_WHITE,COLOR_BLUE);
	attron(COLOR_PAIR(1));
	getmaxyx(stdscr,row_max,col_max);
}

void destr_scr()
{
	endwin();
}

void CreateGameFild()
{
int row, col;

	for (row=0;row<=row_max;row++)
	{
		for (col=0;col<=col_max;col++)
		{
			move(row,col);
			if ((row>=border_y_min)&&(row<=border_y_max)&&(col>=border_x_min)&&(col<=border_x_max))
			{
				if (row==border_y_min)
					addch('X');
				if ((row>=border_y_min)&&(row<=border_y_max)&&(col==border_x_min))
					addch('X');
				if ((row>=border_y_min)&&(row<=border_y_max)&&(col==border_x_max))
					addch('X');
				if (row==border_y_max)
					addch('X');
			}
			addch(' ');
		}
	}
}

int InitUnits()
{
	int i;

	Rabbit=(Unit*)malloc(sizeof(Unit));
	Rabbit->len=1;
	Rabbit->cord=(point*)malloc(sizeof(point)*Rabbit->len);
	Rabbit->cord->_x=0;
	Rabbit->cord->_y=0;
	Rabbit->cord->_d=0;
	Rabbit->num_tpa=0;
	Rabbit->tpa=NULL;

	Snake=(Unit*)malloc(sizeof(Unit));
	Snake->len=1;
	Snake->cord=(point*)malloc(sizeof(point)*Snake->len);
	for (i=0;i<Snake->len;i++)
	 {
		Snake->cord[i]._y=row_max/2;
		Snake->cord[i]._x=col_max/2+i;
		Snake->cord[i]._d=1;
	 }
	Snake->num_tpa=0;
	Snake->tpa=NULL;

	snake_body_frame=(point*)malloc(sizeof(point)*(Snake->len));

	return 0;
}

int DestroyUnits()
{
	if (Rabbit&&Snake)
	{
		free (Rabbit->cord);
		free (Rabbit->tpa);
		free (Rabbit);
	
		free (Snake->cord);
		free (Snake->tpa);
		free (Snake);

		free (snake_body_frame);
	}
	return 0;
}


void gameMenuOpen()
{
	MainMenu=newwin(10,20,border_y_max/2,border_x_max/2);
	wbkgd(MainMenu,COLOR_PAIR(2));
	wattron(MainMenu,COLOR_PAIR(2));
	box(MainMenu,ACS_VLINE,ACS_HLINE);
	wmove(MainMenu,0,5);
	waddstr(MainMenu,"MENU");
	wmove(MainMenu,2,1);
	waddstr(MainMenu,"NEW GAME - 'n'");
	wmove(MainMenu,4,1);
	waddstr(MainMenu,"LEVEL - 1...9");
	wmove(MainMenu,6,1);
	waddstr(MainMenu,"CONTINUE - 'C'");
	wmove(MainMenu,8,1);
	waddstr(MainMenu,"EXIT - 'e'");
	wrefresh(MainMenu);
}

void gameMenuClose()
{
	delwin(MainMenu);
	MainMenu=NULL;
}




//============================= MAIN ======================================
		
int main (int argc, char** argv)
{	
	
	int i;

	int ch;
	int ret;
	struct itimerval tmr1, tmr2;
	char buf1[2]={'0',0x00};
	
	srand(time(NULL));
	signal(SIGALRM, gti_1); //--registering main timer 
	

	init_scr(); // initialize ncurses;
	
	//---------- Make game fild ----------------------
	
	CreateGameFild();

        // initialize  same  variable----------
	
	Snake=NULL;
	Rabbit=NULL;
	GST=game_menu;	
	level=1;
	//timer setpoint value
	tmr1.it_value.tv_sec=0;
	tmr1.it_value.tv_usec=200000;
	tmr1.it_interval.tv_sec=0;
	tmr1.it_interval.tv_usec=200000;

	setitimer(ITIMER_REAL,&tmr1,NULL); // start timer



	//--------------------- main cicle---------------		

	while (ch!='q')
	{
		ch=wgetch(stdscr);
		
		if (ch=='m')
		{
			GST=game_menu;
			//DestroyUnits();
		}


		if (GST==game_menu)
		{
			if (!MainMenu)
				gameMenuOpen();
			switch (ch)
			{
			case 'e':
				GST=game_exit;
				gameMenuClose();
				ch='q';
				break;
			case 'n':
				GST=game_on;
				gameMenuClose();
				CreateGameFild();
				rabbitInFild=0;
				move_flag=1;
				RabbitWasEaten=0;
				calc=0;
				InitUnits();
				//timer setpoint value
				tmr1.it_value.tv_sec=0;
				tmr1.it_value.tv_usec=200000-level*10000;
				tmr1.it_interval.tv_sec=0;
				tmr1.it_interval.tv_usec=200000-level*10000;
				setitimer(ITIMER_REAL,&tmr1,NULL);// timer whith new setpoint by level
				break;
			case '1'...'9':
				buf1[0]=ch;
				level=atoi(buf1);
				break;
			case 'c':
				if(Snake && Rabbit)
				{
					GST=game_on;
					gameMenuClose();
					CreateGameFild();
				}
				break;
			
			}		
		}	
				
				
		if (GST==game_on)
		{
			switch(ch)
			{
				case KEY_LEFT:
					if (Snake->len==1)
						move_flag=1;
					else
						if ((Snake->len>1)&&(!(Snake->cord[0]._d==2)))
							move_flag=1;
					break;
			
				case KEY_RIGHT:
						if (Snake->len==1)
							move_flag=2;
						else
							if ((Snake->len>1)&&(!(Snake->cord[0]._d==1)))
								move_flag=2;
						break;
				case KEY_UP:
						if (Snake->len==1)
							move_flag=3;
							else			
								if ((Snake->len>1)&&(!(Snake->cord[0]._d==4)))
									move_flag=3;	
						break;
				case KEY_DOWN:	
						if (Snake->len==1)
							move_flag=4;
						else			
							if ((Snake->len>1)&&(!(Snake->cord[0]._d==3)))
								move_flag=4;
						break; 
			
				default : break;
			}
		}
	}
	//-----------------------end of main cicle----------------
		

	//----------- delete units  -----------
	DestroyUnits();
	
	//-----------delete screen -------------
	destr_scr();
	
	return 0;
}	



