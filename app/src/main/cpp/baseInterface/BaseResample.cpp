//
// Created by Darren on 2020/2/15.
//

#include "BaseResample.h"
#include "../tools/Log.h"

void BaseResample::Update(BaseData data)
{
    BaseData d = this->Resample(data);
//    LOGE("BaseResample::Update:111 %d",d.size);
    if(d.size > 0){
        this->Notify(d);
    }
}