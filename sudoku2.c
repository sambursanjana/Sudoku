/*
Sudoku heading...DONE
Checking if input is valid...DONE
Board input...DONE
Ask for level...DONE
Ask for name
Leaderboard
*/
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<time.h>

int size;
struct tile
{
    int r,c,v;
}tile;
struct player
{
	char name[11];
	float solvedtime;
}player;

void clear();
void display1(int b[][6],struct tile d[]);
void display2(int b[][9],struct tile d[]);
void display11(int b[][6],struct tile d[],int,int);
void display22(int b[][9],struct tile d[],int,int);
int fn1(int b[][6],struct tile d[],int,int,int,int);
int fn2(int b[][9],struct tile d[],int,int,int,int);
       
int main()
{
    char alph;
    int f=0,num,default_no=0,c,i,j,level;
    double time_;
    time_t start,end;
    clear();
    printf("\033[1;52H\033[1;38;5;11mSUDOKU\n\033[0m");
    usleep(1500000);
    printf("\n\033[38;5;220mINSTRUCTIONS:\033[0m\n1. You can select the dimensions of the board- enter the corresponding number when asked and hit \033[1mENTER\033[0m.\n2. When asked for the tile, give the input in \033[1mmxn\033[0m format, where m represents the row number and n represents the  column number, and hit \033[1mENTER\033[0m.\nExample: \033[1mTile:\033[0m 4x3\nYou can enter anything between (and including) 1x1 and 9x9 in a 9x9 board, and 1x1 to 6x6 in a 6x6 board, except  the tiles having numbers colored in green.\n3. After you have entered a valid tile position, an underscore appears in that tile you had asked access for,     replacing any number that has already been there. Now you are asked to enter a number. Enter what you suppose     should go in there, only it should not have already occupied another tile in the same row, column or box, and hit \033[1mENTER\033[0m\nExample: \033[1mNumber:\033[0m 4\nYou can enter anything between (and including) 1 and 9 in a 9x9 board, and 1 and 6 in a 6x6 board.\nIf you do not want to fill anything, enter 0.\n4. Fill the whole board in a similar way and if you are able to do so, the game ends displaying the time you have taken to solve the puzzle.\n");
    printf("\033[38;5;220mPress any alphabet to continue:\033[0m "); 
    scanf("%c",&alph);
    clear();
    printf("\033[1;52H\033[1;38;5;11mSUDOKU\n\033[0m");
    printf("\033[38;5;220mSelect the dimensions you want:\033[0m\n1.6x6\n2.9x9\n");
    scanf("%d",&size);
    if(size!=1 && size!=2)
    {
        do
        {
        	clear();
        	printf("\033[1;52H\033[1;38;5;11mSUDOKU\n\033[0m");
            printf("\033[38;5;9mInvalid option! Try again: \033[0m");
            scanf("%d",&size);
            if(size==1 || size==2)
                break;
        }while(1);
    }
    if(size==1)
        size=6;
    else
        size=9;
        
	clear();
	printf("\033[1;52H\033[1;38;5;11mSUDOKU\n\033[0m");
	printf("\033[38;5;220mSelect the level:\033[0m\n1.Easy\n2.Medium\n3.Hard\n");
    scanf("%d",&level);
    if(level<1 || level>3)
    {
        do
        {
        	clear();
        	printf("\033[1;52H\033[1;38;5;11mSUDOKU\n\033[0m");
            printf("\033[38;5;9mInvalid option! Try again: \033[0m");
            scanf("%d",&level);
            if(!(level<1 || level>3))
                break;
        }while(1);
    }
    char filename[7]="L0.dat";
    if(level==1)
    	filename[1]='1';
    else if(level==2)
    	filename[1]='2';
    else
    	filename[1]='3';
    	
    //1 read from file with questions and save in board array
    //read the first block's size, if matched read asked, if 0 that's the question
	//if 1 move onto next matching(size) block
	//if all are 1 then make all 0 and first one's the question
    
	FILE* fp;
	fp=fopen(filename,"r+");
	if(!fp)
	{
		printf("Failed to open ");
		for(i=0;i<6;i++)
			printf("%c",filename[i]);
		printf("\n");
		exit(0);
	}
	int counter=0,kk,qn=-1,qno,qsize,asked,board[size][size];
	fread(&qno,sizeof(int),1,fp);
	for(i=0;i<qno;i++)
	{
		fread(&qsize,sizeof(int),1,fp);
		if(size==qsize)
		{
			fread(&asked,sizeof(int),1,fp);
			if(asked==0)
			{
				asked=1;
				fseek(fp,-sizeof(int),SEEK_CUR);
				fwrite(&asked,sizeof(int),1,fp);
				fread(board,qsize*qsize*sizeof(int),1,fp);
				qn=i;
				break;
			}
			else
				fseek(fp,qsize*qsize*sizeof(int),SEEK_CUR);
		}
		else
			fseek(fp,((qsize*qsize)+1)*sizeof(int),SEEK_CUR);
	}
	if(qn==-1)
	{
		fseek(fp,sizeof(int),SEEK_SET);
		for(i=0;i<qno;i++)
		{
			fread(&qsize,sizeof(int),1,fp);
			if(qsize==size)
			{
				if(counter==0)
				{
					fseek(fp,sizeof(int),SEEK_CUR);
					fread(board,qsize*qsize*sizeof(int),1,fp);
					qn=i;
				}
				else
				{
					asked=0;
					fwrite(&asked,sizeof(int),1,fp);
					fseek(fp,qsize*qsize*sizeof(int),SEEK_CUR);
				}
				counter++;
			}
		}
	}
	fclose(fp);
	if(qn==-1)
	{
		printf("\033[38;5;9mUnexpected error in loading the puzzle!\n\033[0m");
		exit(0);
	}
	
	//*Store default tiles' position
	default_no=0;
	for(i=0;i<size;i++)
	    for(j=0;j<size;j++)
	        if(board[i][j]!=0)
	            default_no++;
	struct tile dfault[default_no];
	c=0;
	for(i=0;i<size;i++)
	    for(j=0;j<size;j++)
	        if(board[i][j]!=0)
	        {
	            dfault[c].r=i;
	            dfault[c].c=j;
	            dfault[c].v=board[i][j];
	            c++;
	        }
	start=time(NULL);
	while(1)
	{	
		//1.1 Check if board is full
		f=0;
		for(i=0;i<size;i++)
			for(j=0;j<size;j++)
				if(board[i][j]!=0)
					f++;
		if(f==size*size)
		{
			clear();
			if(size==6)
            	display1(board,dfault);
			else
				display2(board,dfault);
			break;
		}
	    //2 clear screen
	    clear();
	    
	    printf("\033[1;52H\033[1;38;5;11mSUDOKU\n\033[0m");
	    
	    //3 print board...title too
	    if(size==6)                
			display1(board,dfault);
	    else
			display2(board,dfault);
	    
		//4 ask for input(the tile the user intends to fill and with what?)
		int m,n;
		char t[4];
		printf("\033[38;5;220mTile: \033[0m");//The user is supposed to give tile in the format of '3x2'...Indexing for the board starts with 1
		scanf("%s",t);
		m=(int)t[0]-48;
		n=(int)t[2]-48;
		do
		{
			label2: if(!((m>0 && m<size+1) && (n>0 && n<size+1)))
			{
				clear();
				printf("\033[1;52H\033[1;38;5;11mSUDOKU\n\033[0m");
				if(size==6)
	            	display1(board,dfault);
				else
					display2(board,dfault);
                printf("\033[38;5;9mInvalid tile position! Try again: \033[0m");
				scanf("%s",t);
       			m=(int)t[0]-48;
		        n=(int)t[2]-48;
				goto label2;
			}
			for(i=0;i<default_no;i++)
	       	{
	            if(dfault[i].r==m-1 && dfault[i].c==n-1)
	            {
	               	clear();
	               	printf("\033[1;52H\033[1;38;5;11mSUDOKU\n\033[0m");
	                if(size==6)
	                	display1(board,dfault);
	           		else
	      			display2(board,dfault);		
	                printf("\033[38;5;9mCan't modify default tiles! Try again: \033[0m");
					scanf("%s",t);
	        		m=(int)t[0]-48;
	    			n=(int)t[2]-48;
	                goto label2;
	            }
	        }
			if(!(dfault[i].r==m-1 && dfault[i].c==n-1))
	                	break;
		}while(1);
		m--;
		n--;
		clear();
		printf("\033[1;52H\033[1;38;5;11mSUDOKU\n\033[0m");
		if(size==6)
			display11(board,dfault,m,n);
		else
			display22(board,dfault,m,n);
		printf("\033[38;5;220mNumber: \033");//should lie between 1 and size (including the extremes) and should not collide with related blocks' numbers
		scanf("%d",&num);
		do
		{
			label1: if(!(num>=0 && num<size+1))//num<1 || num>size
			{
				clear();
				printf("\033[1;52H\033[1;38;5;11mSUDOKU\n\033[0m");
				if(size==6)
	                		display11(board,dfault,m,n);
	        		else
		                	display22(board,dfault,m,n);
				printf("\033[38;5;9mInvalid number! Try again: \033[0m");
				scanf("%d",&num);
				goto label1;//...
			}
			else
			{
				if(size==6)
				{
					for(i=0;i<6;i++)
					{
						if(i!=m)
							if(board[i][n]==num && num!=0)
							{
								clear();
								printf("\033[1;52H\033[1;38;5;11mSUDOKU\n\033[0m");
								display11(board,dfault,m,n);
								printf("\033[38;5;9m%d is already in the board at %dx%d! Try again: \033[0m",num,i+1,n+1);
								scanf("%d",&num);
								goto label1;
							}
						if(i!=n)
	                        if(board[m][i]==num  && num!=0)
	                        {
	                            clear();
	                            printf("\033[1;52H\033[1;38;5;11mSUDOKU\n\033[0m");
	                            display11(board,dfault,m,n);
	                            printf("\033[38;5;9m%d is already in the board at %dx%d! Try again: \033[0m",num,m+1,i+1);
	                            scanf("%d",&num);
	                            goto label1;
	                        }
	                }
					if((m==0 || m==1) && (n>=0 && n<=2))
	                {
	                    for(i=0;i<=1;i++)
	                        for(j=0;j<=2;j++)
	                        {
	                            if(i!=m && j!=n)
	                                if(board[i][j]==num && num!=0)
	                                {
	                                    num=fn1(board,dfault,m,n,i,j);
	                                    goto label1;
	                                }
	                        }
	                    break;
	                }
					else if((m==0 || m==1) && (n>=3 && n<=5))
	                {
	                    for(i=0;i<=1;i++)
	                        for(j=3;j<=5;j++)
	                        {
	                            if(i!=m && j!=n)
	                                if(board[i][j]==num && num!=0)
	                                {
	                                    num=fn1(board,dfault,m,n,i,j);
	                                    goto label1;
	                                }
	                        }
	                    break;
	                }
					else if((m==2 || m==3) && (n>=0 && n<=2))
	                {
	                    for(i=2;i<=3;i++)
	                        for(j=0;j<=2;j++)
	                        {
	                            if(i!=m && j!=n)
	                                if(board[i][j]==num && num!=0)
	                                {
	                                    num=fn1(board,dfault,m,n,i,j);
	                                    goto label1;
	                                }
	                        }
	                    break;
	                }
					else if((m==2 || m==3) && (n>=3 && n<=5))
	                {
	                    for(i=2;i<=3;i++)
	                        for(j=3;j<=5;j++)
	                        {
	                            if(i!=m && j!=n)
	                                if(board[i][j]==num && num!=0)
	                                {
	                                    num=fn1(board,dfault,m,n,i,j);
	                                    goto label1;
	                                }
	                        }
	                    break;
	                }
					else if((m==4 || m==5) && (n>=0 && n<=2))
	                {
	                    for(i=4;i<=5;i++)
	                        for(j=0;j<=2;j++)
	                        {
	                            if(i!=m && j!=n)
	                                if(board[i][j]==num && num!=0)
	                                {
	                                    num=fn1(board,dfault,m,n,i,j);
	                                    goto label1;
	                                }
	                        }
	                    break;
	                }
					else
	                {
	                    for(i=4;i<=5;i++)
	                        for(j=3;j<=5;j++)
	                        {
	                            if(i!=m && j!=n)
	                                if(board[i][j]==num && num!=0)
	                                {
	                                	num=fn1(board,dfault,m,n,i,j);
	                                    goto label1;
	                                }
	                        }
	                    break;
	                }
	
				}
				else
				{
					for(i=0;i<9;i++)
					{
						if(i!=m)
							if(board[i][n]==num && num!=0)
							{
								clear();
								printf("\033[1;52H\033[1;38;5;11mSUDOKU\n\033[0m");
	                            display22(board,dfault,m,n);
								printf("\033[38;5;9m%d is already in the board at %dx%d! Try again: \033[0m",num,i+1,n+1);
								scanf("%d",&num);
								goto label1;		
							}
						if(i!=n)
							if(board[m][i]==num && num!=0)
							{
								clear();
								printf("\033[1;52H\033[1;38;5;11mSUDOKU\n\033[0m");
	                            display22(board,dfault,m,n);
	                            printf("\033[38;5;9m%d is already in the board at %dx%d! Try again: \033[0m",num,m+1,i+1);
	                            scanf("%d",&num);
	                            goto label1;
							}	
					}
					if((m>=0 && m<=2) && (n>=0 && n<=2))
					{
						for(i=0;i<=2;i++)
							for(j=0;j<=2;j++)
							{
								if(i!=m && j!=n)
	                            	if(board[i][j]==num && num!=0)
	                            	{
	                            		num=fn2(board,dfault,m,n,i,j);
	                                	goto label1;
	                            	}
							}
						break;
					}
					else if((m>=0 && m<=2) && (n>=3 && n<=5))
					{
						for(i=0;i<=2;i++)
	                        for(j=3;j<=5;j++)
	                        {
	                            if(i!=m && j!=n)
	                                if(board[i][j]==num && num!=0)
	                                {
	                                    num=fn2(board,dfault,m,n,i,j);
	                                    goto label1;
	                                }
	                        }
						break;
					}
					else if((m>=0 && m<=2) && (n>=6 && n<=8))
					{
						for(i=0;i<=2;i++)
		                    for(j=6;j<=8;j++)
		                    {
	                            if(i!=m && j!=n)
	                                if(board[i][j]==num && num!=0)
	                                {
	                                    num=fn2(board,dfault,m,n,i,j);
	                                    goto label1;
	                                }
		                    }
						break;
					}
					else if((m>=3 && m<=5) && (n>=0 && n<=2))
					{
						for(i=3;i<=5;i++)
	                        for(j=0;j<=2;j++)
	                        {
	                            if(i!=m && j!=n)
	                                if(board[i][j]==num && num!=0)
	                                {
	                                    num=fn2(board,dfault,m,n,i,j);
	                                    goto label1;
	                                }
	                        }
						break;
					}
					else if((m>=3 && m<=5) && (n>=3 && n<=5))
					{
						for(i=3;i<=5;i++)
	                        for(j=3;j<=5;j++)
	                        {
	                            if(i!=m && j!=n)
	                                if(board[i][j]==num && num!=0)
	                                {
	                                    num=fn2(board,dfault,m,n,i,j);
	                                    goto label1;
	                                }
	                        }
						break;
					}
					else if((m>=3 && m<=5) && (n>=6 && n<=8))
					{
						for(i=3;i<=5;i++)
	                        for(j=6;j<=8;j++)
	                        {
	                            if(i!=m && j!=n)
	                                if(board[i][j]==num && num!=0)
	                                {
	                                    num=fn2(board,dfault,m,n,i,j);
	                                    goto label1;
	                                }
	                        }
						break;
					}
					else if((m>=6 && m<=8) && (n>=0 && n<=2))
					{
						for(i=6;i<=8;i++)
	                        for(j=0;j<=2;j++)
	                        {
	                            if(i!=m && j!=n)
	                                if(board[i][j]==num && num!=0)
	                                {
	                                    num=fn2(board,dfault,m,n,i,j);
	                                    goto label1;
	                                }
	                        }
						break;
					}
					else if((m>=6 && m<=8) && (n>=3 && n<=5))
					{
						for(i=6;i<=8;i++)
	                        for(j=3;j<=5;j++)
	                        {
	                            if(i!=m && j!=n)
	                                if(board[i][j]==num && num!=0)
	                                {
	                                    num=fn2(board,dfault,m,n,i,j);
	                                    goto label1;
	                                }
	                        }
						break;
					}
					else
					{
						for(i=6;i<=8;i++)
	                        for(j=6;j<=8;j++)
	                        {
	                            if(i!=m && j!=n)
	                                if(board[i][j]==num && num!=0)
	                                {
	                                    num=fn2(board,dfault,m,n,i,j);
	                                    goto label1;
	                                }
	                        }
						break;
					}
				}	
			}	
		}while(1); 	

		//5 modify the board accordingly
		board[m][n]=num;
	}
	//6 repeat the steps, 2 to 5 until the board is full

	//7 Also print the time taken to solve the puzzle...time between steps 1 and at submission.
	end=time(NULL);
	time_=(end-start)/60;
	printf("\033[38;5;220mCongrats! You have solved the sudoku puzzle in %.2f minutes\n\033[0m",time_);
	
	/*//11? If there's a leaderboard, print that too
	char playern[11];
	char lbname[8]="lb0.dat";
	int pno=0;
	if(level==1)
		lbname[2]='1';
	else if(level==2)
		lbname[2]='2';
	else
		lbname[2]='3';
	
	printf("\n\033[38;5;220mYour name:\033[0m");
	scanf("%s",playern);
	struct player p1,p2;
	struct player ps[10];
	p2.name=playern;
	p2.solvedtime=time_;
	
	fp=fopen(lbname,"r+");
	if(!fp)
	{
		fopen(lbname,"w");
		fwrite(&pno,sizeof(int),1,fp);
		fclose(fp);
		fp=fopen(lbname,"r+");
		if(!fp)
		{
			printf("\033[38;5;9mCould not display leaderboard!\n\033[0m");
			exit(0);	
		}
	}
	fread(&pno,sizeof(int),1,fp);
	if(pno==0)
	{
		pno=1;
		fseek(fp,0,SEEK_SET);
		fwrite(&pno,sizeof(int),1,fp);
		fwrite(p2,sizeof(player),1,fp);
	}
	else if(pno<=9)
	{
		for(i=0;i<=pno;)
		{
			fread(p1,sizeof(player),1,fp);
			if(p1.solvedtime<p2.solvedtime)
				ps[i++]=p1;
			else if(p1.solvedtime==p2.solvedtime)
			{
				ps[i++]=p1;
				ps[i++]=p2;
				for(;i<=pno;i++)
				{
					fread(p1,sizeof(player),1,fp);
					ps[i]=p1;
				}
				break;
			}
			else
			{
				ps[i++]=p2;
				ps[i++]=p1;
				for(;i<=pno;i++)
				{
					fread(p1,sizeof(player),1,fp);
					ps[i]=p1;
				}
				break;
			}	
		}
		pno++;
		fseek(fp,0,SEEK_SET);
		fwrite(&pno,sizeof(int),1,fp);
		fwrite(ps,pno*sizeof(player),1,fp);
	}
	else if(pno==10)
	{
		for(i=0;i<=pno;)
		{
			fread(p1,sizeof(player),1,fp);
			if(p1.solvedtime<p2.solvedtime)
				ps[i++]=p1;
			else if(p1.solvedtime==p2.solvedtime)
			{
				ps[i++]=p1;
				ps[i++]=p2;
				for(;i<=pno;i++)
				{
					fread(p1,sizeof(player),1,fp);
					ps[i]=p1;
				}
				break;
			}
			else
			{
				ps[i++]=p2;
				ps[i++]=p1;
				for(;i<=pno;i++)
				{
					fread(p1,sizeof(player),1,fp);
					ps[i]=p1;
				}
				break;
			}	
		}
		fseek(fp,sizeof(int),SEEK_SET);
		fwrite(ps,pno*sizeof(player),1,fp);
	}
	else
	{
		printf("\033[38;5;9mUnexpected error in displaying the leaderboard!\n\033[0m");
		exit(0);
	}
	
	clear();
	printf("\033[1;52H\033[1;38;5;11mSUDOKU\n\n\033[0m");
	printf("\033[1;49H\033[38;5;11mLEADERBOARD\n\033[0m")
	printf("\033[38;5;220mRank\t\tName\t\tTime\n\033[0m");
	for(i=0;i<125;i++)
		printf("_");
	printf("\n");
	for(i=0;i<pno;i++)
	{
		if(ps[i].name==p1.name && ps[i].solvedtime==p1.solvedtime)
			printf("\033[32m");
		printf("%d\t\t",i+1);
		for(j=0;ps[i].name[j];j++)
			printf("%c",ps[i].name[j]);
		printf("\t\t%f\n",ps[i].solvedtime);
		if(ps[i].name==p1.name && ps[i].solvedtime==p1.solvedtime)
			printf("\033[0m");
	}
	fclose(fp);*/
	return 0;
}
void clear()
{
    printf("\033[2J");
    printf("\033[1;1H");
}
void display1(int b[][6],struct tile d[])
{
	int i,j,x=0,z=0;
	for(i=0;i<15;i++)
	{
		int y=0;
		printf(" ");
		if(i==4 || i==10)
		{
			for(j=0;j<4;j++)
			    printf("__ ");
			printf("_\n");
		}
		else if(i%2==1)
			printf("\n");
		else
		{
			for(j=0;j<13;j++)
			{
				if(j==6)
				    printf("|");
				else if(j%2==1)
					printf(" ");
				else
				{
					if(d[z].r==x && d[z].c==y)
					{
						printf("\033[32m%d\033[0m",b[x][y]);
						z++;
					}
					else if(b[x][y]==0)
						printf(" ");
					else
				    	printf("%d",b[x][y]);
					y++;
				}
			}
			x++;
			printf("\n");
		}
	}
	printf("\n\n\n\n");
}
void display2(int b[][9],struct tile d[])
{
	int i,j,x=0,z=0;
	for(i=0;i<21;i++)
	{
		int y=0;
		printf(" ");
		if(i==6 || i==14)
		{
			for(j=0;j<7;j++)
			    printf("__ ");
		    printf("_\n");
		}
		else if(i%2==1)
			printf("\n");
		else
		{	
			for(j=0;j<21;j++)
			{
				if(j==6 || j==14)
				    printf("|");
				else if(j%2==1)
					printf(" ");
				else
				{
					if(d[z].r==x && d[z].c==y)
					{
						printf("\033[32m%d\033[0m",b[x][y]);
						z++;
					}
					else if(b[x][y]==0)
						printf(" ");
					else
				    	printf("%d",b[x][y]);
					y++;
				}	
			}
			x++;
			printf("\n");
		}	
	}
	printf("\n\n\n\n");
}
void display11(int b[][6],struct tile d[],int m,int n)
{
	int i,j,x=0,z=0;
	for(i=0;i<15;i++)
	{
		int y=0;
		printf(" ");
		if(i==4 || i==10)
		{
			for(j=0;j<4;j++)
			    printf("__ ");
			printf("_\n");
		}
		else if(i%2==1)
			printf("\n");
		else
		{
			for(j=0;j<13;j++)
			{
				if(j==6)
				    printf("|");
				else if(j%2==1)
					printf(" ");
				else
				{
					if(d[z].r==x && d[z].c==y)
					{
						printf("\033[32m%d\033[0m",b[x][y]);
						z++;
					}
					else if(m==x && n==y)
						printf("_");
					else if(b[x][y]==0)
						printf(" ");
					else
				    	printf("%d",b[x][y]);
					y++;
				}
			}
			x++;
			printf("\n");
		}
	}
	printf("\n\n\n\n");	
}
void display22(int b[][9],struct tile d[],int m,int n)
{
		int i,j,x=0,z=0;
	for(i=0;i<21;i++)
	{
		int y=0;
		printf(" ");
		if(i==6 || i==14)
		{
			for(j=0;j<7;j++)
			    printf("__ ");
		    printf("_\n");
		}
		else if(i%2==1)
			printf("\n");
		else
		{	
			for(j=0;j<21;j++)
			{
				if(j==6 || j==14)
				    printf("|");
				else if(j%2==1)
					printf(" ");
				else
				{
					if(d[z].r==x && d[z].c==y)
					{
						printf("\033[32m%d\033[0m",b[x][y]);
						z++;
					}
					else if(m==x && n==y)
						printf("_");
					else if(b[x][y]==0)
						printf(" ");
					else
				    	printf("%d",b[x][y]);
					y++;
				}	
			}
			x++;
			printf("\n");
		}	
	}
	printf("\n\n\n\n");
}
int fn1(int b[][6],struct tile d[],int m,int n,int i,int j)
{
	int num;
	clear();
	printf("\033[1;52H\033[1;38;5;11mSUDOKU\n\033[0m");
    display11(b,d,m,n);
    printf("\033[38;5;9m%d is already in the board at %dx%d! Try again: \033[0m",num,i+1,j+1);
    scanf("%d",&num);
    return num;
}
int fn2(int b[][9],struct tile d[],int m,int n,int i,int j)
{
	int num;
	clear();
	printf("\033[1;52H\033[1;38;5;11mSUDOKU\n\033[0m");
	display22(b,d,m,n);
	printf("\033[38;5;9m%d is already in the board at %dx%d! Try again: \033[0m",num,i+1,j+1);
	scanf("%d",&num);
	return num;
}