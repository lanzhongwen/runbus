/**********************************************
 * Author : Zhongwen(Alan) Lan
 * Created: 2017/03/16
 *********************************************/
#ifndef _SHARED_DATA_H_
#define _SHARED_DATA_H_
struct SIpcSharedData {
  int value;
  SIpcSharedData() : value(0) {
  }
};
#endif 
