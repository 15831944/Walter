#pragma once

using namespace std;

#include <dbcurve.h>
#include <vector>

//#define  SIMPLE_PLOT

typedef std::vector<AcDbCurve *> vAcDbCurvePointer;

namespace TYPlot
{
	void SetTyPlotConfigFolder(CString p_sFolder);

	//代表识别出来的一个标题框
	typedef struct TYTitle
	{
		AcGePoint3d m_lb; //left bottom point
		AcGePoint3d m_rt;//right top point

		CString m_layout;//模型或者图纸空间
		CString m_paperName;//具体的图纸空间的名字
	}STYTitle;

	typedef std::vector<STYTitle> vSTYTitle;

/*-----------------------------识别部分---------------------------*/
    //读取当前打开的database的模型空间的所有的图框
    int TYPLOT_GetTitlesForModelSpace
        (  
            vSTYTitle &titles, //输出-图框的信息
            bool blockMethod = true, //输入-是否识别图块内的关键字
            bool keyWordMethod = true, //是否单独识别关键字
            bool bLearn = false, //是否根据学习到的内容进行识别
            bool bDraw = false//是否根据学习到的内容进行识别
        );//Model Space

#ifndef SIMPLE_PLOT
     //读取当前打开的database的图纸空间的所有的图框
    int TYPLOT_GetTitlesForPaperSpace
        (
            vSTYTitle &titles, //输出-图框的信息
            CString paperName = _T(""),  //如果为空识别所有的图纸空间，如果非空只识别给出的图纸空间
            bool blockMethod = true, //输入-是否识别图块内的关键字
            bool keyWordMethod = true, //是否单独识别关键字
            bool bLearn = false, //是否根据学习到的内容进行识别
            bool bDraw = false//是否根据学习到的内容进行识别
         );


/*----------------------------打印部分---------------------------------------------*/
	//通过pdffactory打印机打印图框
	int TYPLOT_PrinterWithPdfFactory
		(
		CString pdfFileName,//pdf文件名称
		vSTYTitle &modelTitles, //模型空间内要打印的图框
		vSTYTitle &paperTitles,//图纸空间内要打印的图框
		CString paperStr = L"A4", //paperStr @A0 @A1 @A2 @A3 @A4 A0 A1 A2 A3 A4 自定义
		double height = 0,//如果paperStr是自定义 这个参数有意义 表示打印的高度
		double width = 0//如果paperStr是自定义 这个参数有意义表示打印的宽度	
		);

	//通过cad自带打印机打印图框
	void TYPLOT_PrinterWithCADPrinter
		(
			CString pdfFolder, //文件夹路径 因为cad自带打印机是一张一张输出的pdf 最后需要进行合并
			CString pdfFileName,//pdf文件路径名称（完成路径）
			vSTYTitle &modelTitles, //模型空间内要打印的图框
			vSTYTitle &paperTitles,//图纸空间内要打印的图框
			int outputMode=0,//输出模式，1表示输出一个整体pdf，2表示输出单张按图框文件，3表示同时输出整体的pdf和按图框的pdf
			CString paperStr = L"A4", //paperStr @A0 @A1 @A2 @A3 @A4 A0 A1 A2 A3 A4 自定义
			double height = 0,//如果paperStr是自定义 这个参数有意义 表示打印的高度
			double width = 0//如果paperStr是自定义 这个参数有意义表示打印的宽度
		);

	void CurDocumentModelSpacePrinterToPdf
		(
			CString pdfFolder, //文件夹路径 因为cad自带打印机是一张一张输出的pdf 最后需要进行合并
			CString pdfFileName,//pdf文件路径名称（完成路径）
			int outputMode = 0,//输出模式，1表示输出一个整体pdf，2表示输出单张按图框文件，3表示同时输出整体的pdf和按图框的pdf
			CString paperStr = L"A4", //paperStr @A0 @A1 @A2 @A3 @A4 A0 A1 A2 A3 A4 自定义
			double height = 0,//如果paperStr是自定义 这个参数有意义 表示打印的高度
			double width = 0//如果paperStr是自定义 这个参数有意义表示打印的宽度
			);

	int TYPLOT_PrinterWithRealPrinter
	(
		CString realPrinterName,//实体打印机名称
		vSTYTitle &modelTitles, //模型空间内要打印的图框
		vSTYTitle &paperTitles,//图纸空间内要打印的图框
		CString paperStr = L"A4", //paperStr @A0 @A1 @A2 @A3 @A4 A0 A1 A2 A3 A4 自定义
		double height = 0,//如果paperStr是自定义 这个参数有意义 表示打印的高度
		double width = 0 //如果paperStr是自定义 这个参数有意义表示打印的宽度	
	);

#endif

/*----------------------------其他相关内容---------------------------------------------*/
	 //把当前图纸的一批对象导出到一个单独的dwg文件
	int TYPLOT_ExportDwg(AcDbObjectIdArray& outObjIds, CString filename);
	//把当前图纸的识别出来的某个图框的所有内容 导出为一个dwg图纸
	int TYPLOT_ExportAllObjectsInTitle(STYTitle title, CString filename);

	//数学计算-给出一个种子点，找出从这个种子点出发的一圈线 包含多断线
	 //用法示例
	/*AcGePoint3d seedOri = seedPoint;
	int findIndex = 0;//从0开始找起来
	vAcDbCurveP loop;
	TYPlot::TYJGCOM_LoopCountCurve(cuves, seedOri, seedPoint, findIndex, loop);的判断*/
	int TYJGCOM_LoopCountCurve
		(
		vAcDbCurvePointer &pCurves, //所有的待搜索的曲线
		AcGePoint3d &seedOri, //种子点 一般是某个曲线的起点或者终点
		AcGePoint3d seedPnt, //中间种子点 使用时同seedOri
		int &findIndex, //定义变量 初始化为0
		vAcDbCurvePointer &loop//输出的结果
		);    
}

