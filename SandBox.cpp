#include"SandBox.h"
#include"virTree.h"
#include<iostream>
#include <cstdlib>

using namespace std;

void SandBox(virTree vt, long num, long days){
    people* crowds = new people[num];
    long i, j;
    long cur_num = num;    // 当前存活人数
    long iso_num = 0;      // 当前隔离人数
    // 对人群初始化
    for (i = 0; i < num; i++){
        crowds[i].iso = 0;
        crowds[i].cur_state = 1;
        crowds[i].carrying = new int[vt.total()];
        crowds[i].carried = new int[vt.total()];
        crowds[i].lastDays = new int[vt.total()];
        crowds[i].envolved = new int[vt.total()];
        crowds[i].carNum = crowds[i].active = 0;
        for (j = 0; j < vt.total(); j++)
            crowds[i].carrying[j] = crowds[i].carried[j] = crowds[i].lastDays[j] = crowds[i].envolved[j] = 0;
    }
        
    long* inflected = new long[vt.total()];   // 感染人数
    long* dead = new long[vt.total()];        // 死亡人数
    long* recovered = new long[vt.total()];   // 康复人数
    for (i = 0; i < vt.total(); i++)
        inflected[i] = dead[i] = recovered[i] = 0;
    // 初始化零号病人
    crowds[0].carrying[0] = crowds[0].carried[0] = 1;
    crowds[0].cur_state = 0;
    crowds[0].carNum++;
    inflected[0]++;

    for (long dy = 1; dy <= days && cur_num > 0; dy++) {
        cout << "Day" << dy << ":\n" << "current people alive: " << cur_num << endl << "current people isolated: " << iso_num << endl;
        for (i = 0; i < vt.total(); i++)
            cout << "code-" << i << "-virus:" << endl     
            << "    inflected: " << inflected[i] << "    dead: " << dead[i] << "    recovered: " << recovered[i] << endl;
        // 更新感染时长 
        for (j = 0; j < num; j++)
            if (crowds[j].cur_state == 0)
                for (int k = 0; k < vt.total(); k++)
                    if (crowds[j].carrying[k])
                        crowds[j].lastDays[k]++;

        // 以小时进行遍历
        for (int hours = 0; hours < 24; hours++)
            // 按人群进行遍历
            for (j = 0; j < num; j++)
                if (crowds[j].cur_state != -1)
                    if (crowds[j].cur_state == 0){
                        // 传染病毒
                        if (!crowds[j].iso){
                            // 遍历每一种病毒并随机传染
                            for (int k = 0; k < vt.total(); k++)
                                if (crowds[j].carrying[k] && crowds[j].lastDays[k] != 0){
                                    virus* tmp = vt.find(k);
                                    int inc_num;
                                    // 根据是否处于爆发期确定传染人数
                                    if (crowds[j].lastDays[k] >= tmp->incubation())
                                        inc_num = tmp->infectionrate();
                                    else
                                        inc_num = tmp->infectionrate()/10;
                                    for (int r = 0; r < inc_num; r++){
                                        long inf_people = j;
                                        // 判断该对象是否感染过子代病毒或已经感染过该病毒
                                        while (inf_people == j || crowds[inf_people].iso == 1 || crowds[inf_people].cur_state == -1 )
                                            inf_people = rand() % cur_num;       
                                        // 传染给该对象
                                        int exisited = 0;
                                        for (int x = 0; x < vt.total() && !exisited; x++)
                                            if (crowds[inf_people].carried[x] && vt.isParent(k, x))
                                                exisited = 1;
                                        if (exisited)
                                            continue;
                                        crowds[inf_people].carried[k] = crowds[inf_people].carrying[k] = 1;
                                        crowds[inf_people].cur_state = 0;
                                        crowds[inf_people].carNum++;
                                        inflected[k]++;
                                    }
                                }
                        }
                        // 状态改变
                        for (int k = 0; k < vt.total(); k++){
                            if (crowds[j].carrying[k]){
                                virus* tmp = vt.find(k);
                                if (hours == 0 && !crowds[j].envolved[k]){
                                     // 病毒进化
                                    vector<virus*> envolve;
                                    for (int x = 0; x < tmp->variations(); x++)
                                        if (tmp->_var_time(x) <= crowds[j].lastDays[k])
                                            envolve.push_back(tmp->_child(x));
                                    if (!envolve.empty()){
                                        int r = rand() % envolve.size();
                                        if (crowds[j].carrying[envolve[r]->code()] == 0){
                                            crowds[j].carried[envolve[r]->code()] = crowds[j].carrying[envolve[r]->code()] = 1;
                                            inflected[envolve[r]->code()]++;
                                        }
                                        crowds[j].envolved[k] = 1;
                                        crowds[j].carNum++;
                                    }
                                }
                                else
                                    // 进入爆发期
                                    if (crowds[j].lastDays[k] >= tmp->incubation()){ 
                                        if (crowds[j].lastDays[k] == tmp->incubation())   
                                            crowds[j].active++;  
                                        // 爆发后一天被隔离  
                                        if (!crowds[j].iso && crowds[j].lastDays[k] > tmp->incubation()){
                                            crowds[j].iso = 1;
                                            iso_num++;
                                        }
                                        int death = tmp->deathrate() * 100;
                                        int recover = tmp->recovery() * 100;
                                        int change = rand() % 100;
                                        if (0 <= change && death > change){
                                            // 死亡
                                            crowds[j].cur_state = -1;
                                            dead[k]++;
                                            inflected[k]--;
                                            cur_num--;
                                            if (crowds[j].iso)
                                                iso_num--;
                                        }
                                        else if (death <= change && death + recover > change){
                                            // 该种病毒康复
                                            recovered[k]++;
                                            inflected[k]--;
                                            crowds[j].carrying[k] = 0;
                                            crowds[j].carNum--;
                                            crowds[j].active--;
                                        }
                                    }
                                if (crowds[j].active == 0)
                                    // 无症状则解除隔离
                                     if (crowds[j].iso){
                                        crowds[j].iso = 0;
                                        iso_num--;
                                    }
                                // 所有携带病毒康复则转为健康状态
                                if (crowds[j].carNum == 0)
                                    crowds[j].cur_state = 1;
                            }
                        }
                    }
    }
}