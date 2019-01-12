#include "PointToPoint.h"

//计算相对角度差  正则左转
int getAngleToAim(int now_x,int now_y,int go_x,int go_y)
{
	int delta_x = go_x-now_x;
	int delta_y = go_y-now_y;
	
	//现在（-180 180）之内
	double angle_a1 = atan2(delta_y,delta_x) * 180 / 3.14159;
	/***************/
	
	int angleToAim =(angle_a1-transformYaw());//目标在左为正，在右为负
	if(angleToAim > 180)
	{
		angleToAim = angleToAim - 360;
	}
	else if(angleToAim < -180)
	{
		angleToAim = angleToAim + 360;
	}	
	return angleToAim;
}

