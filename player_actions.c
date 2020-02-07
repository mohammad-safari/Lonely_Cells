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
    if (i + di >= dim || i + di < 0 || j + dj >= dim || j + dj < 0)
    {
        printf("Tried to Pass Borders!\n");
        return -1;
    }
    if (coord[i + di][j + dj] != 0 || *(char *)(map[i + di][j + dj]) == BLOCKED)
    {
        printf("Destinated House is Blocked or Occupied\n");
        return -1;
    }
    coord[i + di][j + dj] = coord[i][j];
    coord[i][j] = 0;
    cursor->j += dj;
    cursor->i += di;
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
        printf("House is Empty!\n");
        return -1;
    }
    if (cursor->energy == 100)
    {
        printf("Cell has Maximum Energy!\n");
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
    struct cell *cursor = find_cell(player_no, cell_no);
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
    for (int q = -1; q < 2; q++)
        for (int p = -1; p < 2; p++)
        {
            if (q != 0 && p == -1 * (find_cell(player_no, cell_no)->i % 2 == 0 ? -1 : 1))
                continue;
            if (i + q <= dim && i + q > 0 && j + p <= dim && j + p > 0)
                if (coord[i + q][j + p] == 0 && *(char *)map[i + q][j + p] != BLOCKED)
                {
                    int e = cursor->energy;
                    struct cell cell1, cell2;
                    cell1.energy = (int)(e / 2);
                    cell2.energy = e - cell1.energy;
                    cell1.i = i;
                    cell1.j = j;
                    cell2.i = i + q;
                    cell2.j = j + p;
                    rand_name(cell2.name);
                    rand_name(find_cell(player_no, cell_no)->name);
                    find_cell(player_no, cell_no)->energy = cell1.energy;
                    int no = add_cell(&(player[player_no]), cell2);
                    coord[i + q][j + p] = (player_no == 1 ? -1 : 1) * no;
                    return 0;
                }
        }
    printf("NOT ENOUGH SPACE FOR THIS DIVISION!\n");
    return -1;
}
void score_board()
{
    int cell[2] = {0, 0};
    int total_energy[2] = {0, 0};
    for (int i = 0; i < mode; i++)
    {
        struct cell *cursor = player[i];
        if (cursor != NULL)
            while (1)
            {
                total_energy[i] += cursor->energy;
                if (cursor->next == NULL)
                    break;
                cursor = cursor->next;
            }
        cell[i] = cursor->no;
    }
    SPACE(65);
    printf("round %d\n", turn);
    SPACE(65);
    printf("player 1");
    SPACE(25);
    printf("player 2\n");
    SPACE(25);
    printf("Total Cells");
    SPACE(32);
    printf("%d", cell[0]);
    SPACE(32);
    printf("%d\n", cell[1]);
    SPACE(25);
    printf("Total Energy");
    SPACE(31);
    printf("%d", total_energy[0]);
    SPACE(32);
    printf("%d", total_energy[1]);
}
int game_save()
{
    int save_no = 1;
    char add[30];
    while (1)
    {
        sprintf(add, "saves\\save_%0*d.lcs", 3, save_no);
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
            char temp = *(char *)map[i][j];
            fwrite(&temp, sizeof(char), 1, svef);
            if (temp == ENERGY)
            {
                fwrite((int *)(map[i][j] + sizeof(char)), sizeof(int), 1, svef);
            }
        }
    }
    fwrite(&mode, sizeof(int), 1, svef);
    fwrite(&turn, sizeof(int), 1, svef);
    for (int p = 0; p < mode; p++)
    {
        int cell_num = 0;
        struct cell *cursor = player[p];
        if (cursor != NULL)
            while (cursor->next != NULL)
            {
                cursor = cursor->next;
            }
        cell_num = cursor->no;
        fwrite(&cell_num, sizeof(int), 1, svef);
        cursor = player[p];
        while (cell_num--)
        {
            fwrite(cursor, sizeof(struct cell), 1, svef);
            cursor = cursor->next;
        }
    }
    SPACE(50);
    printf("game saved in save_%0*d.lcs successfully", 3, save_no);
    fclose(svef);
}