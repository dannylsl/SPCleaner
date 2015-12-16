/*  copyright 2011-3010, CHINA UNIONPAY DATA SERVICES Co., Ltd.  
 *  All right reserved.
 *
 *  THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF CHINA UNIONPAY CUPDATA
 *  SERVICES CO., LTD.  THE CONTENTS OF THIS FILE MAY NOT 
 *  BE DISCLOSED TO THIRD PARTIES, COPIED OR DUPLICATED IN ANY FORM,
 *  IN WHOLE OR IN PART, WITHOUT THE PRIOR WRITTEN PERMIStSiON OF 
 *  CHINA UNIONPAY CUPDATA SERVICES CO., LTD.
 *
 *  会计总账更新及会计分录记录
 * 
 *  Edit History:
 *  版本     日期            修改人       修改内容                
 *  1.0     2011/08/01       徐菁         创建    
 *  1.1     2012/03/05       徐菁         修改 2.2011.04.000
 *                                        CASE-0000-2011-0274 贷记卡农信模式改造
 *  1.2     2012/08/06       徐菁         修改 2.2012.02.000
 *                                        case-xq-0519-20111109 区分个人自助循环贷款业务品种
 *  1.3     2012/08/06       徐菁         修改 2.2012.02.000
 *                                        XYKREQ-6304-2011-0005 呆账核销需求    
 *  1.4     2012/11/18       徐菁         修改 2.2012.05.000
 *                                        CHG-120529-0011       IC卡特色需求 
 *  1.5     2013/05/10       姚应磊       修改 2.2013.02
 *                                        CHG-130115-0024       渣打银行账务需求开发
 *  1.6     2013/07/08       姚应磊       修改 2.2013.03
 *                                        CHG-130218-0019       信用卡小微产品独立核算科目需求
 *  1.7     2013/11/06       姚应磊       修改 2.2013.04.000/N2.2014.01.000
 *                                        CHG-130510-0036       总账系统相关改造
 *  1.8     2013/11/15       姚应磊       修改 2.2013.05.000/N2.2014.01.000
 *                                        CHG-130828-0002       积分消费参数控制需求
 *  1.9     2014/02/11       姚应磊       修改 2.2013.06.000/N2.2014.02.000
 *                                        CHG-130909-0051 浙江农信-CHGS等受理网点号保存优化
 *  2.0     2014/02/11       姚应磊       修改 2.2013.06.000/N2.2014.02.000
 *                                        CHG-131111-0009 浙江农信清算并账文件渠道拆分
 *  2.1     2014/02/11       姚应磊       修改 2.2013.06.000/N2.2014.02.000
 *                                        CHG-131119-0006 浙江农信问题修复
 *  2.2     2014/07/16       姚应磊       修改 2.2014.01.000/N2.2014.04.000
 *                                        CHG-131226-0008 贷记卡2.0渣打银行VISA非法卡号以及收付费交易并入PSGL需求
 *  2.2     2014/07/16       姚应磊       修改 2.2014.02.000/N2.2014.04.000
 *                                        CHG-140219-0001 渣打银行PSGL优化
 *
 *******************************************************************************/

#include "L065_trac_update.h"

/********************************************************************************
*  Function:  L065_trac_amount
*
*  该函数用于会计总账更新及会计分录记录
*
*  @Parameters:
*  输入:      acct_tdef *ptTblAcct 本币账户ACCT表结构体 
*  输入&输出：L065_io_sdef *ptL065InOut           
*  输出：trdef_tdef  *ptTblTrdef  TRDEF表结构体
*
*  @return: 0:成功
*           1:日期不合法
*           2:tracb初始化记录不存在
*           3:O_R_FLAG不合法
*           4:币种不合法
*           5:BP_FLAG不合法
*           6:关账代码与来源不匹配
*           7:TRDEF不存在且无9990或9991可替代
*           9:输入不合法
*           10:BANKS或PRMCN表记录不存在
*           <0:数据库错
********************************************************************************/

/********************************************************************************
* 以下为内部使用子函数的声明
********************************************************************************/
/* DEL CASE-0000-2011-0274
int L065_1_trac_opr(UN4 L065_bank,
                    UN4 L065_trans_type,
                    UN3 L065_curr_num,
                    CH  L065_o_r_flag,
                    CH  L065_tran_status,
                    UN5  L065_proc_day,
                    SN11_2 L065_amount,
                    CH *pL065_bankacct,
                    CH *pL065_bankacct1); */
                    
/*ADD CASE-0000-2011-0274*/
int L065_1_trac_opr(acct_tdef *ptTblAcct,
                UN4 L065_trans_type,
                UN3 L065_curr_num,
                CH  L065_o_r_flag,
                UN5  L065_proc_day,
                SN11_2 L065_amount,
                CH *pL065_bankacct,
                CH *pL065_bankacct1);/*插入或更新总账TRAC*/
                
/*END ADD CASE-0000-2011-0274*/

/* DEL CASE-0000-2011-0274
int L065_2_tracb_opr(UN4 L065_bank,
                     UN4 L065_trans_type,
                     UN3 L065_curr_num,
                     CH  L065_o_r_flag,
                     CH  L065_tran_status,
                     CH *pL065_trans_src,
                     UN5 L065_proc_day,
                     SN11_2 L065_amount); */ 
/*ADD CASE-0000-2011-0274*/
int L065_2_tracb_opr(acct_tdef *ptTblAcct,
                     UN4 L065_trans_type,
                     UN3 L065_curr_num,
                     CH  L065_o_r_flag,
                     CH *pL065_trans_src,
                     UN5 L065_proc_day,
                     SN11_2 L065_amount);
/*END ADD CASE-0000-2011-0274*/
/*插入或更新总账TRACB*/

/* DEL CASE-0000-2011-0274
int L065_3_jorj_ins(trdef_tdef  *ptTblTrdef,
                    CH *pL065_bankacct,
                    CH *pL065_bankacc1,
                    L065_io_sdef *ptL065InOut);*/
                    
/*ADD CASE-0000-2011-0274*/                  
int L065_3_jorj_ins(acct_tdef *ptTblAcct,
                        trdef_tdef *ptTblTrdef,                    
                        CH chDescription[36+1],
                        SN4 snBacctcode,
                        CH *pL065_bankacct,
                        CH *pL065_bankacc1, 
                        L065_io_sdef *ptL065InOut); /*插入会计分录JORJ*/                   
/*END ADD CASE-0000-2011-0274*/    

/*ADD CHG-120529-0011 IC卡特色交易*/
int L065_4_bacct_upd(L065_io_sdef *ptL065InOut,
                     CH *pL065_bankacct,
                     CH *pL065_bankacc1);
/*END ADD CHG-120529-0011*/

/*Begin:added by yyl for CHG-130909-0051*/
int L065_5_brno_chg(acct_tdef *ptTblAcct,
                          L065_io_sdef *ptL065InOut);
/*Begin:added by yyl for CHG-130909-0051*/

 /*****************************************************************************/ 
int L065_trac_update(acct_tdef *ptTblAcct, L065_io_sdef *ptL065InOut,trdef_tdef  *ptTblTrdef)
{
    SN6 snResult = 0;
    UN1 unWrite_off = 0;   /*1:核销特殊处理*/
    CH  chO_R_flag[1+1];
    CH  chBankacct[20+1];  /*借方科目,对应原L065-BANKACCT*/
    CH  chBankacc1[20+1];  /*贷方科目,对应原L065-BANKACC1*/
    CH  chB_P_flag[1+1];   /*ADD CASE-0000-2011-0274 P:个人卡, P:公司卡*/
    CH  chDescription[36+1]; /*使用9990/9991交易类型时记录原始交易信息*/
    
    UN4 unTrans_type_bak = 0;
    SN11_2 snAmount_bak = 0;
    CH cO_R_flag_bak = 'O';  

    trdef_tdef tTblTrdef;
    trdef_tdef tTblTrdef_bak;
    acca_tdef  tTblAcca;  /*added by yyl for CHG-130115-0024 on 2013-05-10*/

    /*Begin:added by yyl for CHG-130218-0019 on 2013-07-08*/
    SN4        snBacctcode = 0;   /*会计核算代码*/
    CH         chBacctbak[20+1];  /*借方科目,对应原L065-ACCTBAK*/
    CH         chBacc1bak[20+1];  /*贷方科目,对应原L065-ACC1BAK*/
    banks_tdef tTblBanks;
    prmcn_tdef tTblPrmcn;
    prbct_tdef tTblPrbct;
    /*End:  added by yyl for CHG-130218-0019 on 2013-07-08*/

    /*变量初始化*/
    memset(chO_R_flag, 0x00, sizeof(chO_R_flag));
    memset(chBankacct, 0x00, sizeof(chBankacct));
    memset(chBankacc1, 0x00, sizeof(chBankacc1));
    memset(chB_P_flag, 0x00, sizeof(chB_P_flag));  /*ADD CASE-0000-2011-0274*/
    memset(chDescription, 0x00, sizeof(chDescription));
    memset(&tTblTrdef, 0x00, sizeof(trdef_tdef));
    memset(&tTblTrdef_bak, 0x00, sizeof(trdef_tdef));
    memset(&tTblAcca, 0x00, sizeof(acca_tdef)); /*added by yyl for CHG-130115-0024 on 2013-05-10*/
    /*Begin:added by yyl for CHG-130218-0019 on 2013-07-08*/
    memset(chBacctbak, 0x00, sizeof(chBacctbak));
    memset(chBacc1bak, 0x00, sizeof(chBacc1bak));
    memset(&tTblBanks, 0x00, sizeof(banks_tdef));
    memset(&tTblPrmcn, 0x00, sizeof(prmcn_tdef));
    /*End:  added by yyl for CHG-130218-0019 on 2013-07-08*/

    /*输入有效性检查*/
    if(NULL == ptTblAcct)
    {
        CmdLog(LOGWAN, "ptTblAcct is NULL");
        return  9;      
    }
    
    if( (0 == CmdGL(ptTblAcct->ROWID)) && (ptL065InOut->unTrans_type >= 1000))
    {
        CmdLog(LOGWAN, "ACCT ROWID is space TRANS_TYPE[%d]", ptL065InOut->unTrans_type);
        return 9;
    }  
    
    if(0 == ptL065InOut->unCurr_num)
    {
        CmdLog(LOGWAN, "CURR_NUM error CURR_NUM[%d]", ptL065InOut->unCurr_num);
        return 4;
    }
    
    if((ptL065InOut->unCurr_num != ptTblAcct->CURR_NUM) 
        && (ptL065InOut->unCurr_num != ptTblAcct->CURR_NUM2) 
        && (ptL065InOut->unTrans_type >= 1000))
    {
        CmdLog(LOGWAN, "CURR_NUM error CURR_NUM[%d] BANK[%d] XACCOUNT[%ld] ACCT.CURR_NUM[%d] CURR_NUM2[%d]", \
            ptL065InOut->unCurr_num, ptTblAcct->BANK, ptTblAcct->XACCOUNT, ptTblAcct->CURR_NUM, ptTblAcct->CURR_NUM2); 
        return 4;       
    }
    
    if(0 == ptL065InOut->unProc_day)
    {
        CmdLog(LOGWAN, "PROC_DAY error PROC_DAY[%d]", ptL065InOut->unProc_day);
        return 9;        
    }
        
    if((ptL065InOut->cO_r_flag != 'O') && (ptL065InOut->cO_r_flag != 'R'))
    {
        CmdLog(LOGWAN, "O_R_FLAG error cO_r_flag[%c]", ptL065InOut->cO_r_flag);
        return 3;
    }
    
    if(0 == CmdDblCmp(ptL065InOut->snAmount, 0, 2))
    {
        CmdLog(LOGINF, "AMOUNT is 0 TRANS_SRC[%s] TRANS_TYPE[%d]", ptL065InOut->chTrans_src, \
            ptL065InOut->unTrans_type);
        return 0;        
    }
    
    /*DEL CASE-0000-2011-0274
    if((ptL065InOut->cTran_status != 'B') && (ptL065InOut->cTran_status != 'P'))
    {
        CmdLog(LOGWAN, "TRAN_STATUS error cTran_status[%c]", ptL065InOut->cTran_status);
        return 5;
    }*/

    /*if((0 == memcmp(ptL065InOut->chClocd, "WQ",2)) && \*/ /*DEL CASE-0000-2011-0274*/
    if((0 == memcmp(ptTblAcct->CLOSE_CODE, "WQ",2)) &&       /*ADD CASE-0000-2011-0274*/
        ( memcmp(ptL065InOut->chInp_source, "WROFFACCT", 9) != 0))
    {
        CmdLog(LOGWAN, "INP_SOURCE error chClocd[%s] chInp_source[%s]", ptTblAcct->CLOSE_CODE,\
            ptL065InOut->chInp_source);
        return 6;
    }  

    /*Begin:added by yyl for CHG-130218-0019 on 2013-07-08*/
    snResult = Fbanks(ptTblAcct->BANK, &tTblBanks);
    if((snResult != 0) && (snResult != DB_NOT_FOUND))
    {
        CmdLog(LOGWAN, "Fbanks error snResult[%ld]", snResult);        
        return -1;
    }
    if(DB_NOT_FOUND == snResult)
    {
        CmdLog(LOGWAN, "Fbanks error snResult[%ld]", snResult);        
        return 10;   /*cichufanhuicuowuma是否需要*/
    }

    if (1 == tTblBanks.BACCT_FLAG)
    {
        snResult = Pprmcnbnk(ptTblAcct->BANK, ptTblAcct->CATEGORY, &tTblPrmcn);
        if((snResult != 0) && (snResult != DB_NOT_FOUND))
        {
            CmdLog(LOGWAN, "Pprmcnbnk error snResult[%ld]", snResult);        
            return -1;
        }
        else if(DB_NOT_FOUND == snResult)
        {
            CmdLog(LOGINF, "Pprmcnbnk not found");        
            return 10;   /*cichufanhuicuowuma是否需要*/
        }
        else
        {
            snBacctcode = tTblPrmcn.BACCT_CODE;
        }       
    }
    /*End:  added by yyl for CHG-130218-0019 on 2013-07-08*/
   
    /*查询TRDEF*/
    memset(&tTblTrdef, 0x00, sizeof(trdef_tdef));
    chO_R_flag[0] = ptL065InOut->cO_r_flag;
    chO_R_flag[1] = '\0';
    /*snResult = Ptrdef(ptL065InOut->unBank, ptL065InOut->unTrans_type,\*/ /*DEL CASE-0000-2011-0274*/
    snResult = Ptrdef(ptTblAcct->BANK, ptL065InOut->unTrans_type, /*ADD CASE-0000-2011-0274*/
        ptL065InOut->unCurr_num, chO_R_flag, &tTblTrdef);
    if((snResult != 0) && (snResult != DB_NOT_FOUND))
    {
        CmdLog(LOGWAN, "Ptrdef error snResult[%ld]", snResult);
        return -1;
    }
    if(DB_NOT_FOUND == snResult)
    {
        /*记录文件及数据库日志*/
        CmdLog(LOGWAN, "Ptrdef error snResult[%ld] BANK[%d] TRANS_TYPE[%d] CURR_NUM[%d] O_R_FLAG[%c]", \
            snResult, ptTblAcct->BANK, ptL065InOut->unTrans_type, ptL065InOut->unCurr_num, ptL065InOut->cO_r_flag);
             
        /*TRDEF不存在,记录输入交易类型相关信息 BANK + TRANS_TYPE + CURR_NUM +  O_R_FLAG + AMT*/
        CmdA2A(CmdN2A(&ptTblAcct->BANK, 4, 0, 0, "001"), sizeof(chDescription), chDescription); 
        CmdATT(CmdN2A(&ptL065InOut->unTrans_type, 4, 0, 0, "001"), 1, sizeof(chDescription), chDescription); 
        CmdATT(CmdN2A(&ptL065InOut->unCurr_num, 3, 0, 0, "001"), 1, sizeof(chDescription), chDescription);         
        CmdATT(chO_R_flag, 1, sizeof(chDescription), chDescription); 
        CmdATT(CmdN2A(&ptL065InOut->snAmount, 11, 2, 1, "010"), 1, sizeof(chDescription), chDescription); 
        
        /*TRDEF不存在,使用9990-本币或9991-外币替换原有交易类型*/
        if(0 == (ptL065InOut->unTrans_type%2))/*偶数,本币交易*/
        {
            ptL065InOut->unTrans_type = 9990;
        }
        else
        {
            ptL065InOut->unTrans_type = 9991;/*奇数,外币交易*/
        }

        if(CmdDblCmp(ptL065InOut->snAmount, 0.00, 2) >= 0)
        {
            ptL065InOut->cO_r_flag = 'O';            
        }
        else
        {
            ptL065InOut->cO_r_flag = 'R';
        }
        chO_R_flag[0] = ptL065InOut->cO_r_flag;
        chO_R_flag[1] = '\0';

        /*查询9990或9991的TRDEF记录*/
        memset(&tTblTrdef, 0x00, sizeof(trdef_tdef));
        snResult = Ptrdef(ptTblAcct->BANK, ptL065InOut->unTrans_type, \
            ptL065InOut->unCurr_num, chO_R_flag, &tTblTrdef);
        if((snResult!= 0) && (snResult != DB_NOT_FOUND))
        {
            CmdLog(LOGWAN, "Ptrdef error snResult[%ld]", snResult);
            return -1;
        }      
        else if(DB_NOT_FOUND == snResult)
        {
            CmdLog(LOGWAN, "Ptrdef error snResult[%ld] BANK[%d] TRANS_TYPE[%d] CURR_NUM[%d] O_R_FLAG[%s]", \
                snResult, ptTblAcct->BANK, ptL065InOut->unTrans_type, ptL065InOut->unCurr_num, chO_R_flag);
            return 7;
        }
    }
    else
    {
        /*核销特殊处理*/
        /*if(('W' == ptL065InOut->chClocd[0]) && (1 == ptL065InOut->unWrof_flag) \*/ /*DEL CASE-0000-2011-0274*/
        if((0 == strcmp(ptTblAcct->CLOSE_CODE, "W")) && (1 == ptTblAcct->WROF_FLAG) /*ADD CASE-0000-2011-0274*/
            && (tTblTrdef.WTRAN_TP != 0))
        {
            /*根据TRDEF.WTRAN_TP查询TRDEF，并加以替换*/
            memcpy(&tTblTrdef_bak, &tTblTrdef, sizeof(trdef_tdef));/*备份TRDEF*/
            
            if((tTblTrdef.WTRAN_TP != 7318) && (tTblTrdef.WTRAN_TP != 7319)    /*ADD XYKREQ-6304-2011-0005*/
                &&(tTblTrdef.WTRAN_TP != 7320) && (tTblTrdef.WTRAN_TP != 7321))/*ADD XYKREQ-6304-2011-0005*/  
            {
                snResult = Ptrdef(ptTblAcct->BANK, tTblTrdef.WTRAN_TP, ptL065InOut->unCurr_num, GC_ORIG, &tTblTrdef);
                if((snResult != 0) && (snResult != DB_NOT_FOUND))
                {
                    CmdLog(LOGINF, "Ptrdef error snResult[%ld]", snResult);
                    return -1;
                }
                if(0 == snResult)
                {
                    unWrite_off = 1;
                    unTrans_type_bak = ptL065InOut->unTrans_type;
                    snAmount_bak = ptL065InOut->snAmount;
                    cO_R_flag_bak = ptL065InOut->cO_r_flag;
                    
                    ptL065InOut->unTrans_type = tTblTrdef.TRANS_TYPE;
                    if(ptL065InOut->snAmount < 0)
                    {
                        ptL065InOut->snAmount *= (-1);
                    }
                    ptL065InOut->cO_r_flag = 'O';
                }  
                else if(DB_NOT_FOUND == snResult)
                {
                    /*如果核销交易类型不存在，则使用原交易类型的TRDEF*/
                    memcpy(&tTblTrdef, &tTblTrdef_bak, sizeof(trdef_tdef));
                    CmdLog(LOGWAN, "TRDEF.WTRAN_TP TRDEF NOT FOUND BANK[%d] TRANS_TYPE[%d] CURR_NUM[%d] WTRAN_TP[%d]",\
                        tTblTrdef.BANK, tTblTrdef.TRANS_TYPE, tTblTrdef.CURR_NUM, tTblTrdef.WTRAN_TP);
                }              
            }
            /*BEGIN ADD XYKREQ-6304-2011-0005 呆账核销需求*/
            else
            {
                snResult = Ptrdef(ptTblAcct->BANK, tTblTrdef.WTRAN_TP, ptL065InOut->unCurr_num, chO_R_flag, &tTblTrdef);
                if((snResult != 0) && (snResult != DB_NOT_FOUND))
                {
                    CmdLog(LOGINF, "Ptrdef error snResult[%ld]", snResult);
                    return -1;
                }
                if(0 == snResult)
                {
                    unWrite_off = 1;
                    unTrans_type_bak = ptL065InOut->unTrans_type;
                    snAmount_bak = ptL065InOut->snAmount;
                    cO_R_flag_bak = ptL065InOut->cO_r_flag;                                        
                    ptL065InOut->unTrans_type = tTblTrdef.TRANS_TYPE;                
                }
                else if(DB_NOT_FOUND == snResult)
                {
                    /*如果核销交易类型不存在，则使用原交易类型的TRDEF*/
                    memcpy(&tTblTrdef, &tTblTrdef_bak, sizeof(trdef_tdef));
                    CmdLog(LOGWAN, "TRDEF.WTRAN_TP TRDEF NOT FOUND BANK[%d] TRANS_TYPE[%d] CURR_NUM[%d] WTRAN_TP[%d]",\
                        tTblTrdef.BANK, tTblTrdef.TRANS_TYPE, tTblTrdef.CURR_NUM, tTblTrdef.WTRAN_TP);
                }
            }
            /*END ADD XYKREQ-6304-2011-0005 呆账核销需求*/
        }
    }

    /*使用TRDEF对借方科目、贷方科目赋值*/
    memset(chBankacct, 0x00, sizeof(chBankacct));
    memset(chBankacc1, 0x00, sizeof(chBankacc1));

    /*if('P' == ptL065InOut->cTran_status)*/    /*DEL CASE-0000-2011-0274*/
    if(0 == CmdGL(ptTblAcct->BUSINESS))/*个人卡*/ /*ADD CASE-0000-2011-0274*/
    {
        CmdA2A(tTblTrdef.BANKACCT, sizeof(chBankacct), chBankacct);
        CmdA2A(tTblTrdef.BANKACC1, sizeof(chBankacc1), chBankacc1);
    }
    else/*公司卡*/                               
    {
        CmdA2A(tTblTrdef.BANKACCTB, sizeof(chBankacct), chBankacct);
        CmdA2A(tTblTrdef.BANKACC1B, sizeof(chBankacc1), chBankacc1);
    }

    /*Begin:added by yyl for CHG-130218-0019 on 2013-07-08*/
    if (snBacctcode != 0)
    {
        /*借方科目替换*/
        memset(&tTblPrbct, 0x00, sizeof(prbct_tdef));
        snResult = PprbctOpen(DB_CURFROM4, ptTblAcct->BANK, chBankacct, snBacctcode, ptL065InOut->unCurr_num, ptL065InOut->unProc_day);
        if (snResult != 0)
        {
            CmdLog(LOGWAN, "PprbctOpen error snResult[%ld], Bankacct[%s]", snResult, chBankacct);
            return -1;
        }

        snResult = PprbctFetch(DB_CURFROM4, &tTblPrbct);
        if ((snResult != 0) && (snResult != DB_NOT_FOUND))
        {
            CmdLog(LOGWAN, "PprbctFetch error snResult[%ld], Bankacct[%s]", snResult, chBankacct);
            PprbctClose(DB_CURFROM4);
            return -1;
        }        
        else if (0 == snResult)
        {
            CmdA2A(tTblPrbct.SUB_BACCT, sizeof(chBacctbak), chBacctbak);
        }

        snResult = PprbctClose(DB_CURFROM4);
        if (snResult != 0)
        {
            CmdLog(LOGWAN, "PprbctClose error snResult[%ld], Bankacct[%s]", snResult, chBankacct);
            return -1;
        } 

        /*贷方科目替换*/
        memset(&tTblPrbct, 0x00, sizeof(prbct_tdef));
        snResult = PprbctOpen(DB_CURFROM4, ptTblAcct->BANK, chBankacc1, snBacctcode, ptL065InOut->unCurr_num, ptL065InOut->unProc_day);
        if (snResult != 0)
        {
            CmdLog(LOGWAN, "PprbctOpen error snResult[%ld], Bankacc1[%s]", snResult, chBankacc1);
            return -1;
        }
        
        snResult = PprbctFetch(DB_CURFROM4, &tTblPrbct);
        if ((snResult != 0) && (snResult != DB_NOT_FOUND))
        {
            CmdLog(LOGWAN, "PprbctFetch error snResult[%ld], Bankacc1[%s]", snResult, chBankacc1);
            PprbctClose(DB_CURFROM4);
            return -1;
        }     
        else if (0 == snResult)
        {
            CmdA2A(tTblPrbct.SUB_BACCT, sizeof(chBacc1bak), chBacc1bak);
        }

        snResult = PprbctClose(DB_CURFROM4);
        if (snResult != 0)
        {
            CmdLog(LOGWAN, "PprbctClose error snResult[%ld], Bankacc1[%s]", snResult, chBankacc1);
            return -1;
        }
    }

    if (CmdGL(chBacctbak) != 0)
    {
        CmdA2A(chBacctbak, sizeof(chBankacct), chBankacct);
    }
    if (CmdGL(chBacc1bak) != 0)
    {
        CmdA2A(chBacc1bak, sizeof(chBankacc1), chBankacc1);
    }
    /*End:  added by yyl for CHG-130218-0019 on 2013-07-08*/

    /*插入或更新总账记录TRAC或TRACB*/
    if(0 == CmdGL(ptL065InOut->chTrans_src))/*TRANS_SRC为空,更新TRAC*/
    {
        /*snResult = L065_1_trac_opr(ptL065InOut->unBank,
                                   ptL065InOut->unTrans_type,
                                   ptL065InOut->unCurr_num,
                                   ptL065InOut->cO_r_flag,
                                   ptL065InOut->cTran_status,
                                   ptL065InOut->unProc_day,
                                   ptL065InOut->snAmount,
                                   chBankacct,
                                   chBankacc1);*/ /*DEL CASE-0000-2011-0274*/
        /*ADD CASE-0000-2011-0274*/                           
        snResult = L065_1_trac_opr(ptTblAcct,
                                   ptL065InOut->unTrans_type,
                                   ptL065InOut->unCurr_num,
                                   ptL065InOut->cO_r_flag,
                                   ptL065InOut->unProc_day,
                                   ptL065InOut->snAmount,
                                   chBankacct,
                                   chBankacc1);
        /*END ADD CASE-0000-2011-0274*/    
    }
    else/*TRANS_SRC非空,插入或更新TRACB*/
    {
        /*snResult = L065_2_tracb_opr(ptL065InOut->unBank,
                                   ptL065InOut->unTrans_type,
                                   ptL065InOut->unCurr_num,
                                   ptL065InOut->cO_r_flag,
                                   ptL065InOut->cTran_status,
                                   ptL065InOut->chTrans_src,
                                   ptL065InOut->unProc_day,
                                   ptL065InOut->snAmount); DEL CASE-0000-2011-0274*/
        /*ADD CASE-0000-2011-0274*/                                
        snResult = L065_2_tracb_opr(ptTblAcct,
                                   ptL065InOut->unTrans_type,
                                   ptL065InOut->unCurr_num,
                                   ptL065InOut->cO_r_flag,
                                   ptL065InOut->chTrans_src,
                                   ptL065InOut->unProc_day,
                                   ptL065InOut->snAmount);
        /*END ADD CASE-0000-2011-0274*/                                        
    }
    if(snResult != 0)
    {
        return snResult;
    }

    /*记录会计分录JORJ*/
    snResult = L065_3_jorj_ins(ptTblAcct, 
                               &tTblTrdef, 
                               chDescription,  /*CHG CASE-0000-2011-0274*/
                               snBacctcode,    /*CHG-130115-0024*/
                               chBankacct, 
                               chBankacc1,
                               ptL065InOut);
    if(snResult != 0)
    {
        return snResult;
    }

    /*Begin: added by yyl for CHG-130115-0024 on 2013-05-10*/
    if (GC_BANK_ZD == ptTblAcct->BANK)
    {
        if (ptTblAcct->XACCOUNT != 0)
        {
            snResult = Pacca(ptTblAcct->BANK, ptTblAcct->XACCOUNT, &tTblAcca);
            if ((snResult != 0) && (snResult != DB_NOT_FOUND))
            {
                CmdLog(LOGINF, "Pacca error snResult[%ld]", snResult);
                return -1;
            }
            else if (0 == snResult)
            {
                if (0 == ptL065InOut->unCust_flag)
                {
                    CmdA2A(tTblAcca.CUST_CLASS, sizeof(ptL065InOut->chCust_class), ptL065InOut->chCust_class);
                }
                /*Begin:deled by yyl for CHG-131226-0008*/
                /*snResult = L065_auto_jonal(ptTblAcct, &tTblAcca, &tTblTrdef, ptL065InOut, chBankacct, chBankacc1);*/ /*moded by yyl for CHG-130416-0027*/
                /*if (snResult != 0)
                {
                    CmdLog(LOGINF, "L065_auto_jonal error snResult[%ld]", snResult);
                    return snResult;
                }*/
                /*End:  deled by yyl for CHG-131226-0008*/
            }
        }
        /*Begin:added by yyl for CHG-140219-0001*/
        else
        {
            ptTblAcct->WROF_FLAG = 0; /*XACCOUNT = 0说明该ACCT表结构无效可直接赋0*/
        }
        /*End:  added by yyl for CHG-140219-0001*/

        /*Begin:added by yyl for CHG-131226-0008*/
        snResult = L065_auto_jonal(ptTblAcct, &tTblAcca, &tTblTrdef, ptL065InOut, chBankacct, chBankacc1);
        if (snResult != 0)
        {
            CmdLog(LOGINF, "L065_auto_jonal error snResult[%ld]", snResult);
            return snResult;
        }       
        /*End:  added by yyl for CHG-131226-0008*/
    }
    /*End:   added by yyl for CHG-130115-0024 on 2013-05-10*/

    /*对核销特殊处理的，将备份信息恢复*/
    if(1 == unWrite_off)
    {
        ptL065InOut->unTrans_type = unTrans_type_bak;
        ptL065InOut->snAmount = snAmount_bak;
        ptL065InOut->cO_r_flag = cO_R_flag_bak;
        memcpy(&tTblTrdef, &tTblTrdef_bak, sizeof(trdef_tdef));
    }

    /*处理返回值*/
    memcpy(ptTblTrdef,&tTblTrdef,sizeof(trdef_tdef));
    /*Begin: moded by yyl for CHG-131119-0006*/
    if ((strcmp(ptL065InOut->chInp_source, "MBKIO") != 0)
             && (strcmp(ptL065InOut->chInp_source, "CUPIO") != 0))
    {
        memset(ptL065InOut->chPipe, 0x00, sizeof(ptL065InOut->chPipe));
        memset(ptL065InOut->chInp_source, 0x00, sizeof(ptL065InOut->chInp_source));
    }
    /*Begin: moded by yyl for CHG-131119-0006*/
    
    memset(ptL065InOut->chCode_value, 0x00, sizeof(ptL065InOut->chCode_value));
    return 0;
    
}

/********************************************************************************
*  Function:  L065_1_trac_opr
*
*  该函数用于插入或更新总账TRAC
*
*  @Parameters:
*  输入：cct_tdef *ptTblAcct    数据库表/本币账户ACCT
*        UN4  L065_trans_type   交易类型
*        UN3  L065_curr_num     交易币种
*        CH   L065_o_r_flag     O:原始交易 R:撤销交易
*        UN5  L065_proc_day     入账日期 
*        SN11_2 L065_amount     交易金额
*        CH   *pL065_bankacct   借方科目
*        CH   *pL065_bankacct1  贷方科目
*  输出：无
*
*  @return: 0:成功
*           1:日期不合法
*           2:trac初始化记录不存在
*           <0:数据库错
********************************************************************************/
int L065_1_trac_opr(acct_tdef *ptTblAcct,
                    UN4 L065_trans_type,
                    UN3 L065_curr_num,
                    CH  L065_o_r_flag,
                    UN5  L065_proc_day,
                    SN11_2 L065_amount,
                    CH *pL065_bankacct,
                    CH *pL065_bankacct1)
{
    SN6 snResult = 0;
    UN5 unProc_day_new = 0;/*TRACB的PROC_DAY*/
    UN5 unNext_day_new = 0;/*TRACB的NEXT_DAY*/
    CH  chO_R_flag[1+1];   /*L065_o_r_flag*/
    CH  chBP_flag[1+1]; 
    SN4 snCategory = 0;    /*added by yyl for CHG-130218-0019 on 2013-06-25*/
    
    trac_tdef tTblTrac;         /*用于存放当前处理的TRACB结果*/
    tracflag_tdef tTblTracFlag; /*用于修改TRAC记录*/
    trac_tdef tTblTrac_init;    /*用于在新增TRACB时获取PROC_DAY和NEXT_DAY*/
    
    /*交易金额为0,不更新会计总账TRAC*/
    if(0 == CmdDblCmp(L065_amount, 0, 2))
    {
        return 0;
    }
    
    /*局部变量初始化*/
    memset(&tTblTrac, 0x00, sizeof(trac_tdef));
    memset(&tTblTracFlag, 0x00, sizeof(tracflag_tdef));
    memset(&tTblTrac_init, 0x00, sizeof(trac_tdef));    
    chO_R_flag[0] = L065_o_r_flag;
    chO_R_flag[1] = '\0';
    /*chBP_flag[0] = L065_tran_status;  DEL CASE-0000-2011-0274 */
    /*ADD CASE-0000-2011-027*/
    if(0 == CmdGL(ptTblAcct->BUSINESS))
    {        
        chBP_flag[0] = 'P';
    }
    else
    {
        chBP_flag[0] = 'B';        
    }
    /*END ADD CASE-0000-2011-027*/            
    chBP_flag[1] = '\0'; 

    /*Begin:added by yyl for CHG-130218-0019 on 2013-06-25*/
    snCategory = ptTblAcct->CATEGORY;
    if (0 == CmdGL(ptTblAcct->ROWID))
    {
        snCategory = 0;
    }
    /*End:  added by yyl for CHG-130218-0019 on 2013-06-25*/
            
    /*加锁查询TRAC记录*/
    /*snResult = Ptracprodsec(L065_bank, L065_trans_type, L065_curr_num, \ */ /*DEL CASE-0000-2011-0274*/
    /*snResult = Ptracprodsec(ptTblAcct->BANK, L065_trans_type, L065_curr_num,*/ /*ADD CASE-0000-2011-0274*//*DEL CHG-130218-0019*/
        /*chO_R_flag, chBP_flag, &tTblTrac);*/
    snResult = Ptracprodsec(ptTblAcct->BANK, snCategory, L065_trans_type, L065_curr_num, /*ADD CHG-130218-0019*/
        chO_R_flag, chBP_flag, &tTblTrac);
    if((snResult != 0) && (snResult != DB_NOT_FOUND))
    {
        CmdLog(LOGINF, "Ptracprodsec error snResult[%ld]", snResult);
        return -1;
    }
    
    if(DB_NOT_FOUND == snResult)/*TRAC不存在,则插入TRAC*/
    {
         /*查询任意一条TRAC记录以获取TRAC的PROC_DAY和NEXT_DAY*/ 
        memset(&tTblTrac_init, 0x00, sizeof(trac_tdef));
        /*snResult = PtracprodOpen(DB_CURFROM1, L065_bank, 0, 0, " ", " ");*//*DEL CASE-0000-2011-0274*/
        /*snResult = PtracprodOpen(DB_CURFROM1, ptTblAcct->BANK, 0, 0, " ", " ");*//*ADD CASE-0000-2011-0274*//*DEL CHG-130218-0019*/
        snResult = PtracprodOpen(DB_CURFROM1, ptTblAcct->BANK, 0, 0, 0, " ", " ");/*ADD ADD CHG-130218-0019*/
        if(snResult != 0)
        {
            CmdLog(LOGINF, "PtracprodOpen error snResult[%d]", snResult);
            return -1;
        }
        snResult = PtracprodFetch(DB_CURFROM1, &tTblTrac_init);
        if((snResult != 0) && (snResult != DB_NOT_FOUND))
        {
            CmdLog(LOGINF, "PtracprodFetch error snResult[%ld]", snResult);
            PtracprodClose(DB_CURFROM1);
            return -1;
        }
        if(DB_NOT_FOUND == snResult)
        {
            CmdLog(LOGERR, "PtracprodFetch error snResult[%ld] BANK[%d]", snResult, ptTblAcct->BANK);
            PtracprodClose(DB_CURFROM1);
            return 2;
        }
        snResult = PtracprodClose(DB_CURFROM1);
        if(snResult != 0)
        {
            CmdLog(LOGINF, "PtracprodClose error snResult[%ld]", snResult);
            return -1;
        }

        unProc_day_new = tTblTrac_init.PROC_DAY;
        unNext_day_new = tTblTrac_init.NEXT_DAY;
        
        /*日期不合法*/
        if((L065_proc_day != unProc_day_new) && (L065_proc_day != unNext_day_new))
        {
            CmdLog(LOGERR, "TRAC DATE error PROC_DAY[%d] NEXT_DAY[%d] L065_proc_day[%d] ",\
                unProc_day_new, unNext_day_new, L065_proc_day);
            return 1;
        }
            
        if((unProc_day_new > 0) && (unNext_day_new > 0))
        {
            tTblTrac.BANK = ptTblAcct->BANK;
            tTblTrac.PROC_DAY = unProc_day_new;
            tTblTrac.NEXT_DAY = unNext_day_new;
            tTblTrac.TRANS_TYPE = L065_trans_type;
            tTblTrac.CURR_NUM = L065_curr_num;
            tTblTrac.O_R_FLAG[0] = L065_o_r_flag;
            /*ADD CASE-0000-2011-0274*/
            if(0 == CmdGL(ptTblAcct->BUSINESS))
            {
                tTblTrac.BP_FLAG[0] = 'P';  
            }
            else
            {
                tTblTrac.BP_FLAG[0] = 'B'; 
            }
            /*END ADD CASE-0000-2011-0274*/
            /*tTblTrac.BP_FLAG[0] = L065_tran_status;*/ /*DEL CASE-0000-2011-0274*/
            CmdA2A(pL065_bankacct, sizeof(tTblTrac.BANKACCT), tTblTrac.BANKACCT);
            CmdA2A(pL065_bankacct1, sizeof(tTblTrac.BANKACC1), tTblTrac.BANKACC1);          
            
            if(L065_proc_day == unProc_day_new)
            {
                tTblTrac.AMT_TODAY = L065_amount;
                tTblTrac.NBR_TODAY = 1;            
            }
            else if(L065_proc_day == unNext_day_new)
            {
                tTblTrac.AMT_NXTDAY = L065_amount;
                tTblTrac.NBR_NXTDAY += 1;            
            }

            /*Begin:added by yyl for CHG-130218-0019 on 2013-06-25*/
            tTblTrac.CATEGORY = snCategory;
            /*End:  added by yyl for CHG-130218-0019 on 2013-06-25*/
            
            snResult = TracInsert(&tTblTrac);
            if(snResult != 0)
            {
                CmdLog(LOGINF, "TracInsert error snResult[%ld]", snResult);
                return -1;
            }  
        }
        else
        {
            CmdLog(LOGERR, "TRAC DATE error snResult[%ld] PROC_DAY[%d] NEXT_DAY[%d]",\
                snResult, unProc_day_new, unNext_day_new);
            return 2;
        }          
    }/*end if DB_NOT_FOUND:TRAC*/
    else
    {
        /*日期不合法*/
        if((L065_proc_day != tTblTrac.PROC_DAY) && (L065_proc_day != tTblTrac.NEXT_DAY))
        {
            CmdLog(LOGERR, "TRAC DATE error PROC_DAY[%d] NEXT_DAY[%d] L065_proc_day[%d] ",\
                tTblTrac.PROC_DAY, tTblTrac.NEXT_DAY, L065_proc_day);
            return 1;
        }
        
        if(L065_proc_day == tTblTrac.PROC_DAY)
        {
            tTblTrac.AMT_TODAY += L065_amount;
            tTblTracFlag.AMT_TODAY = 1;
            tTblTrac.NBR_TODAY += 1;
            tTblTracFlag.NBR_TODAY = 1;

            /*Begin:added by yyl for CHG-130218-0019 on 2013-07-08*/
            CmdA2A(pL065_bankacct, sizeof(tTblTrac.BANKACCT), tTblTrac.BANKACCT);
            tTblTracFlag.BANKACCT = 1;
            CmdA2A(pL065_bankacct1, sizeof(tTblTrac.BANKACC1), tTblTrac.BANKACC1);
            tTblTracFlag.BANKACC1= 1;
            /*End:  added by yyl for CHG-130218-0019 on 2013-07-08*/
        }
        else if(L065_proc_day == tTblTrac.NEXT_DAY)
        {
            tTblTrac.AMT_NXTDAY += L065_amount;
            tTblTracFlag.AMT_NXTDAY = 1;
            tTblTrac.NBR_NXTDAY += 1;
            tTblTracFlag.NBR_NXTDAY = 1; 

            /*Begin:added by yyl for CHG-130218-0019 on 2013-07-08*/
            CmdA2A(pL065_bankacct, sizeof(tTblTrac.BANKACCT), tTblTrac.BANKACCT);
            tTblTracFlag.BANKACCT = 1;
            CmdA2A(pL065_bankacct1, sizeof(tTblTrac.BANKACC1), tTblTrac.BANKACC1);
            tTblTracFlag.BANKACC1= 1;
            /*End:  added by yyl for CHG-130218-0019 on 2013-07-08*/
        }

        snResult = TracUpdate(&tTblTrac,&tTblTracFlag);
        if(snResult != 0)
        {
            CmdLog(LOGINF, "TracUpdate error snResult[%ld]", snResult);
            return -1;
        }      
    }
    return 0;
}

/********************************************************************************
*  Function:  L065_2_tracb_opr
*
*  该函数用于插入或更新总账TRACB
*
*  @Parameters:
*  输入：acct_tdef *ptTblAcct   数据库记录/本币账户记录
*        UN4  L065_trans_type   交易类型
*        UN3  L065_curr_num     交易币种
*        CH   L065_o_r_flag     O:原始交易 R:撤销交易
*        CH   L065_tran_status  B:公司卡 P:个人卡
*        UN5  L065_proc_day     入账日期 
*        SN11_2 L065_amount     交易金额
*  输出：无
*
*  @return: 0:成功
*           1:日期不合法
*           2:tracb初始化记录不存在
*           4:币种不合法
********************************************************************************/
int L065_2_tracb_opr(acct_tdef *ptTblAcct,
                     UN4 L065_trans_type,
                     UN3 L065_curr_num,
                     CH  L065_o_r_flag,
                     CH *pL065_trans_src,
                     UN5 L065_proc_day,
                     SN11_2 L065_amount)
{
    SN6 snResult = 0;
    UN5 unProc_day = 0;    /*TRACB的PROC_DAY*/
    UN5 unNext_day = 0;    /*TRACB的NEXT_DAY*/
    CH  chO_R_flag[1+1];   /*L065_o_r_flag*/
    CH  chBP_flag[1+1];    /*对应L065_tran_status*/
    SN4 snCategory = 0;    /*added by yyl for CHG-130218-0019 on 2013-06-25*/
    
    tracb_tdef tTblTracb;  /*用于存放当前处理的TRACB结果*/
    tracbflag_tdef tTblTracbFlag; /*用于修改TRACB记录*/
    tracb_tdef tTblTracb_init; /*用于在新增TRACB时获取PROC_DAY和NEXT_DAY*/

    if(0 == CmdDblCmp(L065_amount,0,2))
    {
        return 0;
    }
    if(0 == CmdGL(pL065_trans_src))
    {
        return 0;
    }

    /*局部变量初始化*/
    memset(&tTblTracb, 0x00, sizeof(tracb_tdef));
    memset(&tTblTracbFlag, 0x00, sizeof(tracbflag_tdef));
    memset(&tTblTracb_init, 0x00, sizeof(tracb_tdef));
    chO_R_flag[0] = L065_o_r_flag;
    chO_R_flag[1] = '\0';
    /*chBP_flag[0] = L065_tran_status;  DEL CASE-0000-2011-0274 */
    /*ADD CASE-0000-2011-027*/
    if(0 == CmdGL(ptTblAcct->BUSINESS))
    {        
        chBP_flag[0] = 'P';
    }
    else
    {
        chBP_flag[0] = 'B';        
    }
    /*END ADD CASE-0000-2011-027*/   
    chBP_flag[1] = '\0';

    /*Begin:added by yyl for CHG-130218-0019 on 2013-06-25*/
    snCategory = ptTblAcct->CATEGORY;
    if (0 == CmdGL(ptTblAcct->ROWID))
    {
        snCategory = 0;
    }
    /*End:  added by yyl for CHG-130218-0019 on 2013-06-25*/
    
    /*snResult = Ptracbsec(L065_bank, L065_trans_type, L065_curr_num, \      DEL CASE-0000-2011-0274*/
    /*snResult = Ptracbsec(ptTblAcct->BANK, L065_trans_type, L065_curr_num,*/ /*ADD CASE-0000-2011-0274*//*DEL CHG-130218-0019*/
        /*chO_R_flag, chBP_flag, pL065_trans_src, &tTblTracb); */
    snResult = Ptracbsec(ptTblAcct->BANK, snCategory, L065_trans_type, L065_curr_num, /*ADD CHG-130218-0019*/
        chO_R_flag, chBP_flag, pL065_trans_src, &tTblTracb);
    if((snResult != 0) && (snResult != DB_NOT_FOUND))
    {
        CmdLog(LOGINF, "Ptracbsec error snResult[%ld]", snResult);
        return -1;
    }
    if(DB_NOT_FOUND == snResult)/*TRACB不存在,则插入TRACB*/
    {
        /*查询任意一条TRACB记录以获取TRACB的PROC_DAY和NEXT_DAY*/ 
        memset(&tTblTracb_init, 0x00, sizeof(tracb_tdef));
        /*snResult = PtracbOpen(DB_CURFROM1, L065_bank, 0, 0, " ", " ", " "); DEL CASE-0000-2011-0274*/
        /*snResult = PtracbOpen(DB_CURFROM1, ptTblAcct->BANK, 0, 0, " ", " ", " ");*/ /*ADD CASE-0000-2011-0274*//*DEL CHG-130218-0019*/
        snResult = PtracbOpen(DB_CURFROM1, ptTblAcct->BANK, 0, 0, 0, " ", " ", " "); /*ADD CHG-130218-0019*/
        if(snResult != 0)
        {
            CmdLog(LOGINF, "PtracbOpen error snResult[%ld]", snResult);
            return -1;
        }        
        snResult = PtracbFetch(DB_CURFROM1, &tTblTracb_init);
        if((snResult != 0) &(snResult != DB_NOT_FOUND))
        {
            CmdLog(LOGINF, "PtracbFetch error snResult[%ld]", snResult);
            PtracbClose(DB_CURFROM1);
            return -1;
        }
        if(DB_NOT_FOUND == snResult)
        {
            CmdLog(LOGERR, "PtracbFetch error snResult[%ld] BANK[%d]", snResult, ptTblAcct->BANK);
            PtracbClose(DB_CURFROM1);
            return 2;
        }
        snResult = PtracbClose(DB_CURFROM1);
        if(snResult != 0)
        {
            CmdLog(LOGINF, "PtracbClose error snResult[%ld]", snResult);
            return -1;
        }

        unProc_day = tTblTracb_init.PROC_DAY;
        unNext_day = tTblTracb_init.NEXT_DAY;
        
        /*日期不合法*/
        if((L065_proc_day != unProc_day) && (L065_proc_day != unNext_day))
        {
            CmdLog(LOGERR, "TRAC DATE error PROC_DAY[%d] NEXT_DAY[%d] L065_proc_day[%d] ",\
                unProc_day, unNext_day, L065_proc_day);
            return 1;
        }
        
        if((unProc_day > 0) && (unNext_day > 0))
        {
            /*日期合法，插入TRACB记录*/
            /*tTblTracb.BANK = L065_bank; DEL CASE-0000-2011-0274*/
            tTblTracb.BANK = ptTblAcct->BANK; /*ADD CASE-0000-2011-0274*/
            tTblTracb.PROC_DAY = unProc_day;
            tTblTracb.NEXT_DAY = unNext_day;
            tTblTracb.TRANS_TYPE = L065_trans_type;
            tTblTracb.CURR_NUM = L065_curr_num;
            tTblTracb.O_R_FLAG[0] = L065_o_r_flag;
            /*ADD CASE-0000-2011-0274*/
            if(0 == CmdGL(ptTblAcct->BUSINESS))
            {
                tTblTracb.BP_FLAG[0] = 'P';  
            }
            else
            {
                tTblTracb.BP_FLAG[0] = 'B'; 
            }
            /*END ADD CASE-0000-2011-0274*/
            /*tTblTracb.BP_FLAG[0] = L065_tran_status;DEL CASE-0000-2011-0274*/
            CmdA2A(pL065_trans_src,sizeof(tTblTracb.TRANS_SRC),tTblTracb.TRANS_SRC);
            if(L065_proc_day == unProc_day)
            {
                tTblTracb.AMT_TODAY = L065_amount;
                tTblTracb.NBR_TODAY = 1;            
            }
            else if(L065_proc_day == unNext_day)
            {
                tTblTracb.AMT_NXTDAY = L065_amount;
                tTblTracb.NBR_NXTDAY += 1;            
            }
            
            /*Begin:added by yyl for CHG-130218-0019 on 2013-06-25*/
            tTblTracb.CATEGORY = snCategory;
            /*End:  added by yyl for CHG-130218-0019 on 2013-06-25*/
            
            snResult = TracbInsert(&tTblTracb);
            
            if(snResult != 0)
            {
                CmdLog(LOGINF, "TracbInsert error snResult[%ld]", snResult);
                return -1;
            }  
        }
        else
        {
            /*日期不合法，失败返回*/
            CmdLog(LOGERR, "TRACB DATE error snResult[%ld] PROC_DAY[%d] NEXT_DAY[%d]",\
                snResult, unProc_day, unNext_day);
            return 2;
        }        
    }
    else/*TRACB存在,则更新TRACB*/
    {
        /*币种不匹配*/
        if(L065_curr_num != tTblTracb.CURR_NUM)
        {
            CmdLog(LOGERR, "TRACB curr <> L065_curr_num TRACB.CURR_NUM[%d] L065_curr_num[%d]",\
                tTblTracb.CURR_NUM, L065_curr_num);
            return 4;
        }
        
        /*日期不匹配*/
        if((L065_proc_day != tTblTracb.PROC_DAY) && (L065_proc_day != tTblTracb.NEXT_DAY))
        {
            CmdLog(LOGERR, "TRACB DATE error PROC_DAY[%d] NEXT_DAY[%d] L065_proc_day[%d] ",\
                tTblTracb.PROC_DAY, tTblTracb.NEXT_DAY, L065_proc_day);
            return 1;
        }

        /*累计TRACB交易笔数和交易金额*/
        if(L065_proc_day == tTblTracb.PROC_DAY)
        {
            tTblTracb.AMT_TODAY += L065_amount;
            tTblTracbFlag.AMT_TODAY = 1;
            tTblTracb.NBR_TODAY += 1;            
            tTblTracbFlag.NBR_TODAY = 1;
        }
        else if(L065_proc_day == tTblTracb.NEXT_DAY)
        {
            tTblTracb.AMT_NXTDAY += L065_amount;
            tTblTracbFlag.AMT_NXTDAY = 1;
            tTblTracb.NBR_NXTDAY += 1;            
            tTblTracbFlag.NBR_NXTDAY = 1;
        } 
        snResult = TracbUpdate(&tTblTracb,&tTblTracbFlag);
        if(snResult != 0)
        {
            CmdLog(LOGINF, "TracbUpdate error snResult[%d]", snResult);
            return -1;
        }
    }
    
    return 0;
}

/********************************************************************************
*  Function:  L065_3_jorj_ins
*
*  该函数用于插入或更新总账TRACB
*
*  @Parameters:
*  输入：trdef_tdef  *ptTblTrdef 数据库表TRDEF结构体
*        acct_tdef *ptTblAcct    数据库表本币账户ACCT
*        CH chDescription[36+1] 交易描述
*        SN4 snBacctcode        会计核算代码
*  输入&输出：CH pL065_bankacct 借方科目
*             CH pL065_bankacc1 贷方科目
*             L065_io_sdef *ptL065InOut
*
*  @return: 0:成功
*           <>0:失败
********************************************************************************/
int L065_3_jorj_ins(acct_tdef *ptTblAcct,
                        trdef_tdef  *ptTblTrdef,                    
                        CH chDescription[36+1],
                        SN4 snBacctcode,
                        CH *pL065_bankacct,
                        CH *pL065_bankacc1, 
                        L065_io_sdef *ptL065InOut)
{
    merch_tdef tTblMerch;
    bacct_tdef tTblBacct;
    jorj_tdef  tTblJorj;
    codes_tdef tTblCodes;
    time_sdef tTimeInp;
    
    SN6 snResult = 0;
    UN1 unSkip = 0;
    /*Begin:added by yyl for CHG-130218-0019 on 2013-07-08*/
    UN1 unFlag = 0;  
    prbct_tdef tTblPrbct;
    /*End:  added by yyl for CHG-130218-0019 on 2013-07-08*/
    
    /*变量初始化*/
    memset(&tTblJorj, 0x00, sizeof(jorj_tdef));
    memset(&tTimeInp, 0x00, sizeof(time_sdef));
    
    CmdA2A(pL065_bankacct, sizeof(tTblJorj.BANKACCT), tTblJorj.BANKACCT);
    CmdA2A(pL065_bankacc1, sizeof(tTblJorj.BANKACC1), tTblJorj.BANKACC1);
    
    /*流水号要求输入,但随机数有可能为0,因此不报错,仅打印INF日志*/
    if(0 == ptL065InOut->unJorj_serial)
    {
        CmdLog(LOGINF, "SERIAL NO IS 0, TRANS_SRC[%s], TRANS_TYPE[%d]", \
            ptL065InOut->chTrans_src, ptL065InOut->unTrans_type);
    }

    /*商户号非空，查找MERCHANT记录,替换商户清算科目*/
    if(CmdGL(ptL065InOut->chMerchant) != 0)
    {
        memset(&tTblMerch, 0x00, sizeof(merch_tdef));
        /*snResult = Pmerchbnk(ptL065InOut->unBank, ptL065InOut->chMerchant, &tTblMerch); DEL CASE-0000-2011-0274*/
        snResult = Pmerchbnk(ptTblAcct->BANK, ptL065InOut->chMerchant, &tTblMerch); /*ADD CASE-0000-2011-0274*/
        if((snResult != 0) && (snResult != DB_NOT_FOUND))/*查询失败*/
        {
            CmdLog(LOGINF, "Pmerchbnk error snResult[%ld]", snResult);
            return -1;
        }
        if(0 == snResult)
        {
            /*查询借方科目*/
            memset(&tTblBacct, 0x00, sizeof(tTblBacct));
            /*snResult = Pbacct(ptL065InOut->unBank, tTblJorj.BANKACCT, &tTblBacct);DEL CASE-0000-2011-0274*/
            snResult = Pbacct(ptTblAcct->BANK, tTblJorj.BANKACCT, &tTblBacct);/*ADD CASE-0000-2011-0274*/
            if((snResult != 0) && (snResult != DB_NOT_FOUND))/*查询失败*/
            {
                CmdLog(LOGINF, "Pbacct error snResult[%ld]", snResult);
                return -1;
            }

            if(0 == snResult)/*BACCT记录存在*/
            {            
                if('1' == tTblBacct.RES_FLAG[0])
                {
                    /*CmdA2A(tTblMerch.DEP_ACCT, sizeof(tTblJorj.BANKACCT), tTblJorj.BANKACCT);*/ /*Del by yyl for CHG-130218-0019 on 2013-07-24*/
                    CmdA2A(tTblMerch.DEP_ACCT, 21, pL065_bankacct); /*added by yyl for CHG-130218-0019 on 2013-07-24*/
                    unSkip = 1;
                    unFlag = 1; /*added by yyl for CHG-130218-0019 on 2013-07-08*/
                }
            }
            
            /*查询贷方科目*/
            if(0 == unSkip)/*要求SKIP = 0,只替换一方科目*/
            {
                memset(&tTblBacct, 0x00, sizeof(tTblBacct));
                /*snResult = Pbacct(ptL065InOut->unBank, tTblJorj.BANKACC1, &tTblBacct);DEL CASE-0000-2011-0274*/
                snResult = Pbacct(ptTblAcct->BANK, tTblJorj.BANKACC1, &tTblBacct); /*ADD CASE-0000-2011-0274*/
                if((snResult != 0) && (snResult != DB_NOT_FOUND))/*查询失败*/
                {
                    CmdLog(LOGINF, "Pbacct error snResult[%ld]", snResult);
                    return -1;
                }
                if(0 == snResult)/*BACCT记录存在*/
                {
                    if('1' == tTblBacct.RES_FLAG[0])
                    {
                        /*CmdA2A(tTblMerch.DEP_ACCT, sizeof(tTblJorj.BANKACC1), tTblJorj.BANKACC1);*/ /*Del by yyl for CHG-130218-0019 on 2013-07-24*/
                        CmdA2A(tTblMerch.DEP_ACCT, 21, pL065_bankacc1); /*added by yyl for CHG-130218-0019 on 2013-07-24*/
                        unFlag = 1; /*added by yyl for CHG-130218-0019 on 2013-07-08*/
                    }                
                }
            }
        }
    }/*End if 商户号非空*/

    /*费用细分,替换科目*/
    unSkip = 0;
    if(('1' == ptTblTrdef->FFLAG[0]) && (CmdGL(ptL065InOut->chCode_value) > 0))
    {
        memset(&tTblCodes, 0x00, sizeof(codes_tdef));
        /*snResult = Pcodes(ptL065InOut->unBank,GC_FEE_TYPE,ptL065InOut->chCode_value,&tTblCodes);DEL CASE-0000-2011-0274*/
        snResult = Pcodes(ptTblAcct->BANK, GC_FEE_TYPE, ptL065InOut->chCode_value, &tTblCodes);/*ADD CASE-0000-2011-0274*/
        if((snResult != 0) && (snResult != DB_NOT_FOUND))/*查询CODES失败*/
        {
            CmdLog(LOGINF, "Pcodes error snResult[%ld]", snResult);
            return -1;
        }
        if(0 == snResult)/*CODES记录存在*/
        {
            /*查询借方科目*/
            /*snResult = Pbacct(ptL065InOut->unBank, tTblJorj.BANKACCT, &tTblBacct);DEL CASE-0000-2011-0274*/
            snResult = Pbacct(ptTblAcct->BANK, tTblJorj.BANKACCT, &tTblBacct);/*ADD CASE-0000-2011-0274*/
            if((snResult != 0) && (snResult != DB_NOT_FOUND))/*查询失败*/
            {
                CmdLog(LOGWAN, "Pbacct error snResult[%ld]", snResult);
                return -1;
            }
            if(0 == snResult)/*BACCT记录存在*/
            {
                if('1' == tTblBacct.RES_FLAG[0])
                {
                    CmdRTrim(tTblCodes.VALUE_DEF);   
                    /*CmdA2A(tTblCodes.VALUE_DEF, sizeof(tTblJorj.BANKACCT), tTblJorj.BANKACCT);*/ /*Del by yyl for CHG-130218-0019 on 2013-07-24*/
                    CmdA2A(tTblCodes.VALUE_DEF, 21, pL065_bankacct); /*added by yyl for CHG-130218-0019 on 2013-07-24*/
                    unSkip = 1;
                    unFlag = 1; /*added by yyl for CHG-130218-0019 on 2013-07-08*/
                }
            }

            /*查询贷方科目*/
            if(0 == unSkip)/*要求SKIP = 0,只替换一方科目*/
            {
                memset(&tTblBacct,0x00,sizeof(tTblBacct));
                /*snResult = Pbacct(ptL065InOut->unBank,tTblJorj.BANKACC1,&tTblBacct);DEL CASE-0000-2011-0274*/
                snResult = Pbacct(ptTblAcct->BANK, tTblJorj.BANKACC1, &tTblBacct);/*ADD CASE-0000-2011-0274*/
                if((snResult != 0) && (snResult != DB_NOT_FOUND))/*查询失败*/
                {
                    CmdLog(LOGWAN, "Pbacct error snResult[%ld]", snResult);
                    return -1;
                }
                if(0 == snResult)
                {
                    if('1' == tTblBacct.RES_FLAG[0])
                    {
                        CmdRTrim(tTblCodes.VALUE_DEF); 
                        /*CmdA2A(tTblCodes.VALUE_DEF, sizeof(tTblJorj.BANKACC1), tTblJorj.BANKACC1);*/ /*Del by yyl for CHG-130218-0019 on 2013-07-24*/
                        CmdA2A(tTblCodes.VALUE_DEF, 21, pL065_bankacc1); /*added by yyl for CHG-130218-0019 on 2013-07-24*/
                        unFlag = 1; /*added by yyl for CHG-130218-0019 on 2013-07-08*/
                    }
                }                
            }            
        }          
    }/*End 费用细分处理*/

    /*Begin:added by yyl for CHG-130218-0019 on 2013-07-08*/
    if ((snBacctcode != 0) && (1 == unFlag))
    {
        /*借方科目替换*/
        memset(&tTblPrbct, 0x00, sizeof(prbct_tdef));
        snResult = PprbctOpen(DB_CURFROM4, ptTblAcct->BANK, pL065_bankacct, snBacctcode, ptL065InOut->unCurr_num, ptL065InOut->unProc_day);
        if (snResult != 0)
        {
            CmdLog(LOGWAN, "PprbctOpen error snResult[%ld], Bankacct[%s]", snResult, pL065_bankacct);
            return -1;
        }

        snResult = PprbctFetch(DB_CURFROM4, &tTblPrbct);
        if ((snResult != 0) && (snResult != DB_NOT_FOUND))
        {
            CmdLog(LOGWAN, "PprbctFetch error snResult[%ld], Bankacct[%s]", snResult, pL065_bankacct);
            PprbctClose(DB_CURFROM4);
            return -1;
        }        
        else if (0 == snResult)
        {
            CmdA2A(tTblPrbct.SUB_BACCT, 21, pL065_bankacct);
        }

        snResult = PprbctClose(DB_CURFROM4);
        if (snResult != 0)
        {
            CmdLog(LOGWAN, "PprbctClose error snResult[%ld], Bankacct[%s]", snResult, pL065_bankacct);
            return -1;
        } 

        /*贷方科目替换*/
        memset(&tTblPrbct, 0x00, sizeof(prbct_tdef));
        snResult = PprbctOpen(DB_CURFROM4, ptTblAcct->BANK, pL065_bankacc1, snBacctcode, ptL065InOut->unCurr_num, ptL065InOut->unProc_day);
        if (snResult != 0)
        {
            CmdLog(LOGWAN, "PprbctOpen error snResult[%ld], Bankacc1[%s]", snResult, pL065_bankacc1);
            return -1;
        }
        
        snResult = PprbctFetch(DB_CURFROM4, &tTblPrbct);
        if ((snResult != 0) && (snResult != DB_NOT_FOUND))
        {
            CmdLog(LOGWAN, "PprbctFetch error snResult[%ld], Bankacc1[%s]", snResult, pL065_bankacc1);
            PprbctClose(DB_CURFROM4);
            return -1;
        }     
        else if (0 == snResult)
        {
            CmdA2A(tTblPrbct.SUB_BACCT, 21, pL065_bankacc1);
        }

        snResult = PprbctClose(DB_CURFROM4);
        if (snResult != 0)
        {
            CmdLog(LOGWAN, "PprbctClose error snResult[%ld], Bankacc1[%s]", snResult, pL065_bankacc1);
            return -1;
        }
    }
    /*End:  added by yyl for CHG-130218-0019 on 2013-07-08*/   

    /*ADD CHG-120529-0011 IC卡特色交易*/
    snResult = L065_4_bacct_upd(ptL065InOut, pL065_bankacct, pL065_bankacc1);
    if(snResult != 0)
    {
        return snResult;
    }    
    /*END ADD CHG-120529-0011*/
    
    /*对JORJ记录赋值,并插入JORJ*/
    /*Begin:added by yyl for CHG-130218-0019 on 2013-07-08*/
    CmdA2A(pL065_bankacct, sizeof(tTblJorj.BANKACCT), tTblJorj.BANKACCT);
    CmdA2A(pL065_bankacc1, sizeof(tTblJorj.BANKACC1), tTblJorj.BANKACC1);
    /*End:  added by yyl for CHG-130218-0019 on 2013-07-08*/
    
    /*tTblJorj.BANK = ptL065InOut->unBank;DEL CASE-0000-2011-0274*/
    tTblJorj.BANK = ptTblAcct->BANK; /*ADD CASE-0000-2011-0274*/
    if(0 == CmdGL(chDescription))
    {
        CmdA2A(ptTblTrdef->DESC_PRINT, sizeof(tTblJorj.DESCRIPTN), tTblJorj.DESCRIPTN);
    }
    else
    {
        CmdA2A(chDescription, sizeof(tTblJorj.DESCRIPTN), tTblJorj.DESCRIPTN);
    }
    tTblJorj.CURR_NUM = ptTblTrdef->CURR_NUM;
    if(0 == ptL065InOut->unTran_time)
    {
        CmdMT(&tTimeInp);
        tTblJorj.INP_TIME = tTimeInp.time_HHMMSSss_N; 
        ptL065InOut->unTran_time = tTimeInp.time_HHMMSSss_N;        
    }
    else
    {
        tTblJorj.INP_TIME += ptL065InOut->unTran_time + 1;
        /*当L065_trac_update函数在循环中使用，且交易类型，账户等关键索引都一致时，*/
        /*此时由于ptL065InOut->unTran_time不变，会将相同的记录插入JORJ表的INP_TIME字段也一致，*/
        /*这样就会导致JORJ表里相同索引的数据有多条，*/
        /*这样在批处理中使用JORJ表时，由于游标4s会重新打开，这样会导致相同的记录被重复查询处理。*/
        /*例如GLREPORT批量处理JORJ进行会计并账时会出现此问题，GLTMP表重复插入、从而导致分户账不平等一系列问题*/
        ptL065InOut->unTran_time = tTblJorj.INP_TIME;       
    }
    tTblJorj.O_R_FLAG[0] = ptL065InOut->cO_r_flag;
    tTblJorj.O_R_FLAG[1] = '\0';
    tTblJorj.REL_DAY = ptL065InOut->unProc_day;
    tTblJorj.SERIAL_NO = ptL065InOut->unJorj_serial;
    if(ptL065InOut->snAmount < 0)
    {
        CmdA2A("1099",sizeof(tTblJorj.TRAN_CODE), tTblJorj.TRAN_CODE);
        tTblJorj.TRAN_AMT = (-1) * ptL065InOut->snAmount;
    }
    else
    {
        CmdA2A("9099",sizeof(tTblJorj.TRAN_CODE), tTblJorj.TRAN_CODE);
        tTblJorj.TRAN_AMT = ptL065InOut->snAmount;
    }
    tTblJorj.TRANS_TYPE = ptL065InOut->unTrans_type;
    tTblJorj.CARD_BIN = ptL065InOut->unCard_bin;
    /*tTblJorj.XACCOUNT = ptL065InOut->unAccount;DEL CASE-0000-2011-0274*/
    tTblJorj.XACCOUNT = ptTblAcct->XACCOUNT; /*ADD CASE-0000-2011-0274*/
    if(0 == CmdGL(ptL065InOut->chCard_nbr))
    {
        /*记录文件日志*/
        CmdLog(LOGWAN, "CARD_NBR is space BANK[%d] TRANS_TYPE [%d] CURR_NUM[%d] O_R_FLAG[%c] BILL_AMT[%f]", \
            ptTblAcct->BANK, ptL065InOut->unTrans_type,ptL065InOut->unCurr_num, \
            ptL065InOut->cO_r_flag, ptL065InOut->snAmount);
        sprintf(ptL065InOut->chCard_nbr, "%06ld%10ld", ptL065InOut->unCard_bin, ptTblAcct->XACCOUNT);
    }
    else
    {
        CmdA2A(ptL065InOut->chCard_nbr,sizeof(tTblJorj.CARD_NBR),tTblJorj.CARD_NBR);
    }

    /*Begin:added by yyl for CHG-130909-0051*/
    if (GC_BANK_ZJNX == ptTblAcct->BANK)
    {
        snResult = L065_5_brno_chg(ptTblAcct, ptL065InOut);
        if (snResult != 0)
        {
            CmdLog(LOGINF, "L065_5_brno_chg error snResult[%ld]", snResult);
            return -1;
        }
    }
    /*End:  added by yyl for CHG-130909-0051*/
    
    CmdA2A(ptL065InOut->chBrno, sizeof(tTblJorj.BRNO), tTblJorj.BRNO);
    CmdA2A(ptL065InOut->chEmployee, sizeof(tTblJorj.EMPNO), tTblJorj.EMPNO);
    CmdA2A(ptL065InOut->chMerchant, sizeof(tTblJorj.MERCHANT), tTblJorj.MERCHANT);
    /*ADD CASE-0000-2011-0274*/
    CmdA2A(CmdN2A(&ptTblAcct->BRANCH, 4, 0, 0, "001"), sizeof(tTblJorj.OPENBRNO), tTblJorj.OPENBRNO);
    /*EDD ADD CASE-0000-2011-0274*/
    tTblJorj.CATEGORY = ptTblAcct->CATEGORY; /*ADD case-xq-0519-20111109*/
    CmdA2A(ptL065InOut->chPipe, sizeof(tTblJorj.PIPE), tTblJorj.PIPE); /*added by yyl for CHG-130510-0036*/
    snResult = JorjInsert(&tTblJorj);
    if(snResult != 0)
    {
        CmdLog(LOGWAN, "JorjInsert error snResult[%d]", snResult);
        return -1;
    }    
    return 0;
}

/*ADD CHG-120529-0011 IC卡特色交易*/
/********************************************************************************
*  Function:  L065_4_bacct_upd
*
*  该函数用于积分消费交易（652）、积分管理账户资金充值交易（650）的BACCT账户日
*  累计、总累计余额更新逻辑
*
*  @Parameters:
*  输入：L065_io_sdef *ptL065InOut
*        CH *pL065_bankacct 借方科目
*        CH *pL065_bankacc1 贷方科目
*  输出：无
*
*  @return: 0:成功
*           <>0:失败
********************************************************************************/
int L065_4_bacct_upd(L065_io_sdef *ptL065InOut,
                     CH *pL065_bankacct,
                     CH *pL065_bankacc1)
{
    SN6 snResult = 0;
    bacct_tdef tTblBacct;
    bacctflag_tdef tTblBacctFlag;
    banks_tdef tTblBanks; /*added by yyl for CHG-130828-0002*/

    /*初始化*/
    memset(&tTblBacct, 0x00, sizeof(bacct_tdef));
    memset(&tTblBacctFlag, 0x00, sizeof(bacctflag_tdef));
    memset(&tTblBanks, 0x00, sizeof(banks_tdef)); /*added by yyl for CHG-130828-0002*/

    /*Begin:added by yyl for CHG-130828-0002*/
    if ((ptL065InOut->unTrans_type != 650) && (ptL065InOut->unTrans_type != 652))
    {
        return 0;
    }

    /*积分总量更新*/
    if ((ptL065InOut->snAmount > 0) && (ptL065InOut->snPoint < 0))
    {
        ptL065InOut->snPoint *= (-1);
    }
    if ((ptL065InOut->snAmount < 0) && (ptL065InOut->snPoint > 0))
    {
        ptL065InOut->snPoint *= (-1);
    }

    snResult = Fbanks(GLB_BANK, &tTblBanks);
    if((snResult != 0) && (snResult != DB_NOT_FOUND))
    {
        CmdLog(LOGWAN, "Fbanks error snResult[%ld]", snResult);        
        return -1;
    }
    if(DB_NOT_FOUND == snResult)
    {
        CmdLog(LOGWAN, "Fbanks error not found");        
        return 10;   
    }

    if((2 == tTblBanks.PTTOL_YN) && (652 == ptL065InOut->unTrans_type))
    {
        snResult = Pbacctypesec(GLB_BANK, "20", &tTblBacct);
        if((snResult != 0) && (snResult != DB_NOT_FOUND))/*查询失败*/
        {
            CmdLog(LOGWAN, "Pbacctsec error snResult[%ld]", snResult);
            return -1;
        }
        
        if((0 == snResult) && ('B' == tTblBacct.GLFLAG[0]))/*BACCT记录存在*/
        {
            if(ptL065InOut->unProc_day == tTblBacct.PROC_DAY)
            {
                if(GC_ORIG[0] == ptL065InOut->cO_r_flag)
                {
                    if(ptL065InOut->snAmount > 0)
                    {
                        tTblBacct.DRAMT_TODY += ptL065InOut->snPoint;
                        tTblBacctFlag.DRAMT_TODY = 1;
                    }
                    else
                    {
                        tTblBacct.DRAMT_TODY -= ptL065InOut->snPoint;
                        tTblBacctFlag.DRAMT_TODY = 1;
                    }
                }
                else
                {
                    if(ptL065InOut->snAmount > 0)
                    {
                        tTblBacct.DRAMT_TODY -= ptL065InOut->snPoint;
                        tTblBacctFlag.DRAMT_TODY = 1;
                    }
                    else
                    {
                        tTblBacct.DRAMT_TODY += ptL065InOut->snPoint;
                        tTblBacctFlag.DRAMT_TODY = 1;
                    }
                }
            }
            else
            {
                tTblBacct.CRAMT_TOT += tTblBacct.CRAMT_TODY;
                tTblBacctFlag.CRAMT_TOT = 1;
                tTblBacct.CRAMT_TODY = 0;
                tTblBacctFlag.CRAMT_TODY = 1;
                        
                tTblBacct.DRAMT_TOT += tTblBacct.DRAMT_TODY;
                tTblBacctFlag.DRAMT_TOT = 1;
                tTblBacct.DRAMT_TODY = 0;
                tTblBacctFlag.DRAMT_TODY = 1;
                    
                if(GC_ORIG[0] == ptL065InOut->cO_r_flag)
                {                                          
                    if(ptL065InOut->snAmount > 0)
                    {
                        tTblBacct.DRAMT_TODY += ptL065InOut->snPoint;
                        tTblBacctFlag.DRAMT_TODY = 1;
                    }
                    else
                    {
                        tTblBacct.DRAMT_TODY -= ptL065InOut->snPoint;
                        tTblBacctFlag.DRAMT_TODY = 1;
                    }
                }
                else
                {
                    if(ptL065InOut->snAmount > 0)
                    {
                        tTblBacct.DRAMT_TODY -= ptL065InOut->snPoint;
                        tTblBacctFlag.DRAMT_TODY = 1;
                    }
                    else
                    {
                        tTblBacct.DRAMT_TODY += ptL065InOut->snPoint;
                        tTblBacctFlag.DRAMT_TODY = 1;
                    }
                }
                tTblBacct.PROC_DAY = ptL065InOut->unProc_day;
                tTblBacctFlag.PROC_DAY = 1;
            }

            snResult = BacctUpdate(&tTblBacct, &tTblBacctFlag);
            if(snResult != 0)
            {
                CmdLog(LOGWAN, "BacctUpdate error snResult[%d]", snResult);
                return -1;
            }
        }
    }
    /*End:  added by yyl for CHG-130828-0002*/

    if ((tTblBanks.PTTOL_YN != 2) || (650 == ptL065InOut->unTrans_type)) /*modded by yyl for CHG-130828-0002*/
    {
        if(CmdGL(pL065_bankacct) != 0)/*借方科目非空*/
        {
            snResult = Pbacctsec(GLB_BANK, pL065_bankacct, &tTblBacct);
            if((snResult != 0) && (snResult != DB_NOT_FOUND))/*查询失败*/
            {
                CmdLog(LOGWAN, "Pbacctsec error snResult[%ld]", snResult);
                return -1;
            }
            if((0 == snResult) && ('B' == tTblBacct.GLFLAG[0]))/*BACCT记录存在*/
            {
                if(ptL065InOut->unProc_day == tTblBacct.PROC_DAY)
                {
                    if(GC_ORIG[0] == ptL065InOut->cO_r_flag)
                    {
                        if(ptL065InOut->snAmount > 0)
                        {
                            tTblBacct.DRAMT_TODY += ptL065InOut->snAmount;
                            tTblBacctFlag.DRAMT_TODY = 1;
                        }
                        else
                        {
                            tTblBacct.DRAMT_TODY -= ptL065InOut->snAmount;
                            tTblBacctFlag.DRAMT_TODY = 1;
                        }
                    }
                    else
                    {
                        if(ptL065InOut->snAmount > 0)
                        {
                            tTblBacct.CRAMT_TODY -= ptL065InOut->snAmount;
                            tTblBacctFlag.CRAMT_TODY = 1;
                        }
                        else
                        {
                            tTblBacct.CRAMT_TODY += ptL065InOut->snAmount;
                            tTblBacctFlag.CRAMT_TODY = 1; /*moded by yyl for CHG-130828-0002*/
                        }
                    }
                }
                else
                {
                    if(GC_ORIG[0] == ptL065InOut->cO_r_flag)
                    {
                        tTblBacct.DRAMT_TOT += tTblBacct.DRAMT_TODY;
                        tTblBacctFlag.DRAMT_TOT = 1;
                        tTblBacct.DRAMT_TODY = 0;
                        tTblBacctFlag.DRAMT_TODY = 1;

                        tTblBacct.CRAMT_TOT += tTblBacct.CRAMT_TODY;
                        tTblBacctFlag.CRAMT_TOT = 1;
                        tTblBacct.CRAMT_TODY = 0;
                        tTblBacctFlag.CRAMT_TODY = 1;

                        if(ptL065InOut->snAmount > 0)
                        {
                            tTblBacct.DRAMT_TODY += ptL065InOut->snAmount;
                            tTblBacctFlag.DRAMT_TODY = 1;
                        }
                        else
                        {
                            tTblBacct.DRAMT_TODY -= ptL065InOut->snAmount;
                            tTblBacctFlag.DRAMT_TODY = 1;
                        }
                    }
                    else
                    {
                        tTblBacct.CRAMT_TOT += tTblBacct.CRAMT_TODY;
                        tTblBacctFlag.CRAMT_TOT = 1;
                        tTblBacct.CRAMT_TODY = 0;
                        tTblBacctFlag.CRAMT_TODY = 1;

                        tTblBacct.DRAMT_TOT += tTblBacct.DRAMT_TODY;
                        tTblBacctFlag.DRAMT_TOT = 1;
                        tTblBacct.DRAMT_TODY = 0;
                        tTblBacctFlag.DRAMT_TODY = 1;

                        if(ptL065InOut->snAmount > 0)
                        {
                            tTblBacct.CRAMT_TODY -= ptL065InOut->snAmount;
                            tTblBacctFlag.CRAMT_TODY = 1;
                        }
                        else
                        {
                            tTblBacct.CRAMT_TODY += ptL065InOut->snAmount;
                            tTblBacctFlag.CRAMT_TODY = 1;
                        }                          
                    }
                    tTblBacct.PROC_DAY = ptL065InOut->unProc_day;
                    tTblBacctFlag.PROC_DAY = 1;
                }

                snResult = BacctUpdate(&tTblBacct, &tTblBacctFlag);
                if(snResult != 0)
                {
                    CmdLog(LOGWAN, "BacctUpdate error snResult[%d]", snResult);
                    return -1;
                }
            }
        }/*end 借方科目非空*/

        if(CmdGL(pL065_bankacc1) != 0) /*贷方科目非空*/
        {
            memset(&tTblBacct, 0x00, sizeof(bacct_tdef));
            snResult = Pbacctsec(GLB_BANK, pL065_bankacc1, &tTblBacct);
            if((snResult != 0) && (snResult != DB_NOT_FOUND))/*查询失败*/
            {
                CmdLog(LOGWAN, "Pbacctsec error snResult[%ld]", snResult);
                return -1;
            }
            if((0 == snResult) && ('B' == tTblBacct.GLFLAG[0]))
            {
                if(ptL065InOut->unProc_day == tTblBacct.PROC_DAY)
                {
                    if(GC_ORIG[0] == ptL065InOut->cO_r_flag)
                    {
                        if(ptL065InOut->snAmount > 0)
                        {
                            tTblBacct.CRAMT_TODY += ptL065InOut->snAmount;
                            tTblBacctFlag.CRAMT_TODY = 1;
                        }
                        else
                        {
                            tTblBacct.CRAMT_TODY -= ptL065InOut->snAmount;
                            tTblBacctFlag.CRAMT_TODY = 1;
                        }
                    }
                    else
                    {
                        if(ptL065InOut->snAmount > 0)
                        {
                            tTblBacct.DRAMT_TODY -= ptL065InOut->snAmount;
                            tTblBacctFlag.DRAMT_TODY = 1;
                        }
                        else
                        {
                            tTblBacct.DRAMT_TODY += ptL065InOut->snAmount;
                            tTblBacctFlag.DRAMT_TODY = 1;
                        }
                    }                       
                }
                else
                {
                    if(GC_ORIG[0] == ptL065InOut->cO_r_flag)
                    {
                        tTblBacct.CRAMT_TOT += tTblBacct.CRAMT_TODY;
                        tTblBacctFlag.CRAMT_TOT = 1;
                        tTblBacct.CRAMT_TODY = 0;
                        tTblBacctFlag.CRAMT_TODY = 1;

                        tTblBacct.DRAMT_TOT += tTblBacct.DRAMT_TODY;
                        tTblBacctFlag.DRAMT_TOT = 1;
                        tTblBacct.DRAMT_TODY = 0;
                        tTblBacctFlag.DRAMT_TODY = 1;

                        if(ptL065InOut->snAmount > 0)
                        {
                            tTblBacct.CRAMT_TODY += ptL065InOut->snAmount;
                            tTblBacctFlag.CRAMT_TODY = 1;
                        }
                        else
                        {
                            tTblBacct.CRAMT_TODY -= ptL065InOut->snAmount;
                            tTblBacctFlag.CRAMT_TODY = 1;
                        }
                    }
                    else
                    {
                        tTblBacct.CRAMT_TOT += tTblBacct.CRAMT_TODY;
                        tTblBacctFlag.CRAMT_TOT = 1;
                        tTblBacct.CRAMT_TODY = 0;
                        tTblBacctFlag.CRAMT_TODY = 1;

                        tTblBacct.DRAMT_TOT += tTblBacct.DRAMT_TODY;
                        tTblBacctFlag.DRAMT_TOT = 1;
                        tTblBacct.DRAMT_TODY = 0;
                        tTblBacctFlag.DRAMT_TODY = 1;

                        if(ptL065InOut->snAmount > 0)
                        {
                            tTblBacct.DRAMT_TODY -= ptL065InOut->snAmount;
                            tTblBacctFlag.DRAMT_TODY = 1;
                        }
                        else
                        {
                            tTblBacct.DRAMT_TODY += ptL065InOut->snAmount;
                            tTblBacctFlag.DRAMT_TODY = 1;
                        }
                    }
                    tTblBacct.PROC_DAY = ptL065InOut->unProc_day;
                    tTblBacctFlag.PROC_DAY = 1;                  
                }
                 
                snResult = BacctUpdate(&tTblBacct, &tTblBacctFlag);
                if(snResult != 0)
                {
                    CmdLog(LOGWAN, "BacctUpdate error snResult[%ld]", snResult);
                    return -1;
                }                
            }            
        }/*end 贷方科目非空*/
    }
    /*else
    {
        return 0;
    }*/ /*Deled by yyl for CHG-130828-0002*/

    return 0;
}
/*END ADD IC卡特色交易*/

/*Begin:added by yyl for CHG-130909-0051*/
/********************************************************************************
*  Function:  L065_5_brno_chg
*
*  该函数用于浙江农信修改ptL065InOut->chBrno值，从而插入Jorj.Brno
*
*  @Parameters:
*  输入：
*        acct_tdef    *ptTblAcct
*
*  输入&输出：
*        L065_io_sdef *ptL065InOut
*
*  @return: 0:成功
*           <>0:失败
********************************************************************************/
int L065_5_brno_chg(acct_tdef *ptTblAcct,
                          L065_io_sdef *ptL065InOut)
{
    SN6  snResult = 0;
    UN6  unBn     = 0;
    UN2  unNum    =0;
    brnch_tdef tTblBrnch;

    /*参数初始化*/
    memset(&tTblBrnch, 0x00, sizeof(brnch_tdef));

    if (CmdGL(ptL065InOut->chBrno) != 0)
    {
        CmdLTrim(ptL065InOut->chBrno);
        if (CmdGL(ptL065InOut->chBrno) != 6)
        {
            unBn = 99;
        }
        else
        {
            unBn = atol(ptL065InOut->chBrno);
        }
    }

    if (unBn < 9999)
    {
        unNum = 0;
        if (ptTblAcct->BRANCH != 0)  /*此处ptTblAcct->BRANCH即L065-BRANCH*/
        {
            snResult = Pbrnch(ptTblAcct->BANK, ptTblAcct->BRANCH, &tTblBrnch);
            if ((snResult != 0) && (snResult != DB_NOT_FOUND))
            {
                CmdLog(LOGWAN, "Pbrnch error snResult[%ld], Branch[%d]", snResult, ptTblAcct->BRANCH);
                return -1;
            }
            if ((0 == snResult) && (CmdGL(tTblBrnch.BRNCH_CODE) != 0))
            {
                unNum = 1;
            }
        }

        if (1 == unNum)
        {
            CmdA2AEx(tTblBrnch.BRNCH_CODE, 1, 6, 1, sizeof(ptL065InOut->chBrno), ptL065InOut->chBrno);
        }
        else 
        {
            if (ptTblAcct->XACCOUNT != 0) /*added by yyl for CHG-131111-0009*/
            {
                CmdA2AEx(ptTblAcct->BRNCH_CODE, 1, 6, 1, sizeof(ptL065InOut->chBrno), ptL065InOut->chBrno);
            }
        }
    }

    return 0;
}
/*End:added by yyl for CHG-130909-0051 浙江农信使用*/