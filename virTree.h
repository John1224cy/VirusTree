#ifndef _VIRTREE_H
#define _VIRTREE_H

class virTree;
class virus{
    friend class virTree;
    protected:
        int nVar;              // 变异体数量
        virus** child;         // 孩子节点
        int* var_time;            // 病毒变异时间
        int* search_time;         // 研制疫苗时间
        int* searchsucc_time;     // 疫苗研制成功后病毒的变异时间

    public:
        virus();
        virus(int in = 0, int inf = 0, float dth = 0, float rec = 0, int vc = 0, int var = 0);
        
        int incubation()      {return inc;}
        int infectionrate()   {return inf;}
        float deathrate()     {return death;}
        float recovery()      {return recover;}
        int code()            {return vcode;}
        int variations()      {return nVar;}
        virus* _child(int x)            {return child[x];}
        int _var_time(int x)            {return var_time[x];}
        int _search_time(int x)         {return search_time[x];}
        int _searchsucc_time(int x)     {return searchsucc_time[x];}
        
    private:
        int inc;         // 潜伏期
        int inf;         // 传染能力
        float death;     // 致死率
        float recover;   // 康复率
        int vcode;       // 编号
        
};

class virTree{
    public:
        virTree();               // 构建病毒树       
        int total() {return tot;}        // 病毒总数
        virus* find(int cd);               // 返回编号为cd的病毒
        virus* parent(int cd);             // 返回其双亲节点
        virus* public_parent(int cd1, int cd2);  // 公共祖先
        bool isParent(int pt, int ch);     // 判断pt是否为ch的祖先
        void succ_vaccinum();   // 各种病毒变异体的疫苗能否在变异发生前研制成功
           
        virus* root() {return rt;}
        virus* current() {return cur;}

    private:
        virus* rt;  // 根节点
        virus* cur;
        int tot;      // 病毒数目
};




#endif