#include <iostream>
#include <fstream>
#include <omp.h>
#define NUMOFVAR 1000
#define NUMOFSLACK 1000
#define ROWSIZE (NUMOFSLACK+1)
#define COLSIZE (NUMOFSLACK+NUMOFVAR+1)

int count=0;
using namespace std;

bool checkOptimality(float wv[ROWSIZE][COLSIZE])
{
    for(int i=0;i<COLSIZE-1;i++)
    {
        if(wv[ROWSIZE-1][i]<0)//min> max<
            return false;
    }
    return true;
}
bool isUnbounded(float wv[ROWSIZE][COLSIZE],int pivotCol)
{
    for(int j=0;j<ROWSIZE-1;j++)
    {
        if(wv[j][pivotCol]>0)
            return false;
    }
    return true;
}
void print(float wv[ROWSIZE][COLSIZE])
{
    for(int j=0;j<ROWSIZE;j++)
        {
            for(int i=0;i<COLSIZE;i++)
            {
                cout<<wv[j][i]<<" ";
            }
            cout<<endl;
        }
        cout<<endl<<endl<<endl;
}
void makeMatrix(float wv[ROWSIZE][COLSIZE])
{
	for(int j=0;j<ROWSIZE; j++)
	{
		for(int i =0;i<COLSIZE;i++)
		{
			wv[j][i]=0;
		}
	}
	fstream myFile;
    myFile.open("baza1000.txt",ios::in); //otvaram fajl u read modu
	if(myFile.is_open())
    {
        for(int j = 0; j < ROWSIZE; j++)
        {
            for(int i = 0; i< NUMOFVAR; i++)
            {
              myFile >> wv[j][i];
            }
        }
		for(int j = 0;j< NUMOFSLACK;j++)
		{
			myFile >> wv[j][COLSIZE-1];
		}
    }
    myFile.close();
    for(int j=0;j<ROWSIZE-1;j++)
    {
		
	wv[j][NUMOFVAR+j]=1;
		
    }

}
int findPivotCol(float wv[ROWSIZE][COLSIZE])
{
     float minnegval=wv[ROWSIZE-1][0];
       int loc=0;
        for(int i=1;i<COLSIZE-1;i++)
        {
            if(wv[ROWSIZE-1][i]<minnegval)
            {
                minnegval=wv[ROWSIZE-1][i];
                loc=i;
            }
        }
        return loc;
}
int findPivotRow(float wv[ROWSIZE][COLSIZE],int pivotCol)
{
    float rat[ROWSIZE-1];
    for(int j=0;j<ROWSIZE-1;j++)
        {
            if(wv[j][pivotCol]>0)
            {
                rat[j]=wv[j][COLSIZE-1]/wv[j][pivotCol];
            }
            else
            {
                rat[j]=0;
            }
        }

        float minpozval=99999999;
        int loc=0;
        for(int j=0;j<ROWSIZE-1;j++)
        {
            if(rat[j]>0)
            {
                if(rat[j]<minpozval)
                {
                    minpozval=rat[j];
                    loc=j;
                }
            }
        }
        return loc;
}
void doPivoting(float wv[ROWSIZE][COLSIZE],int pivotRow,int pivotCol,float pivot,int tc)
{
    float newRow[COLSIZE];
    float pivotColVal[ROWSIZE];
    //#pragma omp parallel for num_threads(tc)
    for(int i=0;i<COLSIZE;i++)
    {
         newRow[i]=wv[pivotRow][i]/pivot;
    }

    for(int j=0;j<ROWSIZE;j++)
    {
         pivotColVal[j]=wv[j][pivotCol];
    }
    #pragma omp parallel for num_threads(tc)
    for(int j=0;j<ROWSIZE;j++)
    {
         if(j==pivotRow)
         {
         	for(int i=0;i<COLSIZE;i++)
               {
                    wv[j][i]=newRow[i];
               }
         }
         else
         {
         	for(int i=0;i<COLSIZE;i++)
               {
                    wv[j][i]=wv[j][i]-newRow[i]*pivotColVal[j];
               }
        }
    }
}
void solutions(float wv[ROWSIZE][COLSIZE])
{
    for(int i=0;i<NUMOFVAR; i++)  //every basic column has the values, get it form B array
     {
        int count0 = 0;
        int index = 0;
        for(int j=0; j<ROWSIZE-1; j++)
        {
            if(wv[j][i]==0.0)
            {
                count0 = count0+1;
            }
            else if(wv[j][i]==1)
            {
                index = j;
            }


        }

        if(count0 == ROWSIZE - 2 )
        {
            cout<<"variable"<<i+1<<": "<<wv[index][COLSIZE-1]<<endl;  //every basic column has the values, get it form B array
        }
        else
        {
            cout<<"variable"<<i+1<<": "<<0<<endl;
        }
    }

    cout<<""<<endl;
    cout<<endl<<"Optimal solution is "<<wv[ROWSIZE-1][COLSIZE-1]<<endl;
}
void simplexCalculate(float wv[ROWSIZE][COLSIZE],int tc)
{

    //float minnegval;
    //float minpozval;
    //int loc;
    int pivotRow;
    int pivotCol;
    bool unbounded=false;
    float pivot;

    //float solVar[NUMOFVAR];

    while(!checkOptimality(wv))
    {
    	count++;
        pivotCol=findPivotCol(wv);

        if(isUnbounded(wv,pivotCol))
        {
            unbounded=true;
            break;
        }


        pivotRow=findPivotRow(wv,pivotCol);
	//cout<<count<<",pivot="<<wv[pivotRow][pivotCol]<<endl;
        pivot=wv[pivotRow][pivotCol];

        doPivoting(wv,pivotRow,pivotCol,pivot,tc);
        //print(wv);

    }
    //Ispisivanje rezultata
    if(unbounded)
    {
        cout<<"Unbounded"<<endl;
    }
    else
    {
        //print(wv);

        solutions(wv);

    }
}
int main(int argc, char * argv[])
{
	int tc=strtol(argv[1],NULL,10);
   	float wv[ROWSIZE][COLSIZE];
	
	makeMatrix(wv);
	




    	//print(wv);
    	double s=omp_get_wtime();
    	simplexCalculate(wv,tc);
    	s=omp_get_wtime()-s;
    	printf("Time is %lfs\n",s);

    return 0;
}
