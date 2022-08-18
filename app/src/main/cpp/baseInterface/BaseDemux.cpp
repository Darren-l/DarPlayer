//
// Created by Darren on 2020/2/10.
//

#include "BaseDemux.h"
#include "../tools/Log.h"

void BaseDemux::Main() {
    while(!isExit)
    {

        if(IsPause())
        {
            Sleep(2);
            continue;
        }

        BaseData d = Read();
        if(d.size > 0)
            Notify(d);
        else
            Sleep(2);
    }
}
