//
// Created by Darren on 2020/2/12.
//

#include "DObserver.h"


/**
 *
 * 主题添加观察者。
 *
 * @param dObserver
 */
void DObserver::AddObs(DObserver *dObserver) {
    if(!dObserver){
        return;
    }

    mux.lock();
    doverctor.push_back(dObserver);
    mux.unlock();

}

/**
 *
 * 主题通知观察者。
 *
 * @param data
 */

void DObserver::Notify(BaseData data) {
    mux.lock();
    for(int i=0; i<doverctor.size(); i++){
        doverctor[i]->Update(data);
    }
    mux.unlock();
}
