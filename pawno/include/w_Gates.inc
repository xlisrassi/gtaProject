#include <a_samp>

#if defined _w_Gates_included
	#endinput
#endif
#define _w_Gates_included
#pragma library w_Gates

new lsgate,sfgate1,sfgate2,lvgate1,lvgate2;
stock AddLSPDGate()
{
    lsgate = CreateObject(971,1589.022,-1638.016,14.950,0.0,0.0,-180.000);
}

stock OpenLSPDGate(Float:Speed)
{
    MoveObject(lsgate,1596.933,-1638.044,14.924,Float:Speed);
}

stock CloseLSPDGate(Float:Speed)
{
    MoveObject(lsgate,1589.022,-1638.016,14.950,Float:Speed);
}


stock AddSFPDGate()
{
    sfgate1 = CreateObject(986,-1627.624,688.599,7.898,0.0,0.0,-180.000);
    sfgate2 = CreateObject(985,-1635.789,688.605,7.898,0.0,0.0,-180.000);
}

stock OpenSFPDGate(Float:Speed)
{
    MoveObject(sfgate1,-1619.688,688.579,7.901,Float:Speed);
    MoveObject(sfgate2,-1643.742,688.616,7.867,Float:Speed);
}

stock CloseSFPDGate(Float:Speed)
{
    MoveObject(sfgate1,-1627.624,688.599,7.898,Float:Speed);
    MoveObject(sfgate2,-1635.789,688.605,7.898,Float:Speed);
}


stock AddLVPDGate()
{
	lvgate1 = CreateObject(971,2293.811,2499.461,5.817,0.0,0.0,-90.000);
    lvgate2 = CreateObject(971,2334.822,2443.358,8.180,0.0,0.0,59.688);
}

stock OpenLVPDGate(Float:Speed)
{
    MoveObject(lvgate1,2293.888,2499.231,10.711,Float:Speed);
    MoveObject(lvgate2,2339.147,2450.821,8.166,Float:Speed);
}

stock CloseLVPDGate(Float:Speed)
{
    MoveObject(lvgate1,2293.811,2499.461,5.817,Float:Speed);
    MoveObject(lvgate2,2334.822,2443.358,8.180,Float:Speed);
}