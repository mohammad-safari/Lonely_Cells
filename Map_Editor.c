#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define BRGB(A, B, C) printf("\033[48;2;" #A ";" #B ";" #C "m")
#define FRGB(A, B, C) printf("\033[38;2;" #A ";" #B ";" #C "m")
#define TRESET() printf("\033[0m")
#define TCLEAR() printf("\033[2J")
#define SPACE(A) printf("%" #A "s", "")
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
void ***edit;
//shows selected map
int map_sel = 1;

int map_design(void ***map)
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
                // if (l == 0 && *(char *)map[i][j] == ENERGY)
                // {
                //     printf(":%0*d", 3, *(int *)(map[i][j] + sizeof(char)));
                // }
                // else
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
    printf("\n\n\n");
    SPACE(65);
    TRESET();
}
void ***map_reader()
{
    FILE *mf;

    char add[25];
    sprintf(add, "maps\\map_%0*d.lcm", 3, map_sel);
    mf = fopen(add, "rb");

    if (mf == NULL) //check if file available or is readable
    {
        printf("Can't Read Map\n");
        return (void ***)NULL;
    }
    fread(&dim, sizeof(int), 1, mf);
    //each row
    map = (void ***)calloc(dim, sizeof(void **));
    edit = (void ***)calloc(dim, sizeof(void **));
    for (int i = 0; i < dim; i++)
    {
        map[i] = (void **)calloc(dim, sizeof(void *));
        edit[i] = (void **)calloc(dim, sizeof(void *));
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
            map[i][j] = (char *)malloc(sizeof(char));
            edit[i][j] = (char *)malloc(sizeof(char));
            *(char *)map[i][j] = (char)temp;
            *(char *)edit[i][j] = (char)temp;
        }
    }
    fclose(mf);
    return map;
}
int map_text(char *add)
{
    FILE *txt;
    txt = fopen(add, "w");
    fprintf(txt, "map dimension: %d * %d\n", dim, dim);
    for (int i = 0; i < dim; i++)
    {

        for (int j = 0; j < dim; j++)
        {
            char type[50];
            switch (*(char *)map[i][j])
            {
            case BLOCKED:
                strcpy(type, "Blocked House");
                break;
            case MITOSIS:
                strcpy(type, "Mitosis House");
                break;
            case ENERGY:
                strcpy(type, "Energy House");
                break;
            default:
                strcpy(type, "Normal House");
                break;
            }
            fprintf(txt, "%dth line, %dth column is %s\n", i, j, type);
        }
    }
    fclose(txt);
}
int map_save()
{
    int save_no = 1;
    char add[30];
    while (1)
    {
        sprintf(add, "maps\\map_%0*d.lcm", 3, save_no);
        if (!fopen(add, "rb"))
        {
            break;
        }
        save_no++;
    }
    FILE *svef = fopen(add, "wb");
    if (svef == NULL)
    {
        printf("Couldn't Save Game!\n");
        return -1;
    }
    fwrite(&dim, sizeof(int), 1, svef);
    //each line
    for (int i = 0; i < dim; i++)
    {
        //each column
        for (int j = 0; j < dim; j++)
        {
            char temp = *(char *)edit[i][j];
            if (temp - '0' > 4 || temp - '0' < 1)
            {
                printf("map cannot be saved!");
                return -1;
            }
            fwrite(&temp, sizeof(char), 1, svef);
        }
    }

    SPACE(50);
    printf("map saved in map_%0*d.lcm successfully", 3, save_no);
    fclose(svef);
}
int main()
{
    int temp = 1;
    do
    {
        TCLEAR();
        SPACE(55);
        FRGB(255, 0, 0);
        printf("Lonely Cells Map Editor\n");
        TRESET();
        _sleep(500);
        SPACE(60);
        printf("Select Mode\n");
        SPACE(60);
        printf("[1] Load Map\n");
        SPACE(60);
        printf("[2] New Map\n");
        temp = getch() - '0';
    } while (temp > 2 || temp < 1);
    switch (temp)
    {
    case 1:
    {
        int hold = 1;
        int map_no = 1;
        do
        {
            TCLEAR();
            SPACE(60);
            printf("Enter Desired Map For Edit:\n");
            while (1)
            {
                char add[30];
                sprintf(add, "maps\\map_%0*d.lcm", 3, map_no);
                FILE *fptr = fopen(add, "rb");
                if (!fptr)
                {
                    // printf("IS NULL");
                    break;
                }
                SPACE(63);
                printf("<%d>", map_no);
                map_no == 1 ? printf(" - default_map\n") : printf(" -  Map_no_%d\n", map_no);
                map_no++;
                fclose(fptr);
            }
            --map_no;
            scanf("%d", &hold);
            // temp = 1;
        } while (hold < 0 || hold > map_no);
        map_sel = hold;
        map = map_reader();
        if (map == (void ***)NULL)
        {
            return -1;
        }
        map_design(edit);
        getch();
    }
    break;
    case 2:
    {
        SPACE(57);
        FRGB(20, 70, 200);
        printf("Enter Desired Dimension\n");
        scanf("%d", &dim);
        //each row
        edit = (void ***)calloc(dim, sizeof(void **));
        for (int i = 0; i < dim; i++)
        {
            edit[i] = (void **)calloc(dim, sizeof(void *));
            //each column
            for (int j = 0; j < dim; j++)
            {
                edit[i][j] = calloc(1, sizeof(char));
                *(char *)edit[i][j] = NORMAL;
            }
        }
        map_design(edit);
        getch();
    }
    break;
    }
    while (1)
    {
        int menu;
        do
        {
            printf("\n");
            SPACE(66);
            printf("Menu\n");
            SPACE(60);
            printf("<1> change house type\n");
            SPACE(64);
            printf("<2> save map\n");
            SPACE(60);
            printf("<3> save map as text\n");
            SPACE(65);
            printf("<4> exit\n");
            menu = getch() - '0';

        } while (menu < 1 || menu > 4);
        switch (menu)
        {
        case 1:
        {
            int i = 0, j = 0;
            char t = NORMAL;
            do
            {
                TCLEAR();
                map_design(edit);
                // getch();
                printf("\nEnter Line no from bottom\n");
                scanf("%d", &i);
                i--;
                printf("Enter Column no from left\n");
                scanf("%d", &j);
                j--;
                printf("Choose Type\n");
                printf("<%c> - Energy\n", ENERGY);
                printf("<%c> - Mitosis\n", MITOSIS);
                printf("<%c> - Blocked\n", BLOCKED);
                t = getch();
            } while (i < 0 || i >= dim || j < 0 || j >= dim || t - '0' < 1 || t - '0' > 3);
            *(char *)edit[i][j] = t;
            break;
        }
        case 2:
        {
            map_save();
            getch();
            break;
        }
        case 3:
        {
            char add[50] = "map_info.txt";
            map_text(add);getch();
            break;
        }
        case 4:
            exit(0);
            break;
        }
        map_design(edit);   
    }
}