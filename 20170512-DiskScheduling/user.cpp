/**
 * zhangji 2018-06-13
 */

const int MAX = 100000;


int t_size, t_start, t_end;
int hd;
int t_origin[MAX];
int t_left[MAX], t_right[MAX];
int dir;
int min_left,max_right;
void init(int track_size, int head) {
    t_start = t_end = 0;
    t_size = track_size;
    hd = head;
    for (int i = 0; i < MAX; i++) {
        t_origin[i] = t_left[i] = t_right[i] = -2;
    }
    dir = -1;
    min_left=999999999;
    max_right = 0;
}

void request(int track) {
    t_origin[t_end++] = track;

    int left = -2, right = -2;
       
    for (int l = track - 1; l >= 0; l--) {
        if (t_left[l] > -2) {
            left = l;
            break;
        }
    }
    for (int r = track + 1; r < t_size; r++) {
        if (t_right[r] > -2) {
            right = r;
            break;
        }
    }
    if (left != -2) {
        t_left[track] = left;
        t_right[left] = track;
    } else {
        t_left[track] = -1;
    }
    if (right != -2) {
        t_left[right] = track;
        t_right[track] = right;
    } else {
        t_right[track] = -1;
    }

}

void remove_req(int track_no) {
    int left = t_left[track_no];
    int right = t_right[track_no];
    if (right != -1) {
        t_left[right] = left;
    }
    if (left != -1) {
        t_right[left] = right;
    }
    t_right[track_no] = -2;
    t_left[track_no] = -2;

    for (int i = t_start; i < t_end; i++) {
        if (t_origin[i] == track_no) {
            t_origin[i] = -2;
            break;
        }
    }
}

int fcfs() {
    int track_no = -1;
    for (int i = t_start; i < t_end; i++) {
        if (t_origin[i] != -2) {
            track_no = t_origin[i];
            t_start = i;
            break;
        }
    }
    hd = track_no;

    remove_req(track_no);
    return track_no;
}

int sstf() {
    int track_no = -1;    // TO DO : Need to be changed

    int left = -2, right = -2;
    int left_dis = 999999999, right_dis = 999999999;
    for (int l = hd - 1; l >= 0; l--) {
        if (t_left[l] > -2) {
            left = l;
            break;
        }
    }
    for (int r = hd + 1; r < t_size; r++) {
        if (t_right[r] > -2) {
            right = r;
            break;
        }
    }
    if (left != -2) {
        left_dis = hd - left;
    }
    if (right != -2) {
        right_dis = right - hd;
    }
    if (left_dis > right_dis) {
        track_no = right;
    }
    if (left_dis <= right_dis) {
        track_no = left;
    }
    hd = track_no;

    remove_req(track_no);
    return track_no;
}

int look() {
    int track_no = -1;    // TO DO : Need to be changed

    int left = -2, right = -2;
    if (dir == -1) { // left
        for (int l = hd - 1; l >= 0; l--) {
            if (t_left[l] > -2) {
                left = l;
                break;
            }
        }
        if (left != -2) {
            track_no = left;
        } else {
            dir = 1;
            for (int r = hd + 1; r < t_size; r++) {
                if (t_right[r] > -2) {
                    right = r;
                    break;
                }
            }
            if (right != -2) {
                track_no = right;
            }
        }
        hd = track_no;
    } else { // right
        for (int r = hd + 1; r < t_size; r++) {
            if (t_right[r] > -2) {
                right = r;
                break;
            }
        }
        if (right != -2) {
            track_no = right;
        } else {
            dir = -1;
            for (int l = hd - 1; l >= 0; l--) {
                if (t_left[l] > -2) {
                    left = l;
                    break;
                }
            }
            if (left != -2) {
                track_no = left;
            }
        }
        hd = track_no;
    } 
    
    remove_req(track_no);
    return track_no;
}

int clook() {
    int track_no = -1;    // TO DO : Need to be changed

    int left = -2, right = -2;
    for (int l = hd - 1; l >= 0; l--) {
        if (t_left[l] > -2) {
            left = l;
            break;
        }
    }
    if (left != -2) {
        track_no = left;
    } else {
        for (int r = t_size; r >= 0; r--) {
            if (t_right[r] > -2) {
                right = r;
                break;
            }
        }
        if (right != -2) {
            track_no = right;
        }
    }
    hd = track_no;

    remove_req(track_no);
    return track_no;
}

