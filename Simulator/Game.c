#include <stdio.h>
#include <stdlib.h>
#include <time.h>

struct GameState
{
    int Map[12][12];
    int Stepleft;
};

void Initialize(struct GameState *Current){
    for(int i=0;i<12;i++){
        for(int j=0;j<12;j++){
            Current->Map[i][j]=-1;
        }
    }
    Current->Stepleft = 10;
    return;
}


void ReadFile(struct GameState *Current,char FileName[]){
    FILE *fPtr;
    fPtr = fopen(FileName, "r");
    if (fPtr) {
        printf("檔案開啟成功...\n");
    }
    else {
        printf("檔案開啟失敗...\n");
    }
    fscanf(fPtr,"%d",&Current->Stepleft);
    for(int i=0;i<12;i++){
        for(int j=0;j<12;j++){
            fscanf(fPtr,"%d",&Current->Map[i][j]);
        }
    }
    fclose(fPtr);
    return;
}

void PrintState(struct GameState *Current){
    printf("Stepleft...%d\n",Current->Stepleft);
    printf("XY  ");
    for(int i=0;i<12;i++){
        printf("%02d ",i);
    }
    printf("\n");
    for(int i=0; i<12; i++){
        printf("L%02d ",i);
        for(int j=0; j<12; j++){
            if(Current->Map[i][j] == 0)          printf("*  ");
            else if(Current->Map[i][j] == -1)    printf("o  ");
            else       printf("%d  ",Current->Map[i][j]);
        }
        printf("\n");
    }
    printf("----------------------\n");
    return;
}

int LegalMove(struct GameState *Current, int PositionA[], int PositionB[]){
    int x1=PositionA[0],x2=PositionB[0],y1=PositionA[1],y2=PositionB[1];
    if(abs(x1-x2)+abs(y1-y2)!=1){
        printf("Not nearby candy!\n");
        return 0;
    }
    if(x1>=12||x1<0||y1>=12||y1<0||x2>=12||x2<0||y2>=12||y2<0){
        printf("Move out of map!\n");
        return 0;
    }
    int a_candy = Current->Map[PositionA[0]][PositionA[1]];
    int b_candy = Current->Map[PositionB[0]][PositionB[1]];
    int temp_map[12][12];
    for(int i=0;i<12;i++){
        for(int j=0;j<12;j++){
            temp_map[i][j] = Current->Map[i][j];
        }
    }
    temp_map[x1][y1]=b_candy;
    temp_map[x2][y2]=a_candy;
    for(int i=0;i<12;i++){
        int count[2] = {0};
        int last_candy[2] = {0};
        for(int j=0;j<12;j++){
            if(count[0] == 3 || count[1] == 3)  return 1;
            if(temp_map[i][j]==0){   
                count[0] = 0;
            }
            if(temp_map[i][j]==0){ 
                count[1] = 0;
            }
            if(temp_map[i][j]==last_candy[0] && last_candy[0]!=0)  count[0]++;
            else{
                count[0]=1;
                last_candy[0] = temp_map[i][j];
            }
            if(temp_map[j][i]==last_candy[1] && last_candy[1]!=0)  count[1]++;
            else{
                count[1]=1;
                last_candy[1] = temp_map[j][i];
            }
        }
        if(count[0] == 3 || count[1] == 3)  return 1;
    }
    return 0;
}

void Elliminate(struct GameState *Current,int PositionA[],int PositionB[])
{
    int record = 1,last_record=0;
    int temp_map[12][12]={{0}};
    int count[2] = {0};
    int last_candy[2] = {0};
    while(record!=last_record){
        last_record=record;
        for(int i=0;i<12;i++){
            for(int j=0;j<12;j++){
                temp_map[i][j]=0;
            }
        }
        for(int i=0;i<12;i++){
            for(int j=0;j<2;j++){
                count[j]=0;
                last_candy[j]=0;
            }
            for(int j=0;j<12;j++){
                if(count[0] >= 3){
                    for(int k=j-1;k>=j-count[0];k--)   temp_map[i][k]=1;
                    record++;
                }
                if(count[1] >= 3){
                    for(int k=j-1;k>=j-count[1];k--)   temp_map[k][i]=1;
                    record++;
                }
                if(Current->Map[i][j]==0){
                    count[0] = 0;
                }
                if(Current->Map[j][i]==0){
                    count[1] = 0;
                }
                if(Current->Map[i][j]==last_candy[0] && last_candy[0]!=0 && last_candy[0]!=-1)  count[0]++;
                else{
                    count[0]=1;
                    last_candy[0] = Current->Map[i][j];
                }

                if(Current->Map[j][i]==last_candy[1] && last_candy[1]!=0 && last_candy[1]!=-1)   count[1]++;
                else{
                    count[1]=1;
                    last_candy[1] = Current->Map[j][i];
                }
            }
            if(count[0] >= 3){
                while(count[0]>0){
                    temp_map[i][12-count[0]]=1;
                    count[0]--;
                }
            }
            if(count[1] >= 3){
                while(count[1]>0){
                    temp_map[12-count[1]][i]=1;
                    count[1]--;
                }
            }
        }
        for(int i=0;i<12;i++){
            for(int j=0;j<12;j++){
                if(temp_map[i][j] == 1){
                    Current->Map[i][j]=-1;
                }
            }
        }
        for(int i=11;i>=0;i--){
            for(int j=0;j<12;j++){
                if(Current->Map[i][j]==-1){
                    int level=i;
                    while(level>-1){
                        int last=0;
                        int temp_record=level-1;
                        while(temp_record>=0){
                            if(Current->Map[temp_record][j]==0 || Current->Map[temp_record][j]==-1)   temp_record--;
                            else{    
                                Current->Map[level][j]=Current->Map[temp_record][j];
                                Current->Map[temp_record][j]=-1;
                                break;
                            }
                        }
                        level--;
                    }
                }
            }
        }
    }
    return;
}
 void Move(struct GameState *Current, int PositionA[], int PositionB[]){
    int x1=PositionA[0],x2=PositionB[0],y1=PositionA[1],y2=PositionB[1];
    int a_candy = Current->Map[PositionA[0]][PositionA[1]];
    int b_candy = Current->Map[PositionB[0]][PositionB[1]];
    Current->Map[x1][y1]=b_candy;
    Current->Map[x2][y2]=a_candy;
    return;

}

void FullCandy(struct GameState *Current){
    int record = 1,last_record = 0;
    while(last_record!=record){
        last_record = record;
        for(int i=0;i<12;i++){
            for(int j=0;j<12;j++){
                if(Current->Map[i][j] == -1){
                    record++;
                    Current->Map[i][j] = rand()%6 + 1;
                }
            }
        }
        int temp[2] = {1,1};
        Elliminate(Current,temp,temp);
    }
    return ;

}
void Run(struct GameState *Current, int PositionA[], int PositionB[]){ // PositionA to PositionB
    int Illegal = LegalMove(Current,PositionA,PositionB);
    if(Illegal == 0){
        printf("Illegal Move!\n");
        return;
    }
    else{
        printf("Successfully Move!\n");
        Current->Stepleft = Current->Stepleft-1;
    }
    Move(Current,PositionA,PositionB);
    Elliminate(Current,PositionA,PositionB);

    return;
}


int main(){
    int x1,x2,y1,y2;
    srand(time(NULL));
    struct GameState Current;
    //ReadFile(&Current,"./GameStatus.txt");
    Initialize(&Current);
    FullCandy(&Current);
    PrintState(&Current);
    printf("Input x1,y1,x2,y2:....\n");
    while(scanf("%d %d %d %d",&x1,&y1,&x2,&y2)!=-1){
        int MoveA[]={x1,y1},MoveB[]={x2,y2};
        //Elliminate(&Current,MoveA,MoveB);
        //PrintState(&Current);
        //Current = FullCandy(Current);
        Run(&Current,MoveA,MoveB);
        PrintState(&Current);
        FullCandy(&Current);
        PrintState(&Current);
        if(Current.Stepleft == 0){
            printf("Has no steps left....!\n");
            break;
        }
        else{
            printf("Input x1,y1,x2,y2:....\n");
        }
    }
    printf("Game Over!\n");
    return 0;
}