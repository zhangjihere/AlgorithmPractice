//
// Created by zhangji on 4/10/18.
//

#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS    // Ignore build errors when using unsafe functions in Visual Studio.
#endif

#include <iostream>

using namespace std;

static int Score = 0;

extern void init(int N);

extern void addBuilding(int id, int locX, int locY, int w, int h, int px, int py);

extern int getDistance(int from, int to);

typedef enum {
    CMD_INIT,
    CMD_ADD,
    CMD_GET,
    CMD_EXIT = 9
} COMMAND;

static void run() {
    int N, cmd;
    int id, locX, locY, w, h, px, py;
    int from, to, dist, ret;

    cin >> cmd >> N;

    Score = 0;
    init(N);
    while (true) {
        cin >> cmd;
        if (cmd == CMD_ADD) {
            cin >> id >> locX >> locY >> w >> h >> px >> py;
            addBuilding(id, locX, locY, w, h, px, py);
        } else if (cmd == CMD_GET) {
            cin >> from >> to >> dist;
            ret = getDistance(from, to);
            cout << "ret: " << ret << ", dist: " << dist << endl;
            if (ret == dist) {
                Score++;
            }
        } else if (cmd == CMD_EXIT)
            break;
    }
}

int main() {
    freopen("sample_input.txt", "r", stdin);

    int T;
    cin >> T;

    int TotalScore = 0;
    for (int test_case = 1; test_case <= T; test_case++) {
        run();
        TotalScore += Score;
        cout << "#" << test_case << " " << Score << endl;
    }
    cout << "Total Score = " << TotalScore << endl;
    return 0;
}