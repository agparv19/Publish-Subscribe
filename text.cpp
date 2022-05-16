#include <bits/stdc++.h>
using namespace std;

class text{
public:
    int text_id;
    string data;
    int time_posted;
    int uid;
    int type; //0 new, 1 repost, 2 reply
    int repOf;

    text(int _id, string _data, int time, int u, int _type, int og_post_id){
        text_id = _id;
        data = _data;
        time_posted = time;
        uid = u;
        type = _type;
        repOf = og_post_id;

    }

};