#pragma once
#include <vector>
#include <ocdbid.h>
using namespace std;


//#define  AHNO_DRAW_3D

enum E_CutterType//刀具类型
{
	T_DRILL_CUTTER = 0, //钻头
	T_MILLING_CUTTER,	//铣刀
	T_REAMER_CUTTER,	//铰刀f
	T_PROFILE_CUTTER,	//成型刀
	T_DRILL_MILLING_CUTTER, //整硬铣刀
};
CString ToCString(E_CutterType p_type);
E_CutterType CutterTypeFromString(CString str);

enum E_CutterSegLengthType
{
	E_CutterSegLength_刀尖到刀尖 = 0,
	E_CutterSegLength_刀尖到圆柱,//这种后面考虑去掉
	E_CutterSegLength_圆柱到刀尖,
	E_CutterSegLength_圆柱到圆柱,
};
const CString c_sSegLenghtTypes[4] = { _T("钻尖到根部"), _T("钻尖到刀尖"), _T("刀尖到根部"), _T("刀尖到刀尖") };
CString ToCString(E_CutterSegLengthType p_type);
E_CutterSegLengthType ToSegLengthType(CString p_str);


typedef enum E_EdgeType
{
	E_EdgeType_单棱边 = 0,
	E_EdgeType_双棱边,
	E_EdgeType_双棱边背两侧,
	E_EdgeType_不清边
}SEdgeType;
const CString c_sEdgeType[4] = { _T("单棱边"), _T("双棱边"), _T("双棱边背两侧"),_T("不清边") };
CString ToCString(SEdgeType p_type);
SEdgeType ToEdgeType(CString p_str);


enum E_DaoJian_Type
{
	E_DaoJian_平底,
	E_DaoJian_三尖,
	E_DaoJian_刀尖R,
	E_DaoJian_球头,
	E_DaoJian_NULL,
};

CString ToCString(E_DaoJian_Type p_type);
E_DaoJian_Type FromString(CString str );

typedef struct DXY
{
	DXY()
	{
		dx = 0;
		dy = 0;
		buglet = 0;
	}

	DXY(double x, double y)
	{
		dx = x;
		dy = y;
		buglet = 0;
	}

	DXY(double x, double y, double bu)
	{
		dx = x;
		dy = y;
		buglet = bu;
	}
	double dx;//delta x
	double dy;//delta y
	double buglet;
}SDXY;//每一条线（斜线，水平，竖直）对应一个该结构
typedef std::vector<SDXY> vSDXY;



struct CThreadSegData //单个刀具阶段的参数数据
{
public:
	CThreadSegData();

	double m_diameter; //刃径 ---需要设置01
	double m_length; //长度  ---需要设置02

	E_CutterSegLengthType m_lengthType;

	bool m_bDoubleLadder;//是否双阶梯 
	double m_ladderAngle1; //阶梯角   ---需要设置03
	double m_ladderAngle2; //阶梯角
	double m_ladderLength1;//第一阶梯长度 如果第一阶梯角度是单面90度，那么这个输入的算是第二阶梯的长度

	SEdgeType m_lengBianType;//棱边类型
	double m_topGongCha;//上公差 直径 --- 01工艺图
	double m_lowGongCha;//下公差 直径 ---- 02 工艺图
	bool m_bKaiRen;//铣刀铰刀专用 是否开刃（单棱边与否）
	int m_zhiJingJD;//直径标注小数点长度

	bool valid()const
	{
		if (m_diameter < 0 || m_length < 0 || m_ladderAngle1 < 0 || m_ladderAngle2 < 0 || m_ladderLength1 < 0
			|| m_ladderAngle1>180 || m_ladderAngle2 > 180)
			return false;
		return true;
	}
	bool operator == (const CThreadSegData &other);
};

//---这个类也是给四个刀具公用---20190409
class CThreadData
{
public:
	CThreadData();
	~CThreadData(){}

	bool SetLadderCount(int p_count);
	int GetLadderCount()const {return m_nLadderCount;}

	UINT GetCutterSegCount()const {return (UINT)(m_cutterSegs.size());}
	const CThreadSegData* GetCutterSegData(int p_index) const;
	bool SetCutterSegData(CThreadSegData& p_segData, int p_index);

	void AddCutterSegData(CThreadSegData &data){m_cutterSegs.push_back(data);}
	void ClearCutterSegs() { m_cutterSegs.clear(); m_cutterSegs.shrink_to_fit(); }
	

	//给两个阶梯， 计算出前后两个阶梯角的 ladderA.dx ladderA.dy, ladderB.dx, ladderB.dy
	//同时根据判断给出是否确实是两个阶梯角isTwo(譬如前后两个角度一样那么还是一个阶梯角)
	static int GetJieTiTwoDxy(const CThreadSegData &preSeg, const CThreadSegData &thisSeg, bool &isTwo, SDXY &ladderA, SDXY &ladderB);

	//这个函数用来计算这段的水平 长线的长度(x方向)
	//thisSeg 本段
	//nextSeg下一段
	//preLadderABxLen--是本段前端两个阶梯角 可能已经用去的长度
	static double CThreadData::GetHorizontalLengthByType(const CThreadSegData &thisSeg, const CThreadSegData &nextSeg, double preLadderABxLen);
	//把当前类的所有的数据 转换成 一个一个的 dx,dy 数组
	static int ConvertToDxy(double topAngle, //输入
		                    const vector<CThreadSegData> &cutterSegs,//输入
							double totalLength,//输入 辅助计算最后一段水平线长度
							vSDXY &dxys//输出
							);

	static int ConvertToDxyCXD(double topAngle, //输入
		const vector<CThreadSegData> &cutterSegs,//输入
		double &xLen,//计算得到的所有的x方向的刃的长度
		vSDXY &dxys//输出
		);

	AcDbPolyline * CreatePolyline2d(AcGePoint2d offsetXY, vSDXY &dxys, bool closed = true)const;
	//创建螺旋线--给定起点的旋转角度
	int CreateHelix(AcGePoint2d offsetXY, //偏移的点
	                       double firstRad, //开始的半径
						   double angle,//起始点角度，和Y轴的逆时针角度 弧度制
						   double totalHeight,//总的高度
						   AcDbObjectId &id
						   ) const;

	//直接给定起点
	int CreateHelix(AcGePoint2d offsetXY, //偏移的点
		double firstRad, //开始的半径
		AcGePoint3d startPnt,//起点位置
		double totalHeight,//总的高度
		double singleH,
		AcDbObjectId &id
		)const;

	//直接给定起点 给定开始结束半径
	int CreateHelix(AcGePoint2d offsetXY, //偏移的点
		double firstRad, //开始的半径
		double endRad, //结束的半径
		AcGePoint3d startPnt,//起点位置
		double totalHeight,//总的高度
		double singleH,
		AcDbObjectId &id
		)const;
	int CreateModel3D(AcGePoint2d offsetXY,AcDbObjectId &id) const;
	int CreateModel3D_ZhiCao(AcGePoint2d offsetXY, AcDbObjectId &mainid) const;
	void Test_Data();

	double  GetDia(const double dia, const int edgeCount) const;

	void SetHelixClockwise(bool clockWise) {m_isClockwise = clockWise;}

	//根据不同的刃数 给出一个指导的 创建螺旋线的 旋转角度，因为尽量螺旋线不要在0度切了
	double GetHelixStartAngle(int edgeCount) const;

	//创建棱边
	//saoMiaoCirRad 因为一个麻花钻只有一个固定的扫描的圆柱 用来裁剪主体
	//把这个半径传送过来 主要是为了 最后一段计算剩余的长度，因为最后一段不能棱边到最后
	//对于直槽刀可以给0
	//isMaHua 是不是麻花钻

	//saoMiaoCirRad对于直槽钻来说，这个参数也是有意义的，他代表最后一段抬起的长度，生成棱边的时候抬起那里不能有棱边
	int CreateLengBians(AcDbObjectId mainid, AcGePoint2d offsetXY, double saoMiaoCirRad, bool isMaHua) const;
	int CreateLengBianForOneSeg(AcDbObjectId mainId, std::vector<AcGeCurve3d*> &edges, AcGePoint2d offsetXY, double xvalue, double rad, double len, SEdgeType edgeType, bool isMaHua) const;
	
	//为刀具的第几段找到圆柱开始的那个圆弧的x值
	int GetXValueForOneCutterSeg(int index, double &xValue) const;
	//为刀具的第几段找到刀尖开始的那个圆弧的x值
	int GetXValueForOneCutterSeg_2(int index, double &xValue) const;

	//为刀具找到第几段的那个X方向的长度
	int GetXLengthForOneCutterSeg(int index, double &len) const;
	//为刀具找到第几段的那个Y方向的长度
	int GetYLengthForOneCutterSeg(int index, double &len) const;
	
	//得到第index个阶梯的 第一阶梯的起点和终点
	int GetSegLadderAPnts(int index, AcGePoint3d &start, AcGePoint3d &end) const;
	int GetSegLadderBPnts(int index, AcGePoint3d &start, AcGePoint3d &end) const;

	int CreateLengBianForOneArc(AcDbObjectId mainId, AcGeCircArc3d&arc,AcGePoint2d offsetXY, double len, SEdgeType edgeType, bool isMaHua) const;
	int CreateRegionForOneArc(AcGeCircArc3d&arc, AcGePoint2d offsetXY,  SEdgeType edgeType,
		AcGePoint3d &mid, AcDbObjectId &regionId, 
		bool second = false//对于双棱边背两侧 第二个边
		) const;

	int CreateRegionForOneArc_ZhiCao(AcGeCircArc3d&arc, AcGePoint2d offsetXY,  SEdgeType edgeType, 
		AcGePoint3d &mid, AcDbObjectId &regionId, bool second) const;

	//farestPnt---直槽刀的槽的最远点
    int CreateDims(AcGePoint2d offsetXY,AcGePoint3d farestPnt) const;
	int CreateLengQue(AcGePoint2d offsetXY) const;//内冷孔

	//后处理 主要是处理刀尖R 和刀尖倒角
	int PostProcessAngleHead(vSDXY &dxys) const;


	AcGePoint3d GetOffsetPnt(AcGePoint3d pnt, AcGePoint2d offsetXY) const;
	//传入的pnt是实际的offset后的pnt
	AcGePoint3d GetMirrorPnt(AcGePoint3d pnt, AcGePoint2d offsetXY) const;
	   
	int CheckLength(bool isMirror);//长度检测

	bool operator == (const CThreadData &other);

	void Draw(bool IsZC);
	void SetDaoBingName(const CString& DaoBingName);
public:
	//刀柄位置
	CString m_daobing;
	//排屑槽长
	double m_GrooveLength;
	
	vector<CThreadSegData> m_cutterSegs;

	//20190526临时
	enum E_CutterType m_cutterType;		// 刀具类型 默认钻头
	E_DaoJian_Type m_daoJianType;//刀尖类型 默认NULL

	//钻头独有
	CString m_sEdgeType;		//棱边---默认单棱边不用给数据

	Adesk::Int32 m_cuttingEdgeCount;		//刀刃数量 默认2不用给

	double m_topAngle; //顶角 默认135

	double m_handleDiameter; //柄径 默认20
	double m_handleLength; //柄长  默认50

	double m_totalLength; //总长 默认160

	double m_pointCenterDistance; //刀尖中心距， 只针对三尖的刀尖类型有效  默认
	double m_pointR; //刀尖的R值，只针对刀尖类型为刀尖R的有效  默认1
	bool m_isClockwise;//螺旋线逆时针顺时针  默认true
	double m_luoXuanJiaoDu;//7度 和 20 度 7度改为20度

	//20190608增加 解决涂层多行文字问题
	AcDbObjectId m_tuKuangId;

	//20190713补充 过渡带len
	double m_guoDuDaiX;//过渡带 刀柄和前面刀具之间的斜过度宽度和高度 45度

	int m_jingDu;//标注的精度 默认3

	Adesk::Int32 m_nLadderCount; //阶数 暂时不用的数据
};


int TY_CreateHandleA(AcGePoint3d pnt, double dia, double len);

