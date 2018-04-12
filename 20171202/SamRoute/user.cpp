//
// Created by zhangji on 4/10/18.
//

#include <stdio.h>

#define MAX_N                       50
#define MAX_BUILDING                1000
#define MAX_VT                      300
#define DEFAULT_INFINITE_VALUE      88
#define MAX_ADJ_NUM                 8

typedef enum {
    NONE = 0,
    PARK_OUTER = 'P',
    CORNER_OUTER = 'Q',
    EDGE = 'E'
} CELL_TYPE;

typedef enum {
    CHECK_SELF = 's',
    CHECK_ADJACENT = 'r',
} OPT_TYPE;

struct Cell {
    int vt_id;          // vertex id, if Cell is vertex, it will have vt_id
    CELL_TYPE cell_type;// (PARK_OUTER)Park outer or (CORNER_OUTER)4 out corner of building corner or (EDGE)4 Edges of building
    int e_bd_id;        // if the cell is in the edge of a building, it is the id of building which include the edge.
                        // the building's corner is always in the edge.
};

struct Building {
    struct Cell *park;
    int o4c[5][2];// 0 is park coordination and 1~4 is 4 corners out of building
};

struct AdjacentBuilding {
    int ids[MAX_ADJ_NUM];
    int num;
};

int map_wd;
Cell map[MAX_N][MAX_N];
Building buildings[MAX_BUILDING + 1];
int next_vt_id = 1;
AdjacentBuilding adjBuildings;
int dis_mtx[MAX_VT][MAX_VT];

void init(int n) {
    map_wd = n;
    for (auto &col : map) {
        for (auto &cell : col) {
            cell.vt_id = 0;
            cell.cell_type = NONE;
            cell.e_bd_id = 0;
        }
    }
    next_vt_id = 1;
    for (auto &row : dis_mtx) {
        for (int &dis : row) {
            dis = DEFAULT_INFINITE_VALUE;
        }
    }
}

void print_dis_mtx(int mtx[MAX_VT][MAX_VT], int row, int col) {
    for (int vt_id = 0; vt_id < row; vt_id++) {
        printf("%d-\t", vt_id);
        if (vt_id == 0) {
            for (int j = 1; j < col; j++) {
                printf("%d\t", j);
            }
        } else {
            for (int j = 1; j < col; j++) {
                printf("%d\t", mtx[vt_id][j]);
            }
        }
        printf("\n");
    }
}

void print_map() {
    for (int i = 0; i < map_wd; i++) {
        for (int col = 0; col < map_wd; col++) {
            if (map[col][i].cell_type == '\0') {
                printf("%d \t", map[col][i].vt_id);
            } else if (map[col][i].cell_type == CORNER_OUTER || map[col][i].cell_type == PARK_OUTER) {
                printf("%d%c\t", map[col][i].vt_id, map[col][i].cell_type);
            } else {
                printf("%c\t", map[col][i].cell_type);
            }
        }
        printf("\n");
    }
    printf("\n");
}

void print_array(int *array) {
    for (int i = 1; i < next_vt_id; i++) {
        printf("%d ", array[i]);
    }
    printf("\n");
}

int copyArray2D(int src[MAX_VT][MAX_VT], int dst[MAX_VT][MAX_VT]) {
    for (int i = 0; i < next_vt_id; i++) {
        for (int j = 0; j < next_vt_id; j++) {
            dst[i][j] = src[i][j];
        }
    }
}

int copyArray1D(const int src[MAX_VT], int dst[MAX_VT]) {
    for (int i = 0; i < next_vt_id; i++) {
        dst[i] = src[i];
    }
}

void add_Q_vertex(int id, int x, int y, int index) {
    struct Cell *q_cell = &map[x][y];
    if (q_cell->vt_id == 0) {
        q_cell->vt_id = next_vt_id;
        q_cell->cell_type = CORNER_OUTER;
        dis_mtx[next_vt_id][next_vt_id] = 0;
        next_vt_id++;
    }
    buildings[id].o4c[index][0] = x;
    buildings[id].o4c[index][1] = y;
}

void set_building_edge(int id, int a_x, int a_y, int b_x, int b_y) {
    for (int i = a_x; i <= b_x; i++) {
        for (int j = a_y; j <= b_y; j++) {
            map[i][j].cell_type = EDGE;
            map[i][j].e_bd_id = id;
        }
    }
}

void add_4Q_vertext(int id, int locX, int locY, int w, int h) {
    int outA_x = locX - 1, outA_y = locY - 1;
    int outB_x = (locX + w), outB_y = locY - 1;
    int outC_x = (locX + w), outC_y = (locY + h);
    int outD_x = locX - 1, outD_y = (locY + h);
    set_building_edge(id, locX, locY, locX + w - 1, locY);
    set_building_edge(id, locX + w - 1, locY, locX + w - 1, locY + h - 1);
    set_building_edge(id, locX, locY + h - 1, locX + w - 1, locY + h - 1);
    set_building_edge(id, locX, locY, locX, locY + h - 1);
    add_Q_vertex(id, outA_x, outA_y, 1);
    add_Q_vertex(id, outB_x, outB_y, 2);
    add_Q_vertex(id, outC_x, outC_y, 3);
    add_Q_vertex(id, outD_x, outD_y, 4);
}

int add_P_vertex(int id, int locX, int locY, int w, int h, int px, int py) {
    int outP_x, outP_y;
    int locP_x = locX + px;
    int locP_y = locY + py;
    if (py == 0) { //Park cell at UP side
        outP_x = locP_x, outP_y = locP_y - 1;
    } else if (px == (w - 1)) { //Park cell at RIGHT side
        outP_x = locP_x + 1, outP_y = locP_y;
    } else if (py == (h - 1)) { //Park cell at DOWN side
        outP_x = locP_x, outP_y = locP_y + 1;
    } else { //px==0  Park cell at LEFT side
        outP_x = locP_x - 1, outP_y = locP_y;
    }

    struct Cell *p_cell = &map[outP_x][outP_y];
    p_cell->cell_type = PARK_OUTER;
    p_cell->vt_id = next_vt_id;

    buildings[id].park = p_cell;
    buildings[id].o4c[0][0] = outP_x;
    buildings[id].o4c[0][1] = outP_y;
    dis_mtx[next_vt_id][next_vt_id] = 0;
    next_vt_id++;
    return p_cell->vt_id;
}

/**
 * 
 * @param cur_vt_id 
 * @param next_vt_id 
 * @param dis 
 * @param option    CHECK_SELF specifies to clear cur_vt_id vertex distance weight for backward recheck other building to update
 *                  'r' specifies not to clear cur_vt_id vertex distance weight
 */
void update_vertex_dis_mtx(int cur_vt_id, int next_vt_id, int &dis, OPT_TYPE option) {
    if (option == CHECK_SELF) {
        for (int i = 1; i < next_vt_id; i++) {
            if (dis_mtx[cur_vt_id][i] != 0) {
                dis_mtx[cur_vt_id][i] = DEFAULT_INFINITE_VALUE;
            }
        }
    }
    dis_mtx[cur_vt_id][next_vt_id] = dis;
    dis = 0;
}

Cell *check_2_vertex(struct Cell *cur_cell, struct Cell *next_cell, int &dis, int p_vt_id, OPT_TYPE opt) {
    if (next_cell->vt_id == 0) {
        dis++;
        return cur_cell;
    } else { // cur_vt_id != 0, the cur_type 
        dis++;
        if (next_cell->cell_type == CORNER_OUTER) {
            update_vertex_dis_mtx(cur_cell->vt_id, next_cell->vt_id, dis, opt);
            return next_cell;
        } else if (next_cell->cell_type == PARK_OUTER && next_cell->vt_id == p_vt_id) { // cur_type == PARK_OUTER
            update_vertex_dis_mtx(cur_cell->vt_id, next_cell->vt_id, dis, opt);
            return next_cell;
        } else {
            return cur_cell;
        }
    }
}

// 0 1 2
// 3   4
// 5 6 7
int eight_dir[8][2] = {{1,  -1},
                       {1,  0},
                       {1,  1},
                       {0,  -1},
                       {0,  1},
                       {-1, -1},
                       {-1, 0},
                       {-1, 1}};

// Check edge to collect
void collect_adj_buildings(int id, int x, int y) {
    int sx, sy, e_bd_id;
    struct Cell *s_cell;
    for (int d = 0; d < 8; d++) {
        sx = x + eight_dir[d][0];
        sy = y + eight_dir[d][1];
        if (0 <= sx && sx <= map_wd && 0 <= sy && sy <= map_wd) {
            s_cell = &map[sx][sy];
            e_bd_id = s_cell->e_bd_id;
            if (e_bd_id != 0 && e_bd_id != id) {
                int j = 0;
                for (; j < adjBuildings.num; j++) {
                    if (adjBuildings.ids[j] == e_bd_id) {
                        break;
                    }
                }
                if (j == adjBuildings.num) {
                    adjBuildings.ids[adjBuildings.num++] = e_bd_id;
                }
            }
        }
    }
}

//       1
//   * ----->   
// 4 |       |
//   |       | 2
//     <---- *
//       3
int clock_wise[5][2] = {{0,  0},    // ignore
                        {1,  0},    // left_top to right_top
                        {0,  1},    // right_top to right_bottom
                        {-1, 0},    // right_bottom to left_bottom
                        {0,  -1}};  // left_bottom to left_top

/**
 * check the 4 roads around the building of id
 * @param id        building id
 * @param bd        a Building instance pointer
 * @param p_vt_id   park vertex id
 * @param opt       CHECK_SELF specifies to collect all adjacent buildings info based the 4 roads's corners 
 *                  and check building's 4 around roads to update vertex and distance weight
 *                  'r' specifies to recheck building's 4 around roads to update vertex and distance weight
 */
void check_building(int id, struct Building *bd, int p_vt_id, OPT_TYPE opt) {
    int *start, *end;
    int dis = 0;
    struct Cell *cur_cell, *next_cell;
    int dx = 0, dy = 0;
    int ox, oy;
    int next_x, next_y;
    int end_x, end_y;
    for (int c = 1; c <= 4; c++) {  // Based clockwise,from left_top corner outer in order to check the 4 roads.
        start = bd->o4c[c];
        if (c == 4) {
            end = bd->o4c[1];
        } else {
            end = bd->o4c[c + 1];
        }
        cur_cell = &map[start[0]][start[1]];
        dx = clock_wise[c][0];  // justify the clockwise road direction
        dy = clock_wise[c][1];
        ox = dx == 0 ? 1 : dx;
        oy = dy == 0 ? 1 : dy;
        next_x = start[0] + dx;
        next_y = start[1] + dy;
        end_x = end[0];
        end_y = end[1];
        while ((next_x * ox) <= (end_x * ox) && (next_y * oy) <= (end_y * oy)) {
            next_cell = &map[next_x][next_y];
            if (opt == CHECK_SELF) {
                collect_adj_buildings(id, next_x, next_y);
            }
            cur_cell = check_2_vertex(cur_cell, next_cell, dis, p_vt_id, opt);
            next_x += dx;
            next_y += dy;
        }
    }
}

// 1
void addBuilding(int id, int locX, int locY, int w, int h, int px, int py) {
    add_4Q_vertext(id, locX, locY, w, h);
    int p_vt_id = add_P_vertex(id, locX, locY, w, h, px, py);
//    print_map();
    Building *bd = &buildings[id];
    check_building(id, bd, p_vt_id, CHECK_SELF);// check self building
    if (adjBuildings.num != 0) { // check adjacent building
        int adj_bid;
        for (int i = 0; i < adjBuildings.num; i++) {
            adj_bid = adjBuildings.ids[i];
            bd = &buildings[adj_bid];
            check_building(adj_bid, bd, bd->park->vt_id, CHECK_ADJACENT);
        }
        adjBuildings.num = 0;
    }
//    print_dis_mtx(dis_mtx, next_vt_id, next_vt_id);
}

void floyd_shortest_path(int ret_mtx[MAX_VT][MAX_VT]) {
    for (int k = 1; k < next_vt_id; k++) {
        for (int i = 1; i < next_vt_id; i++) {
            for (int j = 1; j < next_vt_id; j++) {
                if (ret_mtx[i][j] > ret_mtx[i][k] + ret_mtx[k][j]) {
                    ret_mtx[i][j] = ret_mtx[i][k] + ret_mtx[k][j];
                }
            }
        }
    }
}

void dijkstra_shortest_path(int dis_mtx[MAX_VT][MAX_VT], int from, int dis[MAX_VT]) {
    int book[MAX_VT];
    for (int i = 1; i < next_vt_id; i++) {
        book[i] = 0;
    }
    book[from] = 1;
    int min = 0;
    int u = 0;
    for (int i = 1; i < (next_vt_id - 1); i++) {
        min = DEFAULT_INFINITE_VALUE;
        for (int j = 1; j < next_vt_id; j++) {
            if (book[j] == 0 && dis[j] < min) {
                min = dis[j];
                u = j;
            }
        }
        book[u] = 1;
        for (int v = 1; v < next_vt_id; v++) {
            if (dis_mtx[u][v] < DEFAULT_INFINITE_VALUE) {
                if (dis[v] > dis[u] + dis_mtx[u][v]) {
                    dis[v] = dis[u] + dis_mtx[u][v];
                }
            }
        }
    }
}

// 2
int getDistance(int from, int to) {
//    printf("getDistance\n");
//    print_map();
//    print_dis_mtx(dis_mtx, next_vt_id, next_vt_id);
    int vt_A = buildings[from].park->vt_id;
    int vt_B = buildings[to].park->vt_id;

    // Floyd algorithm
/*    int ret_mtx[MAX_VT][MAX_VT];
    copyArray2D(dis_mtx, ret_mtx);
    floyd_shortest_path(ret_mtx);
    int ret = ret_mtx[vt_A][vt_B] + 2;
    print_dis_mtx(ret_mtx, next_vt_id, next_vt_id);*/

    // Dijkstra algorithm
    int dis[MAX_VT];
    copyArray1D(dis_mtx[vt_A], dis);
    dijkstra_shortest_path(dis_mtx, vt_A, dis);
    int ret = dis[vt_B] + 2;
//    print_array(dis);

    return ret;
}