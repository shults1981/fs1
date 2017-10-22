// =========================================
// проверка использования библиотеки  ncurses
// ==========================================
// ==========================================
	
#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <signal.h>
#include <sys/time.h>
#include <errno.h>


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
	
} Unit;

/*
typedef struct _TurnPointsArr
{
	point* cord;
	int num;
} TurnPointsArr;
*/

//----------------------define global varablre -------------------

Unit Snake, Rabbit;
static int row,col;
static int row_l,col_l,row_max,col_max;
static int move_flag;
static int rabbitInFild;
static int calc;
static char calc_str[5];
static point r_buf1;
static int RabbitWasEaten; 





//------------------ declaretion  handlers and functions -------------------------
void gti_1(int);
void snake_body_control();
void snake_move(int);
void rabbit_factory(void);
void rander(void);
int addNewElementInBackOfArr( point** Arr, int* len,int mv_flg);
void delElementFromBackOfArr( point* Arr, int* len);



//------------------ define  handlers ----------------------------------
void gti_1 (int signo)
{
	rabbit_factory();
	snake_body_control();
	snake_move(move_flag);
	rander();		
		
}


void rabbit_factory(void)
{
	if (!rabbitInFild)
	{
	 Rabbit.cord->_x=border_x_min+rand()%(border_x_max-border_x_min);
	 Rabbit.cord->_y=border_y_min+rand()%(border_y_max-border_y_min);
	 rabbitInFild=1;
	}
	else 
	{
	 if ((Snake.cord[0]._y==Rabbit.cord->_y)&&(Snake.cord[0]._x==Rabbit.cord->_x))
		{
			RabbitWasEaten=1;
			rabbitInFild=0;
	 		calc++;
		}
	}
}


void snake_body_control()
{
	if (RabbitWasEaten)
	{
		addNewElementInBackOfArr( &Snake.cord, &Snake.len,move_flag);
		RabbitWasEaten=0;
	}

}

void snake_move(int mv_flag)
{
 
 int i;
 
 r_buf1._y=Snake.cord[0]._y;
 r_buf1._x=Snake.cord[0]._x;
 r_buf1._d=Snake.cord[0]._d;
 
 
 for (i=0;i<Snake.len;i++)
 {
  	switch(mv_flag)
  	{ 
  	      
  		case 1:
  			   	if (Snake.cord[0]._x>border_x_min)
				{
  					Snake.cord[i]._x--;
					Snake.cord[i]._d=mv_flag;
  				}
				else 
  					Snake.cord[i]._x=border_x_min;
  				break;			
 		case 2:
 				if (Snake.cord[0]._x<border_x_max)
				{
 					Snake.cord[i]._x++;
 					Snake.cord[i]._d=mv_flag;
				}
				else 
 					Snake.cord[i]._y=border_x_max;
 				break;
 		case 3:
 				if (Snake.cord[0]._y>border_y_min)
				{
 					Snake.cord[i]._y--;
					Snake.cord[i]._d=mv_flag;
				}
 				else 
 					Snake.cord[i]._y=border_y_min;
 				break;
 		case 4:
 				if (Snake.cord[0]._y<border_y_max)
 				{
					Snake.cord[i]._y++;
					Snake.cord[i]._d=mv_flag;
				} 
				else 
 					Snake.cord[i]._y=border_y_max;
 				break; 
 			
  		default : break;		
 	     
 	}
 }


}


void rander (void)
{
	int i;

	if (rabbitInFild)
		mvaddch(Rabbit.cord->_y,Rabbit.cord->_x,'*');

	for(i=0;i<Snake.len;i++ )
	{ 
		if (move_flag==1)
		{
		mvaddch(r_buf1._y,r_buf1._x,' ');
		mvaddch(Snake.cord[i]._y,Snake.cord[i]._x,'@');
		//r_buf1._y++;
		r_buf1._x++;
		}
		if (move_flag==2)
		{
		mvaddch(r_buf1._y,r_buf1._x,' ');
		mvaddch(Snake.cord[i]._y,Snake.cord[i]._x,'@');
		//r_buf1._y--;
		r_buf1._x--;
		}
		if (move_flag==3)
		{
		mvaddch(r_buf1._y,r_buf1._x,' ');
		mvaddch(Snake.cord[i]._y,Snake.cord[i]._x,'@');
		r_buf1._y++;
		//r_buf1._x--;
		}
		if (move_flag==4)
		{
		mvaddch(r_buf1._y,r_buf1._x,' ');
		mvaddch(Snake.cord[i]._y,Snake.cord[i]._x,'@');
		r_buf1._y--;
		//r_buf1._x--;
		}
	}

	sprintf (calc_str,"%d",calc);
	mvaddstr(border_y_max+3,border_x_min,"Calc-");
	mvaddstr(border_y_max+3,border_x_min+7,calc_str);

	wrefresh(stdscr);
}

int  addNewElementInBackOfArr( point** Arr, int* len,int mv_flg)
{
	int i;
	point **tVar1,**tVar2;

	(*len)=(*len)+1;

	tVar1=(point**)malloc(sizeof(point**)*(*len));
	for(i=0;i<(*len);i++)
		tVar1[i]=(point*)malloc(sizeof(point));

	
	for (i=0;i<(*len-1);i++)
	{
                tVar1[i]->_x=Arr[i]->_x;
		tVar1[i]->_y=Arr[i]->_y;
		tVar1[i]->_d=Arr[i]->_d;
	}

/*
	Arr=(point**)realloc(Arr,(*len));
	Arr[*len-1]=(point*)malloc(sizeof(point));
*/
//	tVar1[*len-1]->_x=0;
//	tVar1[*len-1]->_y=0;
//	tVar1[*len-1]->_d=0;

	
	switch(tVar1[*len-2]->_d)
	{
	case 1:
		tVar1[*len-1]->_x=tVar1[*len-2]->_x++;
		tVar1[*len-1]->_y=tVar1[*len-2]->_y;
		tVar1[*len-1]->_d=tVar1[*len-2]->_d;
		break;
	case 2:
		tVar1[*len-1]->_x=tVar1[*len-2]->_x--;
		tVar1[*len-1]->_y=tVar1[*len-2]->_y;
		tVar1[*len-1]->_d=tVar1[*len-2]->_d;
		break;
	case 3:
		tVar1[*len-1]->_x=tVar1[*len-2]->_x;
		tVar1[*len-1]->_y=tVar1[*len-2]->_y--;
		tVar1[*len-1]->_d=tVar1[*len-2]->_d;
		break;

	case 4:
		tVar1[*len-1]->_x=tVar1[*len-2]->_x;
		tVar1[*len-1]->_y=tVar1[*len-2]->_y++;
		tVar1[*len-1]->_d=tVar1[*len-2]->_d;
		break;
	}
	


	tVar2=Arr;
	Arr=tVar1;
	tVar1=tVar2;	


	for (i=0;i<(*len-1);i++)
		free (tVar1[i]);
//	free (tVar1);


	return 1;
}

void delElementFromBackOfArr( point* Arr, int* len)
{
}

//============================= MAIN ======================================
		
int main (int argc, char** argv)
{	
	
	int i;

	int ch;
	int ret;
	struct itimerval del1;
	
	srand(time(NULL));
	//-----------------timer setpoint value--------
	del1.it_value.tv_sec=0;
	del1.it_value.tv_usec=200000;
	del1.it_interval.tv_sec=0;
	del1.it_interval.tv_usec=200000;
	
	signal(SIGALRM, gti_1); //--registering main timer 
	
	//-------------init ncurses -----------------------------------
	initscr();
	start_color();
	nodelay(stdscr,TRUE);
	curs_set(0);
	keypad (stdscr, TRUE);
	noecho();
	
	init_pair (1,COLOR_WHITE,COLOR_BLUE);
	attron(COLOR_PAIR(1));
	
	//---------- clear screen ----------------------
	getmaxyx(stdscr,row_max,col_max);
	
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
	//-----------------initialize units in memory----------
	
	calc=0;
	
	Snake.len=1;
	Snake.cord=(point*)malloc(sizeof(point)*Snake.len);
	for (i=0;i<Snake.len;i++)
	 {
		Snake.cord[i]._y=row_l=row_max/2;
		Snake.cord[i]._x=col_l=col_max/2+i;
		Snake.cord[i]._d=1;
	 }

	Snake.tpa=NULL;

	
	Rabbit.len=1;
	Rabbit.cord=(point*)malloc(sizeof(point)*Rabbit.len);
	Rabbit.cord->_x=0;
	Rabbit.cord->_y=0;
	Rabbit.cord->_d=0;
	Rabbit.tpa=NULL;
	
	rabbitInFild=0;
	move_flag=1;
	RabbitWasEaten=0;

	//TPA.num=0;
	//TPA.cord=NULL;

	//--------------end of initialize -----------------
	
	setitimer(ITIMER_REAL,&del1,NULL); // start timer	
	
	//--------------------- main cicle---------------		
	while (ch!='q')
	{
		
		ch=wgetch(stdscr);
		
		switch(ch)
		{
		case KEY_LEFT:
				if ((Snake.len>1)&&(!(move_flag==2)))
					move_flag=1;
				else 
					move_flag=1;
				break;
			
		case KEY_RIGHT:
				if ((Snake.len>1)&&(!(move_flag==1)))
					move_flag=2;
				else 
					move_flag=2;
				break;
		case KEY_UP:
				if ((Snake.len>1)&&(!(move_flag==4)))
					move_flag=3;	
				else 
					move_flag=3;
				break;
		case KEY_DOWN:	
				if ((Snake.len>1)&&(!(move_flag==3)))
					move_flag=4;	
				else 
					move_flag=4;
				break; 
			
		default : break;
		}
	
	}
	//-----------------------end of main cicle----------------
	


	//----------- destructors : clear memory  -----------
/*	for (i=0;i<Snake.len;i++)
	{
		free(Snake.cord[i]);
	}
*/
		free (Snake.cord);
		free (Snake.tpa);


		free (Rabbit.cord);
		free (Rabbit.tpa);
	endwin();
	return 0;
}	



