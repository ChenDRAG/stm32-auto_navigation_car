#include "PointToPoint.h"

//������ԽǶȲ�  ������ת
int getAngleToAim(int now_x,int now_y,int go_x,int go_y)
{
	int delta_x = go_x-now_x;
	int delta_y = go_y-now_y;
	
	//���ڣ�-180 180��֮��
	double angle_a1 = atan2(delta_y,delta_x) * 180 / 3.14159;
	/***************/
	
	int angleToAim =(angle_a1-transformYaw());//Ŀ������Ϊ��������Ϊ��
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

