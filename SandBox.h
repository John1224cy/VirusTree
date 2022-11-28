#ifndef _SANDBOX_H
#define _SANDBOX_H

#include"virTree.h"
#include<vector>

struct people
{ 
    
    int cur_state;        // -1为死亡， 0为感染， 1为康复
    int iso;              // 是否被隔离

    int* carrying;        // 当前携带病毒种类(若处于感染状态)
    int* carried;         // 已感染病毒
    int* lastDays;        // 感染天数
    int* envolved;        // 是否进化

    int carNum;           // 携带病毒数量
    int active;           // 发病病毒数量
};

void SandBox(virTree vt, long num, long days);


#endif