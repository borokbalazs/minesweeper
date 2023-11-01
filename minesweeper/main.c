#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "debugmalloc.h"

typedef struct Cell{
    bool hidden,bomb,flag;
    int surround;
}Cell;
typedef struct Field{
    int sizex,sizey, bombs;
    Cell *cells;
}Field;

void swapBomb(Field *map, int i, int j){
    bool temp;
    temp = map->cells[i].bomb;
    map->cells[i].bomb = map->cells[j].bomb;
    map->cells[j].bomb=temp;
}
void shuffleBombs(Field *map){
    int fsize = map->sizex*map->sizey;
    for(int i=0;i<fsize;i++){
        swapBomb(map, rand()%fsize, rand()%fsize);
    }
}

Field createField(int sizex, int sizey, int bombs){
    Field newField;
    newField.sizex=sizex;
    newField.sizey=sizey;
    Cell *newCells=malloc(sizex*sizey*sizeof(Cell));
    newField.bombs=bombs;
    for(int i=0;i<(sizex*sizey);i++){
        newCells[i].hidden=true;
        newCells[i].bomb=false;
        newCells[i].flag=false;
    }
    for(int i=0;i<bombs;i++){
        newCells[i].bomb=true;
    }
    newField.cells=newCells;
    shuffleBombs(&newField);
    return newField;
}
typedef struct Player{
    int x,y;
}Player;
void printCell(Cell cell){
    if(cell.flag){
        printf("|F|");
    }
    else if(cell.hidden){
        printf("|#|");
    }else if(cell.bomb==true){
        printf("|B|");
    }else if(cell.surround==0){
        printf("| |");
        }else{
    printf("|%d|", cell.surround);
    }
}


void printMap(Field map, Player player){
    for(int i=0;i<map.sizey;i++){
        for(int j=0;j<map.sizex;j++){
                if(player.y*map.sizex+player.x==i*map.sizex+j){
                    printf("|P|");
                }else{
            printCell(map.cells[i*map.sizex+j]);}
        }
        printf("\n");
    }
}
void revealCell(Field map, Cell *mycell, int x, int y){
    mycell->hidden=false;
    mycell->surround=cellNeighbors(map, x, y);
    if(mycell->surround==0){
        if(map.cells[y*map.sizex+(x-1)].hidden==true && x>0){
        revealCell(map, &map.cells[y*map.sizex+(x-1)], x-1, y);}
        if(map.cells[y*map.sizex+(x+1)].hidden==true && x<map.sizex-1){
        revealCell(map, &map.cells[y*map.sizex+(x+1)], x+1, y);}
        if(map.cells[(y+1)*map.sizex+x].hidden==true && y<map.sizey-1){
        revealCell(map, &map.cells[(y+1)*map.sizex+x], x, y+1);}
        if(map.cells[(y-1)*map.sizex+x].hidden==true && y>0){
        revealCell(map, &map.cells[(y-1)*map.sizex+x], x, y-1);}


        if(map.cells[(y-1)*map.sizex+(x-1)].hidden==true && y>0 && x>0){
        revealCell(map, &map.cells[(y-1)*map.sizex+(x-1)], x-1, y-1);}

        if(map.cells[(y-1)*map.sizex+(x+1)].hidden==true && y>0 && x<map.sizex-1){
        revealCell(map, &map.cells[(y-1)*map.sizex+(x+1)], x+1, y-1);}

        if(map.cells[(y+1)*map.sizex+(x+1)].hidden==true && y<map.sizey-1 && x<map.sizex-1){
        revealCell(map, &map.cells[(y+1)*map.sizex+(x+1)], x+1, y+1);}

        if(map.cells[(y+1)*map.sizex+(x-1)].hidden==true && y<map.sizey-1 && x>0){
        revealCell(map, &map.cells[(y+1)*map.sizex+(x-1)], x-1, y+1);}
    }

}
void flagCell(Field map, Cell *mycell){
    if(mycell->flag==false){
        mycell->flag=true;
    }else{
    mycell->flag=false;}
}
int cellNeighbors(Field map, int x, int y){
    int sum=0;
    if(map.cells[y*map.sizex+(x-1)].bomb==true && x>0){
        sum++;
    }
    if(map.cells[y*map.sizex+(x+1)].bomb==true && x<map.sizex-1){
        sum++;
    }
    if(map.cells[(y+1)*map.sizex+x].bomb==true && y<map.sizey-1){
        sum++;
    }
    if(map.cells[(y-1)*map.sizex+x].bomb==true && y>0){
        sum++;
    }
    if(map.cells[(y-1)*map.sizex+x+1].bomb==true && y>0 && x<map.sizex-1){
        sum++;
    }
    if(map.cells[(y-1)*map.sizex+x-1].bomb==true && y>0 && x>0){
        sum++;
    }
    if(map.cells[(y+1)*map.sizex+x+1].bomb==true && y<map.sizey-1 && x< map.sizex-1){
        sum++;
    }
    if(map.cells[(y+1)*map.sizex+x-1].bomb==true && y<map.sizey-1 && x>0){
        sum++;
    }

    return sum;
}
int main()
{
    int sizex, sizey, bombs;
    printf("Field width, height, bombs |x,y,b|: ");
    scanf("%d %d %d", &sizex, &sizey, &bombs);
    Field map = createField(sizex,sizey, bombs);
    Player player;
    player.x=0;
    player.y=0;
    printMap(map, player);

    while (true){
        switch(getch()){
        case 'a':
            player.x--;
        break;
        case 'd':
            player.x++;
        break;
        case 'w':
            player.y--;
        break;
        case 's':
            player.y++;
        break;
        case 'o':
            revealCell(map, &map.cells[player.y*map.sizex+player.x], player.x, player.y);
        break;
        case 'f':
            flagCell(map, &map.cells[player.y*map.sizex+player.x]);
        break;
        case 'q':
            goto vege;
            break;

        }
        system("cls");
        printMap(map,player);
    }
    vege:
    free(map.cells);
    return 0;
}
