//
// Created by zhangji on 4/10/18.
//

#include <stdio.h>

#define MAX_N                       50
#define MAX_BUILDING                1000
#define MAX_VT                      300
#define DEFAULT_INFINITE_VALUE      88

struct Cell {
    int vt_id;
    char vt_type;// P or Q
    int b_id;
};

struct Building {
    struct Cell *park;
    int o4c[5][2];
};

Cell map[MAX_N][MAX_N];
int next_vt_id = 1;
Building park_list[MAX_BUILDING + 1];
int park_size = 0;
int dis_mtx[MAX_VT][MAX_VT];
int map_wd;

void init(int n) {
    map_wd = n;
    for (auto &col : map) {
        for (auto &cell : col) {
            cell.vt_id = 0;
            cell.vt_type = 0;
        }
    }
    next_vt_id = 1;
    park_size = 0;
    for (auto &row : dis_mtx) {
        for (int &dis : row) {
            dis = DEFAULT_INFINITE_VALUE;
        }
    }
}

void print_dis_mtx() {
    for (int vt_id = 0; vt_id <= map_wd; vt_id++) {
        printf("%d-\t", vt_id);
        if (vt_id == 0) {
            for (int j = 1; j <= map_wd; j++) {
                printf("%d\t", j);
            }
        } else {
            for (int j = 1; j <= map_wd; j++) {
                printf("%d\t", dis_mtx[vt_id][j]);
            }
        }
        printf("\n");
    }
}

void print_map() {
    for (int i = 0; i < map_wd; i++) {
        for (int col = 0; col < map_wd; col++) {
            if (map[col][i].vt_type == '\0') {
                printf("%d \t", map[col][i].vt_id);
            } else {
                printf("%d%c\t", map[col][i].vt_id, map[col][i].vt_type);
            }
        }
        printf("\n");
    }
    printf("\n");
}

void add_Q_vertex(int id, int x, int y, int index) {
    if (map[x][y].vt_id == 0) {
        map[x][y].vt_id = next_vt_id;
        map[x][y].vt_type = 'Q';
        map[x][y].b_id = id;
        dis_mtx[next_vt_id][next_vt_id] = 0;
        next_vt_id++;
    }
    park_list[id].o4c[index][0] = x;
    park_list[id].o4c[index][1] = y;
    park_size = id;
}

int add_P_vertex(int id, int x, int y) {
    Cell *p_cell = &map[x][y];
    p_cell->vt_type = 'P';
    p_cell->vt_id = next_vt_id;
    p_cell->b_id = id;
    park_list[id].park = p_cell;
    park_list[id].o4c[0][0] = x;
    park_list[id].o4c[0][1] = y;
    park_size = id;
    dis_mtx[next_vt_id][next_vt_id] = 0;
    next_vt_id++;
    return p_cell->vt_id;
}

Cell *check_2_vertex(struct Cell *cur_cell, struct Cell *next_cell, int &dis, int park_id) {
    if (next_cell->vt_id == 0) {
        dis++;
        return cur_cell;
    } else { // cur_vt_id != 0, the cur_type 
        dis++;
        if (next_cell->vt_type == 'Q') {
            for (int i = 1; i < map_wd; i++) {
                if (dis_mtx[cur_cell->vt_id][i] != 0) {
                    dis_mtx[cur_cell->vt_id][i] = DEFAULT_INFINITE_VALUE;
                }
            }
            dis_mtx[cur_cell->vt_id][next_cell->vt_id] = dis;
            dis = 0;
            return next_cell;
        } else if (next_cell->vt_type == 'P' && next_cell->vt_id == park_id) { // cur_type == 'P'
            for (int i = 1; i < map_wd; i++) {
                if (dis_mtx[cur_cell->vt_id][i] != 0) {
                    dis_mtx[cur_cell->vt_id][i] = DEFAULT_INFINITE_VALUE;
                }
            }
            dis_mtx[cur_cell->vt_id][next_cell->vt_id] = dis;
            dis = 0;
            return next_cell;
        } else {
            return cur_cell;
        }
    }
}

// clockwise, have 4 dir R/D/L/U
int check_road(int id, int start_x, int start_y, int end_x, int end_y, char dir, int park_id) {
    int cur_x = start_x, cur_y = start_y;
    int next_x = start_x, next_y = start_y;
    int dis = 0;
    struct Cell *cur_cell = &map[cur_x][cur_y];
    struct Cell *next_cell;
    int adj_b_id = id;
    if (dir == 'R') {
        next_x += 1;
        while (next_x <= end_x) {
            next_cell = &map[next_x][next_y];
            cur_cell = check_2_vertex(cur_cell, next_cell, dis, park_id);
            if (cur_cell->b_id != id) {
                adj_b_id = cur_cell->b_id;
            }
            next_x += 1;
        }
    } else if (dir == 'D') {
        next_y += 1;
        while (next_y <= end_y) {
            next_cell = &map[next_x][next_y];
            cur_cell = check_2_vertex(cur_cell, next_cell, dis, park_id);
            if (cur_cell->b_id != id) {
                adj_b_id = cur_cell->b_id;
            }
            next_y += 1;
        }
    } else if (dir == 'L') {
        next_x += -1;
        while (next_x >= end_x) {
            next_cell = &map[next_x][next_y];
            cur_cell = check_2_vertex(cur_cell, next_cell, dis, park_id);
            if (cur_cell->b_id != id) {
                adj_b_id = cur_cell->b_id;
            }
            next_x += -1;
        }
    } else { // 'U'
        next_y += -1;
        while (next_y >= end_y) {
            next_cell = &map[next_x][next_y];
            cur_cell = check_2_vertex(cur_cell, next_cell, dis, park_id);
            if (cur_cell->b_id != id) {
                adj_b_id = cur_cell->b_id;
            }
            next_y += -1;
        }
    }
    return adj_b_id;
}

Cell *recheck_2_vertex(struct Cell *cur_cell, struct Cell *next_cell, int &dis, int park_id) {
    if (next_cell->vt_id == 0) {
        dis++;
        return cur_cell;
    } else { // cur_vt_id != 0, the cur_type 
        dis++;
        if (next_cell->vt_type == 'Q') {
            dis_mtx[cur_cell->vt_id][next_cell->vt_id] = dis;
            dis = 0;
            return next_cell;
        } else if (next_cell->vt_type == 'P' && next_cell->vt_id == park_id) { // cur_type == 'P'
            dis_mtx[cur_cell->vt_id][next_cell->vt_id] = dis;
            dis = 0;
            return next_cell;
        } else {
            return cur_cell;
        }
    }
}

void recheck_road_adj_building(int id, int start_x, int start_y, int end_x, int end_y, char dir, int park_id) {
    int cur_x = start_x, cur_y = start_y;
    int next_x = start_x, next_y = start_y;
    int dis = 0;
    struct Cell *cur_cell = &map[cur_x][cur_y];
    struct Cell *next_cell;
    int adj_b_id;
    if (dir == 'R') {
        next_x += 1;
        while (next_x <= end_x) {
            next_cell = &map[next_x][next_y];
            cur_cell = recheck_2_vertex(cur_cell, next_cell, dis, park_id);
            next_x += 1;
        }
    } else if (dir == 'D') {
        next_y += 1;
        while (next_y <= end_y) {
            next_cell = &map[next_x][next_y];
            cur_cell = recheck_2_vertex(cur_cell, next_cell, dis, park_id);
            next_y += 1;
        }
    } else if (dir == 'L') {
        next_x += -1;
        while (next_x >= end_x) {
            next_cell = &map[next_x][next_y];
            cur_cell = recheck_2_vertex(cur_cell, next_cell, dis, park_id);
            next_x += -1;
        }
    } else { // 'U'
        next_y += -1;
        while (next_y >= end_y) {
            next_cell = &map[next_x][next_y];
            cur_cell = recheck_2_vertex(cur_cell, next_cell, dis, park_id);
            next_y += -1;
        }
    }
}

// 1
void addBuilding(int id, int locX, int locY, int w, int h, int px, int py) {
    int locP_x = locX + px;
    int locP_y = locY + py;
    int a_x = locX - 1, a_y = locY - 1;
    int b_x = (locX + w), b_y = locY - 1;
    int c_x = (locX + w), c_y = (locY + h);
    int d_x = locX - 1, d_y = (locY + h);
    int p_x, p_y;
    add_Q_vertex(id, a_x, a_y, 1);
    add_Q_vertex(id, b_x, b_y, 2);
    add_Q_vertex(id, c_x, c_y, 3);
    add_Q_vertex(id, d_x, d_y, 4);
    int p_vt_id;
    if (py == 0) { //Park cell at UP side
        p_x = locP_x, p_y = locP_y - 1;
    } else if (px == (w - 1)) { //Park cell at RIGHT side
        p_x = locP_x + 1, p_y = locP_y;

    } else if (py == (h - 1)) { //Park cell at DOWN side
        p_x = locP_x, p_y = locP_y + 1;
    } else { //px==0  Park cell at LEFT side
        p_x = locP_x - 1, p_y = locP_y;
    }
    p_vt_id = add_P_vertex(id, p_x, p_y);

    int adj_b_id;
    Building *bd;
    adj_b_id = check_road(id, a_x, a_y, b_x, b_y, 'R', p_vt_id);
    if (adj_b_id != id) {
        bd = &park_list[adj_b_id];
        recheck_road_adj_building(adj_b_id, bd->o4c[3][0], bd->o4c[3][1], bd->o4c[4][0], bd->o4c[4][1], 'L',
                                  bd->park->vt_id);
    }

    adj_b_id = check_road(id, b_x, b_y, c_x, c_y, 'D', p_vt_id);
    if (adj_b_id != id) {
        bd = &park_list[adj_b_id];
        recheck_road_adj_building(adj_b_id, bd->o4c[4][0], bd->o4c[4][1], bd->o4c[1][0], bd->o4c[1][1], 'U',
                                  bd->park->vt_id);
    }

    adj_b_id = check_road(id, c_x, c_y, d_x, d_y, 'L', p_vt_id);
    if (adj_b_id != id) {
        bd = &park_list[adj_b_id];
        recheck_road_adj_building(adj_b_id, bd->o4c[1][0], bd->o4c[1][1], bd->o4c[2][0], bd->o4c[2][1], 'R',
                                  bd->park->vt_id);
    }

    adj_b_id = check_road(id, d_x, d_y, a_x, a_y, 'U', p_vt_id);
    if (adj_b_id != id) {
        bd = &park_list[adj_b_id];
        recheck_road_adj_building(adj_b_id, bd->o4c[2][0], bd->o4c[2][1], bd->o4c[3][0], bd->o4c[3][1], 'D',
                                  bd->park->vt_id);
    }

    print_map();
    print_dis_mtx();
}

// 2
int getDistance(int from, int to) {
    printf("getDistance\n");
    print_dis_mtx();
    return 0;
}