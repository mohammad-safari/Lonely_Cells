#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//keep player cells data
struct cell
{
    char name[10];
    int no;
    int i;
    int j;
    int energy;
    struct cell *next;
    struct cell *prev;
} * *player[2];
//relate types of houses to char for better interpretation
enum house
{
    ENERGY = '1',
    MITOSIS,
    BLOCKED,
    NORMAL
};
//each house have feature variables  1-type 2-energy level if type 1(energy)
void ***map;
//demonstrates using which saved map
int saves = 0;
//demonstrates using which map
int maps = 1;
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
        printf("(%d, %d)\n", cursor->j, cursor->i);
        cursor = cursor->next;
    }
}
void ***map_reader()
{
    int dim;
    FILE *mf;

    char add[25];
    saves == 0 ? sprintf(add, "maps\\map_%0*d.lcm", 3, maps) : sprintf(add, "saves\\saved_map_%0*d.lcm", 3, saves);
    mf = fopen(add, "rb");

    if (mf == NULL) //check if file ecursorists or is readable
    {
        printf("Can't Read Map");
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
                printf("Map Has Been Damaged!");
                return (void ***)NULL;
            }
            char temp;
            fread(&temp, sizeof(char), 1, mf);
            if (temp > '4' || temp < '1') // if data is not acceptable
            {
                printf("Map Has Been Damaged!");
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

int main()
{
    // map = map_reader();
    // if (map != NULL)
    //     printf("%d\n", *(int *)(map[0][3] + sizeof(char)));
    // system("pause");
}