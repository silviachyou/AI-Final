#include<stdio.h>
#include<stdlib.h>

typedef struct GameState
{
    int Map[12][12];
    int Stepleft;
} Current;

struct GameState Initialize(){
    struct GameState Current;
    for(int i=0;i<12;i++){
        for(int j=0;j<12;j++){
            Current.Map[i][j]=0;
        }
    }
    Current.Stepleft = 10;
    Current.Map[0][0] = 1;
    Current.Map[0][1] = 2;
    Current.Map[0][2] = 3;
    Current.Map[1][0] = 1;
    Current.Map[1][1] = 2;
    Current.Map[1][2] = 1;
    Current.Map[2][0] = 3;
    Current.Map[2][1] = 1;
    Current.Map[2][2] = 2;
    Current.Map[3][0] = 3;
    Current.Map[3][1] = 2;
    Current.Map[3][2] = 2;
    return Current;
}

struct GameState ReadFile(char FileName[]){
    struct GameState Current;
    FILE *fPtr;
    fPtr = fopen(FileName, "r");
    if (fPtr) {
        printf("檔案開啟成功...\n");
    }
    else {
        printf("檔案開啟失敗...\n");
    }
    fscanf(fPtr,"%d",&Current.Stepleft);
    for(int i=0;i<12;i++){
        for(int j=0;j<12;j++){
            fscanf(fPtr,"%d",&Current.Map[i][j]);
        }
    }
    fclose(fPtr);
    return Current;
}

void PrintState(struct GameState Current){
    printf("Stepleft...%d\n",Current.Stepleft);
    for(int i=0; i<12; i++){
        for(int j=0; j<12; j++){
            if(Current.Map[i][j] == 0)          printf("* ");
            else if(Current.Map[i][j] == -1)    printf("o ");
            else       printf("%d ",Current.Map[i][j]);
        }
        printf("\n");
    }
    printf("----------------------\n");
    return;
}

int LegalMove(struct GameState Current, int PositionA[], int PositionB[]){
    int x1=PositionA[0],x2=PositionB[0],y1=PositionA[1],y2=PositionB[1];
    if(x1>=12||x1<0||y1>=12||y1<0||x2>=12||x2<0||y2>=12||y2<0){
        printf("Move out of map!\n");
        return 0;
    }
    int a_candy = Current.Map[PositionA[0]][PositionA[1]];
    int b_candy = Current.Map[PositionB[0]][PositionB[1]];
    int temp_map[12][12];
    for(int i=0;i<12;i++){
        for(int j=0;j<12;j++){
            temp_map[i][j] = Current.Map[i][j];
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

struct GameState Elliminate(struct GameState Current,int PositionA[],int PositionB[])
{
    int record = 1,last_record=0;
    while(record!=last_record){
        last_record=record;
        int temp_map[12][12]={{0}};
        for(int i=0;i<12;i++){
            int count[2] = {0};
            int last_candy[2] = {0};
            for(int j=0;j<12;j++){
                if(count[0] >= 3){
                    for(int k=j-1;k>=j-count[0];k--)   temp_map[i][k]=1;
                    record++;
                }
                if(count[1] >= 3){
                    for(int k=j-1;k>=j-count[1];k--)   temp_map[k][i]=1;
                    record++;
                }
                if(Current.Map[i][j]==0){
                    count[0] = 0;
                }
                if(Current.Map[j][i]==0){
                    count[1] = 0;
                }
                if(Current.Map[i][j]==last_candy[0] && last_candy[0]!=0 && last_candy[0]!=-1)  count[0]++;
                else{
                    count[0]=1;
                    last_candy[0] = Current.Map[i][j];
                }

                if(Current.Map[j][i]==last_candy[1] && last_candy[1]!=0 && last_candy[1]!=-1){
                    count[1]++;
                }
                else{
                    count[1]=1;
                    last_candy[1] = Current.Map[j][i];
                }
            }
            if(count[0] >= 3){
                while(count[0]>0){
                    count[0]--;
                    temp_map[11][12-count[0]]=1;
                }
            }
            if(count[1] >= 3){
                while(count[1]>0){
                    count[1]--;
                    temp_map[12-count[1]][11]=1;
                }
            }
        }
        for(int i=0;i<12;i++){
            for(int j=0;j<12;j++){
                if(temp_map[i][j] == 1){
                    Current.Map[i][j]=-1;
                }
            }
        }
        for(int i=11;i>=0;i--){
            for(int j=0;j<12;j++){
                if(Current.Map[i][j]==-1){
                    int level=i;
                    while(level>=0){
                        int last=0;
                        int record=level-1;
                        while(record>=0){
                            if(Current.Map[record][j]==0 || Current.Map[record][j]==-1)   record--;
                            else{    
                                Current.Map[level][j]=Current.Map[record][j];
                                Current.Map[record][j]=-1;
                                break;
                            }
                        }
                        level--;
                    }
                }
            }
        }
    }
    return Current;
}
struct GameState Move(struct GameState Current, int PositionA[], int PositionB[]){
    int x1=PositionA[0],x2=PositionB[0],y1=PositionA[1],y2=PositionB[1];
    int a_candy = Current.Map[PositionA[0]][PositionA[1]];
    int b_candy = Current.Map[PositionB[0]][PositionB[1]];
    Current.Map[x1][y1]=b_candy;
    Current.Map[x2][y2]=a_candy;
    return Current;

}

struct GameState Run(struct GameState Current, int PositionA[], int PositionB[]){ // PositionA to PositionB
    int Illegal = LegalMove(Current,PositionA,PositionB);
    if(Illegal == 0){
        printf("Illegal Move!\n");
        return Current;
    }
    Current = Move(Current,PositionA,PositionB);
    Current = Elliminate(Current,PositionA,PositionB);


    return Current;
}


int main(){
    struct GameState Current = ReadFile("./GameStatus.txt");
    PrintState(Current);
    int MoveA[]={1,1},MoveB[]={2,1};
    Current = Run(Current,MoveA,MoveB);
    PrintState(Current);
    return 0;
}