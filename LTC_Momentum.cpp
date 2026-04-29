#include "sierrachart.h"

SCDLLName("LTC Momentum")

SCSFExport scsf_LTC_Momentum(SCStudyInterfaceRef sc)
{
    SCSubgraphRef BarColor = sc.Subgraph[0];
    SCSubgraphRef LongBarColor = sc.Subgraph[1];
    SCSubgraphRef ShortBarColor = sc.Subgraph[2];
    SCSubgraphRef BarState = sc.Subgraph[3];
    SCSubgraphRef Line50 = sc.Subgraph[4];

    if (sc.SetDefaults)
    {
        sc.GraphName = "LTC Momentum";
        sc.AutoLoop = 1;
	sc.GraphRegion = 0;

        BarColor.Name = "Chop Bar Color";
        BarColor.DrawStyle = DRAWSTYLE_COLOR_BAR;
        BarColor.PrimaryColor = RGB(255,255,255); // Default White
        BarColor.DrawZeros = false;

        LongBarColor.Name = "Long Bar Color";
        LongBarColor.DrawStyle = DRAWSTYLE_COLOR_BAR;
        LongBarColor.PrimaryColor = RGB(0,255,0); // Green
        LongBarColor.DrawZeros = false;

        ShortBarColor.Name = "Short Bar Color";
        ShortBarColor.DrawStyle = DRAWSTYLE_COLOR_BAR;
        ShortBarColor.PrimaryColor = RGB(255,0,0); // Red
        ShortBarColor.DrawZeros = false;

        BarState.Name = "Bar State";
        BarState.DrawStyle = DRAWSTYLE_HIDDEN;
        BarState.PrimaryColor = RGB(255,255,255); // White
        BarState.DrawZeros = true;

        Line50.Name = "50 Line";
        Line50.DrawStyle = DRAWSTYLE_HIDDEN;
        Line50.PrimaryColor = RGB(255,255,255); // White
        Line50.DrawZeros = true;

        return;
    }

    // Need at least 1 previous bar
    if (sc.Index == 0)
        return;

    n_ACSIL::s_BarPeriod bp;
    sc.GetBarPeriodParameters(bp);
    bp.ChartDataType = INTRADAY_DATA;

    if(bp.IntradayChartBarPeriodType != IBPT_RANGE_IN_TICKS_TRUE) return;

    int i = sc.Index;

    float currentOpen = sc.Open[i];
    float currentHigh = sc.High[i];
    float currentLow = sc.Low[i];
    float currentClose = sc.Close[i];

    float prevClose = sc.Close[i - 1];
    float prevHigh = sc.High[i - 1];
    float prevLow = sc.Low[i - 1];

    // Midpoint of previous bar
    float prevMid = prevLow + (prevHigh - prevLow) * 0.5f;

    // Midpoint of current bar
    float currentMid = currentLow + (currentHigh - currentLow) * 0.5f;

    // Default color = White
    BarColor[i] = 1;
    BarColor.DataColor[i] = BarColor.PrimaryColor;
    BarState[i] = 0; // Chop
    Line50[i] = 0; // 50% line, true/false

    // Green condition
    if (currentOpen > prevClose && currentLow >= prevMid)
    {
        BarColor.DataColor[i] = LongBarColor.PrimaryColor;
        BarState[i] = 1; // Long
	if (prevClose > prevMid) Line50[i] = 1; // Buy it
    }
    // Red condition
    else if (currentOpen < prevClose && currentHigh <= prevMid)
    {
        BarColor.DataColor[i] = ShortBarColor.PrimaryColor;
        BarState[i] = -1; // Short
	if (prevClose < prevMid) Line50[i] = -1; // Sell it
    }
}
