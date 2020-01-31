#define BRGB(A, B, C) printf("\033[48;2;" #A ";" #B ";" #C "m")
#define FRGB(A, B, C) printf("\033[38;2;" #A ";" #B ";" #C "m")
#define TRESET() printf("\033[0m")
#define TCLEAR() printf("\033[2J")
#define SPACE(A) printf("%" #A "s", "")
//keep neccessary data
FILE *rec;
//keep number of saved game in rec
int saves = 0;
//keep number of saved maps in rec
int maps = 1;
//keep player cells data
struct cell
{
    char name[5];
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
int save_sel = 0;
//demonstrates using which map
int map_sel = 1;
//one player(1) or two player(2) mode
int mode = 1;
//computer plays or not
int computer = 0;
//show ehich player is gameing
int turn;
//add cells
int add_cell(struct cell **, struct cell);
//remove cell
int remove_cell(struct cell **, int);
//search for cell
struct cell* find_cell(int, int);
//print list of cells acoording to their name
void print_list(struct cell *);
//load map in ram
void ***map_reader();
//load player cells saved data
struct cell ***load_player(FILE **);
//initialize player cells on map
int init_coord();
//load player cells on map
int load_coord();
//choosing random name
void rand_name(char *name);
//access a special cell
struct cell *find_cell(int, int);
//move cell
int move(int, int, int, int);
//gather energy
int gather(int, int);
//divide cell
int divide(int, int);
//score_board
void score_board();
//save game
int game_save();
//design map
int map_design();
//splash screen
void splash();
//start menu
int start_menu();
//action menu
int action_menu(int , int *);
//exit
void EXIT();