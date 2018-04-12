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

struct AdjacentBuilding {
    int num;
    int ids[3];
};

Cell map[MAX_N][MAX_N];
int next_vt_id = 1;
Building park_list[MAX_BUILDING + 1];
int park_size = 0;
int dis_mtx[MAX_VT][MAX_VT];
int map_wd;
AdjacentBuilding adjBD;

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

    Cell *p_cell = &map[outP_x][outP_y];
    p_cell->vt_type = 'P';
    p_cell->vt_id = next_vt_id;
    p_cell->b_id = id;
    park_list[id].park = p_cell;
    park_list[id].o4c[0][0] = outP_x;
    park_list[id].o4c[0][1] = outP_y;
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
            for (int i = 1; i < next_vt_id; i++) {
                if (dis_mtx[cur_cell->vt_id][i] != 0) {
                    dis_mtx[cur_cell->vt_id][i] = DEFAULT_INFINITE_VALUE;
                }
            }
            dis_mtx[cur_cell->vt_id][next_cell->vt_id] = dis;
            dis = 0;
            return next_cell;
        } else if (next_cell->vt_type == 'P' && next_cell->vt_id == park_id) { // cur_type == 'P'
            for (int i = 1; i < next_vt_id; i++) {
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
AdjacentBuilding *check_road(int id, int start_x, int start_y, int end_x, int end_y, char dir, int park_id) {
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
            if (cur_cell->b_id != 0 && cur_cell->b_id != id) {
                adjBD.ids[adjBD.num++] = cur_cell->b_id;
            }
            cur_cell = check_2_vertex(cur_cell, next_cell, dis, park_id);
            next_x += 1;
        }
    } else if (dir == 'D') {
        next_y += 1;
        while (next_y <= end_y) {
            next_cell = &map[next_x][next_y];
            if (cur_cell->b_id != 0 && cur_cell->b_id != id) {
                adjBD.ids[adjBD.num++] = cur_cell->b_id;
            }
            cur_cell = check_2_vertex(cur_cell, next_cell, dis, park_id);
            next_y += 1;
        }
    } else if (dir == 'L') {
        next_x += -1;
        while (next_x >= end_x) {
            next_cell = &map[next_x][next_y];
            if (cur_cell->b_id != 0 && cur_cell->b_id != id) {
                adjBD.ids[adjBD.num++] = cur_cell->b_id;
            }
            cur_cell = check_2_vertex(cur_cell, next_cell, dis, park_id);
            next_x += -1;
        }
    } else { // 'U'
        next_y += -1;
        while (next_y >= end_y) {
            next_cell = &map[next_x][next_y];
            if (cur_cell->b_id != 0 && cur_cell->b_id != id) {
                adjBD.ids[adjBD.num++] = cur_cell->b_id;
            }
            cur_cell = check_2_vertex(cur_cell, next_cell, dis, park_id);
            next_y += -1;
        }
    }
    return &adjBD;
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
    int outA_x = locX - 1, outA_y = locY - 1;
    int outB_x = (locX + w), outB_y = locY - 1;
    int outC_x = (locX + w), outC_y = (locY + h);
    int outD_x = locX - 1, outD_y = (locY + h);
    add_Q_vertex(id, outA_x, outA_y, 1);
    add_Q_vertex(id, outB_x, outB_y, 2);
    add_Q_vertex(id, outC_x, outC_y, 3);
    add_Q_vertex(id, outD_x, outD_y, 4);
    int p_vt_id = add_P_vertex(id, locX, locY, w, h, px, py);
    print_map();
    int adj_b_id;
    AdjacentBuilding *_adjDB;
    Building *bd;
    _adjDB = check_road(id, outA_x, outA_y, outB_x, outB_y, 'R', p_vt_id);
    if (_adjDB->num != 0) {
        for (int i = 0; i < _adjDB->num; i++) {
            adj_b_id = _adjDB->ids[i];
            bd = &park_list[adj_b_id];
            recheck_road_adj_building(adj_b_id, bd->o4c[3][0], bd->o4c[3][1], bd->o4c[4][0], bd->o4c[4][1], 'L',
                                      bd->park->vt_id);
        }
        _adjDB->num = 0;
    }
    _adjDB = check_road(id, outB_x, outB_y, outC_x, outC_y, 'D', p_vt_id);
    if (_adjDB->num != 0) {
        for (int i = 0; i < _adjDB->num; i++) {
            adj_b_id = _adjDB->ids[i];
            bd = &park_list[adj_b_id];
            recheck_road_adj_building(adj_b_id, bd->o4c[4][0], bd->o4c[4][1], bd->o4c[1][0], bd->o4c[1][1], 'U',
                                      bd->park->vt_id);
        }
        _adjDB->num = 0;
    }
    _adjDB = check_road(id, outC_x, outC_y, outD_x, outD_y, 'L', p_vt_id);
    if (_adjDB->num != 0) {
        for (int i = 0; i < _adjDB->num; i++) {
            adj_b_id = _adjDB->ids[i];
            bd = &park_list[adj_b_id];
            recheck_road_adj_building(adj_b_id, bd->o4c[1][0], bd->o4c[1][1], bd->o4c[2][0], bd->o4c[2][1], 'R',
                                      bd->park->vt_id);
        }
    }
    _adjDB = check_road(id, outD_x, outD_y, outA_x, outA_y, 'U', p_vt_id);
    if (_adjDB->num != 0) {
        for (int i = 0; i < _adjDB->num; i++) {
            adj_b_id = _adjDB->ids[i];
            bd = &park_list[adj_b_id];
            recheck_road_adj_building(adj_b_id, bd->o4c[2][0], bd->o4c[2][1], bd->o4c[3][0], bd->o4c[3][1], 'D',
                                      bd->park->vt_id);
        }
        _adjDB->num = 0;
    }

    print_dis_mtx();
}

// 2
int getDistance(int from, int to) {
    printf("getDistance\n");
    print_dis_mtx();
    return 0;
}