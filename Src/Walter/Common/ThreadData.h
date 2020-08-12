#pragma once
#include <vector>
#include <ocdbid.h>
using namespace std;


//#define  AHNO_DRAW_3D

enum E_CutterType//��������
{
	T_DRILL_CUTTER = 0, //��ͷ
	T_MILLING_CUTTER,	//ϳ��
	T_REAMER_CUTTER,	//�µ�f
	T_PROFILE_CUTTER,	//���͵�
	T_DRILL_MILLING_CUTTER, //��Ӳϳ��
};
CString ToCString(E_CutterType p_type);
E_CutterType CutterTypeFromString(CString str);

enum E_CutterSegLengthType
{
	E_CutterSegLength_���⵽���� = 0,
	E_CutterSegLength_���⵽Բ��,//���ֺ��濼��ȥ��
	E_CutterSegLength_Բ��������,
	E_CutterSegLength_Բ����Բ��,
};
const CString c_sSegLenghtTypes[4] = { _T("��⵽����"), _T("��⵽����"), _T("���⵽����"), _T("���⵽����") };
CString ToCString(E_CutterSegLengthType p_type);
E_CutterSegLengthType ToSegLengthType(CString p_str);


typedef enum E_EdgeType
{
	E_EdgeType_����� = 0,
	E_EdgeType_˫���,
	E_EdgeType_˫��߱�����,
	E_EdgeType_�����
}SEdgeType;
const CString c_sEdgeType[4] = { _T("�����"), _T("˫���"), _T("˫��߱�����"),_T("�����") };
CString ToCString(SEdgeType p_type);
SEdgeType ToEdgeType(CString p_str);


enum E_DaoJian_Type
{
	E_DaoJian_ƽ��,
	E_DaoJian_����,
	E_DaoJian_����R,
	E_DaoJian_��ͷ,
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
}SDXY;//ÿһ���ߣ�б�ߣ�ˮƽ����ֱ����Ӧһ���ýṹ
typedef std::vector<SDXY> vSDXY;



struct CThreadSegData //�������߽׶εĲ�������
{
public:
	CThreadSegData();

	double m_diameter; //�о� ---��Ҫ����01
	double m_length; //����  ---��Ҫ����02

	E_CutterSegLengthType m_lengthType;

	bool m_bDoubleLadder;//�Ƿ�˫���� 
	double m_ladderAngle1; //���ݽ�   ---��Ҫ����03
	double m_ladderAngle2; //���ݽ�
	double m_ladderLength1;//��һ���ݳ��� �����һ���ݽǶ��ǵ���90�ȣ���ô�����������ǵڶ����ݵĳ���

	SEdgeType m_lengBianType;//�������
	double m_topGongCha;//�Ϲ��� ֱ�� --- 01����ͼ
	double m_lowGongCha;//�¹��� ֱ�� ---- 02 ����ͼ
	bool m_bKaiRen;//ϳ���µ�ר�� �Ƿ��У���������
	int m_zhiJingJD;//ֱ����עС���㳤��

	bool valid()const
	{
		if (m_diameter < 0 || m_length < 0 || m_ladderAngle1 < 0 || m_ladderAngle2 < 0 || m_ladderLength1 < 0
			|| m_ladderAngle1>180 || m_ladderAngle2 > 180)
			return false;
		return true;
	}
	bool operator == (const CThreadSegData &other);
};

//---�����Ҳ�Ǹ��ĸ����߹���---20190409
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
	

	//���������ݣ� �����ǰ���������ݽǵ� ladderA.dx ladderA.dy, ladderB.dx, ladderB.dy
	//ͬʱ�����жϸ����Ƿ�ȷʵ���������ݽ�isTwo(Ʃ��ǰ�������Ƕ�һ����ô����һ�����ݽ�)
	static int GetJieTiTwoDxy(const CThreadSegData &preSeg, const CThreadSegData &thisSeg, bool &isTwo, SDXY &ladderA, SDXY &ladderB);

	//�����������������ε�ˮƽ ���ߵĳ���(x����)
	//thisSeg ����
	//nextSeg��һ��
	//preLadderABxLen--�Ǳ���ǰ���������ݽ� �����Ѿ���ȥ�ĳ���
	static double CThreadData::GetHorizontalLengthByType(const CThreadSegData &thisSeg, const CThreadSegData &nextSeg, double preLadderABxLen);
	//�ѵ�ǰ������е����� ת���� һ��һ���� dx,dy ����
	static int ConvertToDxy(double topAngle, //����
		                    const vector<CThreadSegData> &cutterSegs,//����
							double totalLength,//���� �����������һ��ˮƽ�߳���
							vSDXY &dxys//���
							);

	static int ConvertToDxyCXD(double topAngle, //����
		const vector<CThreadSegData> &cutterSegs,//����
		double &xLen,//����õ������е�x������еĳ���
		vSDXY &dxys//���
		);

	AcDbPolyline * CreatePolyline2d(AcGePoint2d offsetXY, vSDXY &dxys, bool closed = true)const;
	//����������--����������ת�Ƕ�
	int CreateHelix(AcGePoint2d offsetXY, //ƫ�Ƶĵ�
	                       double firstRad, //��ʼ�İ뾶
						   double angle,//��ʼ��Ƕȣ���Y�����ʱ��Ƕ� ������
						   double totalHeight,//�ܵĸ߶�
						   AcDbObjectId &id
						   ) const;

	//ֱ�Ӹ������
	int CreateHelix(AcGePoint2d offsetXY, //ƫ�Ƶĵ�
		double firstRad, //��ʼ�İ뾶
		AcGePoint3d startPnt,//���λ��
		double totalHeight,//�ܵĸ߶�
		double singleH,
		AcDbObjectId &id
		)const;

	//ֱ�Ӹ������ ������ʼ�����뾶
	int CreateHelix(AcGePoint2d offsetXY, //ƫ�Ƶĵ�
		double firstRad, //��ʼ�İ뾶
		double endRad, //�����İ뾶
		AcGePoint3d startPnt,//���λ��
		double totalHeight,//�ܵĸ߶�
		double singleH,
		AcDbObjectId &id
		)const;
	int CreateModel3D(AcGePoint2d offsetXY,AcDbObjectId &id) const;
	int CreateModel3D_ZhiCao(AcGePoint2d offsetXY, AcDbObjectId &mainid) const;
	void Test_Data();

	double  GetDia(const double dia, const int edgeCount) const;

	void SetHelixClockwise(bool clockWise) {m_isClockwise = clockWise;}

	//���ݲ�ͬ������ ����һ��ָ���� ���������ߵ� ��ת�Ƕȣ���Ϊ���������߲�Ҫ��0������
	double GetHelixStartAngle(int edgeCount) const;

	//�������
	//saoMiaoCirRad ��Ϊһ���黨��ֻ��һ���̶���ɨ���Բ�� �����ü�����
	//������뾶���͹��� ��Ҫ��Ϊ�� ���һ�μ���ʣ��ĳ��ȣ���Ϊ���һ�β�����ߵ����
	//����ֱ�۵����Ը�0
	//isMaHua �ǲ����黨��

	//saoMiaoCirRad����ֱ������˵���������Ҳ��������ģ����������һ��̧��ĳ��ȣ�������ߵ�ʱ��̧�����ﲻ�������
	int CreateLengBians(AcDbObjectId mainid, AcGePoint2d offsetXY, double saoMiaoCirRad, bool isMaHua) const;
	int CreateLengBianForOneSeg(AcDbObjectId mainId, std::vector<AcGeCurve3d*> &edges, AcGePoint2d offsetXY, double xvalue, double rad, double len, SEdgeType edgeType, bool isMaHua) const;
	
	//Ϊ���ߵĵڼ����ҵ�Բ����ʼ���Ǹ�Բ����xֵ
	int GetXValueForOneCutterSeg(int index, double &xValue) const;
	//Ϊ���ߵĵڼ����ҵ����⿪ʼ���Ǹ�Բ����xֵ
	int GetXValueForOneCutterSeg_2(int index, double &xValue) const;

	//Ϊ�����ҵ��ڼ��ε��Ǹ�X����ĳ���
	int GetXLengthForOneCutterSeg(int index, double &len) const;
	//Ϊ�����ҵ��ڼ��ε��Ǹ�Y����ĳ���
	int GetYLengthForOneCutterSeg(int index, double &len) const;
	
	//�õ���index�����ݵ� ��һ���ݵ������յ�
	int GetSegLadderAPnts(int index, AcGePoint3d &start, AcGePoint3d &end) const;
	int GetSegLadderBPnts(int index, AcGePoint3d &start, AcGePoint3d &end) const;

	int CreateLengBianForOneArc(AcDbObjectId mainId, AcGeCircArc3d&arc,AcGePoint2d offsetXY, double len, SEdgeType edgeType, bool isMaHua) const;
	int CreateRegionForOneArc(AcGeCircArc3d&arc, AcGePoint2d offsetXY,  SEdgeType edgeType,
		AcGePoint3d &mid, AcDbObjectId &regionId, 
		bool second = false//����˫��߱����� �ڶ�����
		) const;

	int CreateRegionForOneArc_ZhiCao(AcGeCircArc3d&arc, AcGePoint2d offsetXY,  SEdgeType edgeType, 
		AcGePoint3d &mid, AcDbObjectId &regionId, bool second) const;

	//farestPnt---ֱ�۵��Ĳ۵���Զ��
    int CreateDims(AcGePoint2d offsetXY,AcGePoint3d farestPnt) const;
	int CreateLengQue(AcGePoint2d offsetXY) const;//�����

	//���� ��Ҫ�Ǵ�����R �͵��⵹��
	int PostProcessAngleHead(vSDXY &dxys) const;


	AcGePoint3d GetOffsetPnt(AcGePoint3d pnt, AcGePoint2d offsetXY) const;
	//�����pnt��ʵ�ʵ�offset���pnt
	AcGePoint3d GetMirrorPnt(AcGePoint3d pnt, AcGePoint2d offsetXY) const;
	   
	int CheckLength(bool isMirror);//���ȼ��

	bool operator == (const CThreadData &other);

	void Draw(bool IsZC);
	void SetDaoBingName(const CString& DaoBingName);
public:
	//����λ��
	CString m_daobing;
	//��м�۳�
	double m_GrooveLength;
	
	vector<CThreadSegData> m_cutterSegs;

	//20190526��ʱ
	enum E_CutterType m_cutterType;		// �������� Ĭ����ͷ
	E_DaoJian_Type m_daoJianType;//�������� Ĭ��NULL

	//��ͷ����
	CString m_sEdgeType;		//���---Ĭ�ϵ���߲��ø�����

	Adesk::Int32 m_cuttingEdgeCount;		//�������� Ĭ��2���ø�

	double m_topAngle; //���� Ĭ��135

	double m_handleDiameter; //���� Ĭ��20
	double m_handleLength; //����  Ĭ��50

	double m_totalLength; //�ܳ� Ĭ��160

	double m_pointCenterDistance; //�������ľ࣬ ֻ�������ĵ���������Ч  Ĭ��
	double m_pointR; //�����Rֵ��ֻ��Ե�������Ϊ����R����Ч  Ĭ��1
	bool m_isClockwise;//��������ʱ��˳ʱ��  Ĭ��true
	double m_luoXuanJiaoDu;//7�� �� 20 �� 7�ȸ�Ϊ20��

	//20190608���� ���Ϳ�������������
	AcDbObjectId m_tuKuangId;

	//20190713���� ���ɴ�len
	double m_guoDuDaiX;//���ɴ� ������ǰ�浶��֮���б���ȿ�Ⱥ͸߶� 45��

	int m_jingDu;//��ע�ľ��� Ĭ��3

	Adesk::Int32 m_nLadderCount; //���� ��ʱ���õ�����
};


int TY_CreateHandleA(AcGePoint3d pnt, double dia, double len);

