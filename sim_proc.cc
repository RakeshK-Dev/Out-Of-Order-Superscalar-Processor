#include <iostream>
#include <stdlib.h>
//#include <stdio.h>
#include <string.h>
#include <math.h>
#include <fstream>
#include <vector>
//#include <inttypes.h>
#include <iomanip>
#include <algorithm>
#include "sim_proc.h"

using namespace std;

/*  argc holds the number of command line arguments
    argv[] holds the commands themselves

    Example:-
    sim 256 32 4 gcc_trace.txt
    argc = 5
    argv[0] = "sim"
    argv[1] = "256"
    argv[2] = "32"
    ... and so on
*/

bool DE_empty = true, RN_empty = true , RR_empty = true, DI_empty = true;// IQ_empty = true, EX_empty = true, WB_empty = true, RT_empty = true; 
//int DE_size = 0, RN_size = 0, RR_size = 0, DI_size = 0;//, IQ_size = 0;
//int DE_cnt = 0, RN_cnt = 0, RR_cnt = 0, DI_cnt = 0;// IS_cnt = 0;
bool Pipeline_empty = false;
int Instr_cnt = 0, gb_cnt = 0;//, cnt = 0;

void Reg_copy ( Reg_const src_reg, Reg_const dst_reg_reg, int size)
{
    //cout << "Reg_copy entry" << endl ;
    for (int i = 0; i < size; i++)
    {
		dst_reg_reg.reg[i].FE_en = src_reg.reg[i].FE_en;
        dst_reg_reg.reg[i].FE_dur = src_reg.reg[i].FE_dur;
        dst_reg_reg.reg[i].DE_en = src_reg.reg[i].DE_en;
        dst_reg_reg.reg[i].DE_dur = src_reg.reg[i].DE_dur;
        dst_reg_reg.reg[i].RN_en = src_reg.reg[i].RN_en;
        dst_reg_reg.reg[i].RN_dur = src_reg.reg[i].RN_dur;
        dst_reg_reg.reg[i].RR_en = src_reg.reg[i].RR_en;
        dst_reg_reg.reg[i].RR_dur = src_reg.reg[i].RR_dur;
        dst_reg_reg.reg[i].DI_en = src_reg.reg[i].DI_en;
        dst_reg_reg.reg[i].DI_dur = src_reg.reg[i].DI_dur;
        dst_reg_reg.reg[i].IS_en = src_reg.reg[i].IS_en;
        dst_reg_reg.reg[i].IS_dur = src_reg.reg[i].IS_dur;
        dst_reg_reg.reg[i].EX_en = src_reg.reg[i].EX_en;
        dst_reg_reg.reg[i].EX_dur = src_reg.reg[i].EX_dur;
        dst_reg_reg.reg[i].WB_en = src_reg.reg[i].WB_en;
        dst_reg_reg.reg[i].WB_dur = src_reg.reg[i].WB_dur;
        dst_reg_reg.reg[i].RT_en = src_reg.reg[i].RT_en;
        dst_reg_reg.reg[i].RT_dur = src_reg.reg[i].RT_dur;
        dst_reg_reg.reg[i].addr = src_reg.reg[i].addr;
        dst_reg_reg.reg[i].op = src_reg.reg[i].op;
        dst_reg_reg.reg[i].dst_reg = src_reg.reg[i].dst_reg;
        dst_reg_reg.reg[i].src1_rob = src_reg.reg[i].src1_rob;
        dst_reg_reg.reg[i].src2_rob = src_reg.reg[i].src2_rob;
        //dst_reg_reg.reg[i].dst_reg_reg.reg_rob = src_reg.reg[i].dst_reg_reg.reg_rob;				
        dst_reg_reg.reg[i].src1_reg = src_reg.reg[i].src1_reg;
        dst_reg_reg.reg[i].src2_reg = src_reg.reg[i].src2_reg;
        dst_reg_reg.reg[i].cycles = src_reg.reg[i].cycles;
        dst_reg_reg.reg[i].src1_reg_rdy = src_reg.reg[i].src1_reg_rdy;
        dst_reg_reg.reg[i].src2_reg_rdy = src_reg.reg[i].src2_reg_rdy;
        dst_reg_reg.reg[i].src1_rob_rdy = src_reg.reg[i].src1_rob_rdy;
        dst_reg_reg.reg[i].src2_rob_rdy = src_reg.reg[i].src2_rob_rdy;
        dst_reg_reg.reg[i].IS_cnt = src_reg.reg[i].IS_cnt;
        dst_reg_reg.reg[i].EX_cnt = src_reg.reg[i].EX_cnt;
        dst_reg_reg.reg[i].RT_cnt = src_reg.reg[i].RT_cnt;
        src_reg.reg[i].FE_en = src_reg.reg[i].FE_dur = src_reg.reg[i].DE_en = src_reg.reg[i].DE_dur = src_reg.reg[i].RN_en = src_reg.reg[i].RN_dur = src_reg.reg[i].RR_en = src_reg.reg[i].RR_dur = src_reg.reg[i].DI_en = src_reg.reg[i].DI_dur = 0;
        src_reg.reg[i].IS_en = src_reg.reg[i].IS_dur = src_reg.reg[i].EX_en = src_reg.reg[i].EX_dur = src_reg.reg[i].WB_en = src_reg.reg[i].WB_dur = src_reg.reg[i].RT_en = src_reg.reg[i].RT_dur = 0;
        src_reg.reg[i].addr = 0;
        src_reg.reg[i].op = src_reg.reg[i].dst_reg = src_reg.reg[i].src1_rob = src_reg.reg[i].src2_rob = src_reg.reg[i].src1_reg = src_reg.reg[i].src2_reg = src_reg.reg[i].cycles = 0; //src_reg.reg[i].cycles = 0;
        src_reg.reg[i].src1_reg_rdy = src_reg.reg[i].src2_reg_rdy = src_reg.reg[i].src1_rob_rdy = src_reg.reg[i].src2_rob_rdy = false;
    }
    return;
}

void Reorder_Buffer::reset()
{
    //val = 0;
    ds_t = 0;
    ds_rdy = false;
    //ds_ex = false;
    //ds_mis = false;
    addr = 0;
    return;
}

void Instr::reset()
{
    FE_en = FE_dur = DE_en = DE_dur = RN_en = RN_dur = RR_en = RR_dur = DI_en = DI_dur = 0;
    IS_en = IS_dur = EX_en = EX_dur = WB_en = WB_dur = RT_en = RT_dur = 0;
    addr = 0;
    op = dst_reg = src1_reg = src2_reg = src1_rob = src2_rob = delay = cycles = 0;
    src1_reg_rdy = src2_reg_rdy = src1_rob_rdy = src2_rob_rdy = false;
    return;
}

void RMT_Table::reset()
{
	for (int i = 0; i < Max_REG; i++)
	{
		RMT.rmt[i].valid = false;
		RMT.rmt[i].ROB_t = -1;
	}
	return;
}

void ROB_Table::set_rob_table()
{
	ROB_Size = rob_size;
	rob = new Reorder_Buffer[ROB_Size];
	for (int i = 0; i < ROB_Size; i++)
	{
		rob[i].reset();
	} 
	return; 
}


/*void ROB_retire (Reorder_Buffer ROB[], int width)
{
    if(head_idx == tail_idx)
    {
        return;
    }

    int dist = 0;
    dist = head_idx - tail_idx;
    //dist = minimum(width, dist);

    for(int i = 0; i < dist ; i++)
    {
        if(ROB[head_idx].ds_rdy == 0)
        {
            return;
        } 
        else if(ROB[head_idx].ds_rdy == 1)
        {
            int temp = ROB[head_idx].ds_t;
            if(RMT[temp]_t == head_idx)
            {
                RMT[temp].valid = 0;
            }

            if(head_idx < (Max_REG - 1))
            {
                head_idx += 1;
            }
            else if (head_idx == (Max_REG - 1))
            {
                head_idx == 0;
            }
        }
    }
    return;
}*/


void Reg_const::set_reg()
{
	//empty = true;
	size = width;
	reg = new Instr[size];
	for (int i = 0; i < size; i++)
	{
		reg[i].reset();
	}  
	return;
}

void Reg_var::update_cycle()
{
	for (unsigned int i = 0; i < reg.size(); i++)
			reg[i].cycles--;

	return;
}

/*void Writeback (Instr WB_reg[], Reorder_Buffer ROB[], int width)
{
    for (int i = 0; i < width; i++)
    {
        ROB[WB_reg[i].dst_rob].ds_rdy = 1;
    }
    return;
}*/

/*void RMT_update (int reg, int rob_addr, bool set)
{
    if(set)
    {
        RMT[reg].valid = 1;
        RMT[reg]_t = rob_addr;
    }
    else
    {
        RMT[reg].valid = 0;
    }
    return;
}*/

/*void ROB_update (int val, int ds_t, bool ds_rdy, bool ds_ex, bool ds_mis, uint64_t addr, Reorder_Buffer ROB[], int rob_addr)
{
    ROB[rob_addr].val = val;
    ROB[rob_addr].ds_t = ds_t;
    ROB[rob_addr].ds_rdy = ds_rdy;
    ROB[rob_addr].ds_ex = ds_ex;
    ROB[rob_addr].ds_mis = ds_mis;
    ROB[rob_addr].addr = addr;
    tail_addr += 1;
    return;
}*/

void Retire(int width, int iq_size, int rob_size)
{
	int i = 0;

	while ((i < width))
	{
		//cout << "RT entry : head_idx_idx " << ROB.head_idx << " tail_idx_idx " << ROB.tail_idx << " ROB[head_idx_idx].ds_rdy " << ROB.rob[ROB.head_idx].ds_rdy << endl;
		if (ROB.head_idx == ROB.tail_idx && ROB.head_idx != ROB.ROB_Size - 1)					
		{
			if (ROB.rob[ROB.head_idx + 1].addr == 0)
				return;
		}

		if (ROB.rob[ROB.head_idx].ds_rdy)		
		{
			//cout << "RT entry " <<endl; // : width " << width << " iq_size " << iq_size << " rob_size " << rob_size  << endl ;
			for (int j = 0; j < RR_reg.size; j++)
			{
				if (RR_reg.reg[j].src1_rob == ROB.head_idx)
					RR_reg.reg[j].src1_reg_rdy = true;

				if (RR_reg.reg[j].src2_rob == ROB.head_idx)
					RR_reg.reg[j].src2_reg_rdy = true;
			}

			for (unsigned int x = 0; x < RT_reg.reg.size(); x++)
			{
				//cout << " RT_reg.size " << RT_reg.size() << " RT_reg[x].idx " << RT_reg[x].idx <<" ROB[head_idx].addr" << ROB[head_idx].addr << endl ;
                //cout << "idx "<< head_idx << " " << ROB[head_idx].ds_t << " " << ROB[head_idx].addr << " " << tail_idx << " " << ROB[tail_idx].ds_t << " " << ROB[tail_idx].addr << endl;
				if (RT_reg.reg[x].addr == ROB.rob[ROB.head_idx].addr)//ROB[head_idx].addr
				{
					//cout << " RT_reg.size " << RT_reg.size() << " RT_reg[x].idx " << RT_reg[x].idx <<" ROB[head_idx].addr" << ROB[head_idx].addr << endl ;
					//RT_reg[x].RT_dur = RT_reg.reg[x].RT_cnt + 1;

					RT_reg.reg[x].RT_dur = (gb_cnt + 1) - RT_reg.reg[x].RT_en;		
                    //RT_reg.reg[x].RT_cnt = 0;
					//cout << " RT blk gb_cnt " << gb_cnt << endl;

					cout << RT_reg.reg[x].addr << " fu{" << RT_reg.reg[x].op << "} src{" << RT_reg.reg[x].src1_reg << "," << RT_reg.reg[x].src2_reg << "} ";
					cout << "dst{" << ROB.rob[ROB.head_idx].ds_t << "} FE{" << RT_reg.reg[x].FE_en << "," << RT_reg.reg[x].FE_dur << "} ";
					cout << "DE{" << RT_reg.reg[x].DE_en << "," << RT_reg.reg[x].DE_dur << "} RN{" << RT_reg.reg[x].RN_en << "," << RT_reg.reg[x].RN_dur << "} ";
					cout << "RR{" << RT_reg.reg[x].RR_en << "," << RT_reg.reg[x].RR_dur << "} DI{" << RT_reg.reg[x].DI_en << "," << RT_reg.reg[x].DI_dur << "} ";
					cout << "IS{" << RT_reg.reg[x].IS_en << "," << RT_reg.reg[x].IS_dur << "} EX{" << RT_reg.reg[x].EX_en << "," << RT_reg.reg[x].EX_dur << "} ";
					cout << "WB{" << RT_reg.reg[x].WB_en << "," << RT_reg.reg[x].WB_dur << "} RT{" << RT_reg.reg[x].RT_en << "," << RT_reg.reg[x].RT_dur << "} " << endl;

					RT_reg.reg.erase(RT_reg.reg.begin() + x);
					break;
				}

			}
			//cout << "RT exit : head_idx_idx" << ROB.head_idx << endl;

			for (int z = 0; z < RMT.RMT_Size; z++)							
			{
				if (RMT.rmt[z].ROB_t == ROB.head_idx)
				{
					RMT.rmt[z].ROB_t = 0;
					RMT.rmt[z].valid = false;
				}
			}
			ROB.rob[ROB.head_idx].reset();										

			if (ROB.head_idx != (ROB.ROB_Size - 1))
				ROB.head_idx++;
			else
				ROB.head_idx = 0;

			i++;
		}
		else
			break;
        /*{
            for (int x = 0; x < RT_reg.reg.size(); x++)
			{
                RT_reg.reg[x].RT_cnt += 1;
            }
			break;
        }*/
	}
	return;
}

void Writeback(int width, int iq_size, int rob_size)
{
	//cout << "WB entry " <<endl; // : width " << width << " iq_size " << iq_size << " rob_size " << rob_size << endl ;
	if (WB_reg.reg.size() != 0)
	{
		for (unsigned int i = 0; i < WB_reg.reg.size(); i++)
		{
			//cout << "ROB[head_idx].ds_rdy " << ROB[head_idx].ds_rdy << " WB_reg.size() " << WB_reg.size() << " i " << i << " WB_reg[i].dst_reg " << WB_reg[i].dst_reg << endl;
			WB_reg.reg[i].RT_en = gb_cnt + 1;//WB_reg.reg[i].WB_en + WB_reg.reg[i].WB_dur;
			WB_reg.reg[i].WB_dur = WB_reg.reg[i].RT_en - WB_reg.reg[i].WB_en;

			ROB.rob[WB_reg.reg[i].dst_reg].ds_rdy = true;			
			//WB_reg[i].WB_dur = WB_reg[i].RT_en - WB_reg[i].WB_en; 									
			RT_reg.reg.push_back(WB_reg.reg[i]);										
		}
		WB_reg.reg.clear();
		//cout << "WB exit " <<endl; // : width " << width << " iq_size " << iq_size << " rob_size " << rob_size  << endl;
	}
	return;
}

void Execute(int width, int iq_size, int rob_size)
{
	if (EX_reg.reg.size() != 0)
	{
		//cout << "EX entry : EX_reg.size() " << EX_reg.size()<<endl; //: width " << width << " iq_size " << iq_size << " rob_size " << rob_size  << endl ;
		EX_reg.update_cycle();
		int check = 1;
		
		while (check != 0)
		{
			check = 0;

			for (unsigned int i = 0; i < EX_reg.reg.size(); i++)
			{
				//cout << "EX : EX_reg.reg.size() " << EX_reg.reg.size() << " EX_reg.reg[i].cycles " << EX_reg.reg[i].cycles << " " << EX_reg.reg[i].dst_reg << " " << EX_reg.reg[i].src1_reg << " " << EX_reg.reg[i].src2_reg << endl;
				if (EX_reg.reg[i].cycles == 0)
				{	
					for (int j = 0; j < RR_reg.size; j++)								
					{
						if (RR_reg.reg[j].src1_rob == EX_reg.reg[i].dst_reg)
							RR_reg.reg[j].src1_reg_rdy = true;

						if (RR_reg.reg[j].src2_rob == EX_reg.reg[i].dst_reg)
							RR_reg.reg[j].src2_reg_rdy = true;
					}	

					for (int j = 0; j < DI_reg.size; j++)									
					{
						if (DI_reg.reg[j].src1_rob == EX_reg.reg[i].dst_reg)
							DI_reg.reg[j].src1_reg_rdy = true;

						if (DI_reg.reg[j].src2_rob == EX_reg.reg[i].dst_reg)
							DI_reg.reg[j].src2_reg_rdy = true;
					}									

					for (unsigned int j = 0; j < IQ_reg.reg.size(); j++)							
					{
						if (IQ_reg.reg[j].src1_rob == EX_reg.reg[i].dst_reg)
							IQ_reg.reg[j].src1_reg_rdy = true;

						if (IQ_reg.reg[j].src2_rob == EX_reg.reg[i].dst_reg)
							IQ_reg.reg[j].src2_reg_rdy = true;
					}
					//EX_reg[i].WB_en = EX_reg[i].EX_cnt + 1;										
					//EX_reg[i].EX_dur = EX_reg[i].WB_en - EX_reg[i].EX_en;
					
					EX_reg.reg[i].WB_en = gb_cnt + 1;//EX_reg.reg[i].EX_cnt;//EX_reg[i].EX_cnt + 1;									
					EX_reg.reg[i].EX_dur = EX_reg.reg[i].WB_en - EX_reg.reg[i].EX_en;	
					//EX_reg.reg[i].EX_cnt = 0;

					WB_reg.reg.push_back(EX_reg.reg[i]);
					EX_reg.reg.erase(EX_reg.reg.begin() + i);
					check++;
					//EX_reg[i].EX_cnt = 0;
					break;
				}
				/*else if (EX_reg[i].cycles != 0)
                {
                    EX_reg[i].EX_cnt += 1;
                }*/
			}
		}
		//cout << "EX exit " <<endl; // : width " << width << " iq_size " << iq_size << " rob_size " << rob_size  << endl;
	}
	return;
}

void Issue(int width, int iq_size, int rob_size)
{
	//cout << "IQ_reg.size" << IQ_reg.size() << endl;
	if (IQ_reg.reg.size() != 0)
	{
		//cout << "issue entry : IQ_reg.size() " << IQ_reg.size() << endl ; // : width " << width << " iq_size " << iq_size << " rob_size " << rob_size  << endl ;
		sort(IQ_reg.reg.begin(), IQ_reg.reg.end());
		int i = 0;
		int check = 1;
		while (check != 0)
		{
			check = 0;
			for (unsigned int j = 0; j < IQ_reg.reg.size(); j++)
			{
				if ((IQ_reg.reg[j].src1_reg_rdy && IQ_reg.reg[j].src2_reg_rdy))// && ((EX_reg.reg.size() < 5 * width)))
				{
					//IQ_reg[j].EX_en = IQ_reg.reg[j].IS_cnt + 1; + 1;									
					//IQ_reg[j].IS_dur = IQ_reg[j].EX_en - IQ_reg[j].IS_en;
                    //cout << " IS blk gb_cnt " << gb_cnt << " IQ_reg[j].IS_en "<< IQ_reg[j].IS_en << " IQ_reg[j].IS_cnt " << IQ_reg[j].IS_cnt <<" j "<<j <<" IQ_reg.size "<<IQ_reg.size() << endl;
					
					IQ_reg.reg[j].EX_en = gb_cnt + 1;//IQ_reg.reg[j].IS_cnt + 1;
					IQ_reg.reg[j].IS_dur = IQ_reg.reg[j].EX_en - IQ_reg.reg[j].IS_en;
														
					//IQ_reg.reg[j].IS_cnt = 0;
					EX_reg.reg.push_back(IQ_reg.reg[j]);								
					IQ_reg.reg.erase(IQ_reg.reg.begin() + j);								 
					i++;
					check++;
					//IQ_reg[j].IS_cnt = 0;
                    //cout<< "IQ_reg if "<< IQ_reg[j].dst_reg <<" "<< IQ_reg[j].src1_reg<<" "<< IQ_reg[j].src1_reg_rdy<<" "<< IQ_reg[j].src2_reg<<" "<< IQ_reg[j].src2_reg_rdy<<" "<< IQ_reg[j].IS_cnt <<endl;
					break;
				}
				/*else
                {
                    IQ_reg.reg[j].IS_cnt += 1;
					//cout<< "IQ_reg else "<< IQ_reg[j].dst_reg <<" "<< IQ_reg[j].src1_reg<<" "<< IQ_reg[j].src1_reg_rdy<<" "<< IQ_reg[j].src2_reg<<" "<< IQ_reg[j].src2_reg_rdy<<" "<< IQ_reg[j].IS_cnt <<endl;
                }*/
			}
			if (i == width)
				break;
		}
		//cout << "issue exit " << endl ; // : width " << width << " iq_size " << iq_size << " rob_size " << rob_size  << endl;	
	}
	return;	
}

void Dispatch(int width, int iq_size, int rob_size)
{
	int diff = iq_size - IQ_reg.reg.size();
	if ((DI_empty == false) && (diff >= DI_reg.size))// &&(IQ_empty == true))
	{
		for (int i = 0; i < DI_reg.size; i++)
		{
			//DI_reg[i].IS_en = DI_cnt + 1;
			//DI_reg[i].DI_dur = DI_reg[i].IS_en - DI_reg[i].DI_en;
			//cout << " dispatch " << i << "  "<< DI_size << endl;
			DI_reg.reg[i].IS_en = gb_cnt + 1;//DI_cnt + 1;
			DI_reg.reg[i].DI_dur =  DI_reg.reg[i].IS_en - DI_reg.reg[i].DI_en;
												
			IQ_reg.reg.push_back(DI_reg.reg[i]);									
		}
		//Reg_copy(DI_reg, IQ, diff);
		//IQ_empty = false;
		DI_empty = true;
		//DI_cnt = 0;
		//cout << "dispatch exit " << endl ; // : width " << width << " iq_size " << iq_size << " rob_size " << rob_size << "IQ_reg.size" << IQ_reg.size() << endl;
	}
	/*else if ((DI_empty == false) && ((iq_size - IQ_reg.reg.size()) < DI_reg.size))
	{
		DI_cnt += 1;
	} */ 
	return;
}

void RegRead(int width, int iq_size, int rob_size)
{
	if ((RR_empty == false) && (DI_empty == true))
	{
		//cout << "regread entry " << endl ; // : width " << width << " iq_size " << iq_size << " rob_size " << rob_size  << endl ;
		for (int i = 0; i < RR_reg.size; i++)
		{
			//RR_reg.reg[i].DI_dur = Cycle + 1;									
			//RR_reg.reg[i].RR_dur = RR_reg.reg[i].DI_en - RR_reg.reg[i].RR_en;

			RR_reg.reg[i].DI_en = gb_cnt + 1;//RR_cnt + 1;
			RR_reg.reg[i].RR_dur = RR_reg.reg[i].DI_en - RR_reg.reg[i].RR_en;

			if (RR_reg.reg[i].src1_rob_rdy)											
			{
				if (ROB.rob[RR_reg.reg[i].src1_rob].ds_rdy == 1)
					RR_reg.reg[i].src1_reg_rdy = true;
			}
			else
				RR_reg.reg[i].src1_reg_rdy = true;

			if (RR_reg.reg[i].src2_rob_rdy)
			{
				if (ROB.rob[RR_reg.reg[i].src2_rob].ds_rdy == 1)
					RR_reg.reg[i].src2_reg_rdy = true;
			}
			else
				RR_reg.reg[i].src2_reg_rdy = true;
								
		}
		Reg_copy(RR_reg, DI_reg, RR_reg.size);
		DI_reg.size = RR_reg.size;
		RR_empty = true;
		DI_empty = false;
		//RR_cnt = 0;
		//cout << "regread exit " << endl ; // : width " << width << " iq_size " << iq_size << " rob_size " << rob_size  << endl;
	}
	/*else if ((RR_empty == false) && (DI_empty == false))
	{
		RR_cnt += 1;
	}*/
	return;
}


void Rename(int width, int iq_size, int rob_size)
{
	if ((RN_empty == false) && (RR_empty == true))
	{
		int diff;
		//cout << "rename entry " << endl ; // : width " << width << " iq_size " << iq_size << " rob_size " << rob_size << " RN_empty " << RN_empty << " RR_empty " << RR_empty << endl ;
		if (ROB.tail_idx < ROB.head_idx)
			diff = ROB.head_idx - ROB.tail_idx;
		else if (ROB.head_idx < ROB.tail_idx)
			diff = ROB.ROB_Size - (ROB.tail_idx - ROB.head_idx);
		else
		{
			if (ROB.tail_idx < (ROB.ROB_Size - 1))
			{
				if (ROB.rob[ROB.tail_idx + 1].ds_t == 0 && ROB.rob[ROB.tail_idx + 1].addr == 0 && ROB.rob[ROB.tail_idx + 1].ds_rdy == 0)
					diff = ROB.ROB_Size;
				else
					diff = 0;
			}
			else
			{
				if (ROB.rob[ROB.tail_idx - 1].ds_t == 0 && ROB.rob[ROB.tail_idx - 1].addr == 0 && ROB.rob[ROB.tail_idx - 1].ds_rdy == 0)
					diff = ROB.ROB_Size;
				else
					diff = 0;
			}
		}
		

		if (diff < RN_reg.size)
			return;
		else
		{
			for (int i = 0; i < RN_reg.size; i++)
			{
				if (RN_reg.reg[i].src1_rob != -1)
					if (RMT.rmt[RN_reg.reg[i].src1_rob].valid == true)							
					{
						RN_reg.reg[i].src1_rob = RMT.rmt[RN_reg.reg[i].src1_rob].ROB_t;			
						RN_reg.reg[i].src1_rob_rdy = true;									
					}


				if (RN_reg.reg[i].src2_rob != -1)
					if (RMT.rmt[RN_reg.reg[i].src2_rob].valid == true)							
					{
						RN_reg.reg[i].src2_rob = RMT.rmt[RN_reg.reg[i].src2_rob].ROB_t;		
						RN_reg.reg[i].src2_rob_rdy = true;										
					}
				
				ROB.rob[ROB.tail_idx].ds_t = RN_reg.reg[i].dst_reg;								
				ROB.rob[ROB.tail_idx].addr = RN_reg.reg[i].addr;								
				ROB.rob[ROB.tail_idx].ds_rdy = false;										
				//cout << "idx "<< head_idx << " " << ROB[head_idx].ds_t << " " << ROB[head_idx].addr << " " << tail_idx << " " << ROB[tail_idx].ds_t << " " << ROB[tail_idx].addr << endl;

				if (RN_reg.reg[i].dst_reg != -1)												
				{
					RMT.rmt[RN_reg.reg[i].dst_reg].ROB_t = ROB.tail_idx;								
					RMT.rmt[RN_reg.reg[i].dst_reg].valid = true;								
				}

				RN_reg.reg[i].dst_reg = ROB.tail_idx;
				//cout << "RN : i "<< i << " RN_reg[i].dst_reg " << RN_reg[i].dst_reg << endl;																							

				if (ROB.tail_idx != (ROB.ROB_Size - 1))
					ROB.tail_idx++;
				else
					ROB.tail_idx = 0;

					
				RN_reg.reg[i].RR_en = gb_cnt + 1;//RN_cnt + 1;
				RN_reg.reg[i].RN_dur = RN_reg.reg[i].RR_en - RN_reg.reg[i].RN_en;
				//RN_reg[i].RR_en = gb_cnt + 1;											
				//RN_reg[i].RN_dur = RN_reg[i].RR_en - RN_reg[i].RN_en;
				//cout << "rename : head_idx " << head_idx << " tail_idx " << tail_idx << " ROB[head_idx].ds_rdy " << ROB[head_idx].ds_rdy << endl;							
				
			}
			Reg_copy(RN_reg, RR_reg, RN_reg.size);
			RR_reg.size = RN_reg.size;
			RN_empty = true;
			RR_empty = false;
			//RN_cnt = 0;
		}
		//cout << "rename exit " << endl ; //: width " << width << " iq_size " << iq_size << " rob_size " << rob_size << " RN_empty " << RN_empty << " RR_empty " << RR_empty << endl ;
	}
	/*else if ((RN_empty == false) && (RR_empty == false))
	{
		RN_cnt += 1;
	}*/
	return;
}

void Decode(int width, int iq_size, int rob_size)
{
	if ((DE_empty == false) && (RN_empty == true))
	{
		//cout << "decode entry " << endl ; // : width " << width << " iq_size " << iq_size << " rob_size " << rob_size << " DE_empty  " << DE_empty << " RN_empty " << RN_empty << endl ;
		for (int i = 0; i < DE_reg.size; i++)
		{			
			DE_reg.reg[i].RN_en = gb_cnt + 1 ;//DE_cnt + 1;
			DE_reg.reg[i].DE_dur = DE_reg.reg[i].RN_en - DE_reg.reg[i].DE_en;
			//DE_reg[i].RN_en = gb_cnt + 1;
			//DE_reg[i].DE_dur = DE_reg[i].RN_en - DE_reg[i].DE_en;
		}
		Reg_copy (DE_reg, RN_reg, DE_reg.size);
		RN_reg.size = DE_reg.size;
		DE_empty = true;
		RN_empty = false;
		//DE_cnt = 0;
		//cout << "decode exit " << endl ; // : width " << width << " iq_size " << iq_size << " rob_size " << rob_size << " DE_empty " << DE_empty << " RN_empty " << RN_empty << endl ;
	}
	/*else if ((DE_empty == false) && (RN_empty == false))
	{
		DE_cnt += 1;
	}*/
	return;
}

void Fetch(FILE *FP, int width, int iq_size, int rob_size)
{
	int op, ds_t, src1, src2, i = 0, check = 0;  
    long int addr;           
    if(feof(FP) || (DE_empty == false))
        return;
    /*else if (DE_empty == false)
    {
        cnt += 1;
        return;
    }*/

	if (DE_empty)
	{
		//cout << "fetch entry " << endl; //: width " << width << " iq_size " << iq_size << " rob_size " << rob_size  << endl ;
		while(fscanf(FP, "%lx %d %d %d %d", &addr, &op, &ds_t, &src1, &src2) != EOF)
		{
			DE_reg.reg[i].addr = Instr_cnt;					
			DE_reg.reg[i].op = op;
			DE_reg.reg[i].dst_reg = ds_t;
			DE_reg.reg[i].src1_rob = src1;
			DE_reg.reg[i].src1_reg = src1;
			DE_reg.reg[i].src2_rob = src2;
			DE_reg.reg[i].src2_reg = src2;
			DE_reg.reg[i].src1_reg_rdy = false;
			DE_reg.reg[i].src2_reg_rdy = false;
			DE_reg.reg[i].src1_rob_rdy = false;
			DE_reg.reg[i].src2_rob_rdy = false;
			
			if (DE_reg.reg[i].op == 0)
				DE_reg.reg[i].cycles = DE_reg.reg[i].EX_cnt = 1;
			else if (DE_reg.reg[i].op == 1)
				DE_reg.reg[i].cycles = DE_reg.reg[i].EX_cnt = 2;
			else if (DE_reg.reg[i].op == 2)
				DE_reg.reg[i].cycles = DE_reg.reg[i].EX_cnt = 5;

			DE_reg.reg[i].FE_en = gb_cnt;
			DE_reg.reg[i].FE_dur = 1;
			DE_reg.reg[i].DE_en = DE_reg.reg[i].FE_en + 1;

			Instr_cnt++;
			check++;
			i++;
			//cnt++;
			DE_reg.size = i;
			//printf("%lx %d %d %d %d %d %d\n", DE_reg.reg[i].addr, op, ds_t, src1, src2, cnt, DE_reg.size); //Print to check if inputs have been read correctly
			if (i == (width))
				break;
		}
		if (check != 0)
			DE_empty = false;

		//cout << "fetch exit " << endl; // : width " << width << " iq_size " << iq_size << " rob_size " << rob_size  << " DE_empty " << DE_empty << endl;
	}
	return;
}


/*int Advance_Cycle(FILE *FP, uint64_t pc, int op_type, int dest, int src1, int src2, char* argv[])
{
    //int op_type, dest, src1, src2;
    //uint64_t pc;
    char *trace_file;
    trace_file = argv[4];
    FP = fopen(trace_file, "r");
    for( ; cnt>=0; cnt++)
    {
        //Fetch();//FE_reg);
        if(fscanf(FP, "%lx %d %d %d %d", &pc, &op_type, &dest, &src1, &src2) == EOF)
        {
        cnt = -1;
        }
    else
    {
        printf("%lx %d %d %d %d\n", pc, op_type, dest, src1, src2); //Print to check if inputs have been read correctly
        cnt += 1;
    }
    }
    return cnt;
}*/

bool Advance_Cycle(FILE *FP)
{
	if (feof(FP) && Pipeline_empty == true) 
		return false;
	else
		return true;
}

int main(int argc, char* argv[])
{
    FILE *FP;               // File handler
    char *trace_file;       // Variable that holds trace file name;
    proc_params params;       // look at sim_bp.h header file for the the definition of struct proc_params
    //int op_type, dest, src1, src2;  // Variables are read from trace file
    //uint64_t pc; // Variable holds the pc read from input file     
    
	if (argc != 5)
    {
        printf("Error: Wrong number of inputs:%d\n", argc-1);
        exit(EXIT_FAILURE);
    }
    
    params.rob_size     = strtoul(argv[1], NULL, 10);
    params.iq_size      = strtoul(argv[2], NULL, 10);
    params.width        = strtoul(argv[3], NULL, 10);
    trace_file          = argv[4];
	/*printf("rob_size:%lu "
            "iq_size:%lu "
            "width:%lu "
            "tracefile:%s\n", params.rob_size, params.iq_size, params.width, trace_file);
    // Open trace_file in read mode
    FP = fopen(trace_file, "r");
    if(FP == NULL)
    {
        // Throw error and exit if fopen() failed
        printf("Error: Unable to open file %s\n", trace_file);
        exit(EXIT_FAILURE);
    }
	*/
	rob_size = params.rob_size;
	iq_size = params.iq_size;
	width = params.width;

	ROB.set_rob_table();								
	ROB.head_idx = ROB.tail_idx = 3;

	/*for (int i = 0; i < rob_size; i++)
    {
        ROB[i].addr = ROB[i].ds_t = 0 ;
        ROB[i].ds_rdy = false;
        //cout << "ROB " << ROB[i].addr << " " << ROB[i].ds_t << endl;
    } */		

	//Issue_Queue IQ[iq_size];
    //Instr FE_reg[width];
    //Instr DE_reg[width];
    //Instr RN_reg[width];
    //Instr RR_reg[width];
    //Instr DI_reg[width];
    //vector <Instr> IQ_reg;
    //vector <Instr> EX_reg;
    //vector <Instr> WB_reg;
    //vector <Instr> RT_reg;

	DE_reg.set_reg();							
	RN_reg.set_reg();
	RR_reg.set_reg();
	DI_reg.set_reg();
	DE_empty = RN_empty = RR_empty = DI_empty = true;

	FP = fopen(trace_file, "r");
    if(FP == NULL)
    {
        printf("Error: Unable to open file %s\n", trace_file);
        exit(EXIT_FAILURE);
    }

	RMT.RMT_Size = Max_REG;								
	RMT.reset();

    /*do {
    Retire(); // Retire up to WIDTH consecutive “ready” instructions from the head of the ROB.

    Writeback(); // Process the writeback bundle in WB: For each instruction in WB, mark the instruction as “ready” in its entry in the ROB.

    Execute(); // From the execute_list, check for instructions that are finishing execution this cycle and:
    1) Remove the instruction from the execute_list. 
    2) Add the instruction to WB. 
    3) Wakeup dependent instructions (set their source operand ready flags) in the IQ, DI (the dispatch bundle), and RR (the register-read bundle).

    Issue(); // Issue up to WIDTH oldest instructions from the IQ. 
    (One approach to implement oldest-first issuing is to make multiple passes through the IQ, each time finding the next oldest ready instruction and then issuing it. 
    One way to annotate the age of an instruction is to assign an incrementing sequence number to each instruction as it is fetched from the trace file.) 
    To issue an instruction: 1) Remove the instruction from the IQ. 
    2) Add the instruction to the execute_list. Set a timer for the instruction in the execute_list that will allow you to model its execution latency.

    Dispatch(); // If DI contains a dispatch bundle: If the number of free IQ entries is less than the size of the dispatch bundle in DI, then do nothing. 
    If the number of free IQ entries is greater than or equal to the size of the dispatch bundle in DI, then dispatch all instructions from DI to the IQ.

    RegRead(); // If RR contains a register-read bundle: If DI is not empty (cannot accept a new dispatch bundle), then do nothing. 
    If DI is empty (can accept a new dispatch bundle), then process (see below) the register-read bundle and advance it from RR to DI. 
    Since values are not explicitly modeled, the sole purpose of the Register Read stage is to ascertain the readiness of the renamed source operands. 
    Apply your learning from the class lectures/notes on this topic. 
    Also, take care that producers in their last cycle of execution wake up dependent operands not just in the IQ, but also in two other stages including RegRead() (this is required to avoid deadlock). 
    See Execute() description above.

    Rename(); // If RN contains a rename bundle: If either RR is not empty (cannot accept a new register-read bundle) or the ROB does not have enough free entries to accept the entire rename bundle, then do nothing. 
    If RR is empty (can accept a new register-read bundle) and the ROB has enough free entries to accept the entire rename bundle, then process (see below) the rename bundle and advance it from RN to RR. 
    Apply your learning from the class lectures/notes on the steps for renaming: 
    (1) allocate an entry in the ROB for the instruction, 
    (2) rename its source registers, and 
    (3) rename its destination register (if it has one). Note that the rename bundle must be renamed in program order 
    (fortunately, the instructions in the rename bundle are in program order).

    Decode(); // If DE contains a decode bundle: If RN is not empty (cannot accept a new rename bundle), then do nothing. 
    If RN is empty (can accept a new rename bundle), then advance the decode bundle from DE to RN.

    Fetch(); // Do nothing if either (1) there are no more instructions in the trace file or (2) DE is not empty (cannot accept a new decode bundle). 
    If there are more instructions in the trace file and if DE is empty (can accept a new decode bundle), then fetch up to WIDTH instructions from the trace file into DE. 
    Fewer than WIDTH instructions will be fetched only if the trace file has fewer than WIDTH instructions left.

    } while (Advance_Cycle(FP, pc, op_type, dest, src1, src2, argv)!=0);*/
    // Advance_Cycle performs several functions.

	while (Advance_Cycle(FP))
	{
		//Retire()
        //ROB_retire (ROB, width, iq_size, rob_size);
        //Writeback (WB_reg[][5], ROB, width);
		Retire(width, iq_size, rob_size);
		Writeback(width, iq_size, rob_size);
		Execute(width, iq_size, rob_size);
		Issue(width, iq_size, rob_size);
		Dispatch(width, iq_size, rob_size);
		RegRead(width, iq_size, rob_size);
		Rename(width, iq_size, rob_size);
		Decode(width, iq_size, rob_size);
		Fetch(FP, width, iq_size, rob_size);
		
		if (DE_empty && RN_empty && RR_empty && DI_empty && IQ_reg.reg.size() == 0 && EX_reg.reg.size() == 0 && WB_reg.reg.size() == 0 && (ROB.head_idx == ROB.tail_idx) && (ROB.rob[ROB.tail_idx].addr == 0))
					Pipeline_empty = true;

		gb_cnt++;
		//cout << " gb_cnt " << gb_cnt << endl;// << " cnt " << cnt << endl;
        //Advance_Cycle(FP, pc, op_type, dest, src1, src2, argv)!=0
	}

	cout << "# === Simulator Command =========" << endl;
	cout << "# ./sim " << params.rob_size << " " << params.iq_size<< " " << params.width << " " << trace_file << endl;
	cout << "# === Processor Configuration ===" << endl;
	cout << "# ROB_SIZE = " << params.rob_size << endl;
	cout << "# IQ_SIZE  = " << params.iq_size << endl;
	cout << "# WIDTH    = " << params.width << endl;
	cout << "# === Simulation Results ========" << endl;
	cout << "# Dynamic Instruction Count    = " << Instr_cnt << endl;
	cout << "# Cycles                       = " << gb_cnt << endl;
	cout << "# Instructions Per Cycle (IPC) = " << fixed << setprecision(2) << ((float)Instr_cnt / (float)gb_cnt) << endl;

	return 0;
}