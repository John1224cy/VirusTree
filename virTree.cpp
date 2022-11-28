#include<iostream>
#include<stack>
#include<queue>
#include"virTree.h"

using namespace std;
    virus::virus(int in, int inf, float dth, float rec, int vc, int var){
        this->inc = in;     this->inf = inf;
        this->death = dth;  this->recover = rec;
        this->vcode = vc;   this->nVar = var;
        // 根据输入的变异子病毒数量分配各权值的空间
        child = new virus*[var];
        var_time = new int[var];
        search_time = new int[var];
        searchsucc_time = new int[var];
        // 输入权值
        for (int i = 0; i < var; i++)
            cin >> var_time[i] >> search_time[i] >> searchsucc_time[i];              
    }

    // 构建病毒树
    virTree::virTree(){
        int inc, inf, vcode, var;        
        float rec, death;
        cin >> inc >> inf >> death >> rec >> vcode >> var;
        virus* root = new virus(inc, inf, death, rec, vcode, var);   // 初始化树根
        rt = cur = root;
        tot = 0;
        // 完成子树的建立 (BFS)
        queue<virus*> vq;
        vq.push(rt);
        while (!vq.empty()){
            cur = vq.front();  vq.pop();    
            tot++;
            int k = cur->variations();
            for (int i = 0; i < k; i++){
                cin >> inc >> inf >> death >> rec >> vcode >> var;
                cur->child[i] = new virus(inc, inf, death, rec, vcode, var);
                vq.push(cur->child[i]);
            } 
        }
    }   

    // 返回编号为cd的病毒
    virus* virTree::find(int cd){
        if (cd == 0)
            return root();
        cur = root();
        queue<virus*> q;
        q.push(cur);
        while (!q.empty()){
            cur = q.front();    q.pop();
            for (int i = 0; i < cur->variations(); i++)
                if (cur->child[i]->code() != cd)
                    q.push(cur->child[i]);
                else
                    return cur->child[i];
        }
        cerr << "NOT FOUND" << endl;
        return NULL;
    }

    // 返回其双亲节点
    virus* virTree::parent(int cd){
        if (cd == 0)
            return rt;
        cur = rt;
        queue<virus*> q;
        q.push(cur);
        while (!q.empty()){
            cur = q.front();    q.pop();
            for (int i = 0; i < cur->variations(); i++)
                if (cur->child[i]->code() < cd)
                    q.push(cur->child[i]);
                else if (cur->child[i]->code() == cd)
                    return cur;
        }
    }

    // 公共祖先
    virus* virTree::public_parent(int cd1, int cd2){
        stack<virus*> s1, s2;
        s1.push(parent(cd1));   s2.push(parent(cd2));
        cur = s1.top();
        while (cur != rt){
            s1.push(parent(cur->code()));
            cur = s1.top();
        }
        cur = s2.top();
        while (cur != rt){
            s2.push(parent(cur->code()));
            cur = s2.top();
        }

        while (!s1.empty() && !s2.empty()){
            if (s1.top() == s2.top()){
                cur = s1.top();
                s1.pop();   s2.pop();
            }
            else
                return cur;
        }

        if (s1.empty() && s2.empty())
            return cur;
        else if (s1.empty())
            for (int i = 0; i < cur->variations(); i++)
                if (cur->child[i]->code() == cd1)
                    return cur->child[i];       // 返回编号为cd1的病毒
            
        else if (s2.empty())
            for (int i = 0; i < cur->variations(); i++)
                if (cur->child[i]->code() == cd2)
                    return cur->child[i];       // 返回编号为cd2的病毒
               
    }

    // 判断pt是否为ch的祖先
    bool virTree::isParent(int pt, int ch){
        if (pt == ch)
            return true;
        if (ch < pt)
            return false;
        return (public_parent(pt, ch)->code() == pt);
    }  

    // 各种病毒变异体的疫苗能否在变异发生前研制成功
    void virTree::succ_vaccinum(){;
        int* lag_time = new int[total()];
        for (int i = 0; i < total(); i++)
            lag_time[i] = -1;

        queue<virus*> vq;
        vq.push(root());    
        while (!vq.empty()){
            virus* tmp = vq.front(); vq.pop();
            for (int i = 0; i < tmp->variations(); i++){
                int j = tmp->child[i]->code();
                // 不能在变异前研制成功
                if (tmp->search_time[i] > tmp->var_time[i]){
                    lag_time[j] = tmp->search_time[i] - tmp->var_time[i];     
                    vq.push(tmp->child[i]);     // 后续还会变异
                }   
                // 在变异前研制成功，且后续不会变异 
                else
                    lag_time[j] = 0;        
            }
        }
        for (int i = 1; i < total(); i++){
            cout << "code-" << i << "-virus"  << ": ";
            switch (lag_time[i])
            {
            case -1:
                cout << "won't exisit" << endl;
                break;

            case 0:
                cout << "vaccinum can be developed" << endl;
                break;
                
            default:
                cout << "vaccinum cannot be developed, lag time: " << lag_time[i] << endl;
                break;
            }
        }
    }