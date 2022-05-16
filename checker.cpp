#include"messageBroker.cpp"

void Execute(messageBroker* broker, string &action){
    
    string s1 = "SUBSCRIBE";
    string s2 = "UNSUBSCRIBE";
    string s3 = "PUBLISH";
    string s4 = "READ";

    if (action.substr(0, s1.size()) == s1){

        int i = s1.size()+1;

        for (; i<action.size(); i++){
            if (action[i]==',') break;
        }

        string uid = "";

        i++;

        for (;i<action.size(); i++){
            if (action[i]==',') break;
            uid.push_back(action[i]);
        }

        int u = stoi(uid);

        i++;

        string pid = "";

        for (;i<action.size(); i++){
            pid.push_back(action[i]);
        }

        int p = stoi(pid);

        broker->subscribe(u, p);
    }

    if (action.substr(0, s2.size()) == s2){

        int i = s2.size()+1;

        for (; i<action.size(); i++){
            if (action[i]==',') break;
        }

        string uid = "";

        i++;

        for (;i<action.size(); i++){
            if (action[i]==',') break;
            uid.push_back(action[i]);
        }

        int u = stoi(uid);

        i++;

        string pid = "";

        for (;i<action.size(); i++){
            pid.push_back(action[i]);
        }

        int p = stoi(pid);

        broker->unsubscribe(u, p);
    }

    if (action.substr(0, s3.size()) == s3){

        int i = s3.size()+1;

        string time = "";

        for (; i<action.size(); i++){
            if (action[i]==',') break;
            time +=action[i];
        }

        string uid = "";

        i++;

        for (; i<action.size(); i++){
            if (action[i]==',') break;
            uid +=action[i];
        }

        string type = "";

        i++;

        for (; i<action.size(); i++){
            if (action[i]==',') break;
            type +=action[i];
        }

        string p1 = "REPOST";
        string p2 = "REPLY";

        int ptid = -1;
        int post_type = 0;

        if (type.substr(0, p1.size()) == p1){
            //repost
            post_type = 1;
            int j = p1.size()+1;

            string id = "";

            for (; j<type.size(); j++){
                if (type[j]== ')') break;
                id += type[j];
            }

            ptid = stoi(id);
        }
        else if (type.substr(0, p2.size()) == p2){
            //reply

            post_type = 2;
            int j = p2.size()+1;

            string id = "";

            for (; j<type.size(); j++){
                if (type[j]== ')') break;
                id += type[j];
            }

            ptid = stoi(id);
        }
        string data = "";

        if (post_type != 1){
            i++;
            for (; i<action.size(); i++){
                if (action[i]==',')break;
                data += action[i];
            }
        }

        i++;

        string tid = "";

        for (; i<action.size(); i++){
            tid += action[i];
        }

        if (post_type == 0){
            broker->publish_NEW(stoi(time), stoi(uid), data, stoi(tid), 0);
        }
        else if(post_type == 1){
            broker->publish_REPOST(stoi(time), stoi(uid), ptid, stoi(tid));
        }
        else{
            broker->publish_REPLY(stoi(time), stoi(uid), ptid, data, stoi(tid));
        }
    }
    if (action.substr(0, s4.size()) == s4){

        int i = s4.size()+1;

        string time = "";

        for (; i<action.size(); i++){
            if (action[i]==',') break;
            time +=action[i];
        }

        string uid = "";

        i++;

        for (; i<action.size(); i++){
            uid +=action[i];
        }

        broker->read(stoi(time), stoi(uid));
    }
}

int main(){
    
    fstream actionFile;
    actionFile.open("actions2.txt", ios::in);

    string action;

    messageBroker* broker = new messageBroker();

    while(getline(actionFile, action)){
        cout<<action<<endl;
        Execute(broker, action);
        //Note that time in 'action' is redundant. The action that comes later in actions file
        //is treated to have happened on later instant of time
    }

    actionFile.close();
}