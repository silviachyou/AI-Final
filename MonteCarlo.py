import numpy as np
import random
exp_time = 10

def eliminate(olda,n,m):
    score = 0.
    Delete = [[0 for j in range(m)] for i in range(n)]
    for i in range(n):
        last = 0
        count = 1
        for j in range(m):
            if olda[i][j] == 0:
                continue
            if olda[i][j] == last:
                count += 1
            else:
                if count >= 3:
                    score += count
                count = 1
                last = olda[i][j]
            if count >= 3:
                for k in range(count):
                    Delete[i][j-k] = 1
        if count >= 3:
            score += count
            for k in range(count):
                Delete[i][j-k] = 1
    for i in range(m):
        last = 0
        count = 1
        for j in range(n):
            if olda[j][i] == 0:
                continue
            if olda[j][i] == last:
                count += 1
            else:
                if count >= 3:
                    score += count
                count = 1
                last = olda[j][i]
            if count >= 3:
                for k in range(count):
                    Delete[j-k][i] = 1
        if count >= 3:
            score += count
            for k in range(count):
                Delete[j-k][i] = 1
    for i in range(n):
        for j in range(m):
            if Delete[i][j] == 1:
                olda[i][j] = -1
    return olda,score

def countscore(olda,n,m):
    score = 0.
    for i in range(n):
        for j in range(m):
            if olda[i][j] == -1:
                score += 1

def generatemap(olda,n,m):
    score = 0.
    while(1):
        #print "-------------------"
        #print olda
        for i in range(n):
            for j in range(m):
                if olda[i][j] == -1:
                    olda[i][j] = random.randint(1,6)
        #print olda
        olda,temp_score = eliminate(olda,n,m)
        if temp_score == 0:
            break
        score += temp_score
    return score

def exp(x1,y1,x2,y2,a,olda,n,m):
    
    temp = olda[x1][y1]
    olda[x1][y1] = olda[x2][y2]
    olda[x2][y2] = temp
    olda,score = eliminate(olda,n,m)
    exp_score = 0.
    for i in range(exp_time):
        newa = olda.copy()
        temp_score = generatemap(newa,n,m)
        exp_score += temp_score
    #print exp_score
    score += exp_score/exp_time
    #print score


    return score