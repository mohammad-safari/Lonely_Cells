#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "game_header.h"
#include "cell_list.c"

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
int init_coord()
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
int load_coord() ///one ply!
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
    if (cursor->energy == 100)
    {
        printf("Cell has Maximum Energy!");
        return -1;
    }
    if (*e < 15)
    {
        if (cursor->energy + *e <= 100)
        {
            cursor->energy += *e;
            *e = 0;
            return 0;
        }
    }
    if (*e >= 15)
    {
        if (cursor->energy + 15 <= 100)
        {
            cursor->energy += 15;
            *e -= 15;
            return 0;
        }
    }
    //if energy overflows
    e -= 100 - cursor->energy;
    cursor->energy = 100;
    return 0;
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
                cell1.energy = (int)(e / 2);
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
int map_design()
{
    FRGB(100, 255, 200);
    //each line
    for (int i = dim - 1; i >= 0; i--)
    {
        /* upper level of upper zigzag ->(on odd rows except top row)\
        (in loop)  /\  /\  /\  /\  ...
        (on even rows except top row) / */
        (i % 2 == 0) ? (i != dim - 1) ? printf(" \\ ") : printf("  ") : printf("");
        for (int j = dim - 1; j >= 0; j--)
        {
            printf("  /\\ ");
        }
        (i % 2 == 1 && i != dim - 1) ? printf("  /") : printf("");
        printf("\n");
        /* down level of upper zigzag ->(on odd rows except top row) \
        (in loop)/  \/  \/  \/  \...
        (on even rows except top row)/*/
        (i % 2 == 0) ? (i != dim - 1) ? printf("  \\") : printf("  ") : printf("");
        for (int j = dim - 1; j >= 0; j--)
        {
            printf(" /");
            TRESET();
            char type[3];
            switch (*(char *)map[i][j])
            {
            case BLOCKED:
                strcpy(type, "BL");
                FRGB(255, 0, 0);
                break;
            case ENERGY:
                strcpy(type, "EN");
                FRGB(200, 20, 190);
                break;
            case MITOSIS:
                strcpy(type, "MI");
                FRGB(250, 250, 10);
                break;
            default:
                strcpy(type, "NR");
                FRGB(0, 255, 0);
                break;
            }
            printf("%s", type);
            TRESET();
            FRGB(100, 255, 200);
            printf("\\");
        }
        (i % 2 == 1 && i != dim - 1) ? printf(" /") : printf("");
        printf("\n");
        // the main content of each house
        for (int l = 0; l < 2; l++)
        {
            (i % 2 == 0) ? printf("   ") : printf("");
            for (int j = dim - 1; j >= 0; j--)
            {
                printf("|");
                TRESET();
                switch (*(char *)map[i][j])
                {
                case BLOCKED:
                    FRGB(255, 0, 0);
                    break;
                case ENERGY:
                    FRGB(200, 20, 190);
                    break;
                case MITOSIS:
                    FRGB(250, 250, 10);
                    break;
                default:
                    FRGB(0, 255, 0);
                    break;
                }
                (l == 0 && coord[i][j] != 0 && *(char *)map[i][j] != BLOCKED) ? printf("%s", find_cell(coord[i][j] > 0 ? 0 : 1, (coord[i][j] > 0 ? 1 : -1) * coord[i][j])->name) : printf("    ");
                TRESET();
                FRGB(100, 255, 200);
            }
            printf("|\n");
        }
        // end of main content
    }
    //last zigzag row(neccesserily  \/\/\/\/)
    printf("   ");
    for (int j = dim - 1; j >= 0; j--)
    {
        printf(" \\  /");
    }
    printf("\n");
    printf("   ");
    for (int j = dim - 1; j >= 0; j--)
    {
        printf("  \\/ ");
    }
    TRESET();
}
int main()
{
    srand(time(NULL));
    init_coord();
    map = map_reader();
    map == NULL ? exit(0) : map_design();
}