#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
//load map in ram
void ***map_reader();
//load player cells saved data
struct cell ***load_player(FILE **);


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