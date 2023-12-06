#ifndef SIM_PROC_H
#define SIM_PROC_H

#include <iostream>
//#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <inttypes.h>
#include <math.h>
#include <fstream>
#include <vector>
#include <iomanip>
#include <algorithm>

using namespace std;

#define Max_REG 67
//#define operands 4

typedef struct proc_params{
    unsigned long int rob_size;
    unsigned long int iq_size;
    unsigned long int width;
}proc_params;

int width, rob_size, iq_size;

struct Instr
{
	int FE_en;
    int FE_dur;
    int DE_en;
    int DE_dur;
    int RN_en;
    int RN_dur;
    int RR_en;
    int RR_dur;
    int DI_en;
    int DI_dur;
    int IS_en;
    int IS_dur;
    int EX_en;
    int EX_dur;
    int WB_en;
    int WB_dur;
    int RT_en;
    int RT_dur;
    int addr;
    int op;
    int dst_reg;
    int src1_reg;
    int src2_reg;
    //int dst_rob;
    int src1_rob;
    int src2_rob;
    int delay;
    bool src1_reg_rdy;
    bool src2_reg_rdy;
    bool src1_rob_rdy;
    bool src2_rob_rdy;
    int cycles;
    int IS_cnt;
    int EX_cnt;
    //int WB_cnt;
    int RT_cnt;

	void reset();
	//Instr() : FE_en(0), FE_dur(0), DE_en(0), DE_dur(0), RN_en(0), RN_dur(0), RR_en(0), RR_dur(0), DI_en(0), DI_dur(0), IS_en(0), IS_dur(0), EX_en(0), EX_dur(0), WB_en(0), WB_dur(0), RT_en(0), RT_dur(0), addr(0), op(0), dst_reg(0), src1_reg(0), src2_reg(0), src1_rob(0), src2_rob(0), delay(0), src1_reg_rdy(false), src2_reg_rdy(false), src1_rob_rdy(false), src2_rob_rdy(false), cycles(0), IS_cnt(0), EX_cnt(0), RT_cnt(0)  {}

    auto operator<(const Instr &temp) const -> bool {
        return addr < temp.addr;
}

};

struct Rename_Map_Table
{
	bool valid;
	int ROB_t;

    //Rename_Map_Table() : valid(false), ROB_t(-1) {}
};

class RMT_Table
{
public:
	int RMT_Size;
	Rename_Map_Table rmt[Max_REG];
	void reset();
};
RMT_Table RMT;

struct Reorder_Buffer
{
	//int val;
    int ds_t;
    bool ds_rdy;
    //bool ds_ex;
    //bool ds_mis;
	int addr;
	void reset();

	//Reorder_Buffer() : ds_t(0), ds_rdy(false), addr(0) {}// ds_ex(false), ds_mis(false), addr(0) {}
};

class ROB_Table
{
public:
	int head_idx, tail_idx, ROB_Size;
	Reorder_Buffer *rob = NULL;
	void set_rob_table();
};
ROB_Table ROB;

class Reg_const
{
public:
	int size;
	//bool empty;
	Instr *reg = NULL;
	void set_reg();
};
Reg_const DE_reg, RN_reg, RR_reg, DI_reg;

class Reg_var
{
public:
	vector <Instr> reg;
	void update_cycle();
};
Reg_var IQ_reg, EX_reg, WB_reg, RT_reg;

#endif
