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
