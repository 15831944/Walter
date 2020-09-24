
#include "stdafx.h"
#include "ThreadData.h"
#include <dbhelix.h>
#include <dbobjptr.h>
#include <dbxutil.h>
#include <dbfcf.h>
#include "Com.h"
#include "Common/MathUtil.h"
#include "3D/BrepUtil.h"

#define HELIX_X_SUB (1)

static int CreateTopTrangle(AcGePoint2d offset, double radius, double startangle, double zhangKai, double len, AcDbObjectId &regionId);
int CreateTailTrangleAndRolve(AcGePoint2d offset, //三角形所在的位置
	double radius, //理解成三角形是沿着这个半径的圆弧滑动的 这个一半是开始圆的1/3这样一个圆弧上
	double startangle, //三角形在这个圆弧滑动时候 他所处的开始角度位置
	double zhangKai, //三角形的张开角
	double len, //三角形直角边长
	AcDbObjectId &mainid, AcDbObjectId &regionId, AcGePoint3d &farestIntersectPnt);

CString ToCString(E_CutterSegLengthType p_type)
{
	return c_sSegLenghtTypes[(int)p_type];
}

CString ToCString(SEdgeType p_type)
{
	return c_sEdgeType[(int)p_type];
}

E_CutterSegLengthType ToSegLengthType(CString p_str)
{
	for (int i = 0; i < 4; i++)
	{
		if (p_str.CompareNoCase(c_sSegLenghtTypes[i]) ==0)
		{
			return (E_CutterSegLengthType)i;
		}
	}

	return E_CutterSegLength_刀尖到刀尖;
}

SEdgeType ToEdgeType(CString p_str)
{
	for (int i = 0; i < 4; i++)
	{
		if (p_str.CompareNoCase(c_sEdgeType[i]) ==0)
		{
			return (SEdgeType)i;
		}
	}

	return E_EdgeType_单棱边;
}

//钻头数据

CThreadSegData::CThreadSegData()
{
	m_diameter = 10; //刃径
	m_length = 20; //长度
	m_lengthType = E_CutterSegLength_圆柱到刀尖;

	m_bDoubleLadder = false;
	m_ladderAngle1 = 180; //阶梯角
	m_ladderAngle2 = 180; //阶梯角
	m_ladderLength1 = 0;

	m_lengBianType = E_EdgeType_单棱边;
	m_topGongCha = 0.021;
	m_lowGongCha = 0.006;

	m_zhiJingJD = 3;

	m_bKaiRen = true;
}

//////////////////////////////////////////////////////////////////////////
CThreadData::CThreadData()
{
	m_cuttingEdgeCount = 2;		//刀刃数量
	m_sEdgeType = _T("单棱边");		//棱边

	m_topAngle = 135; //顶角

	m_handleDiameter=20; //柄径
	m_handleLength = 50; //柄长

	m_totalLength=160; //总长 20200412本来默认160 修改为0

	m_pointCenterDistance = 0; //刀尖中心距， 只针对三尖的刀尖类型有效
	m_pointR = 1.0; //刀尖的R值，只针对刀尖类型为刀尖R的有效
	m_isClockwise = true;

	m_luoXuanJiaoDu = CMathUtil::PI/3;//默认60度

	SetLadderCount(0); //阶数

	m_guoDuDaiX = 0;
	m_jingDu = 3;

	m_cutterType = T_DRILL_CUTTER;
	m_daoJianType = E_DaoJian_平底;
}


bool CThreadData::SetLadderCount(int p_count)
{
	if (p_count<0 || p_count>10)
	{
		return false;
	}


	m_nLadderCount = p_count;
	m_cutterSegs.resize(m_nLadderCount+1);

	return true;
}

const CThreadSegData* CThreadData::GetCutterSegData(int p_index) const
{
	if (p_index<0 || p_index>= m_cutterSegs.size())
	{
		return NULL;
	}
	else
	{
		return &(m_cutterSegs[p_index]);
	}
}
bool CThreadData::SetCutterSegData(CThreadSegData & p_segData, int p_index)
{
	if (p_index<0 || p_index>= m_cutterSegs.size())
	{
		assert(false);
		return false;
	}

	m_cutterSegs[p_index] = p_segData;

	return true;
}

int CThreadData::GetJieTiTwoDxy(const CThreadSegData &preSeg, const CThreadSegData &thisSeg, bool &isTwo, SDXY &first, SDXY &second)
{
	if(preSeg.valid() == false || thisSeg.valid() == false)
		return -1;

	double preR = preSeg.m_diameter/2;
	double thisR = thisSeg.m_diameter/2;
	if(CMathUtil::IsEqual(thisR, preR, USER_TOL * 10000))
		return -2;//前后两个R不能相等

	isTwo = thisSeg.m_bDoubleLadder;
	//修改为前一个角度
	double ra1 = CMathUtil::AngleToRadian(preSeg.m_ladderAngle1/2.0);//转换成单侧弧度
	double ra2 = CMathUtil::AngleToRadian(preSeg.m_ladderAngle2/2.0);//转换成单侧弧度

	//两个阶梯Y向的差距的绝对值
	//double YDis = fabs(thisR - preR);---20190610
	double YDis = thisR - preR;


	//阶梯是跟着下一段走的
	if(!isTwo || CMathUtil::IsEqual(ra1, ra2, USER_TOL*100))//单阶梯 或者双阶梯但是两个角度相等
	{
		isTwo = false;
		first.dy = YDis;
		if(CMathUtil::IsEqual(ra1, CMathUtil::PI/2, USER_TOL * 100))//90度直接算
			first.dx = 0;
		else
			first.dx = first.dy/tan(ra1);

		if (first.dx < 0)//20190610
			first.dx = -first.dx;
		return 0;
	}
	else
	{
		if(CMathUtil::IsEqual(ra1, CMathUtil::PI/2, USER_TOL * 100))//第一个是90度 第二个也是90度的情况上面已经排除掉了
		{
			first.dx = 0;
			second.dx = thisSeg.m_ladderLength1;
			second.dy = second.dx * tan(ra2);
			first.dy = YDis - second.dy;//注意此时如果给的数据有问题 first.dy 可能会小于0
			if(first.dy < 0)
			{
				//阶梯角2的 角度过大或者长度过长
				return -3;
			}
		}
		else if(CMathUtil::IsEqual(ra2, CMathUtil::PI/2, USER_TOL * 100))//第二个是90度
		{
			first.dx = thisSeg.m_ladderLength1;
			first.dy = first.dx * tan(ra1);

			second.dx = 0;
			second.dy = YDis - first.dy;//注意此时如果给的数据有问题 first.dy 可能会小于0
			if(second.dy < 0)
			{
				//阶梯角1的 角度过大或者长度过长
				return -4;
			}
		}
		else//正常情况
		{
			first.dx = thisSeg.m_ladderLength1;
			first.dy = first.dx * tan(ra1);

			if (first.dx < 0)
				first.dx = -first.dx;

			second.dy = YDis - first.dy;
			second.dx = second.dy/tan(ra2);

			if (second.dx < 0)
				second.dx = -second.dx;
		}
	}

	return 0;
}

//下面这个函数求出thisSeg 这一段的 圆柱部分的长度
double CThreadData::GetHorizontalLengthByType(const CThreadSegData &thisSeg, const CThreadSegData &nextSeg, double preLadderABxLen)
{
	double xlength = 0;
	if(thisSeg.m_lengthType == E_CutterSegLength_刀尖到刀尖)//如果只有一个直径 那么也是直接计算
	{
		xlength = thisSeg.m_length - preLadderABxLen;
	}
	else if(thisSeg.m_lengthType == E_CutterSegLength_刀尖到圆柱)
	{
		//cutterSegs.size() == 1 已经排除
		bool isTwo =false;
		SDXY ladderA, ladderB;
		GetJieTiTwoDxy(thisSeg, nextSeg, isTwo, ladderA, ladderB);
		xlength = thisSeg.m_length- preLadderABxLen - ladderA.dx - ladderB.dx;
	}
	else if(thisSeg.m_lengthType == E_CutterSegLength_圆柱到刀尖)
	{
		xlength = thisSeg.m_length;
	}
	else if(thisSeg.m_lengthType == E_CutterSegLength_圆柱到圆柱)
	{
		bool isTwo =false;
		SDXY ladderA, ladderB;
		GetJieTiTwoDxy(thisSeg, nextSeg, isTwo, ladderA, ladderB);
		xlength = thisSeg.m_length - ladderA.dx - ladderB.dx;
	}
	return xlength;
}

int CThreadData::ConvertToDxy(double topAngle, //输入
							const vector<CThreadSegData> &cutterSegs,//输入
							double totalLength,//辅助计算最后一段长度
							vSDXY &dxys//输出
							)
{
	int ret = 0;
	if(cutterSegs.size() == 0)
		return -1;

	double topRad = CMathUtil::AngleToRadian(topAngle/2.0);
	double r = cutterSegs[0].m_diameter/2.0;

	SDXY firstxy,secondxy;//firstxy是第一个竖直线， secondxy是第一个阶梯的水平线
	firstxy.dy = r;
	if(CMathUtil::IsEqual(topRad, CMathUtil::PI/2, USER_TOL * 100))
		firstxy.dx = 0;
	else
	    firstxy.dx = r/tan(topRad);
	dxys.push_back(firstxy);//第一个斜线

	secondxy.dy = 0;
	if(cutterSegs.size() == 1)
	{
		if (cutterSegs[0].m_lengthType == E_CutterSegLength_刀尖到刀尖 || cutterSegs[0].m_lengthType == E_CutterSegLength_刀尖到圆柱)
			secondxy.dx = totalLength- firstxy.dx;
		else
			secondxy.dx = totalLength;
	}
	else//cutterSegs.size() 大于1（0和1已经排除）
		secondxy.dx = GetHorizontalLengthByType(cutterSegs[0],cutterSegs[1],firstxy.dx);

	dxys.push_back(secondxy);//第一个水平线放进去
	//到此为止  第一段处理结束产生了两个 dxy


	//--------------处理中间段和最后一段------------- //
	for(int i = 1; i < cutterSegs.size(); i++)
	{
		//第一步：排除
		double preR = cutterSegs[i-1].m_diameter/2;
		double curR = cutterSegs[i].m_diameter/2;
		if(CMathUtil::IsEqual(curR, preR, USER_TOL * 100 *100))
		{
			acutPrintf(L"\n错误：前后段直径相同\n");
			return -2;//前后两个R不能相等
		}

		//第二步：处理两个阶梯
		bool isTwo =false;
		SDXY ladderA, ladderB;
		ret = GetJieTiTwoDxy(cutterSegs[i-1], cutterSegs[i], isTwo, ladderA, ladderB);
		if(ret != 0)
		{
			CString info;
			info.Format(L"第%d段参数有错误\n", i+1);
			acutPrintf(info);
			return -3;
		}
		dxys.push_back(ladderA);//放进去ladderA
		if(isTwo)
		    dxys.push_back(ladderB);//放进去ladderB

		//第三步：处理一个水平线
		secondxy.dy = 0;
		if(i < cutterSegs.size() - 1)
		{
		    secondxy.dx = GetHorizontalLengthByType(cutterSegs[i], cutterSegs[i+1], ladderA.dx + ladderB.dx);
		}
		else//最后一段
		{
			for(int i = 0; i < dxys.size(); i++)
			{
				totalLength -= dxys[i].dx;
			}
			

			int last = (int)cutterSegs.size() - 1;

			if(cutterSegs[last].m_lengthType == E_CutterSegLength_刀尖到刀尖 || cutterSegs[last].m_lengthType == E_CutterSegLength_刀尖到圆柱)
				//secondxy.dx = cutterSegs[last].m_length - ladderA.dx - ladderB.dx;
				secondxy.dx = totalLength;// 20190424临时去掉 - ladderA.dx - ladderB.dx;
		    else
			   // secondxy.dx = cutterSegs[last].m_length;
			   secondxy.dx = totalLength;

			if(secondxy.dx < 0)
			{
			      acutPrintf(L"总长参数有误\n");
				  return -4;
			}
		}
		dxys.push_back(secondxy);//放进去水平线
	}

	//再添加一个竖直方向的直线
	firstxy.dx = 0;
	firstxy.dy = -cutterSegs[cutterSegs.size()-1].m_diameter/2;
	dxys.push_back(firstxy);
	
	return 0;
}

int CThreadData::ConvertToDxyCXD(double topAngle, //输入
	const vector<CThreadSegData> &cutterSegs,//输入
	double &xLen,//输出 计算得到的所有的x方向的刃的长度
	vSDXY &dxys//输出
	)
{
	int ret = 0;
	if(cutterSegs.size() == 0)
		return -1;

	xLen = 0;
	double topRad = CMathUtil::AngleToRadian(topAngle/2.0);
	double r = cutterSegs[0].m_diameter/2.0;

	SDXY firstxy,secondxy;//firstxy是第一个竖直线， secondxy是第一个阶梯的水平线
	firstxy.dy = r;
	if(CMathUtil::IsEqual(topRad, CMathUtil::PI/2, USER_TOL * 100))
		firstxy.dx = 0;
	else
		firstxy.dx = r/tan(topRad);
	dxys.push_back(firstxy);//第一个斜线

	secondxy.dy = 0;
	if(cutterSegs.size() == 1)
	{
		if(cutterSegs[0].m_lengthType == E_CutterSegLength_刀尖到刀尖 || cutterSegs[0].m_lengthType == E_CutterSegLength_刀尖到圆柱)
			secondxy.dx = cutterSegs[0].m_length - firstxy.dx;
		else
			secondxy.dx = cutterSegs[0].m_length;
	}
	else//cutterSegs.size() 大于1（0和1已经排除）
		secondxy.dx = GetHorizontalLengthByType(cutterSegs[0],cutterSegs[1],firstxy.dx);

	dxys.push_back(secondxy);//第一个水平线放进去
	//到此为止  第一段处理结束产生了两个 dxy


	//--------------处理中间段和最后一段------------- //
	for(int i = 1; i < cutterSegs.size(); i++)
	{
		//第一步：排除
		double preR = cutterSegs[i-1].m_diameter/2;
		double curR = cutterSegs[i].m_diameter/2;
		if(CMathUtil::IsEqual(curR, preR, USER_TOL * 100 *100))
			return -2;//前后两个R不能相等

		//第二步：处理两个阶梯
		bool isTwo =false;
		SDXY ladderA, ladderB;
		ret = GetJieTiTwoDxy(cutterSegs[i-1], cutterSegs[i], isTwo, ladderA, ladderB);
		if(ret != 0)
		{
			CString info;
			info.Format(L"第%d段参数有错误\n", i+1);
			acutPrintf(info);
			return -3;
		}
		dxys.push_back(ladderA);//放进去ladderA
		if(isTwo)
			dxys.push_back(ladderB);//放进去ladderB

		//第三步：处理一个水平线
		secondxy.dy = 0;
		if(i < cutterSegs.size() - 1)
		{
			secondxy.dx = GetHorizontalLengthByType(cutterSegs[i], cutterSegs[i+1], ladderA.dx + ladderB.dx);
		}
		else//最后一段的水平线
		{
			if(cutterSegs[i].m_lengthType == E_CutterSegLength_刀尖到刀尖 || cutterSegs[i].m_lengthType == E_CutterSegLength_刀尖到圆柱)
				secondxy.dx = cutterSegs[i].m_length - ladderA.dx - ladderB.dx;
			else
				secondxy.dx = cutterSegs[i].m_length;
		}
		dxys.push_back(secondxy);//放进去水平线
	}

	for(int i = 0; i < dxys.size(); i++)
		xLen += dxys[i].dx;

	return 0;
}

int CThreadData::PostProcessAngleHead(vSDXY &dxys) const
{
	if (dxys.size() < 2)
		return -1;

	if (m_daoJianType == E_DaoJian_刀尖R && m_cutterType != T_PROFILE_CUTTER)//钻头铣刀和铰刀
	{
		//根据倒角R处理第一个点和第二个点 第三个点
		//第一个点 是斜的点 需要斜着缩短
		//第二个点 是横着的点 就是圆弧结束点---是增加的点
		//第三个点 需要dx缩小一点
		if ( m_pointR > 0)//处理刀尖R
		{
			double A = CMathUtil::AngleToRadian(m_topAngle/4.0);
			double B = 2*A;

			double tanA = tan(A);
			double len = m_pointR * tanA;

			double dx = len * cos(B);
			double dy = len * sin(B);

			//第一个点 ---缩短 加上圆弧
			dxys[0].dx -= dx;
			dxys[0].dy -= dy;

			double buchang = -0.023;//补偿
			dxys[0].buglet = - B/4.0 + buchang;

			//增加第二个点
			SDXY newDxy;
			newDxy.dx = dx + len;
			newDxy.dy = dy;
			newDxy.buglet = 0;
			dxys.insert(dxys.begin() + 1, newDxy);

			//处理第三段
			dxys[2].dx -= len;
		}
		return 0;
	}

	//处理成45度斜角的
	if (m_daoJianType == E_DaoJian_球头)
	{
		double r = m_cutterSegs[0].m_diameter/2.0;
		if (dxys.size() >= 2)
		{
			dxys[1].dx = dxys[0].dx + dxys[1].dx - r;
			dxys[0].dx = r;
			dxys[0].dy = r;
		}

		return 0;
	}

	if (m_cutterType == T_DRILL_CUTTER)//钻头
	{
		double r = m_cutterSegs[0].m_diameter/2.0;
		if (m_daoJianType == E_DaoJian_三尖 && m_pointCenterDistance > USER_TOL * 100
			&& m_topAngle > USER_TOL * 100 && m_topAngle < 180 && m_pointCenterDistance/2 < r)//处理中心距
		{
			double topRad = CMathUtil::AngleToRadian(m_topAngle/2.0);
	
			SDXY firstxy,secondxy;//firstxy是第一个竖直线， secondxy是第一个阶梯的水平线
			firstxy.dy = m_pointCenterDistance/2;
			firstxy.dx = firstxy.dy/tan(topRad);
			
			secondxy.dx = 0;
			secondxy.dy = r - m_pointCenterDistance/2;

			double xAdd = dxys[0].dx - firstxy.dx;
			
			dxys.erase(dxys.begin());
			dxys.insert(dxys.begin(),secondxy);
			dxys.insert(dxys.begin(),firstxy);

			if (m_cutterSegs[0].m_lengthType == E_CutterSegLength_圆柱到刀尖 || m_cutterSegs[0].m_lengthType == E_CutterSegLength_圆柱到圆柱)
			{
				int size = (int)dxys.size();
				dxys[size-2].dx += xAdd;
			}
			else
			    dxys[2].dx += xAdd;
		}
		return 0;
	}

	if ((m_daoJianType == E_DaoJian_平底 || m_daoJianType == E_DaoJian_NULL) && (m_cutterType == T_MILLING_CUTTER || m_cutterType == T_REAMER_CUTTER))//铣刀和铰刀
	{
		if (m_pointR > 0)//处理平底 倒角
		{
			 double dx = m_pointR;
			 double dy = m_pointR;

			//第一个点 ---缩短
			dxys[0].dx -= 0;
			dxys[0].dy -= dy;
			dxys[0].buglet = 0;

			//增加第二个点
			SDXY newDxy;
			newDxy.dx = dx;
			newDxy.dy = dy;
			newDxy.buglet = 0;
			dxys.insert(dxys.begin() + 1, newDxy);

			//处理第三段
			dxys[2].dx -= dx;
		}
		return 0;
	}
	//整硬铣刀的处理部分
	if (m_cutterType == T_DRILL_MILLING_CUTTER)
	{
		if (fabs(m_pointR) > 0)
		{

			double A = CMathUtil::AngleToRadian(m_topAngle / 4.0);
			double B = 2 * A;

			double tanA = tan(A);
			double len = m_pointR;// *tanA;

			double dx = m_pointR * cos(B);
			double dy = m_pointR * sin(B);

			//第一个点 ---缩短 加上圆弧
			dxys[0].dx -= dx;
			dxys[0].dy -= dy;

			double buchang = -0.023;//补偿
			dxys[0].buglet = -B / 4.0 ;

			//增加第二个点
			SDXY newDxy;
			newDxy.dx = dx + len;
			newDxy.dy = dy;
			newDxy.buglet = 0;
			dxys.insert(dxys.begin() + 1, newDxy);

			//处理第三段
			dxys[2].dx -= len;
		}
		return 0;
	}
	return 0;

	
}

AcDbPolyline * CThreadData::CreatePolyline2d(AcGePoint2d offsetXY, vSDXY &dxys, bool closed) const
{
	AcGePoint2dArray points; 

	//根据倒角R 和倒角 做后续处理
	PostProcessAngleHead(dxys);

	int start  = 0;
	points.append(offsetXY);

	for (int i = start; i < (int)dxys.size(); i++)
	{
		offsetXY = AcGePoint2d(offsetXY.x + dxys[i].dx, offsetXY.y + dxys[i].dy);
		points.append(offsetXY);
	}

	AcDbPolyline *pPoly= new AcDbPolyline();
	for (int i = 0; i < (int)points.length() ; i++)
	{
		if (i >= 1 && fabs(dxys[i - 1].buglet) > USER_TOL * 100)
		{
			pPoly->addVertexAt(i, points.at(i), dxys[i - 1].buglet);
		}
		else
		    pPoly->addVertexAt(i,points.at(i),0);
	}
	//pPoly->setBulgeAt(2, m_pointR);
	if (closed)
	    pPoly->setClosed(Adesk::kTrue);//将其闭合
	else
		pPoly->setClosed(Adesk::kFalse);
	pPoly->setNormal(AcGeVector3d(0,0,1));

	//AcGeMatrix3d transMX;
	//transMX.setToTranslation(AcGeVector3d(origin.x,origin.y,origin.z));//3维矩阵
	//pPoly->transformBy(transMX);
	pPoly->close();

	return pPoly;
}

int CThreadData::CreateModel3D(AcGePoint2d offsetXY, AcDbObjectId &mainid) const
{
	double angle = acos(-1);
	//AcGePoint2d offsetXY = AcGePoint2d(300,100);
	//第一步： 转换点
	vSDXY dxys;
	//这里计算出来的len已经不用了 后面进行了重新计算
	int ret = ConvertToDxy(m_topAngle, m_cutterSegs, m_totalLength - m_guoDuDaiX, dxys);//ConvertToDxy(m_topAngle, m_cutterSegs, m_totalLength - m_handleLength - m_guoDuDaiX, dxys);

	if(ret != 0)
	{
		acutPrintf(L"ConvertToDxy参数错误\n");
		return ret;
	}
	acDocManager->lockDocument(curDoc());

	//第二步：创建基础的面域
	AcDbPolyline *pline = CreatePolyline2d(offsetXY, dxys);
	Acad::ErrorStatus eks =  pline->setLayer(L"1");

	AcDb3dSolid *pSolid = NULL;
	AcDbRegion * region = CRegionUtil::CreateRegionFromCurve(pline);
	if (region == 0)
	{
		acutPrintf(L"创建界面失败，请检查阶梯第二段参数是否正确");
		acDocManager->unlockDocument(curDoc());
		return -1;
	}


	//第二步 附属 创建参考线镜像部分
	AcGeMatrix3d mxMirror;
	mxMirror.setToMirroring(AcGeLine3d(AcGePoint3d(offsetXY.x, offsetXY.y, 0), AcGeVector3d(1,0,0)));
	AcDbEntity * pMirrorPline = 0;
	pline->getTransformedCopy(mxMirror,pMirrorPline);
	AcDbObjectId plineId1 = CDwgDatabaseUtil::PostToModelSpace(pline);
	CEntityUtil::SetLayer(plineId1, L"1");
	AcDbObjectId plineId2 = CDwgDatabaseUtil::PostToModelSpace(pMirrorPline);
	CEntityUtil::SetLayer(plineId2, L"1");
	//JHCOM_SetEntityType(plineId1, L"ACAD_ISO02W100");
	//JHCOM_SetEntityType(plineId2, L"ACAD_ISO02W100");
#ifdef MIRROR
	CEntityUtil::Rotate(plineId1, AcGePoint2d(offsetXY.x, offsetXY.y), angle);
	CEntityUtil::Rotate(plineId2, AcGePoint2d(offsetXY.x, offsetXY.y), angle);
	/*CEntityUtil::Mirror(plineId1, AcGePoint3d(offsetXY.x, offsetXY.y, 0), AcGeVector3d(0, 1, 0));
	CEntityUtil::Mirror(plineId2, AcGePoint3d(offsetXY.x, offsetXY.y, 0), AcGeVector3d(0, 1, 0));*/
#endif
	//第三步：创建基础模型
	Acad::ErrorStatus es = Acad::eOk;
	if (region != NULL)
	{
		pSolid = new AcDb3dSolid(); 
		AcDbRevolveOptions options;
		es = pSolid->createRevolvedSolid(region,AcGePoint3d(offsetXY.x,offsetXY.y,0),AcGeVector3d(1,0,0), CMathUtil::PI*2, 0, options); //GetExAngle()*acos(0.0)/90
		
		//特殊处理球头的情况 钻头铣刀 铰刀 都有球头的选项---20190816
		if (m_daoJianType == E_DaoJian_球头 && m_cutterSegs.size() > 0)
		{
			//首先创建一个box把这个体裁剪掉
			AcDb3dSolid * pSolidSubBox = new AcDb3dSolid(); 
			pSolidSubBox->createBox(m_cutterSegs[0].m_diameter,m_cutterSegs[0].m_diameter,m_cutterSegs[0].m_diameter);
			AcGeMatrix3d mx;
			mx.setToTranslation(AcGeVector3d(offsetXY.x, offsetXY.y, 0));
			pSolidSubBox->transformBy(mx);
			es = pSolid->booleanOper(AcDb::kBoolSubtract,pSolidSubBox);

			//补充一个球形
			AcDb3dSolid * pSolidAddSphere = new AcDb3dSolid(); 
			pSolidAddSphere->createSphere(m_cutterSegs[0].m_diameter/2);
			mx.setToTranslation(AcGeVector3d(offsetXY.x+m_cutterSegs[0].m_diameter/2, offsetXY.y, 0));
			pSolidAddSphere->transformBy(mx);
			es = pSolid->booleanOper(AcDb::kBoolUnite,pSolidAddSphere);

			//CDwgDatabaseUtil::PostToModelSpace(pSolidAddSphere); 
		}
		//-----处理球头结束--------------//


		mainid = CDwgDatabaseUtil::PostToModelSpace(pSolid); 
	    pSolid->close();
	}

	int ladderNum = (int)m_cutterSegs.size();
	//第四步： 创建螺旋线 并根据螺旋线起点参数生成圆形
	AcDbObjectId helixId = 0, helixId2 = 0;

	//注意这个直径绝对不能大于螺旋线的直径 否则无法扫掠---扫描的圆弧直径
	//cirrad --- 扫描的那个圆形的半径
	double cirrad = GetDia(m_cutterSegs[ladderNum-1].m_diameter, m_cuttingEdgeCount);

	double xLen = 0;
	//尾巴切除方法 //目前所有钻头和7度的铰刀用球形生成方法
	int tailRemoveType = -1;
	if (m_cutterType == T_DRILL_CUTTER || m_cutterType == T_DRILL_MILLING_CUTTER)
		tailRemoveType = 1;
	else if (m_cutterType == T_MILLING_CUTTER)
	{
		if (m_cutterSegs[m_cutterSegs.size() - 1].m_bKaiRen == true)//铣刀开刃（标注刃长）
		{
			tailRemoveType = 3;
		}
		else
			tailRemoveType = 2;//铰刀不开刃 标注槽长
	}
	else if (m_cutterType == T_REAMER_CUTTER)
	{
		if (m_cutterSegs[m_cutterSegs.size() - 1].m_bKaiRen == true)//铰刀开刃（标注刃长）
		{
			tailRemoveType = 5;
		}
		else
			tailRemoveType = 4;//铰刀不开刃 标注槽长
	}
	
	

	for(int i = 0; i < m_cuttingEdgeCount; i++)
	{
		xLen = m_GrooveLength;//最后一段的长度 取做槽长

		if (tailRemoveType == 1)
		    xLen -= cirrad;//最后留一个球形的长度（体积）
		else if (tailRemoveType == 2)//铣刀标注槽长
			xLen -= 1.3*m_cutterSegs[ladderNum-1].m_diameter;//
		else if (tailRemoveType == 4)//铰刀标注槽长
			xLen -= 1.7*m_cutterSegs[ladderNum-1].m_diameter;//
			
		CreateHelix(offsetXY, m_cutterSegs[ladderNum-1].m_diameter/2.0, 
			              i * 2 * CMathUtil::PI/m_cuttingEdgeCount + GetHelixStartAngle(m_cuttingEdgeCount),
						   xLen,//总的高度
						   helixId
						   );

		AcGePoint3d stpnt = CCurveUtil::GetStartPoint(helixId);
		AcGeVector3d stdir = CCurveUtil::GetCurveStartPointVector(helixId);

		AcDbObjectId circleid = CCircleUtil::CreateCircle(stpnt, stdir, cirrad);
#ifndef TY_DRAW_3D
		CEntityUtil::ShowObject(circleid, false, true);
		CEntityUtil::ShowObject(helixId, false, true);
#endif

		AcDbObjectId extrudeId = 0;
		if(circleid != 0 && helixId != 0)
		{
			AcDbObjectId extrudeSolidId;
			CBrepUtil::ExtrudeAlongPath(circleid, helixId, extrudeSolidId);
			if (extrudeSolidId == 0)
                 continue;

			if (tailRemoveType == 1)
			{
				AcGePoint3d sphereCenter = CCurveUtil::GetEndPoint(helixId);
				AcDbObjectId id;
				CBrepUtil::CreateSphere(sphereCenter, cirrad, id);

				CBrepUtil::AN_Subtract(mainid, extrudeSolidId);
				CBrepUtil::AN_Subtract(mainid, id);
			}
			else
			{
				vAcBrFace faces;
				CBrepUtil::GetBrepFaces2(extrudeSolidId,faces);
				int index = CBrepUtil::GetMaxAreaFace(faces);
				CBrepUtil::AN_ExtrudeFace(extrudeSolidId, faces[1], faces[2], 60);
				CBrepUtil::AN_Subtract(mainid, extrudeSolidId);
			}
		}
	}

	//生成棱边--钻头支持多种棱边
	//铣刀铰刀支持单棱边
	{
		//CreateLengBians(mainid, offsetXY,cirrad,true);
	}
	AcDbObjectIdArray idarr;
	//投影
	TY_Project3DSolidTo2D(mainid, false, false, false,false,idarr);

#ifndef TY_DRAW_3D
	CObjectUtil::DeleteObject(mainid);
#endif

	
	//第五步：创建中心线
	AcDbObjectId centerLine = CLineUtil::CreateLine(AcGePoint3d(offsetXY.x - 3, offsetXY.y, 0),AcGePoint3d(offsetXY.x + m_totalLength + 3, offsetXY.y, 0));
	//JHCOM_SetEntityType(centerLine, L"ACAD_ISO04W100");
	CEntityUtil::SetLayer(centerLine, L"SK4");
	
	//第六步：创建标注
	CreateDims(offsetXY,AcGePoint3d(0,0,0));
#ifdef MIRROR
	for (AcDbObjectId id : idarr)
	{
		CEntityUtil::Rotate(id, AcGePoint2d(offsetXY.x, offsetXY.y), angle);
		//CEntityUtil::Mirror(id, AcGePoint3d(offsetXY.x, offsetXY.y, 0), AcGeVector3d(0, 1, 0));
	}
	CEntityUtil::Rotate(centerLine, AcGePoint2d(offsetXY.x, offsetXY.y), angle);
	//CEntityUtil::Mirror(centerLine, AcGePoint3d(offsetXY.x, offsetXY.y, 0), AcGeVector3d(0, 1, 0));
#endif
	//第七步：内冷孔处理
	//CreateLengQue(offsetXY);

	acDocManager->unlockDocument(curDoc());
	return 0;
}

AcDbPolyline * CreatePolyline(vAcGePoint3d &pnts, AcGeVector3d normal, AcGePoint3d origin)
{
	if(pnts.size() != 3)
		return NULL;

    AcGePoint2dArray points; 
	for (int i = 0; i < (int)pnts.size(); i++)
	{
		points.append(AcGePoint2d(pnts[i][0], pnts[i][1]));
	}

	//生成圆角矩形
	AcDbPolyline *pPoly= new AcDbPolyline(3);
	pPoly->addVertexAt(0,points.at(0),0);
	pPoly->addVertexAt(1,points.at(1),0);
	pPoly->addVertexAt(2,points.at(2),0);
	pPoly->setClosed(Adesk::kTrue);//将其闭合

	pPoly->setNormal(normal);

	AcGeMatrix3d transMX;
	AcGeVector3d vec=origin.asVector();//点转换成相同向量
	transMX.setToTranslation(vec);//3维矩阵
	pPoly->transformBy(transMX);
	
	pPoly->close();

	return pPoly;
}
//radius 小圆半径
//angle 弧度 角度
// 边len长度
/*
int CreateTrangle(AcGePoint2d offset, double radius, double startangle, double zhangKai, double len, AcDbObjectId &regionId)
{
	MD2010_CreateCycle(AcGePoint3d(offset.x, offset.y, 0), radius, AcGeVector3d(1,0,0));

	//
	AcGePoint3d cStart, cEnd;
	cStart.x = offset.x;
	cStart.y = offset.y + radius * sin(startangle);
	cStart.z = radius * cos(startangle);

	cEnd.x = offset.x;
	cEnd.y = offset.y + (radius + len) * sin(startangle);
	cEnd.z = (radius + len) * cos(startangle);

	AcDbObjectId lineId = MD2010_CreateLine(cStart, cEnd);



	AcDbObject *pEnt = NULL;

	acdbOpenAcDbObject(pEnt,lineId,AcDb::kForRead);
	AcDbLine *pLine = AcDbLine ::cast(pEnt);
	if (pLine == NULL)
		return 1;

	AcGeMatrix3d mx;
	mx.setToRotation(zhangKai/2, AcGeVector3d(1,0,0), cStart);
	AcDbEntity *pentCopy1;
	Acad::ErrorStatus es = pLine->getTransformedCopy(mx, pentCopy1);
	AcDbObjectId copylineID = CDwgDatabaseUtil::PostModalToBlockTable(pentCopy1, ACDB_MODEL_SPACE);

	AcDbEntity *pentCopy2;
	mx.setToRotation(-zhangKai/2, AcGeVector3d(1,0,0), cStart);
	es = pLine->getTransformedCopy(mx, pentCopy2);

	AcDbObjectId copylineID2 = CDwgDatabaseUtil::PostModalToBlockTable(pentCopy2, ACDB_MODEL_SPACE);

	
	AcDbLine *pLine1 = AcDbLine ::cast(pentCopy1);
	AcDbLine *pLine2 = AcDbLine ::cast(pentCopy2);


	AcGePoint3d pnt1 = cStart;
	AcGePoint3d pnt2 = pLine1->endPoint();
	AcGePoint3d pnt3 = pLine2->endPoint();

	AcDbLine *pLine3 = new AcDbLine(pnt2,pnt3);
	pLine3->setColorIndex(3);
	AcDbObjectId lineID = CDwgDatabaseUtil::PostModalToBlockTable(pLine3, ACDB_MODEL_SPACE);
	pLine3->close();
	pLine1->close();
	pLine2->close();

	AcDbVoidPtrArray paRect,paRegion;   
	paRect.append(pLine1);
	paRect.append(pLine2);
	paRect.append(pLine3);
	AcDbRegion::createFromCurves(paRect,paRegion); 
	
	regionId = CDwgDatabaseUtil::PostModalToBlockTable((AcDbRegion *)(paRegion[0]), ACDB_MODEL_SPACE);
	((AcDbRegion *)(paRegion[0]))->close();
	return 0;
}*/

int CreateTopTrangle(AcGePoint2d offset, double radius, double startangle, double zhangKai, double len, AcDbObjectId &regionId)
{
	AcGePoint3d cStart, cEnd;
	cStart.x = offset.x;
	cStart.y = offset.y + radius * sin(startangle);
	cStart.z = radius * cos(startangle);

	cEnd.x = offset.x;
	cEnd.y = offset.y + (radius + len) * sin(startangle);
	cEnd.z = (radius + len) * cos(startangle);

	AcDbLine *pLine = new AcDbLine(cStart,cEnd);


	AcGeMatrix3d mx;
	mx.setToRotation(zhangKai/2, AcGeVector3d(1,0,0), cStart);
	AcDbEntity *pentCopy1;
	Acad::ErrorStatus es = pLine->getTransformedCopy(mx, pentCopy1);
	AcDbObjectId copylineID = CDwgDatabaseUtil::PostModalToBlockTable(pentCopy1, ACDB_MODEL_SPACE);

	AcDbEntity *pentCopy2;
	mx.setToRotation(-zhangKai/2, AcGeVector3d(1,0,0), cStart);
	es = pLine->getTransformedCopy(mx, pentCopy2);
	AcDbObjectId copylineID2 = CDwgDatabaseUtil::PostModalToBlockTable(pentCopy2, ACDB_MODEL_SPACE);

	pLine->close();
	delete pLine;
	
	AcDbLine *pLine1 = AcDbLine ::cast(pentCopy1);
	AcDbLine *pLine2 = AcDbLine ::cast(pentCopy2);


	AcGePoint3d pnt1 = cStart;
	AcGePoint3d pnt2 = pLine1->endPoint();
	AcGePoint3d pnt3 = pLine2->endPoint();

	AcDbLine *pLine3 = new AcDbLine(pnt2,pnt3);
	//pLine3->setColorIndex(3);
	AcDbObjectId lineID = CDwgDatabaseUtil::PostModalToBlockTable(pLine3, ACDB_MODEL_SPACE);
	
	pLine1->close();
	pLine2->close();
	pLine3->close();

	AcDbVoidPtrArray paRect,paRegion;   
	paRect.append(pLine1);
	paRect.append(pLine2);
	paRect.append(pLine3);
	AcDbRegion::createFromCurves(paRect,paRegion); 
	
	regionId = CDwgDatabaseUtil::PostModalToBlockTable((AcDbRegion *)(paRegion[0]), ACDB_MODEL_SPACE);
	((AcDbRegion *)(paRegion[0]))->close();

	CObjectUtil::DeleteObject(lineID);
	CObjectUtil::DeleteObject(copylineID);
	CObjectUtil::DeleteObject(copylineID2);
	return 0;
}


int CreateTailTrangleAndRolve(AcGePoint2d offset, //三角形所在的位置
	double radius, //理解成三角形是沿着这个半径的圆弧滑动的 这个一半是开始圆的1/3这样一个圆弧上
	double startangle, //三角形在这个圆弧滑动时候 他所处的开始角度位置
	double zhangKai, //三角形的张开角
	double len, //三角形直角边长
	AcDbObjectId &mainid, AcDbObjectId &regionId, AcGePoint3d &farestIntersectPnt)
{
	AcGePoint3d cStart, cEnd;
	cStart.x = offset.x;
	cStart.y = offset.y + radius * sin(startangle);
	cStart.z = radius * cos(startangle);

	cEnd.x = offset.x;
	cEnd.y = offset.y + (radius + len) * sin(startangle);
	cEnd.z = (radius + len) * cos(startangle);

	AcDbLine *pLine = new AcDbLine(cStart,cEnd);

	//pLine 这个是张开角度的中线 就是沿着中心点 伸出去的一个中线

#ifdef TY_DRAW_3D
	pLine->setColorIndex(4);
	AcGeMatrix3d mx1;
	AcDbEntity * newEnt = 0;
	pLine->getTransformedCopy(mx1, newEnt);
	CDwgDatabaseUtil::PostModalToBlockTable(newEnt, ACDB_MODEL_SPACE);
#endif

	//转一个角度得到上边线
	AcGeMatrix3d mx;
	mx.setToRotation(zhangKai/2, AcGeVector3d(1,0,0), cStart);
	AcDbEntity *pentCopy1;
	Acad::ErrorStatus es = pLine->getTransformedCopy(mx, pentCopy1);
	pentCopy1->setColorIndex(1);
	AcDbObjectId copylineID = CDwgDatabaseUtil::PostModalToBlockTable(pentCopy1, ACDB_MODEL_SPACE);

	//转一个角度得到下边线
	AcDbEntity *pentCopy2;
	mx.setToRotation(-zhangKai/2, AcGeVector3d(1,0,0), cStart);
	es = pLine->getTransformedCopy(mx, pentCopy2);
	pentCopy2->setColorIndex(2);
	AcDbObjectId copylineID2 = CDwgDatabaseUtil::PostModalToBlockTable(pentCopy2, ACDB_MODEL_SPACE);

	pLine->close();
	delete pLine;
	
	AcDbLine *pLine1 = AcDbLine ::cast(pentCopy1);
	AcDbLine *pLine2 = AcDbLine ::cast(pentCopy2);


	AcGePoint3d pnt1 = cStart;
	AcGePoint3d pnt2 = pLine1->endPoint();
	AcGePoint3d pnt3 = pLine2->endPoint();

	//通过上边线和下边线的点得到第三根线
	AcDbLine *pLine3 = new AcDbLine(pnt2,pnt3);
	pLine3->setColorIndex(3);
	AcDbObjectId lineID = CDwgDatabaseUtil::PostModalToBlockTable(pLine3, ACDB_MODEL_SPACE);
	
	pLine1->close();
	pLine2->close();
	pLine3->close();

	AcDbVoidPtrArray paRect,paRegion;   
	paRect.append(pLine1);
	paRect.append(pLine2);
	paRect.append(pLine3);
	AcDbRegion::createFromCurves(paRect,paRegion); 
	
	regionId = CDwgDatabaseUtil::PostModalToBlockTable((AcDbRegion *)(paRegion[0]), ACDB_MODEL_SPACE);
	((AcDbRegion *)(paRegion[0]))->close();


	//
	AcGePoint3d line1Start = pLine1->startPoint();
	AcGePoint3d line1End = pLine1->endPoint();

	AcGeVector3d xDir(1,0,0);
	AcGeVector3d lineDir(line1Start.x - line1End.x, line1Start.y - line1End.y, line1Start.z - line1End.z);//Y direction
	AcGeVector3d dir = xDir.crossProduct(lineDir);//Z direction
	dir = dir.normalize();//Z direction


	AcDb3dSolid * pSolidRol = new AcDb3dSolid(); 
	es = pSolidRol->revolve(((AcDbRegion *)(paRegion[0])),line1End, dir, CMathUtil::PI*2);
	AcDbObjectId toolid = CDwgDatabaseUtil::PostToModelSpace(pSolidRol); 
	pSolidRol->close();

	//裁剪之前做一个辅助计算 计算出三角形旋转与主模型相交的最远点
	vAcGePoint3d pntsArray;
	AcDbCircle * pcircle = new AcDbCircle(line1End, AcGeVector3d(0,0,1), len);
	CBrepUtil::IntersectCurveWithSolid(mainid, pcircle, pntsArray);
	if (pntsArray.size() > 0)
	{
		//farestIntersectPnt = pntsArray[0];
		for(int j = 1; j < pntsArray.size(); j++ )	
		{
			if (pntsArray[j].x > farestIntersectPnt.x)
			{
				farestIntersectPnt = pntsArray[j];
			}
		}
	}
	delete pcircle;

	CBrepUtil::AN_Subtract(mainid, toolid);

#ifndef TY_DRAW_3D
	CObjectUtil::DeleteObject(regionId);
	CObjectUtil::DeleteObject(lineID);
	CObjectUtil::DeleteObject(copylineID);
	CObjectUtil::DeleteObject(copylineID2);
#endif
	
	return 0;
}

//经验获得直槽的抬起长度
double GetTaiQiLen_ZhiCao(double dia)
{
	if (dia < 12)
		return dia+1.56;
	if (dia < 22)
		return 11.56 + 4.02 * (dia-12)/10 + 0.8;
	if (dia < 32)
		return 16.28 + 2.08 * (dia-22)/10;

	return 20;
}
int CThreadData::CreateModel3D_ZhiCao(AcGePoint2d offsetXY, AcDbObjectId &mainid) const
{
	//AcGePoint2d offsetXY = AcGePoint2d(300,100);
	const volatile double angle = acos(-1);
	//第一步： 转换点
	vSDXY dxys;
	
	int ret = ConvertToDxy(m_topAngle, m_cutterSegs, m_totalLength - m_guoDuDaiX, dxys);//ConvertToDxy(m_topAngle, m_cutterSegs, m_totalLength - m_handleLength - m_guoDuDaiX, dxys);
	//for (int i = 0; i < dxys.size(); i++)
		//dxys[i].dy *= -1;
	if(ret != 0)
	{
		acutPrintf(L"ConvertToDxy参数错误\n");
		return ret;
	}
	//
	acDocManager->lockDocument(curDoc());

	//第二步：创建基础的面域
	AcDbPolyline *pline = CreatePolyline2d(offsetXY, dxys);
	AcDbObjectId plineId = CDwgDatabaseUtil::PostToModelSpace(pline); 
	//JHCOM_SetEntityType(plineId, L"ACAD_ISO02W100");
	AcDb3dSolid *pSolid = NULL;
	AcDbRegion * region = CRegionUtil::CreateRegionFromCurve(pline);


	//第二步 附属 创建参考线镜像部分
	AcGeMatrix3d mxMirror;
	mxMirror.setToMirroring(AcGeLine3d(AcGePoint3d(offsetXY.x, offsetXY.y, 0), AcGeVector3d(1,0,0)));
	AcDbEntity * pMirrorPline = 0;
	pline->getTransformedCopy(mxMirror,pMirrorPline);
	AcDbObjectId plineId2 = CDwgDatabaseUtil::PostToModelSpace(pMirrorPline);
	//JHCOM_SetEntityType(plineId2, L"ACAD_ISO02W100");
#ifdef MIRROR
	CEntityUtil::Rotate(plineId, AcGePoint2d(offsetXY.x, offsetXY.y), angle);
	CEntityUtil::Rotate(plineId2, AcGePoint2d(offsetXY.x, offsetXY.y), angle);
	/*CEntityUtil::Mirror(plineId, AcGePoint3d(offsetXY.x, offsetXY.y, 0), AcGeVector3d(0, 1, 0));
	CEntityUtil::Mirror(plineId2, AcGePoint3d(offsetXY.x, offsetXY.y, 0), AcGeVector3d(0, 1, 0));*/
#endif
	//第三步：创建基础模型
	Acad::ErrorStatus es = Acad::eOk;
	if (region != NULL)
	{
		pSolid = new AcDb3dSolid(); 
		AcDbRevolveOptions options;
		es = pSolid->createRevolvedSolid(region,AcGePoint3d(offsetXY.x,offsetXY.y,0),AcGeVector3d(1,0,0), CMathUtil::PI*2, 0, options); //GetExAngle()*acos(0.0)/90
		mainid = CDwgDatabaseUtil::PostToModelSpace(pSolid); 
	    pSolid->close();
	}

	double xLen = 0;
	xLen = m_GrooveLength;// m_cutterSegs[m_cutterSegs.size() - 1].m_length;//最后一段的长度 取做槽长
	double taiQiLen = 0;

	//钻头和铰刀的不开刃需要标注槽长
	if (m_cutterType == T_DRILL_CUTTER ||(m_cutterType == T_REAMER_CUTTER && !m_cutterSegs[m_cutterSegs.size() - 1].m_bKaiRen))
	    taiQiLen = GetTaiQiLen_ZhiCao(m_cutterSegs[m_cutterSegs.size() - 1].m_diameter);

	//第四步： 创建三角形 拉伸 并剪切
	for(int i = 0; i < m_cuttingEdgeCount; i++)
	{
		AcDbObjectId regionId = 0;
		double startAngle = i * 2 * CMathUtil::PI/m_cuttingEdgeCount - CMathUtil::PI/2;
		double zhangKaiAngle = CMathUtil::PI/m_cuttingEdgeCount;//张开稍微大一点
		CreateTopTrangle(offsetXY, m_cutterSegs[0].m_diameter/6, startAngle + zhangKaiAngle/2 /*开始角度 + 张开角度一半*/
			,zhangKaiAngle , 20, regionId);
		if (regionId != NULL)
		{
			AcDbObject *pEntTriganleReg = NULL;
			acdbOpenAcDbObject(pEntTriganleReg,regionId,AcDb::kForRead);
			AcDbRegion *preg = AcDbRegion ::cast(pEntTriganleReg);

			AcDb3dSolid * pSolidCao = new AcDb3dSolid(); 
			es = pSolidCao->extrude(preg,xLen-taiQiLen); //GetExAngle()*acos(0.0)/90
			AcDbObjectId toolid = CDwgDatabaseUtil::PostToModelSpace(pSolidCao); 
			pSolidCao->close();
			preg->close();
			CBrepUtil::AN_Subtract(mainid, toolid);
		}
		CObjectUtil::DeleteObject(regionId);
	}

	//第五步 创建底部三角形
	AcGePoint3d farestPnt;
	for(int i = 0; i < m_cuttingEdgeCount; i++)
	{
		AcGePoint2d offsetTail=offsetXY;
		offsetTail.x += xLen-taiQiLen;

		AcDbObjectId regionId = 0;

		double startAngle = i * 2 * CMathUtil::PI/m_cuttingEdgeCount- CMathUtil::PI/2;
		double zhangKaiAngle = CMathUtil::PI/m_cuttingEdgeCount;//张开稍微大一点
		CreateTailTrangleAndRolve(offsetTail, m_cutterSegs[0].m_diameter/6, startAngle + zhangKaiAngle/2 /*开始角度 + 张开角度一半*/
			,zhangKaiAngle, 20, mainid, regionId, farestPnt);
	}

	CreateLengBians(mainid, offsetXY, taiQiLen, false);

	//投影
	AcDbObjectIdArray idarr;
	TY_Project3DSolidTo2D(mainid, false, false, false,false,idarr) ;
#ifdef MIRROR
	for (auto id : idarr)
	{
		CEntityUtil::Rotate(id, AcGePoint2d(offsetXY.x, offsetXY.y), angle);
		//CEntityUtil::Mirror(id, AcGePoint3d(offsetXY.x, offsetXY.y,0),AcGeVector3d(0,1,0));
	}

#endif
#ifndef TY_DRAW_3D
	CObjectUtil::DeleteObject(mainid);
#endif
	
	//第六步：创建中心线
	AcDbObjectId centerLine = CLineUtil::CreateLine(AcGePoint3d(offsetXY.x - 3, offsetXY.y, 0),AcGePoint3d(offsetXY.x + m_totalLength + 3, offsetXY.y, 0));
	//JHCOM_SetEntityType(centerLine, L"ACAD_ISO04W100");
	CEntityUtil::SetLayer(centerLine, L"SK4");
#ifdef MIRROR
	CEntityUtil::Rotate(centerLine, AcGePoint2d(offsetXY.x, offsetXY.y), angle);
	//CEntityUtil::Mirror(centerLine, AcGePoint3d(offsetXY.x, offsetXY.y, 0), AcGeVector3d(0, 1, 0));
#endif
	CreateDims(offsetXY,farestPnt);

	//第八步：冷却
	CreateLengQue(offsetXY);

	acDocManager->unlockDocument(curDoc());
	return 0;
}
int CThreadData::CreateHelix(AcGePoint2d offsetXY, //偏移的点
	                       double firstRad, //开始的半径
						   double angle,//起始点角度，和Y轴的逆时针角度 弧度制
						   double totalHeight,//总的高度
						   AcDbObjectId &id
						   ) const
{
	AcDbObjectPointer<AcDbHelix> aHelix;
	aHelix.create();
	aHelix->setAxisPoint(AcGePoint3d(offsetXY.x, offsetXY.y, 0));
	aHelix->setAxisVector(AcGeVector3d(1, 0, 0));
	AcGePoint3d st = AcGePoint3d(0, offsetXY.y + firstRad * cos(angle), firstRad * sin(angle));
	aHelix->setStartPoint(st);//第一个点


	aHelix->setBaseRadius(firstRad);
	aHelix->setTopRadius(firstRad);
	aHelix->setTwist(m_isClockwise);

	// case
	/*aHelix->setConstrain(AcDbHelix::kTurnHeight);
	aHelix->setTurnHeight(15);
	aHelix->setHeight(100); // Turns = Height / TurnHeight

	// case
	aHelix->setConstrain(AcDbHelix::kTurnHeight);
	aHelix->setTurnHeight(15);
	aHelix->setTurns(100); // Height = TurnHeight * Turns

	// case
	aHelix->setConstrain(AcDbHelix::kTurns);
	aHelix->setTurns(7);
	aHelix->setHeight(100); // TurnHeight = Height / Turns

	aHelix->setConstrain(AcDbHelix::kTurns);
	aHelix->setTurns(7);
	aHelix->setTurnHeight(15); // Height = TurnHeight * Turns

	// case
	aHelix->setConstrain(AcDbHelix::kHeight);
	aHelix->setHeight(100);
	aHelix->setTurns(7);*/ // TurnHeight = Height / Turns
	//螺杆螺旋角为a, 直径为D,螺距为H,螺旋角的正切tana=H/3.14D
	double tanLuoXuanJiao = tan(m_luoXuanJiaoDu);

	double singleH = CMathUtil::PI * firstRad * 2 * tanLuoXuanJiao;
	// case
	aHelix->setConstrain(AcDbHelix::kHeight);
	aHelix->setHeight(totalHeight);
	aHelix->setTurnHeight(singleH); // Turns = Height / TurnHeight
	aHelix->setLayer(L"1");
	aHelix->createHelix();

	id = CDwgDatabaseUtil::PostToModelSpace(aHelix); 

	return 0;
}

int CThreadData::CreateHelix(AcGePoint2d offsetXY, //偏移的点
	                       double firstRad, //开始的半径
						   AcGePoint3d startPnt,//起点位置
						   double totalHeight,//总的高度
						   double singleH,
						   AcDbObjectId &id
						   ) const
{
	AcDbObjectPointer<AcDbHelix> aHelix;
	aHelix.create();
	aHelix->setAxisPoint(AcGePoint3d(offsetXY.x, offsetXY.y, 0));
	aHelix->setAxisVector(AcGeVector3d(1, 0, 0));
	aHelix->setStartPoint(startPnt);//第一个点

	aHelix->setBaseRadius(firstRad);
	aHelix->setTopRadius(firstRad);
	aHelix->setTwist(m_isClockwise);

	//螺杆螺旋角为a, 直径为D,螺距为H,螺旋角的正切tana=H/3.14D
	// case
	aHelix->setConstrain(AcDbHelix::kHeight);
	aHelix->setHeight(totalHeight);
	aHelix->setTurnHeight(singleH); // Turns = Height / TurnHeight
	aHelix->setLayer(L"1");
	aHelix->createHelix();

	id = CDwgDatabaseUtil::PostToModelSpace(aHelix); 

	return 0;
}

int CThreadData::CreateHelix(
	AcGePoint2d offsetXY, //偏移的点
	double firstRad, //开始的半径
	double endRad, //结束的半径
	AcGePoint3d startPnt,//起点位置
	double totalHeight,//总的高度
	double singleH,
	AcDbObjectId &id
	) const
{
	AcDbObjectPointer<AcDbHelix> aHelix;
	aHelix.create();
	aHelix->setAxisPoint(AcGePoint3d(offsetXY.x, offsetXY.y, 0));
	aHelix->setAxisVector(AcGeVector3d(1, 0, 0));
	aHelix->setStartPoint(startPnt);//第一个点


	aHelix->setBaseRadius(firstRad);
	aHelix->setTopRadius(endRad);
	aHelix->setTwist(m_isClockwise);

	//螺杆螺旋角为a, 直径为D,螺距为H,螺旋角的正切tana=H/3.14D
	// case
	aHelix->setConstrain(AcDbHelix::kHeight);
	aHelix->setHeight(totalHeight);
	//aHelix->setTurnHeight(singleH); // Turns = Height / TurnHeight

	aHelix->createHelix();
	id = CDwgDatabaseUtil::PostToModelSpace(aHelix); 

	return 0;
}

void CThreadData::Test_Data()
{
	m_topAngle = 120;
	m_totalLength = 160;
	m_cuttingEdgeCount = 2;
	m_sEdgeType = L"单棱边";

	CThreadSegData oneSeg;
	oneSeg.m_diameter = 10;
	oneSeg.m_length = 30;
	oneSeg.m_lengthType = E_CutterSegLength_刀尖到刀尖;

	AddCutterSegData(oneSeg);

	oneSeg.m_diameter = 12;
	oneSeg.m_length = 40;
	oneSeg.m_lengthType = E_CutterSegLength_刀尖到刀尖;
	oneSeg.m_bDoubleLadder = true;
	oneSeg.m_ladderAngle1 = 180;
	oneSeg.m_ladderAngle2 = 120;
	oneSeg.m_ladderLength1 = 0.5;
	AddCutterSegData(oneSeg);

	oneSeg.m_diameter = 16;
	oneSeg.m_length = 50;
	oneSeg.m_lengthType = E_CutterSegLength_刀尖到刀尖;
	oneSeg.m_bDoubleLadder = true;
	oneSeg.m_ladderAngle1 = 120;
	oneSeg.m_ladderAngle2 = 180;
	oneSeg.m_ladderLength1 = 0.5;
	AddCutterSegData(oneSeg);
}

double CThreadData::GetDia(const double dia, const int edgeCount) const
{
    if (edgeCount == 2)
    {
		return dia/3.0;
    }

	else if (edgeCount == 3 || edgeCount == 4)
	{
		return dia/3.5;
	}

	else if (edgeCount == 5 || edgeCount == 6)
	{
		return dia/6.0;
	}

	else if (edgeCount == 7 || edgeCount == 8)
	{
		return dia/8.0;
	}
	else
	{
		return dia/10.0;
	}
}

double CThreadData::GetHelixStartAngle(int edgeCount) const
{
	if (edgeCount == 2)
		return CMathUtil::PI *4/6;

	if (edgeCount == 3)
		return CMathUtil::PI/3;

	return 0;
}


//创建棱边
//saoMiaoCirRad 因为一个麻花钻只有一个固定的扫描的圆柱 用来裁剪主体
//把这个半径传送过来 主要是为了 最后一段计算剩余的长度，因为最后一段不能棱边到最后
int CThreadData::CreateLengBians(AcDbObjectId mainid, AcGePoint2d offsetXY, double saoMiaoCirRad, bool isMaHua) const
{
	vAcBrEdge edges;
	CBrepUtil::GetBrepEdges(mainid, edges);
	std::vector<AcGeCurve3d*> edgeCurves;

	AcGe::EntityId ctype;
	//bool is;
	for (int i = 0; i < edges.size(); i++)
	{
		AcGeCurve3d *oneCurve;
		edges[i].getCurve(oneCurve);
		edges[i].getCurveType(ctype);
		if (AcGe::kCircArc3d == ctype)
		{
			edgeCurves.push_back(oneCurve);
		}
	}

	for (int i = 0; i < m_cutterSegs.size(); i++)
	{
		//铣刀铰刀不开刃的情况 直接跳过
		if (m_cutterType == T_MILLING_CUTTER || m_cutterType == T_REAMER_CUTTER)
		{
			if (m_cutterSegs[i].m_bKaiRen == false)
				continue;
		}

		double rad = m_cutterSegs[i].m_diameter/2;
		double xValue = 0;
		double len = 0;
		GetXValueForOneCutterSeg(i, xValue);
		GetXLengthForOneCutterSeg(i, len);

		//if (i == 0)//第一段特殊处理
			//xValue = xValue + m_pointR;

		if (i == m_cutterSegs.size() - 1)//最后一段特殊处理
		{
			if (m_cutterType == T_DRILL_CUTTER && m_cutterSegs.size() > 1)
				len = m_cutterSegs[i].m_length - 1;// -xValue - saoMiaoCirRad;
			else//铣刀铰刀
			{
				if (!isMaHua)//铣刀铰刀-直槽-最后一段
					len = m_cutterSegs[i].m_length -xValue - saoMiaoCirRad;
				else//铣刀铰刀-麻花-最后一段
				    len = m_cutterSegs[i].m_length - xValue;
			}
		}

		//20190729---
		//统一把len减少一点，否则开刃开的太长了 开到下一个立面去了
		//实际生产也是这样的 一般不能开到底
		//对于单阶梯或者最后一个阶梯 不需要这样
		if (m_cutterSegs.size() > 1 && (i != m_cutterSegs.size() - 1))
		    len = len - 1;
		
		CreateLengBianForOneSeg(mainid, edgeCurves, offsetXY, xValue, rad, len,m_cutterSegs[i].m_lengBianType,isMaHua);
	}
	return 0;
}

//专门为某个半径的阶梯创建棱边
int CThreadData::CreateLengBianForOneSeg(AcDbObjectId mainId, std::vector<AcGeCurve3d*> &edges,AcGePoint2d offsetXY, double xvalue, double rad, double len, SEdgeType edgeType, bool isMaHua) const
{
	xvalue = offsetXY.x + xvalue;

	//第一步找到所有的半径是rad的圆弧
	std::vector<AcGeCircArc3d> allRadCurves;
//	Acad::ErrorStatus es;
	for (int i = 0; i < edges.size(); i++)
	{
		AcGeCurve3d *crv = edges[i];
		AcDbCurve *pDbCurve = 0;

		AcGePoint3d start = CCurveUtil::GetCurveStart(crv);
		AcGePoint3d end = CCurveUtil::GetCurveEnd(crv);
		AcGePoint3d mid = CCurveUtil::GetCurveMid(crv);
		AcGeCircArc3d arc = AcGeCircArc3d(start,mid,end);
		double thisRad = arc.radius();
		if (fabs(thisRad-rad) < USER_TOL * 100 && fabs(xvalue - start.x) < USER_TOL * 100)
		{
			allRadCurves.push_back(arc);
		}
		continue;
	}

	//第二步： 所有的通直径的圆弧中 找到x值固定
	//SEdgeType et = SEdgeType::E_EdgeType_单棱边;
	for (int i = 0; i < allRadCurves.size(); i++)
	{
		CreateLengBianForOneArc(mainId, allRadCurves[i], offsetXY, len, edgeType, isMaHua);
	}
	return 0;
}


int CThreadData::GetXLengthForOneCutterSeg(int index, double &len) const
{
	if (index >= m_cutterSegs.size())
		return -1;

	vSDXY dxys;
	ConvertToDxy(m_topAngle, m_cutterSegs, m_totalLength-m_handleLength - m_guoDuDaiX, dxys);

	//特殊情况处理
	if (m_cutterSegs.size() == 1 && index == 0)
	{
		len = m_cutterSegs[0].m_length - dxys[0].dx;
		return 0;
	}
	
	int cou = 0;
	for (int i = 0; i < dxys.size(); i++)
	{
		if (dxys[i].dx > USER_TOL*100 * 100 && fabs(dxys[i].dy) < USER_TOL * 100)
		{
			if (index == cou)
			{
				len = dxys[i].dx;
			}
			cou++;
		}
	}

	return 0;
}

int CThreadData::GetYLengthForOneCutterSeg(int index, double &len) const
{
	if (index >= m_cutterSegs.size() || index < 0)
		return -1;

	if (index == 0)
		len = m_cutterSegs[index].m_diameter/2;
	else
		len = (m_cutterSegs[index].m_diameter - m_cutterSegs[index-1].m_diameter)/2;

	return 0;
}

int CThreadData::GetXValueForOneCutterSeg(int index, double &x) const
{
	if (index >= m_cutterSegs.size())
		return -1;


	vector<CThreadSegData> tempcutterSegs;
	for (int i = 0; i <= index; i++)
	{
		tempcutterSegs.push_back(m_cutterSegs[i]);
	}


	vSDXY dxys;
	ConvertToDxy(m_topAngle, tempcutterSegs, m_totalLength-m_handleLength - m_guoDuDaiX, dxys);
	PostProcessAngleHead(dxys);//20190702
	dxys.erase(dxys.end() -1);
	dxys.erase(dxys.end() -1);
	x = 0;
	for (int i = 0; i < dxys.size(); i++)
	{
		x += dxys[i].dx;
	}
	return 0;
}

int CThreadData::GetXValueForOneCutterSeg_2(int index, double &x) const
{
	if (index >= m_cutterSegs.size())
		return -1;

	vector<CThreadSegData> tempcutterSegs;
	for (int i = 0; i <= index; i++)
	{
		tempcutterSegs.push_back(m_cutterSegs[i]);
	}


	vSDXY dxys;
	ConvertToDxy(m_topAngle, tempcutterSegs, m_totalLength-m_handleLength - m_guoDuDaiX, dxys);
	dxys.erase(dxys.end() -1);
	dxys.erase(dxys.end() -1);
	x = 0;
	for (int i = 0; i < dxys.size(); i++)
	{
		if (i == dxys.size() - 1)//最后一段肯定不要
			continue;

		if (i == dxys.size() - 2 && dxys[i].dy > 100 * USER_TOL * 100)//最后一段肯定不要
			continue;

		x += dxys[i].dx;
	}
	return 0;
}

//得到第index个阶梯的 第一阶梯的起点和终点
int CThreadData::GetSegLadderAPnts(int index, AcGePoint3d &start, AcGePoint3d &end)const
{
	if (index < 1 || index >= m_cutterSegs.size())
		return -1;

	vSDXY dxys;
	ConvertToDxy(m_topAngle, m_cutterSegs, m_totalLength-m_handleLength - m_guoDuDaiX, dxys);

	int count = 0;
	double xlen = 0, ylen = 0;
	for (int i = 0; i < dxys.size(); i++)
	{
		if (count == index && fabs(dxys[i].dx) > USER_TOL * 100 && fabs(dxys[i].dy) > USER_TOL * 100)
		{
			start = AcGePoint3d(xlen,ylen,0);
			end = AcGePoint3d(xlen+dxys[i].dx,ylen+dxys[i].dy,0);
			return 0;
		}
		xlen+=dxys[i].dx;
		ylen+=dxys[i].dy;
		if (fabs(dxys[i].dy) < USER_TOL * 100)
		{
			count++;
			continue;
		}
	}

	return -1;
}

int CThreadData::GetSegLadderBPnts(int index, AcGePoint3d &start, AcGePoint3d &end) const
{
	if (index < 1 || index >= m_cutterSegs.size())
		return -1;

	if (m_cutterSegs[index].m_bDoubleLadder == false)
		return -1;

	vSDXY dxys;
	ConvertToDxy(m_topAngle, m_cutterSegs, m_totalLength-m_handleLength - m_guoDuDaiX, dxys);

	int count = 0;
	double xlen = 0, ylen = 0;
	for (int i = 0; i < dxys.size(); i++)
	{
		if (count == index && fabs(dxys[i].dx) > USER_TOL * 100 && fabs(dxys[i].dy) > USER_TOL * 100)
		{
			if (dxys.size() > i+1 && 
				fabs(dxys[i+1].dx) > USER_TOL * 100 &&
				fabs(dxys[i+1].dy) > USER_TOL * 100
				)
			{
				start = AcGePoint3d(xlen+dxys[i].dx,ylen+dxys[i].dy,0);
				end = AcGePoint3d(xlen+dxys[i].dx+dxys[i+1].dx,ylen+dxys[i].dy+dxys[i+1].dy,0);
				return 0;
			}
		}
	

		xlen+=dxys[i].dx;
		ylen+=dxys[i].dy;

		if (fabs(dxys[i].dy) < USER_TOL * 100)
		{
			count++;
			continue;
		}

	}	

	return -1;
}

AcGePoint3d CThreadData::GetOffsetPnt(AcGePoint3d pnt, AcGePoint2d offsetXY) const
{
	return AcGePoint3d(pnt.x + offsetXY.x, pnt.y + offsetXY.y, pnt.z);
}

//传入的pnt是实际的offset后的pnt
AcGePoint3d CThreadData::GetMirrorPnt(AcGePoint3d pnt, AcGePoint2d offsetXY)const
{
	return AcGePoint3d(pnt.x, offsetXY.y - (pnt.y - offsetXY.y), pnt.z);
}

int CThreadData::CreateRegionForOneArc(AcGeCircArc3d&arc, AcGePoint2d offsetXY,  SEdgeType edgeType, 
	AcGePoint3d &mid, AcDbObjectId &regionId, bool second) const
{
	//如果是双棱边---中间生成
	AcGeCircArc3d midArc;
	AcGeCircArc3d outerArc;
	AcGeCircArc3d innerArc;
	AcDbCurve* pcurvIn = 0;
	AcDbCurve* pcurvOut = 0;

	if (edgeType == SEdgeType::E_EdgeType_双棱边背两侧)
	{
		AcGeInterval range;
		arc.getInterval(range);
		double low = range.lowerBound();
		double high = range.upperBound();

		AcGePoint3d start = arc.evalPoint(low + (high - low)/6);
		AcGePoint3d end = arc.evalPoint(low + 5*(high - low)/6);
		mid = arc.evalPoint(low + (high - low)/2);


		//这个是往前推一段---因为扫描时候 前端会有一段 不够裁剪
		start.x -= 0;
		end.x -= 0;
		mid.x -= 0;

		/*start.x -= HELIX_X_SUB;
		end.x -= HELIX_X_SUB;
		mid.x -= HELIX_X_SUB;*/

		midArc = AcGeCircArc3d(start,mid,end);

		outerArc = AcGeCircArc3d(midArc.center(), midArc.normal(), midArc.refVec(), midArc.radius() + 0.02,
			midArc.startAng(), midArc.endAng());
		innerArc = AcGeCircArc3d(midArc.center(), midArc.normal(), midArc.refVec(), midArc.radius() - 0.02,
			midArc.startAng(), midArc.endAng());

		//内圈圆弧
		pcurvIn = CConvertUtil::ConvertGeCurveToDbCurve(&innerArc);

		//外圈圆弧
		pcurvOut = CConvertUtil::ConvertGeCurveToDbCurve(&outerArc);
	}

	if (edgeType == SEdgeType::E_EdgeType_双棱边)
	{
		if (!second)//第一个
		{
			AcGeInterval range;
			arc.getInterval(range);
			
			AcGePoint3d start = arc.evalPoint(range.lowerBound() + (range.upperBound()-range.lowerBound())*1/6);//往前推一点
			AcGePoint3d end = arc.evalPoint(range.lowerBound() + (range.upperBound()-range.lowerBound())*5/12);
			mid = arc.evalPoint(range.lowerBound() + (range.upperBound()-range.lowerBound())*3.0/12);


			//这个是往前推一段---因为扫描时候 前端会有一段 不够裁剪
			start.x -= 0;
			end.x -= 0;
			mid.x -= 0;

			midArc = AcGeCircArc3d(start,mid,end);

			outerArc = AcGeCircArc3d(midArc.center(), midArc.normal(), midArc.refVec(), midArc.radius() + 0.1,
				midArc.startAng(), midArc.endAng());
			innerArc = AcGeCircArc3d(midArc.center(), midArc.normal(), midArc.refVec(), midArc.radius() - 0.1,
				midArc.startAng(), midArc.endAng());

			//内圈圆弧
			pcurvIn = CConvertUtil::ConvertGeCurveToDbCurve(&innerArc);

			//外圈圆弧
			pcurvOut = CConvertUtil::ConvertGeCurveToDbCurve(&outerArc);
		}
		else//单棱边
		{
			AcGeInterval range;
			arc.getInterval(range);
			AcGePoint3d start = arc.evalPoint(range.lowerBound() + (range.upperBound()-range.lowerBound())*7/12);
			AcGePoint3d end = arc.evalPoint(range.upperBound());
			mid = arc.evalPoint(range.lowerBound() + (range.upperBound()-range.lowerBound())*9.5/12);


			//这个是往前推一段---因为扫描时候 前端会有一段 不够裁剪
			start.x -= 0;
			end.x -= 0;
			mid.x -= 0;

			midArc = AcGeCircArc3d(start,mid,end);

			outerArc = AcGeCircArc3d(midArc.center(), midArc.normal(), midArc.refVec(), midArc.radius() + 0.2,
				midArc.startAng(), midArc.endAng());
			innerArc = AcGeCircArc3d(midArc.center(), midArc.normal(), midArc.refVec(), midArc.radius() - 0.2,
				midArc.startAng(), midArc.endAng());

			//内圈圆弧
			pcurvIn = CConvertUtil::ConvertGeCurveToDbCurve(&innerArc);

			//外圈圆弧
			pcurvOut = CConvertUtil::ConvertGeCurveToDbCurve(&outerArc);
		}
		
	}

	else if (edgeType == SEdgeType::E_EdgeType_单棱边)
	{
		AcGeInterval range;
		arc.getInterval(range);

		double low = range.lowerBound();
		double high = range.upperBound();

		double parm = low + (high- low)*1/6;
		AcGePoint3d start = arc.evalPoint(parm);
		parm = high;
		AcGePoint3d end = arc.evalPoint(parm);
		parm = low + (high- low)*7/12;
		mid = arc.evalPoint(parm);

		//这个是往前推一段---因为扫描时候 前端会有一段 不够裁剪
		//这里暂时不需要HELIX_X_SUB？ 20190702
		double helixXSub = 0;//-1
		start.x -= helixXSub;
		end.x -= helixXSub;
		mid.x -= helixXSub;

		midArc = AcGeCircArc3d(start,mid,end);

		outerArc = AcGeCircArc3d(midArc.center(), midArc.normal(), midArc.refVec(), midArc.radius() + 0.2,
			midArc.startAng()/*-PI/12*/, //因为上面helixXSub 延申了1长度 所以如果这里不减去一部分 不能刚好裁剪掉
			midArc.endAng());
		innerArc = AcGeCircArc3d(midArc.center(), midArc.normal(), midArc.refVec(), midArc.radius() - 0.2,
			midArc.startAng()/*-PI/12*/, 
			midArc.endAng());

		//内圈圆弧
		pcurvIn = CConvertUtil::ConvertGeCurveToDbCurve(&innerArc);

		//外圈圆弧
		pcurvOut = CConvertUtil::ConvertGeCurveToDbCurve(&outerArc);

		//中间圆弧
#ifndef TY_DRAW_3D
		CConvertUtil::ConvertGeCurveToDbCurve(&midArc);
#endif
	}
	
	AcDbLine *pLine1 = new AcDbLine(innerArc.startPoint(), outerArc.startPoint());
	AcDbLine *pLine2 = new AcDbLine(innerArc.endPoint(), outerArc.endPoint());

	AcDbVoidPtrArray paRect,paRegion;   
	paRect.append(pcurvIn);
	paRect.append(pLine1);
	paRect.append(pcurvOut);
	paRect.append(pLine2);
	Acad::ErrorStatus es = AcDbRegion::createFromCurves(paRect,paRegion); 

	regionId = CDwgDatabaseUtil::PostModalToBlockTable((AcDbRegion *)(paRegion[0]), ACDB_MODEL_SPACE);
	((AcDbRegion *)(paRegion[0]))->close();

	return 0;
}


int CThreadData::CreateLengBianForOneArc(AcDbObjectId mainId, AcGeCircArc3d&arc, AcGePoint2d offsetXY, double len, SEdgeType edgeType, bool isMaHua) const
{
	if (edgeType == SEdgeType::E_EdgeType_不清边)
		return 0;

	//创建一个面域
	AcDbObjectId regionId;
	AcGePoint3d mid;
    CreateRegionForOneArc(arc, offsetXY, edgeType, mid, regionId);

	if (isMaHua)//麻花类型的
	{
		//创建螺旋线
		double tanLuoXuanJiao = tan(m_luoXuanJiaoDu);
		double singleH = CMathUtil::PI * m_cutterSegs[m_cutterSegs.size() - 1].m_diameter/2 * 2 * tanLuoXuanJiao;
		AcGePoint2d offsetXYThis(mid.x, offsetXY.y);
		AcDbObjectId helixId;
		CreateHelix(offsetXYThis, arc.radius(), mid, 
			len+0.0,//总的高度
			singleH,
			helixId
			);

		//拉伸裁剪
		AcDbObjectId extrudeSolidId;
		JHCOM_ExtrudeAlongPath2(regionId, helixId, extrudeSolidId);

		CBrepUtil::AN_Subtract(mainId, extrudeSolidId);

		//隐藏对象
#ifndef TY_DRAW_3D
		CEntityUtil::ShowObject(regionId, false, true);
		CEntityUtil::ShowObject(helixId, false, true);
#endif

		//--------------------单独处理E_EdgeType_双棱边---------------//
		if (edgeType == SEdgeType::E_EdgeType_双棱边)//第二个
		{
			AcDbObjectId regionId;
			CreateRegionForOneArc(arc, offsetXY, edgeType, mid, regionId, true);

			//创建螺旋线
			double tanLuoXuanJiao = tan(m_luoXuanJiaoDu);
			double singleH = CMathUtil::PI * m_cutterSegs[m_cutterSegs.size() - 1].m_diameter/2 * 2 * tanLuoXuanJiao;
			AcGePoint2d offsetXYThis(mid.x, offsetXY.y);
			AcDbObjectId helixId;
			CreateHelix(offsetXYThis, arc.radius(), mid, 
				len+1.0,//总的高度
				singleH,
				helixId
				);

			JHCOM_ExtrudeAlongPath2(regionId, helixId, extrudeSolidId);

			CBrepUtil::AN_Subtract(mainId, extrudeSolidId);
			//隐藏对象
#ifndef TY_DRAW_3D
			CEntityUtil::ShowObject(regionId, false, true);
			CEntityUtil::ShowObject(helixId, false, true);
#endif

		}
	}
	else//直槽类型的
	{
		//创建直线
		AcGePoint3d end = mid;

		if (edgeType == SEdgeType::E_EdgeType_双棱边)
		    end.x += (len + HELIX_X_SUB);
		else
            end.x += len;

		AcDbObjectId lineId = CLineUtil::CreateLine(mid, end);
	
		//拉伸裁剪
		AcDbObjectId extrudeSolidId;
		JHCOM_ExtrudeAlongPath2(regionId, lineId, extrudeSolidId);

		CBrepUtil::AN_Subtract(mainId, extrudeSolidId);

		//隐藏对象
#ifndef TY_DRAW_3D
		CEntityUtil::ShowObject(lineId, false, true);
		CEntityUtil::ShowObject(regionId, false, true);
#endif


		//--------------------单独处理E_EdgeType_双棱边---------------//
		if (edgeType == SEdgeType::E_EdgeType_双棱边)//第二个
		{
			AcDbObjectId regionId;
			CreateRegionForOneArc(arc, offsetXY, edgeType, mid, regionId, true);

			AcGePoint3d end = mid;
			end.x += (len + HELIX_X_SUB);
			AcDbObjectId lineId = CLineUtil::CreateLine(mid, end);

			JHCOM_ExtrudeAlongPath2(regionId, lineId, extrudeSolidId);

			CBrepUtil::AN_Subtract(mainId, extrudeSolidId);

			CEntityUtil::ShowObject(regionId, false, true);
			CEntityUtil::ShowObject(lineId, false, true);

			//隐藏对象
#ifndef TY_DRAW_3D
			CEntityUtil::ShowObject(lineId, false, true);
#endif
		}
	}
	

	//隐藏对象
#ifndef TY_DRAW_3D
	CEntityUtil::ShowObject(regionId, false, true);
#endif

	return 0;
}

int CThreadData::CreateDims(AcGePoint2d offsetXY,AcGePoint3d farestPnt) const
{
	//标注图层
	CLayerSwitch layer(DIMLAYERNAME);
	//
	int size = (int)m_cutterSegs.size();
	if (size == 0)
		return -1;

	//创建X方向的dims
	double yvalue = offsetXY.y -m_cutterSegs[size - 1].m_diameter/2 - 10;
	//for (int i = 0; i < m_cutterSegs.size()-1; i++)
	//{
	//	if (m_cutterSegs[i].m_lengthType == E_CutterSegLength_刀尖到刀尖 || m_cutterSegs[i].m_lengthType == E_CutterSegLength_刀尖到圆柱)
	//	{
	//		AcGePoint3d start(0,offsetXY.y,0),end(0,offsetXY.y,0);
	//		GetXValueForOneCutterSeg_2(i,start.x);
	//		start.x += offsetXY.x;

	//		end.x = start.x + m_cutterSegs[i].m_length;
	//		AcGePoint3d dimpt(0,yvalue,0);
	//		dimpt.x = (start.x + end.x)/2;
	//		CDimensionUtil::AddDimAligned(start, end, dimpt,L"");
	//	}

	//	if (m_cutterSegs[i].m_lengthType == E_CutterSegLength_圆柱到刀尖 || m_cutterSegs[i].m_lengthType == E_CutterSegLength_圆柱到圆柱)
	//	{
	//		AcGePoint3d start(0,offsetXY.y,0),end(0,offsetXY.y,0);
	//		GetXValueForOneCutterSeg(i,start.x);
	//		start.x += offsetXY.x;

	//		end.x = start.x + m_cutterSegs[i].m_length;
	//		AcGePoint3d dimpt(0,yvalue,0);
	//		dimpt.x = (start.x + end.x)/2;
	//		CDimensionUtil::AddDimAligned(start, end, dimpt,L"");
	//	}
	//}
	AcDbObjectId id;
	for (int i = 0; i < m_cutterSegs.size() ; i++)
	{
		AcGePoint3d start(0,offsetXY.y,0), end(0,offsetXY.y,0);
		double x = 0;
		if (m_cutterType != T_DRILL_MILLING_CUTTER) {
			GetXValueForOneCutterSeg(0, x);
		}
		else {
			GetXValueForOneCutterSeg_2(0, x);
		}
		start.x = offsetXY.x + x;
			double sum = 0.0;
			for (int j = 0; j<=i;j++)
			{
				sum += m_cutterSegs[j].m_length;
			}
			end.x = start.x + sum ;
			AcGePoint3d dimpt(0, yvalue, 0);
			dimpt.x = (start.x + end.x) / 2;
			dimpt.y -= i * DIMDISTANCE;
			id = CDimensionUtil::AddDimAligned(start, end, dimpt, L"");
#ifdef MIRROR
			CEntityUtil::Mirror(id, AcGePoint3d(offsetXY.x, offsetXY.y, 0), AcGeVector3d(0, 1, 0));
#endif
	}
	//排屑槽长不需要标注
	////标注一个槽长
	AcGePoint3d start(offsetXY.x, offsetXY.y, 0);
	AcGePoint3d end(offsetXY.x + m_GrooveLength, offsetXY.y, 0);
	//AcGePoint3d dimpt(start.x/2 + end.x/2,yvalue,0);
	//dimpt.y -= m_cutterSegs.size() * 4;
	//id = CDimensionUtil::AddDimAligned(start, end, dimpt,L"");
	//铣刀添加一个刀尖圆角标注
	if (m_cutterType == T_DRILL_MILLING_CUTTER)
	{
		double x;
		AcGePoint3d start(0, offsetXY.y - m_cutterSegs[0].m_diameter / 2, 0);
		GetXValueForOneCutterSeg(0, x);
		start.x = offsetXY.x + x;
		AcGePoint3d ptCenter(start.x, start.y + m_pointR, start.z);
		AcDbObjectId id = CDimensionUtil::AddDimRadial(ptCenter,m_pointR,acos(-0.5) *2 );
		CEntityUtil::Mirror(id, AcGePoint3d(offsetXY.x, offsetXY.y, 0), AcGeVector3d(0, 1, 0));
	}
#ifdef MIRROR
	//CEntityUtil::Mirror(id, AcGePoint3d(offsetXY.x, offsetXY.y, 0), AcGeVector3d(0, 1, 0));
#endif
	//标注一个总长
	AcGePoint3d start2(offsetXY.x, offsetXY.y, 0);
	//总长标注改变 需要加上刀柄的长度
	double len = GetHandleLengthFromDaoBing(m_daobing); 

	AcGePoint3d end2(offsetXY.x + m_totalLength + len, offsetXY.y, 0);
	AcGePoint3d dimpt2(start.x/2 + end.x/2,yvalue,0);
	dimpt2.y = dimpt2.y - m_cutterSegs.size() * 10 - 10;
	id = CDimensionUtil::AddDimAligned(start2, end2, dimpt2,nullptr);
#ifdef MIRROR
	CEntityUtil::Mirror(id, AcGePoint3d(offsetXY.x, offsetXY.y, 0), AcGeVector3d(0, 1, 0));
#endif
	//Y方向标注
	double backX = DIMDISTANCE;
	//AcDbHandle tkhandle;
	//tkhandle = CObjectUtil::IDtoHandle(m_tuKuangId);
	for (int i = 0; i < m_cutterSegs.size(); i++)
	{
		AcGePoint3d start(0,offsetXY.y+m_cutterSegs[i].m_diameter/2,0),
			          end(0,offsetXY.y-m_cutterSegs[i].m_diameter/2,0);
		GetXValueForOneCutterSeg(i,start.x);
		start.x += offsetXY.x;
		end.x = start.x;

		AcGePoint3d dim(offsetXY.x - (i+1)*backX, offsetXY.y+10, 0);

		CString rePlaceText;
		if (m_cutterSegs[i].m_topGongCha < 0 && m_cutterSegs[i].m_lowGongCha < 0)
		{
			if (m_cutterSegs[i].m_zhiJingJD == 1)
			    rePlaceText.Format(L"%%%%C%.1f{\\H0.7x;\\S%.3f^%.3f;}",m_cutterSegs[i].m_diameter, m_cutterSegs[i].m_topGongCha,m_cutterSegs[i].m_lowGongCha);
			else if(m_cutterSegs[i].m_zhiJingJD == 2)
				rePlaceText.Format(L"%%%%C%.2f{\\H0.7x;\\S%.3f^%.3f;}",m_cutterSegs[i].m_diameter, m_cutterSegs[i].m_topGongCha,m_cutterSegs[i].m_lowGongCha);
			else
				rePlaceText.Format(L"%%%%C%.1f{\\H0.7x;\\S%.3f^%.3f;}",m_cutterSegs[i].m_diameter, m_cutterSegs[i].m_topGongCha,m_cutterSegs[i].m_lowGongCha);
		}
		else if (m_cutterSegs[i].m_topGongCha < 0)
		{
			if (m_cutterSegs[i].m_zhiJingJD == 1)
			    rePlaceText.Format(L"%%%%C%.1f{\\H0.7x;\\S%.3f^+%.3f;}",m_cutterSegs[i].m_diameter, m_cutterSegs[i].m_topGongCha,m_cutterSegs[i].m_lowGongCha);
			else if(m_cutterSegs[i].m_zhiJingJD == 2)
			    rePlaceText.Format(L"%%%%C%.2f{\\H0.7x;\\S%.3f^+%.3f;}",m_cutterSegs[i].m_diameter, m_cutterSegs[i].m_topGongCha,m_cutterSegs[i].m_lowGongCha);
			else
			    rePlaceText.Format(L"%%%%C%.3f{\\H0.7x;\\S%.3f^+%.3f;}",m_cutterSegs[i].m_diameter, m_cutterSegs[i].m_topGongCha,m_cutterSegs[i].m_lowGongCha);
		}
		else if (m_cutterSegs[i].m_lowGongCha < 0)
		{
			if (m_cutterSegs[i].m_zhiJingJD == 1)
			    rePlaceText.Format(L"%%%%C%.1f{\\H0.7x;\\S+%.3f^%.3f;}",m_cutterSegs[i].m_diameter, m_cutterSegs[i].m_topGongCha,m_cutterSegs[i].m_lowGongCha);
		    else if(m_cutterSegs[i].m_zhiJingJD == 2)
			    rePlaceText.Format(L"%%%%C%.2f{\\H0.7x;\\S+%.3f^%.3f;}",m_cutterSegs[i].m_diameter, m_cutterSegs[i].m_topGongCha,m_cutterSegs[i].m_lowGongCha);
			else
			    rePlaceText.Format(L"%%%%C%.3f{\\H0.7x;\\S+%.3f^%.3f;}",m_cutterSegs[i].m_diameter, m_cutterSegs[i].m_topGongCha,m_cutterSegs[i].m_lowGongCha);
		}
		else
		{
			if (m_cutterSegs[i].m_zhiJingJD == 1)
		        rePlaceText.Format(L"%%%%C%.1f{\\H0.7x;\\S+%.3f^+%.3f;}",m_cutterSegs[i].m_diameter, m_cutterSegs[i].m_topGongCha,m_cutterSegs[i].m_lowGongCha);
		    else if(m_cutterSegs[i].m_zhiJingJD == 2)
			    rePlaceText.Format(L"%%%%C%.2f{\\H0.7x;\\S+%.3f^+%.3f;}",m_cutterSegs[i].m_diameter, m_cutterSegs[i].m_topGongCha,m_cutterSegs[i].m_lowGongCha);
			else
			    rePlaceText.Format(L"%%%%C%.3f{\\H0.7x;\\S+%.3f^+%.3f;}",m_cutterSegs[i].m_diameter, m_cutterSegs[i].m_topGongCha,m_cutterSegs[i].m_lowGongCha);
		}
		
		CString temp;
		temp.Format(L"%%%%C%s", removeLastZero(m_cutterSegs[i].m_diameter));
		//直径标注解决
		id = CDimensionUtil::AddDimAligned(start, end, dim,temp);
#ifdef MIRROR
		CEntityUtil::Mirror(id, AcGePoint3d(offsetXY.x, offsetXY.y, 0), AcGeVector3d(0, 1, 0));
#endif
		if (m_cutterSegs[i].m_lengBianType != SEdgeType::E_EdgeType_不清边)
		{
			dim.y += 20;
			dim.x -=7;
		}
	}
	if (!IsFormE) {
		//顶角
		if (m_topAngle < 180)
		{
			double topRad = CMathUtil::AngleToRadian(m_topAngle / 2.0);
			double dx = 10;
			double dy = 10 * tan(topRad);
#ifdef MIRROR
			AcGePoint3d xline1Start = getMirrorPoint(AcGePoint3d(offsetXY.x, offsetXY.y, 0), AcGePoint3d(offsetXY.x, offsetXY.y, 0), AcGeVector3d(0, 1, 0));
			AcGePoint3d xline1End = getMirrorPoint(AcGePoint3d(offsetXY.x + dx, offsetXY.y - dy, 0), AcGePoint3d(offsetXY.x, offsetXY.y, 0), AcGeVector3d(0, 1, 0));
			AcGePoint3d xline2Start = getMirrorPoint(AcGePoint3d(offsetXY.x + dx, offsetXY.y + dy, 0), AcGePoint3d(offsetXY.x, offsetXY.y, 0), AcGeVector3d(0, 1, 0));
			AcGePoint3d xline2End = getMirrorPoint(AcGePoint3d(offsetXY.x - 15, offsetXY.y, 0), AcGePoint3d(offsetXY.x, offsetXY.y, 0), AcGeVector3d(0, 1, 0));
			MD2010_AddAngleDimension2(xline1Start, xline1End, xline2Start, xline2End, ACDB_MODEL_SPACE);
#else
			MD2010_AddAngleDimension2(AcGePoint3d(offsetXY.x, offsetXY.y, 0),
				AcGePoint3d(offsetXY.x + dx, offsetXY.y - dy, 0), AcGePoint3d(offsetXY.x + dx, offsetXY.y + dy, 0), AcGePoint3d(offsetXY.x - 15, offsetXY.y, 0), ACDB_MODEL_SPACE);
#endif
			//MD2010_AddAngleDimension(AcGePoint3d(0,0,0), AcGePoint3d (-5,-3,0),AcGePoint3d(-5,3,0), AcGePoint3d(-5,0,0));
		}
	}

	//标注各个倒角
	for (int i = 1; i < m_cutterSegs.size(); i++)
	{
		AcGePoint3d ladderUpStart, ladderUpEnd;
		AcGePoint3d ladderDownStart, ladderDownEnd;
		GetSegLadderAPnts(i, ladderUpStart, ladderUpEnd);
		ladderUpStart = GetOffsetPnt(ladderUpStart,offsetXY);
		ladderUpEnd = GetOffsetPnt(ladderUpEnd,offsetXY);
		if (IsFormE)
		{
			double delta_x = 0.0;
			if (m_topAngle < 180 && m_topAngle> 0)
			 delta_x = m_pointCenterDistance / tan(CMathUtil::AngleToRadian(m_topAngle / 2.0));
			ladderUpStart.x -= delta_x;
			ladderUpEnd.x -= delta_x;
		}
		ladderDownStart = GetMirrorPnt(ladderUpStart,offsetXY);
		ladderDownEnd = GetMirrorPnt(ladderUpEnd,offsetXY);

		AcGePoint3d arcPnt = ladderUpStart;
		arcPnt.x += 10;
		arcPnt.y = offsetXY.y;

		AcGePoint3d textPnt = arcPnt;
		textPnt.x -= 3;

		CString replaceText;
		if (m_cutterSegs[i].m_lengBianType == SEdgeType::E_EdgeType_不清边)
		{
			double angle = m_cutterSegs[i-1].m_ladderAngle1;
			replaceText.Format(L"%s°(仅倒角)",removeLastZero(angle));
			//replaceText.Format(L"%.1f°(仅倒角)", angle);
		}
		else
		{
			double angle = m_cutterSegs[i-1].m_ladderAngle1;
			replaceText.Format(L"%s°", removeLastZero(angle));
			//replaceText.Format(L"%.1f°", angle);
		}
#ifdef MIRROR
		AcGePoint3d xline1Start = getMirrorPoint(ladderUpStart, AcGePoint3d(offsetXY.x, offsetXY.y, 0), AcGeVector3d(0, 1, 0));
		AcGePoint3d xline1End = getMirrorPoint(ladderUpEnd, AcGePoint3d(offsetXY.x, offsetXY.y, 0), AcGeVector3d(0, 1, 0));
		AcGePoint3d xline2Start = getMirrorPoint(ladderDownStart, AcGePoint3d(offsetXY.x, offsetXY.y, 0), AcGeVector3d(0, 1, 0));
		AcGePoint3d xline2End = getMirrorPoint(ladderDownEnd, AcGePoint3d(offsetXY.x, offsetXY.y, 0), AcGeVector3d(0, 1, 0));
		AcGePoint3d dimPnt = getMirrorPoint(textPnt, AcGePoint3d(offsetXY.x, offsetXY.y, 0), AcGeVector3d(0, 1, 0));
		AcGePoint3d ArcPnt = getMirrorPoint(arcPnt, AcGePoint3d(offsetXY.x, offsetXY.y, 0), AcGeVector3d(0, 1, 0));
		MD2010_AddAngleDimension3(xline1Start, xline1End, xline2Start, xline2End,
			ArcPnt, dimPnt, replaceText, ACDB_MODEL_SPACE, L"2");
#else
	AcDbObjectId id = MD2010_AddAngleDimension3(ladderUpStart, ladderUpEnd, ladderDownStart, ladderDownEnd,
			arcPnt,textPnt, replaceText, ACDB_MODEL_SPACE, L"2");
	CEntityUtil::Mirror(id, AcGeLine3d(AcGePoint3d(offsetXY.x,offsetXY.y,0),AcGePoint3d(offsetXY.x,offsetXY.y+10,0)));
#endif
	}
	if (!IsFormE) {
		//钻头三尖标注
		if (m_cutterType == T_DRILL_CUTTER &&  m_daoJianType == E_DaoJian_三尖 && m_pointCenterDistance > USER_TOL * 100
			&& m_topAngle > USER_TOL * 100 && m_topAngle < 180)//处理中心距标注
		{
			AcGePoint3d start(offsetXY.x, offsetXY.y - m_pointCenterDistance / 2, 0);
			AcGePoint3d end(offsetXY.x, offsetXY.y + m_pointCenterDistance / 2, 0);
			AcGePoint3d dimpt(offsetXY.x - 3, offsetXY.y, 0);
			//CDimensionUtil::AddDimAligned(start, end, dimpt,L"",ACDB_MODEL_SPACE,L"尺寸",L"", PI/2);
			CString replace;
			//replace.Format(L"%%%%C%.1f{\\H0.7x;\\S-0.1^+0.1;}", m_pointCenterDistance);
			replace.Format(L"%%%%C%.1f%%%%P0.1", m_pointCenterDistance);
#ifdef MIRROR
			AcGePoint3d start_m = getMirrorPoint(start, AcGePoint3d(offsetXY.x, offsetXY.y, 0), AcGeVector3d(0, 1, 0));
			AcGePoint3d end_m = getMirrorPoint(end, AcGePoint3d(offsetXY.x, offsetXY.y, 0), AcGeVector3d(0, 1, 0));
			AcGePoint3d dimpt_m = getMirrorPoint(dimpt, AcGePoint3d(offsetXY.x, offsetXY.y, 0), AcGeVector3d(0, 1, 0));
			MD2010_AddAlignedDimension_GongCha2(start_m, end_m, dimpt_m, 0.1, -0.1, L"%%C", ACDB_MODEL_SPACE, L"2", replace, CMathUtil::PI / 2);
#else
			MD2010_AddAlignedDimension_GongCha2(start, end, dimpt, 0.1, -0.1, L"%%C", ACDB_MODEL_SPACE, L"2", replace, CMathUtil::PI / 2);
#endif
		}
	}
}


int CThreadData::CreateLengQue(AcGePoint2d offsetXY) const
{
	CString sFilePathName = CCommonUtil::GetAppPath();
	return 0;
}

int CThreadData::CheckLength(bool isMirror)
{
	double len = 0;
	for (int i = 0; i < m_cutterSegs.size(); i++)
	{
		len += m_cutterSegs[i].m_length;
	}

	if (isMirror)
	{
		if (len + USER_TOL*100 > m_totalLength)
			return 1;
	}
	else
	{
		if (len + USER_TOL * 100 > m_totalLength)
			return 1;
	}
	return 0;
}

void CThreadData::Draw(bool IsZC)
{
	CDocLock lock;
	
	//插入点
	AcDbObjectId id;
	AcGePoint3d ptInsert;
	CGetInputUtil::GetPoint(L"请选择一个插入点:", ptInsert);
	AcGePoint2d pInt;
	pInt.x = ptInsert.x;
	pInt.y = ptInsert.y;
	//刀柄插入点
	int ret = -1;
#ifdef MIRROR
	ptInsert.x = pInt.x - m_totalLength;
	ptInsert.y = pInt.y;
	CString filePath = TY_GetDaoBingZtFolder() + m_daobing + L".dwg";
	CString blkName = CCommonUtil::GenStrByTime();
	CBlockUtil::InsertDwgAsBlockRef(filePath, blkName, ACDB_MODEL_SPACE, ptInsert, 0, 1);
#else
	ptInsert.x = pInt.x + m_totalLength;
	ptInsert.y = pInt.y;
	CString filePath = TY_GetDaoBingZtFolder() + m_daobing + L".dwg";
	CString blkName = CCommonUtil::GenStrByTime();
	CBlockUtil::InsertDwgAsBlockRef(filePath, blkName, ACDB_MODEL_SPACE, ptInsert, 0, 1);
#endif // MIRROR

	if (IsZC)
	{ 
		ret = CreateModel3D_ZhiCao(pInt, id);
	}
	else
		ret = CreateModel3D(pInt, id);

}

void CThreadData::SetDaoBingName(const CString & DaoBingName)
{
	m_daobing = DaoBingName;
}

bool CThreadSegData::operator == (const CThreadSegData &other)
{
	return (m_bDoubleLadder == other.m_bDoubleLadder && m_bKaiRen == other.m_bKaiRen &&\
			CMathUtil::IsEqual(m_diameter, other.m_diameter) && CMathUtil::IsEqual(m_ladderAngle1, other.m_ladderAngle1) && CMathUtil::IsEqual(m_ladderAngle2, other.m_ladderAngle2) &&\
			CMathUtil::IsEqual(m_ladderLength1, other.m_ladderLength1) && m_lengBianType == other.m_lengBianType && CMathUtil::IsEqual(m_length, other.m_length) &&\
			m_lengthType == other.m_lengthType && CMathUtil::IsEqual(m_lowGongCha, other.m_lowGongCha) &&\
			CMathUtil::IsEqual(m_topGongCha, other.m_topGongCha) && m_zhiJingJD == other.m_zhiJingJD);
}

int TY_CreateHandleA(AcGePoint3d pnt, double dia, double len)
{
	double djxlen = 0.5;//倒角xlen
	if (djxlen > 20)
		djxlen = 1;

	AcGePoint3dArray pnts;

	pnts.append(AcGePoint3d(pnt.x, pnt.y - dia / 2, 0));
	pnts.append(AcGePoint3d(pnt.x, pnt.y + dia / 2, 0));
	pnts.append(AcGePoint3d(pnt.x + len - djxlen, pnt.y + dia / 2, 0));
	pnts.append(AcGePoint3d(pnt.x + len, pnt.y + dia / 2 - djxlen, 0));
	pnts.append(AcGePoint3d(pnt.x + len, pnt.y - dia / 2 + djxlen, 0));
	pnts.append(AcGePoint3d(pnt.x + len - djxlen, pnt.y - dia / 2, 0));
	pnts.append(AcGePoint3d(pnt.x, pnt.y - dia / 2, 0));


	CPolylineUtil::CreatePolyline(pnts);

	CLineUtil::CreateLine(AcGePoint3d(pnt.x + len - djxlen, pnt.y + dia / 2, 0), AcGePoint3d(pnt.x + len - djxlen, pnt.y - dia / 2, 0), 7);

	//x方向
	AcGePoint3d start(pnt.x, pnt.y, 0);
	AcGePoint3d end(pnt.x + len, pnt.y, 0);
	AcGePoint3d dim((start.x + end.x) / 2, pnt.y - dia, 0);
	CDimensionUtil::AddDimAligned(start, end, dim, L"");

	start = AcGePoint3d(pnt.x + len, pnt.y - dia / 2, 0);
	end = AcGePoint3d(pnt.x + len, pnt.y + dia / 2, 0);
	dim = AcGePoint3d(end.x + 15, (start.y + end.y) / 2, 0);
	//y方向
	/*double gc1 = 0, gc2 = 0;
	AN_GetGongCha(dia, gc1, gc2);
	CString rePlaceText;
	rePlaceText.Format(L"%%%%C%.1fh6({\\H0.7x;\\S 0^%.3f;})", dia, gc1, gc2);
	MD2010_AddAlignedDimension_GongCha(start, end, dim, gc1, gc2, L"%%C", ACDB_MODEL_SPACE, L"尺寸", rePlaceText, -PI / 2);
	*/
	return 0;
}