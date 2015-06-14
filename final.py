import numpy as np
import win32com, win32api, win32con
import subprocess
import time
from MonteCarlo import *
#n*m

#prev row: n - 4
#next row: n + 4
def search_u(x,v,b,m,n):
    #r:1 l:2 u:3 d:4
    direction = []
    #b = a.reshape(m*n)
    if x%m!=m-1 and b[x+1]  == v  :
        direction.append(1)
    if x%m!=0 and b[x-1]  == v :
        direction.append(2)
    #elif  x+4<=(m*n)-1 and b[x+4]  == v :
        #return 1
    if x-m>=0 and b[x-m] == v:
        direction.append(3)
    return direction
def search_d(x,v,b,m,n):
    
    direction = []
    #b = a.reshape(m*n)
    if x%m!=m-1 and b[x+1]  == v  :
        direction.append(1)
    if x%m!=0 and b[x-1]  == v :
        direction.append(2)
    if  x+m<=(m*n)-1 and b[x+m]  == v :
        direction.append(4)
    #elif x-4>=0 and b[x-4] == v:
        #return 1
    
    return direction
def search_l(x,v,b,m,n):
    direction = []
    #b = a.reshape(m*n)
    #if x%4!=4-1 and b[x+1]  == v  :
        #return 1
    if x%m!=0 and b[x-1]  == v :
        direction.append(2)
    if  x+m<=(m*n)-1 and b[x+m]  == v :
        direction.append(4)
    if x-m>=0 and b[x-m] == v:
        direction.append(3)
    return direction
def search_r(x,v,b,m,n):
    direction = []
    #b = a.reshape(m*n)
    if x%m!=m-1 and b[x+1]  == v  :
        direction.append(1)
    #elif x%4!=0 and b[x-1]  == v :
        #return 1
    if  x+m<=(m*n)-1 and b[x+m]  == v :
        direction.append(4)
    if x-m>=0 and b[x-m] == v:
        direction.append(3)
    return direction

def search(a,n,m):
    flag=0
    ans = []
    b = a.reshape(m*n)
    for index,i in enumerate(b):
        #print(index,i)
        if i==0:
            continue
        #check right row:
        if (index+1)<(m*n)and(index+1)%m!=0 and (index+1)%m<=2 and b[index+2]!=0:
            if i == b[index+1]:
                flag = search_r(index+2,i,b,m,n)
                if len(flag) >0:
                    e = dict()
                    e[index+2] = []
                    for j in flag:
                        e[index+2].append(j)
                        
                    ans.append(e)
        #check left row:
        if (index-1)>=0 and (index-1)%m!=m-1 and(index-1)%m>=1 and b[index-2]!=0:
            if i==b[index-1]:
                flag = search_l(index-2,i,b,m,n)
                if len(flag) >0:
                    e = dict()
                    e[index-2] = []
                    for j in flag:
                        e[index-2].append(j)
                        
                    ans.append(e)
        #check down row:
        if (index+m)<(m*n) and (int)((index+m)/m)<=n-2 and b[index+2*m]!=0:
            #print(index+m,n)
            if i==b[index+m]:
                flag = search_d(index+2*m,i,b,m,n)
                if len(flag) >0:
                    e = dict()
                    e[index+2*m] = []
                    for j in flag:
                        e[index+2*m].append(j)
                        
                    ans.append(e)
        #check up row:
        if (index-m)>=0 and int((index-m)/m)>=1 and b[index-2*m]!=0:
            if i==b[index-m]:
                flag = search_u(index-2*m,i,b,m,n)
                if len(flag) >0:
                    e = dict()
                    e[index-2*m] = []
                    for j in flag:
                        e[index-2*m].append(j)
                        
                    ans.append(e)
    return ans

#move = search()
#print(move)
def line_search(L):
    window_size = 3
    for i in range(len(L)):
        while(1):
            if i+window_size > len(L):
                break
            New = L[i:i+window_size]
            if New.count(L[i]) == window_size:
                window_size+=1
            else:
                break
    if window_size==3:
        return 0
    return(window_size-1)
def swap(x1,y1,x2,y2,a,olda,n,m):
    v = []
    #print(x1,y1,x2,y2)
    temp = olda[x1][y1]
    olda[x1][y1] = olda[x2][y2]
    olda[x2][y2] = temp
    #count row
    for i in range(n):
        v.append(line_search(list(olda[i])))
    #count col
    for i in range(m):
        v.append(line_search(list(olda[:,i])))
    #print(olda)
    #print("==============")
    #print(a)
    #print(v)
    #print(max(v))
    return max(v)
    
#r:1 l:2 u:3 d:4
def Make_move(move,n,m,a):
    Max = -1
    for i in move:
        
        for j in i:
            for kk in i[j]:
                olda = a.copy()
                s = kk
                #print(i,j,s)
                _x = (int)(j/m)
                _y = j%m
                _xprime = _x
                _yprime = _y
                if s==1:
                    _yprime = _y+1
                elif s==2:
                    _yprime = _y-1
                elif s==3:
                    _xprime = _x-1
                elif s==4:
                    _xprime = _x+1
                ll = swap(_x,_y,_xprime,_yprime,a,olda,n,m)
                if (ll>Max):
                    Max = ll
                    iin = (j,s)
    #print(iin)
    (pos,dire) = iin
    xx = (int)(pos/m)
    yy =  pos%m
    return pos,xx,yy,dire
def read_map():
    file1 = open("map.txt")
    MAP = np.zeros((9,9))
    Cor = dict()
    row=0
    col=0
    iin = 0
    for index,i in enumerate(file1):
        if i=='\n':
            continue
        i = i.strip().split(' ')
        #print(index)
        if len(i)==0 :
            continue
        
        if index>col:
            #print("WOo\n")
            for x in i:
                #print(x)
                x = x.split(",")
                Cor[iin] = []
                Cor[iin].append(int(x[0]))
                Cor[iin].append(int(x[1]))
                iin+=1
        else:
            if index==0:
                row = int(i[0])
                col=  int(i[1])
                MAP = np.zeros((row,col))
            else:
                Y = np.asarray([int(_) for _ in i ])
                Y = Y[:col]
                MAP[index-1] = Y
    print(Y)
    #print(iin)
    #print(Cor)
    return Cor,MAP,row,col
def click(x,y,x1,y1):
    win32api.SetCursorPos((x,y))
    win32api.mouse_event(win32con.MOUSEEVENTF_LEFTDOWN,x,y,0,0)
    win32api.SetCursorPos((x1,y1))

    
def main():
    PrevMAP = None
    subprocess.Popen(["CandyCrush.exe"])
    u_input = raw_input("Start? (y/n)\n")
    if u_input == 'y' or u_input == 'Y':
        print("Go")
    else:
        exit(1)
    while(1):
        #u_input = raw_input("next move? (y/n)\n")
        #print(u_input)
        time.sleep(2)
        win32api.keybd_event(win32con.VK_F2, 0, 0, 0)
        time.sleep(2)
        Cor,MAP,row,col = read_map()
        if np.array_equal(PrevMAP ,MAP):
            continue
        else:
            PrevMAP = MAP
        if PrevMAP==None:
            PrevMAP = MAP
        
        print(row,col)
        leagal_move = search(MAP,row,col)
        Pos,x,y,direction = Make_move(leagal_move,row,col,MAP)
        print(Pos,x,y,direction)
        if direction==1:
            click(Cor[Pos][0],Cor[Pos][1],Cor[Pos+1][0],Cor[Pos+1][1])
        elif direction==2:
            click(Cor[Pos][0],Cor[Pos][1],Cor[Pos-1][0],Cor[Pos-1][1])
        elif direction==3:
            click(Cor[Pos][0],Cor[Pos][1],Cor[Pos-col][0],Cor[Pos-col][1])
        elif direction==4:
            click(Cor[Pos][0],Cor[Pos][1],Cor[Pos+col][0],Cor[Pos+col][1])
        
        #exit(1)
        
        
        


if __name__=="__main__":
    main()
            
