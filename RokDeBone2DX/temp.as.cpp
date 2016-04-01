	//四元数をオイラー角に変換
 #deffunc qToEuler int targetQ, var eulerX, var eulerY, var eulerZ
	DDim axisXVec,3 : DDim axisYVec,3 : DDim axisZVec,3
	DDim targetVec,3
	DDim shadowVec,3

	axisXVec.0= 1.0 : axisXVec.1= 0.0 : axisXVec.2= 0.0
	axisYVec.0= 0.0 : axisYVec.1= 1.0 : axisYVec.2= 0.0
	axisZVec.0= 0.0 : axisZVec.1= 0.0 : axisZVec.2= 1.0

	E3DMultQVec targetQ,0.0,0.0,1.0,targetVec.0,targetVec.1,targetVec.2
	shadowVec.0= vecDotVec(targetVec,axisXVec)
	shadowVec.1= 0.0
	shadowVec.2= vecDotVec(targetVec,axisZVec)
	if( lengthVec(shadowVec) == 0 ){
		eulerY= 90.0
	}else{
		E3DACos ( vecDotVec(shadowVec,axisZVec)/ lengthVec(shadowVec) ), eulerY		//Y軸回転度数
	}
	if( vecDotVec(shadowVec,axisXVec) < 0.0 ){ eulerY= -eulerY }				//右回りに修正

	E3DVec3RotateY targetVec.0,targetVec.1,targetVec.2,(-eulerY),shadowVec.0,shadowVec.1,shadowVec.2
	if( lengthVec(shadowVec) == 0 ){
		eulerX= 90.0
	}else{
		E3DACos ( vecDotVec(shadowVec,axisZVec)/ lengthVec(shadowVec) ), eulerX		//X軸回転度数
	}
	if( vecDotVec(shadowVec,axisYVec) > 0.0 ){ eulerX= -eulerX }				//右回りに修正

	E3DMultQVec targetQ,0.0,1.0,0.0,targetVec.0,targetVec.1,targetVec.2
	E3DVec3RotateY (targetVec.0+0.0),(targetVec.1+0.0),(targetVec.2+0.0),(-eulerY),targetVec.0,targetVec.1,targetVec.2
	E3DVec3RotateY targetVec.2,targetVec.0,targetVec.1,(-eulerX),shadowVec.2,shadowVec.0,shadowVec.1
	if( lengthVec(shadowVec) == 0 ){
		eulerZ= 90.0
	}else{
		E3DACos ( vecDotVec(shadowVec,axisYVec)/ lengthVec(shadowVec) ), eulerZ		//Z軸回転度数
	}
	if( vecDotVec(shadowVec,axisXVec) > 0.0 ){ eulerZ= -eulerZ }				//右回りに修正

	return
