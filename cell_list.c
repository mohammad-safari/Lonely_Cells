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
    //make sure that list is available or create list anyway!
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
    return no;
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
        cursor = *player;
        int n = 1;
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
    cursor = *player;
    int n = 1;
    free((struct cell *)cursor);
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
void print_list(struct cell *cursor)
{
    // struct cell *cursor = player;
    while (cursor != NULL)
    {
        printf("%d) ", cursor->no);
        printf("%s: with Energy -> %d on house", cursor->name, cursor->energy);
        printf("(%d, %d)\n", cursor->i + 1, cursor->j + 1);
        cursor = cursor->next;
    }
}
void rand_name(char *name)
{
    for (int c = 0; c < 4; c++)
    {
        name[c] = rand() % 26 + 97;
    }
}
