//
// Created by Darren on 2020/2/13.
//

#include "BaseVideoView.h"
#include "../tools/Log.h"


/**
 *
 * 观察者回调：接收解码后的data包。
 *
 * 框架层，在基类中做实现，调用子类的方法。
 *
 *
 * @param data
 */
void BaseVideoView::Update(BaseData data) {
//    LOGI("BaseVideoView unpdate data=%d",data.size);
    this->Render(data);
}
