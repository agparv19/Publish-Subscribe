#include"text.cpp"

class cmp{
public:
    bool operator()(text* t1, text* t2){
        return t2->time_posted < t1->time_posted;
    }
};


class messageBroker{
public:
    unordered_map<int, text*> mp_text; //map to get the text given its text_id
    unordered_map<int, set<int>> sub; //sub[i][j] denotes the id of jth subscriber of user with id 'i'
    unordered_map<int, priority_queue<text*, vector<text*>, cmp>> feed; //feed[uid] represents all relevant unread posts for the user uid

    messageBroker(){}

    void subscribe(int uid, int pid){

        if (sub[pid].find(uid) != sub[pid].end()){
            cout<<"User "<<uid<<" is already subscribed to publisher "<<pid<<"\n";
            return;
        }

        sub[pid].insert(uid);
    }

    void unsubscribe(int uid, int pid){

        if (sub.find(pid) == sub.end()){
            cout<<"Publisher "<<pid<<" does not exist"<<"\n";
            return;
        }

        if (sub[pid].find(uid) == sub[pid].end()){
            cout<<"User "<<uid<<" is already unsubscribed to publisher "<<pid<<"\n";
            return;
        }

        sub[pid].erase(uid);
    }

    void publish_NEW(int time, int uid, string data, int tid, int type){

        if (mp_text.find(tid) !=mp_text.end()){
            cout<<"Can't publish with text_id "<<tid<<" as it is already occupied\n";
            return;
        }

        text* textOb = new text(tid, data, time, uid, type, -1);

        mp_text[tid] = textOb;

        int pid = uid; //this is the publisher of this post

        for (auto itr = sub[pid].begin(); itr !=sub[pid].end(); itr++){

            //add this post to feed of every subscriber

            int user = *itr;

            feed[user].push(textOb);

        }

    }

    void publish_REPOST(int time, int uid, int og_post_id, int tid){

        if (mp_text.find(og_post_id) == mp_text.end()){
            cout<<"Can not Repost. Original Post with id = "<<og_post_id<<" does not exist\n";
            return;
        }

        publish_NEW(time, uid, mp_text[og_post_id]->data, tid, (int)1);
    }

    void publish_REPLY(int time, int uid, int og_post_id, string data, int tid){

        if (mp_text.find(og_post_id) == mp_text.end()){
            cout<<"Can not Reply. Original Post with id = "<<og_post_id<<" does not exist\n";
            return;
        }

        text* textOb = new text(tid, data, time, uid, 2, og_post_id);

        mp_text[tid] = textOb;

        int og_user = mp_text[og_post_id]->uid;

        feed[og_user].push(textOb);

    }

    void read(int time, int user){

        if (feed[user].size() == 0){
            cout<<"Feed of user "<<user<<" is empty"<<endl;
        }

        while(feed[user].size() > 0){

            if (feed[user].top()->time_posted < time){

                if (feed[user].top()->type == 2){

                    int og_post_id = feed[user].top()->repOf;

                    cout<<mp_text[og_post_id]->data<<", ";
                    cout<<feed[user].top()->data<<"\n";
                }
                else{
                    int pid = feed[user].top()->uid;

                    if (sub[pid].find(user) != sub[pid].end()){
                        cout<<feed[user].top()->data<<"\n";
                    }
                }

                feed[user].pop();
            }
            else{
                break;
            }
        }
    }

    



};
