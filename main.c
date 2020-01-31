#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "game_header.h"
#include "cell_list.c"
#include "player_actions.c"

void ***map_reader()
{
    FILE *mf;

    char add[25];
    save_sel == 0 ? sprintf(add, "maps\\map_%0*d.lcm", 3, map_sel) : sprintf(add, "saves\\save_%0*d.lcs", 3, save_sel);
    mf = fopen(add, "rb");

    if (mf == NULL) //check if file available or is readable
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
                if (save_sel != 0)                    //there any saved map
                    fread(&hold, sizeof(int), 1, mf); //read energy from saved data
                *(int *)(map[i][j] + sizeof(char)) = (int)hold;
            }
            else
                map[i][j] = (char *)malloc(sizeof(char));
            *(char *)map[i][j] = (char)temp;
        }
    }
    if (save_sel != 0)
    {
        fread(&mode, sizeof(int), 1, mf);
        fread(&turn, sizeof(int), 1, mf);
        if (mode < 1 || mode > 2)
        {
            printf("file has been damaged!\n");
            return NULL;
        }
        for (int i = 0; i < mode; i++)
        {
            int cell_num;
            fread(&cell_num, sizeof(int), 1, mf);
            struct cell writer;
            while (cell_num--)
            {
                fread(&writer, sizeof(struct cell), 1, mf);
                add_cell(&player[i], writer);
            }
        }
    }
    fclose(mf);
    return map;
}
int init_coord()
{
    //making all houses zero
    if (coord == NULL)
    {
        coord = (int **)calloc(dim, sizeof(int *));
        for (int i = 0; i < dim; i++)
        {
            coord[i] = (int *)calloc(dim, sizeof(int));
        }
    }
    else
    {
        for (int i = 0; i < dim; i++)
            for (int j = 0; j < dim; j++)
            {
                coord[i][j] = 0;
                // printf("%d", coord[i][j]);
            }
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
                    printf("INSUFFICIENT SPACE FOR SUCH NUMBER OF CELLS!\n");
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
int load_coord()
{
    //allocating coord.
    //making all houses zero
    if (coord == NULL)
    {
        coord = (int **)calloc(dim, sizeof(int *));
        for (int i = 0; i < dim; i++)
        {
            coord[i] = (int *)calloc(dim, sizeof(int));
        }
    }
    else
    {
        for (int i = 0; i < dim; i++)
            for (int j = 0; j < dim; j++)
            {
                coord[i][j] = 0;
            }
    }
    for (int p = 0; p < mode; p++)
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
int map_design()
{
    TCLEAR();
    FRGB(100, 255, 200);
    //each line
    for (int i = dim - 1; i >= 0; i--)
    {
        SPACE(75);
        for (int s = 0; s < dim; s++)
            printf("\b\b\b\b");
        /* upper level of upper zigzag ->(on odd rows except top row)\
        (in loop)  /\  /\  /\  /\  ...
        (on even rows except top row) / */
        (i % 2 == 0) ? (i != dim - 1) ? printf(" \\ ") : printf("  ") : printf("");
        for (int j = 0; j < dim; j++)
        {
            printf("  /\\ ");
        }
        (i % 2 == 1 && i != dim - 1) ? printf("  /") : printf("");
        printf("\n");
        SPACE(75);
        for (int s = 0; s < dim; s++)
            printf("\b\b\b\b");
        /* down level of upper zigzag ->(on odd rows except top row) \
        (in loop)/  \/  \/  \/  \...
        (on even rows except top row)/*/
        (i % 2 == 0) ? (i != dim - 1) ? printf("  \\") : printf("   ") : printf("");
        for (int j = 0; j < dim; j++)
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
            SPACE(75);
            for (int s = 0; s < dim; s++)
                printf("\b\b\b\b");
            (i % 2 == 0) ? printf("   ") : printf("");
            for (int j = 0; j < dim; j++)
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
                if (l == 1 && coord[i][j] != 0 && *(char *)map[i][j] != BLOCKED)
                {
                    coord[i][j] > 0 ? FRGB(255, 150, 70) : FRGB(255, 255, 100);
                    printf("%s", find_cell(coord[i][j] > 0 ? 0 : 1, (coord[i][j] > 0 ? 1 : -1) * coord[i][j])->name);
                }
                else if (l == 0 && *(char *)map[i][j] == ENERGY)
                {
                    printf(":%0*d", 3, *(int *)(map[i][j] + sizeof(char)));
                }
                else
                    printf("    ");

                TRESET();
                FRGB(100, 255, 200);
            }
            printf("|\n");
        }
        // end of main content
    }
    //last zigzag row(neccesserily  \/\/\/\/)
    SPACE(75);
    for (int s = 0; s < dim; s++)
        printf("\b\b\b\b");
    printf("   ");
    for (int j = 0; j < dim; j++)
    {
        printf(" \\  /");
    }
    printf("\n");
    SPACE(75);
    for (int s = 0; s < dim; s++)
        printf("\b\b\b\b");
    printf("   ");
    for (int j = 0; j < dim; j++)
    {
        printf("  \\/ ");
    }
    printf("\n\n");
    SPACE(65);
    FRGB(255, 150, 70);
    mode == 1 ? printf("player 1") : printf("\b\b\b\bplayer 1");
    FRGB(255, 255, 100);
    mode == 1 ? TRESET() : printf(" - player 2\n");
    TRESET();
}
void splash()
{
    TCLEAR();
    _sleep(800);
    SPACE(65);
    FRGB(10, 255, 25);
    printf("Lonely Cells\n\n");
    _sleep(800);
    TRESET();
    FRGB(240, 10, 40);
    SPACE(66);
    printf("By M_Safari\n\n\n");
    TRESET();
    _sleep(200);
    SPACE(65);
    rec = fopen("record.lcr", "rb+");
    printf("[");
    for (int k = 11; k > 0; k--)
    {
        printf("#");
        _sleep(200);
    }
    printf("]\n");
    _sleep(400);
    if (rec == NULL)
        rec = fopen("record.lcr", "wb+");
}
int start_menu()
{
    int ret;
    do
    {
        TCLEAR();
        SPACE(65);
        printf("[1] Load_Game\n");
        SPACE(56);
        printf("[2] Start New Single Player Game\n");
        SPACE(60);
        printf("[3] Start New Multiplyer\n");
        SPACE(67);
        printf("[4] Exit\n");
        ret = getch() - '0';
        // ret = 1;
    } while (ret > 4 || ret < 1);
    return ret;
}
int action_menu(int player_no, int *cell_no)
{
    int ret;
    do
    {
        // TCLEAR();
        printf("\n");
        SPACE(60);
        FRGB(60, 200, 100);
        printf("player no %d\n", player_no + 1);
        SPACE(60);
        printf("[1] Move Cell\n");
        SPACE(58);
        printf("[2] Gather Energy\n");
        SPACE(61);
        printf("[3] Divide\n");
        SPACE(62);
        printf("[4] Undo\n");
        SPACE(62);
        printf("[5] Save\n");
        SPACE(59);
        printf("[6] Score Board\n");
        SPACE(62);
        printf("[7] Exit\n");
        ret = getch() - '0';
        // ret = 3;
        switch (ret)
        {
        case 7:
            EXIT();
        case 6:
            score_board();
            break;
        case 5:
            game_save();
            break;
        case 4:
            break;
            //undo;
        }
        if (ret < 1 || ret > 3)
            continue;
        SPACE(50);
        FRGB(150, 60, 200);
        printf("Select Preferred Cell\n");
        print_list(player[player_no]);
        scanf("%d", cell_no);
        // *cell_no = 2;
        if (find_cell(player_no, *cell_no) != NULL)
            return ret;
    } while (1);
}
void EXIT()
{
    FRGB(120, 90, 255);
    TCLEAR();
    printf("\n\n\n\n\n");
    SPACE(65);
    printf("Hope You Enjoyed\n\n\n\n\n");
    _sleep(800);
    TRESET();
    exit(0);
}
int main()
{
    srand(time(NULL));
    // splash();
    switch (start_menu())
    {
    case 1:
    {
        int temp = 0;
        int save_no = 1;
        do
        {
            TCLEAR();
            SPACE(60);
            printf("Select Map to Load:\n");
            while (1)
            {
                char add[30];
                sprintf(add, "saves\\save_%0*d.lcs", 3, save_no);
                FILE *fptr = fopen(add, "rb");
                if (!fptr)
                {
                    // printf("IS NULL");
                    break;
                }
                SPACE(62);
                printf("<%d> save_number_%d\n", save_no, save_no);
                save_no++;
                fclose(fptr);
            }
            temp = getchar() - '0';
            // temp = 1;
        } while (temp <= 0 || temp > --save_no);
        save_sel = temp;
        break;
    }
    case 2:
    {
        mode = 1;
        break;
    }
    case 3:
    {
        mode = 2;
        break;
    }
    case 4:
    {
        EXIT();
    }
    }
    if (save_sel == 0)
    {
        int temp = 1;
        do
        {
            TCLEAR();
            SPACE(60);
            printf("Enter Desired Map For New Game:\n");
            for (int i = 1; i <= maps; i++)
            {
                SPACE(63);
                printf("%d", i);
                i == 1 ? printf(" - default_map\n") : printf(" - %s", "read from file\n");
            }
            scanf("%d", &temp);
            // temp = 1;
        } while (temp < 0 || temp > maps);
        map_sel = temp;
        map = map_reader();
        if (map == (void ***)NULL)
        {
            return -1;
        }
        int r = -1;
        temp = 0;
        do
        {
            SPACE(50);
            printf("Enter the number of cells you want to start with:\n");
            scanf("%d", &temp);
            // temp = 3;
            for (int p = 0; p < mode; p++)
            {
                player[p] = NULL;
                for (int i = temp; i > 0; i--)
                {
                    struct cell tmp = {"!!!!", 0, 0, 0, 0, NULL, NULL};
                    rand_name(tmp.name);
                    add_cell(&(player[p]), tmp);
                }
            }
            if (!init_coord())
            {
                // TCLEAR();
                // SPACE(60);
                // printf("PLAYER %d GOT READY! ...\n\n", p + 1);
                break;
            }
            for (int i = 0; i < dim; i++)
                for (int j = 0; j < dim; j++)
                {
                    printf("%d", coord[i][j]);
                }
        } while (1);
        turn = 0;
    }
    else
    {
        player[1] = NULL;
        player[2] = NULL;
        map = map_reader();
        if (map == (void ***)NULL)
        {
            return -1;
        }
        load_coord();
    }
    map_design();
    getch();
    int cell_no, err = 0;
    while (1)
    {
        err = 0;
        switch (action_menu(turn % mode, &cell_no))
        {
        case 1:
        {
            int i = 0, j = 0;
            // do
            // {
            i = 0;
            j = 0;
            int temp = 0;
            do
            {
                SPACE(50);
                printf("[1] Northwest");
                SPACE(5);
                printf("[2] Northeast\n");
                SPACE(40);
                printf("[6] Left");
                SPACE(35);
                printf("[3] Right\n");
                SPACE(50);
                printf("[4] Southwest");
                SPACE(5);
                printf("[5] Southeast\n");
                temp = getch() - '0';
                // temp = 2;
            } while (temp < 0 || temp > 6);
            /* int d[dim][dim];
                for (int i = 0; i < dim; i++)
                for (int j = 0; j < dim; j++)
                d[i][j] = coord[i][j]; */
            switch (temp)
            {
            case 1:
            {
                i = 1;
                j = (find_cell(turn % mode, cell_no)->i) % 2 == 0 ? 0 : -1;
                break;
            }
            case 2:
            {
                i = 1;
                j = (find_cell(turn % mode, cell_no)->i) % 2 == 1 ? 0 : 1;
                break;
            }
            case 3:
            {
                i = 0;
                j = 1;
                break;
            }
            case 4:
            {
                i = -1;
                j = (find_cell(turn % mode, cell_no)->i) % 2 == 0 ? 0 : -1;
                break;
            }
            case 5:
            {
                i = -1;
                j = (find_cell(turn % mode, cell_no)->i) % 2 == 1 ? 0 : +1;
                break;
            }
            case 6:
            {
                i = 0;
                j = -1;
                break;
            }
            default:
                err = 1;
            }
            // }
            if (move(turn % mode, cell_no, i, j))
            {
                err = 1;
            }
            break;
        }
        case 2:
        {
            if (gather(turn % mode, cell_no))
            {
                err = 1;
            }
            break;
        }
        case 3:
        {
            if (divide(turn % mode, cell_no))
            {
                err = 1;
            }
            break;
        }
        }
        if (err)
        {
            printf("an_error_occured_press_any_key_to_play_your_round\n");
            getch();
            TCLEAR();
            map_design();
            continue;
        }
        turn++;
        map_design();
        getch();
    }
}