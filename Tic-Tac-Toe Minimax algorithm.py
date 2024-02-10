# -*- coding: utf-8 -*-
"""
Created on Fri Feb  9 19:19:32 2024

@author: hp
"""

import numpy as np 

player='X'
opponent='O'
gamesStatesCount=0

#maxDepth variable can be made to 

def printBoard(board):
    for i in range(3):
        for j in range(3):
            print(board[i][j],end=' ')
        print('\n')

def isWinner(board):
    for row in range(3):
        if board[row][0]==board[row][1]==board[row][2]!='_':
            if board[row][0]==player:
                return 1
            else:
                return 2
    
    for col in range(3):
        if board[0][col]==board[1][col]==board[2][col]!='_':
            if board[0][col]==player:
               return 1
            else:
                return 2
            
        
    if [board[i][i] for i in range(3)].count(player)==3 or [board[i][2-i] for i in range(3)].count(player)==3:  
        return 1
    
    if [board[i][i] for i in range(3)].count(opponent)==3 or [board[i][2-i] for i in range(3)].count(opponent)==3:  
        return 2
    
    return 0

def isMovesLeft(board):
    for row in board:
        if '_' in row:
            return True
    return False

def getMoves(board):
    moves=[]
    for i,row in enumerate(board): #gives row index and row
        for j,cell in enumerate(row): #gives column index and value of that cell
            if cell=='_':
                moves.append((i,j)) #storing all the available moves
    return moves

def evaluateValue(board):
    #agent is using this function for its own score
    #return 10 if agent wins and -10 if player wins and 0 if no one wins
    
    for row in board:
        if row.count(opponent) == 3:
            return 10
        if row.count(player) == 3:
            return -10

    # Check columns for victory
    for col in range(3):
        if board[0][col] == board[1][col] == board[2][col] != '_':
            if board[0][col] == opponent:
                return 10
            if board[0][col] == player:
                return -10

    # Check diagonals for victory
    if (board[0][0] == board[1][1] == board[2][2] != '_') or (board[0][2] == board[1][1] == board[2][0] != '_'):
        if board[1][1] == opponent:
            return 10
        if board[1][1] == player:
            return -10

    return 0

#Minimax Algorithm (without using alpha beta pruning)
def minimax(board,depth,isMax):
    #returning the best move for the agent and the value of the best move
    global gamesStatesCount
    gamesStatesCount+=1
    score=evaluateValue(board)
    if score==10: #opponent wins
        return None,score-depth
    if score==-10: #player wins
        return None,score+depth
    if isMovesLeft(board)==False:
        return None,0
    
    if isMax: #opponent's move
        bestScore=float('-inf')
        bestMove=None
        for move in getMoves(board): 
            #move is the pair of indexes of board having empty slot
            #backtracking the move
            board[move[0]][move[1]]=opponent
            _,score=minimax(board,depth+1,False)
            board[move[0]][move[1]]='_'
            if score>bestScore: # we want to maximize the score
                bestScore=score
                bestMove=move 

    else: #player's move
        bestScore=float("inf")
        bestMove=None
        for move in getMoves(board):
            board[move[0]][move[1]]=player
            _,score=minimax(board,depth+1,True,)
            board[move[0]][move[1]]='_'
            if score<bestScore: #player wants to minimize the score
                bestScore=score
                bestMove=move
        
    return bestMove,bestScore

#Minimax Algorithm using Alpha Beta pruning
def OptMinimax(board,depth,isMax,alpha,beta):
    #returning the best move for the agent and the value of the best move
    global gamesStatesCount
    gamesStatesCount+=1
    score=evaluateValue(board)
    if score==10: #opponent wins
        return None,score-depth
    if score==-10: #player wins
        return None,score+depth
    if isMovesLeft(board)==False:
        return None,0
    
    if isMax: #opponent's move
        bestScore=float('-inf')
        bestMove=None
        for move in getMoves(board): 
            #move is the pair of indexes of board having empty slot
            #backtracking the move
            board[move[0]][move[1]]=opponent
            _,score=OptMinimax(board,depth+1,False,alpha,beta)
            board[move[0]][move[1]]='_'
            if score>bestScore: # we want to maximize the score
                bestScore=score
                alpha=max(alpha,bestScore)
                bestMove=move 
            if alpha>=beta:
                break

    else: #player's move
        bestScore=float("inf")
        bestMove=None
        for move in getMoves(board):
            board[move[0]][move[1]]=player
            _,score=OptMinimax(board,depth+1,True,alpha,beta)
            board[move[0]][move[1]]='_'
            if score<bestScore: #player wants to minimize the score
                bestScore=score
                beta=min(beta,bestScore)
                bestMove=move
            if alpha>=beta:
                break
        
    return bestMove,bestScore


def AgentMove(board):
    move,_=minimax(board,0,True)
    return move

def AgentMoveOpt(board,alpha,beta):
    move,_=OptMinimax(board, 0, True,alpha,beta)
    return move

def TicTacToe():
    board=[["_"]*3 for _ in range(3)]
    
    i=1
    print("==============WELCOME TO THE TIC-TAC-TOE GAME===============\n")
        
    print("Enter 1 to play first.")
    print("Enter 2 to play second.")
    chance=input("Your turn: ")
    if(chance=='1'):
        currentPlayer=player
    else:
        currentPlayer=opponent

    print("Enter 1 to run the minimax algorithm without using alpha beta pruning.")
    print("Enter 2 to run the optimised minimax algorithm using alpha beta pruning.")
    a=input("Enter your choice: ")
    
    while True:
        print(f"Board state {i}: ")
        printBoard(board)
        if currentPlayer==player:
            print("Player's move: ")
            if a=='1' and minimax(board, 0, True)[0] is not None:
                #for automatically generated output
                #x, y = minimax(board, 0, True)[0]
                #x,y = OptiMinimax(board,0,True,float())
                x,y=map(int,input().split())
                if board[x][y]!='_':
                    print("Invalid move! Try again")
                    continue
                board[x][y]=player
            elif a=='2' and OptMinimax(board,0,True,float('-inf'),float('inf')) is not None:
                x,y=map(int,input().split())
                if board[x][y]!='_':
                    print("Invalid move! Try again")
                    continue
                board[x][y]=player
                
            currentPlayer=opponent
            
        else:
            print("Agent's move: ")
            if a=='1' and minimax(board, 0, True)[0] is not None:
                x,y=AgentMove(board)
                board[x][y]=opponent
                
            if a=='2' and OptMinimax(board, 0, True,float('-inf'),float('inf'))[0] is not None:
                x,y=AgentMoveOpt(board,float('-inf'),float('inf'))
                board[x][y]=opponent
                
            currentPlayer=player
        i=i+1
        
        if isWinner(board)==1:
            print(f"Player wins with {player}!")
            print("The final board is:")
            printBoard(board)
            break
        
        elif isWinner(board)==2:
            print(f"Agent wins with {opponent}!")
            print("The final board is: ")
            printBoard(board)
            break
        
        elif isMovesLeft(board)==False:
            print("It's a TIE!")
            print("The final board is:")
            printBoard(board)
            break 
        
    #global gamesStatesCount
    print(f"Total number of evaluated game states: {gamesStatesCount}.")

def main():
    TicTacToe()
    
if __name__=="__main__":
    main()
    
 
''' TO Plot the graphs '''

# Creating two separate plots: one for the averages and one for the individual points

# Plot for the averages
import matplotlib.pyplot as plt

# Data for plotting -> by running 5 test cases and finding their states travelled for both minimax and optimal minimax
minimax1 = [678350, 689230, 678310, 670458, 689784]
optMinimax1 = [27637, 29659, 33108, 27039, 33765]

# Calculate averages
avgMin = sum(minimax1) / len(minimax1)
avgOpt = sum(optMinimax1) / len(optMinimax1)

indices = list(range(1, len(minimax1) + 1))

plt.figure(figsize=(10, 5))
plt.bar(['Minimax', 'OptMinimax'], [avgMin, avgOpt], color=['pink', 'orange'])
plt.title('Average Number of States Evaluated')
plt.ylabel('Average States Count')
plt.show()

# Plot for the individual points
plt.figure(figsize=(14, 7))
plt.plot(indices, minimax1, 'go-', label='Minimax Points')
plt.plot(indices, optMinimax1, 'ro-', label='OptMinimax Points')
plt.title('Number of States Evaluated for Each Test Case')
plt.xlabel('Test Case Index')
plt.ylabel('Number of States Evaluated')
plt.legend()
plt.grid(True)
plt.show()


'''Sample Code Run

//Minimax without using alpha beta pruning

==============WELCOME TO THE TIC-TAC-TOE GAME===============

Enter 1 to run the minimax algorithm without using alpha beta pruning.
Enter 2 to run the optimised minimax algorithm using alpha beta pruning.

Enter your choice: 1
Board state 1: 
_ _ _ 

_ _ _ 

_ _ _ 

Player's move: 

0 0
Board state 2: 
X _ _ 

_ _ _ 

_ _ _ 

Agent's move: 
Board state 3: 
X _ _ 

_ O _ 

_ _ _ 

Player's move: 

0 2
Board state 4: 
X _ X 

_ O _ 

_ _ _ 

Agent's move: 
Board state 5: 
X O X 

_ O _ 

_ _ _ 

Player's move: 

2 1
Board state 6: 
X O X 

_ O _ 

_ X _ 

Agent's move: 
Board state 7: 
X O X 

O O _ 

_ X _ 

Player's move: 

1 2
Board state 8: 
X O X 

O O X 

_ X _ 

Agent's move: 
Board state 9: 
X O X 

O O X 

_ X O 

Player's move: 

2 0
It's a TIE!
The final board is:
X O X 

O O X 

X X O 

Total number of evaluated game states: 678350.


//Minimax using alpha beta pruning

==============WELCOME TO THE TIC-TAC-TOE GAME===============

Enter 1 to run the minimax algorithm without using alpha beta pruning.
Enter 2 to run the optimised minimax algorithm using alpha beta pruning.

Enter your choice: 2
Board state 1: 
_ _ _ 

_ _ _ 

_ _ _ 

Player's move: 

0 0
Board state 2: 
X _ _ 

_ _ _ 

_ _ _ 

Agent's move: 
Board state 3: 
X _ _ 

_ O _ 

_ _ _ 

Player's move: 

0 2
Board state 4: 
X _ X 

_ O _ 

_ _ _ 

Agent's move: 
Board state 5: 
X O X 

_ O _ 

_ _ _ 

Player's move: 

2 1
Board state 6: 
X O X 

_ O _ 

_ X _ 

Agent's move: 
Board state 7: 
X O X 

O O _ 

_ X _ 

Player's move: 

1 2
Board state 8: 
X O X 

O O X 

_ X _ 

Agent's move: 
Board state 9: 
X O X 

O O X 

_ X O 

Player's move: 

2 0
It's a TIE!
The final board is:
X O X 

O O X 

X X O 

Total number of evaluated game states: 27637.
 '''
 
 
