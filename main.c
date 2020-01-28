#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
//keep player cells data
struct cell
{
    char name[4];
    int no;
    int i;
    int j;
    int energy;
    struct cell *next;
    struct cell *prev;
} * player[2];
//relate types of houses to char for better interpretation
enum house
{
    ENERGY = '1',
    MITOSIS,
    BLOCKED,
    NORMAL
};
//map dimension
int dim;
//each house have feature variables  1-type 2-energy level if type 1(energy)
void ***map;
//keep coord. of player cells
int **coord;
//demonstrates using which saved map
int saves = 0;
//demonstrates using which map
int maps = 1;
//one player(0) or two player(1)
int mode = 0;
//computer plays or not
int computer = 0;
//add cells
int add_cell(struct cell **, struct cell);
//remove cell
int remove_cell(struct cell **, int);
//print list of cells acoording to their name
void print_list(struct cell *);
//load map in ram
void ***map_reader();
//load player cells saved data
struct cell ***load_player(FILE **);

int add_cell(struct cell **player, struct cell data)
{
    //putting data in a cell in heap!
    struct cell *cell = (struct cell *)malloc(sizeof(struct cell));
    if (cell == NULL)
    {
        printf("NOT Enough Memeory Available!");
        return -1;
    }
    strcpy(cell->name, data.name);
    cell->i = data.i;
    cell->j = data.j;
    cell->energy = data.energy;
    cell->next = NULL;
    cell->prev = NULL;
    int no = 1;
    //make sure that list ecursorists or create list anyway!
    if (*player == NULL)
    {
        cell->no = no;
        *player = cell; //link head to cell!
        return 1;
    }
    //move toward the last cell
    struct cell *cursor = *player;
    struct cell *insert = cursor; // default for second cell
    while (cursor->next != NULL)
    {
        if (cursor->next->next == NULL)
            insert = cursor->next->prev;
        cursor = cursor->next;
        insert = insert->next;
        no++;
    }
    cell->prev = insert;
    cursor->next = cell; //append cell to list
    cell->no = ++no;
    return 0;
}
int remove_cell(struct cell **player, int no)
{
    struct cell *cursor = *player;
    while (cursor != NULL && cursor->no != no)
    {
        cursor = cursor->next;
    }
    if (cursor == NULL)
    {
        return -1; //not found!
    }
    if (cursor->prev == NULL)
    {
        *player = cursor->next; //changing head of list
        cursor->next->prev = NULL;
        free((struct cell *)cursor);
        return 1;
    }
    if (cursor->next == NULL)
    {
        cursor->prev->next = NULL; //put away end of list
        free((struct cell *)cursor);
        return 2;
    }
    cursor->prev->next = cursor->next;
    cursor->next->prev = cursor->prev;
    //:)
    free((struct cell *)cursor);
    return 0;
}
void print_list(struct cell *cursor)
{
    while (cursor != NULL)
    {
        printf("%d) ", cursor->no);
        printf("%s ", cursor->name);
        printf("(%d, %d)\n", cursor->j + 1, cursor->i + 1);
        cursor = cursor->next;
    }
}
void ***map_reader()
{
    FILE *mf;

    char add[25];
    saves == 0 ? sprintf(add, "maps\\map_%0*d.lcm", 3, maps) : sprintf(add, "saves\\saved_map_%0*d.lcm", 3, saves);
    mf = fopen(add, "rb");

    if (mf == NULL) //check if file ecursorists or is readable
    {
        printf("Can't Read Map\n");
        return (void ***)NULL;
    }
    fread(&dim, sizeof(int), 1, mf);
    //each row
    map = (void ***)calloc(dim, sizeof(void **));
    for (int i = 0; i < dim; i++)
    {
        map[i] = (void **)calloc(dim, sizeof(void *));
        //each column
        for (int j = 0; j < dim; j++)
        {
            if (feof(mf)) //if data is incomplete or dim is changed illegaly
            {
                printf("Map Has Been Damaged!\n");
                return (void ***)NULL;
            }
            char temp;
            fread(&temp, sizeof(char), 1, mf);
            if (temp > '4' || temp < '1') // if data is not acceptable
            {
                printf("Map Has Been Damaged!\n");
                return (void ***)NULL;
            }
            if (temp == ENERGY)
            {
                int hold = 100; //default energy level
                map[i][j] = (void *)malloc(sizeof(char) + sizeof(int));
                if (saves != 0)                       //there any saved map
                    fread(&hold, sizeof(int), 1, mf); //read energy from saved data
                *(int *)(map[i][j] + sizeof(char)) = (int)hold;
            }
            else
                map[i][j] = (char *)malloc(sizeof(char));
            *(char *)map[i][j] = (char)temp;
        }
    }
    // FILE **pmf = &mf;
    // if (saves != 0)
    //     players = load_player(pmf);
    fclose(mf);
    return map;
}
int init_coord() ////kjbbjj ///one ply!
{
    //making all houses zero
    coord = (int **)calloc(dim, sizeof(int *));
    for (int i = 0; i < dim; i++)
    {
        coord[i] = (int *)calloc(dim, sizeof(int));
    }
    //setting coordinations
    for (int p = 0; p < mode; p++)
    {
        struct cell *cursor = player[p];
        while (cursor != NULL)
        {
            int i, j, count = 0;
            do
            {
                i = rand() % dim;
                j = rand() % dim;
                count++;
                if (count > dim * dim * dim)
                {
                    printf("NOT ENOUGH SPACE FOR SUCH NUMBER OF CELLS!\n");
                    return -1;
                }
            } while (coord[i][j] != 0 || *(char *)map[i][j] == BLOCKED);
            int ce = 1;
            if (p == 1)
            {
                ce = -1;
            }
            coord[i][j] = cursor->no * ce; //no starts from 1
            cursor->i = i;
            cursor->j = j;
            cursor = cursor->next;
        }
    }
    return 0;
}
int load_coord() ////kjbbjj ///one ply!
{
    for (int p = 0; p < 2; p++)
    {
        struct cell *cursor = player[p];
        while (cursor != NULL)
        {
            int ce = 1;
            if (p == 1)
            {
                ce = -1;
            }
            coord[cursor->i][cursor->j] = cursor->no * ce; //no starts from 1
            cursor = cursor->next;
        }
    }
    return 0;
}
struct cell *find_cell(int player_no, int cell_no)
{
    struct cell *cursor = player[player_no];
    while (--cell_no && cursor != NULL)
    {
        cursor = cursor->next;
    }
    if (cursor == NULL)
    {
        printf("Cell does not Exist!\n");
        return NULL;
    }
    return cursor;
}
int move(int player_no, int cell_no, int di, int dj) //use find cell
{
    struct cell *cursor = player[player_no];
    while (--cell_no && cursor != NULL)
    {
        cursor = cursor->next;
    }
    if (cursor == NULL)
    {
        printf("Cell does not Exist!\n");
        return -2;
    }
    int i = cursor->i;
    int j = cursor->j;
    if (coord[i + di][j + dj] != 0 || *(char *)(map[i + di][j + dj]) == BLOCKED)
    {
        printf("Can not do such Move\n");
        return -1;
    }
    cursor->i += di;
    cursor->j += dj;
    coord[i + di] = coord[i];
    coord[j + dj] = coord[j];
    coord[i] = 0;
    coord[j] = 0;
    return 0;
}
int gather(int player_no, int cell_no) //use find cell
{
    struct cell *cursor = player[player_no];
    while (--cell_no && cursor != NULL)
    {
        cursor = cursor->next;
    }
    if (cursor == NULL)
    {
        printf("Cell does not Exist!\n");
        return -3;
    }
    int i = cursor->i;
    int j = cursor->j;
    if (*(char *)(map[i][j]) != ENERGY)
    {
        printf("Cell Can Gather Energy Only on ENERGY HOUSES!\n");
        return -2;
    }
    int *e = (int *)(map[i][j] + sizeof(char));
    if (*e <= 0)
    {
        printf("House is Empty!");
        return -1;
    }
    if (*e < 15 && *e != 0)
    {
        cursor->energy += *e;
        *e = 0;
        return 0;
    }
    if (*e >= 15)
    {
        cursor->energy += 15;
        *e -= 15;
        return 0;
    }
}
int divide(int player_no, int cell_no)
{
    struct cell *cursor = NULL;
    find_cell(player_no, cell_no);
    if (cursor == NULL)
        return -4;
    int i = cursor->i;
    int j = cursor->j;
    if (*(char *)map[i][j] != MITOSIS)
    {
        printf("Cell Division Only Happens on MITOSIS HOUSES!\n");
        return -3;
    }
    if (cursor->energy < 80)
    {
        printf("Cell is not Ready for Division!\n(At least 80 Levels of Energy is Needed)\n");
        return -2;
    }
    int di, dj;
    for (int q = -1; q < 2; q++)
        for (int p = -1; p < 2; p++)
            if (coord[i + di][j + dj] == 0 && *(char *)map[i + di][j + dj] != BLOCKED)
            {
                int e = cursor->energy;
                struct cell cell1, cell2;
                cell1.energy = (int)(e/2);
                cell2.energy = e - cell1.energy;
                cell1.i = i;
                cell1.j = j;
                cell2.i = i + q;
                cell2.j = j + p;
                remove_cell(&(player[player_no]), cell_no);
                add_cell(&(player[player_no]), cell1);
                add_cell(&(player[player_no]), cell2);
            }
    printf("NOT ENOUGH SPACE FOR THIS DIVISION!\n");
    return -1;
}

int main()
{
    srand(time(NULL));
    // map = map_reader();
    // if (map != NULL)
    //     printf("%d\n", *(int *)(map[0][3] + sizeof(char)));
    system("pause");
}