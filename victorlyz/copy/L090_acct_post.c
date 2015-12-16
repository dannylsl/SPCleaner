/********************************************************************************
 *  Copyright 2011-3010, CHINA UNIONPAY DATA SERVICES Co., Ltd.
 *  All right reserved.
 *
 *  THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF CHINA UNIONPAY CUPDATA
 *  SERVICES CO., LTD.  THE CONTENTS OF THIS FILE MAY NOT
 *  BE DISCLOSED TO THIRD PARTIES, COPIED OR DUPLICATED IN ANY FORM,
 *  IN WHOLE OR IN PART, WITHOUT THE PRIOR WRITTEN PERMIStSiON OF
 *  CHINA UNIONPAY CUPDATA SERVICES CO., LTD.
 *
 *  用于对交易流水进行联机入账。包含以下函数：
 *  1.  L090_acct_post      原主函数
 *  2.  L090_1_pay_ageing   还款冲减账户逾期
 *  4.  L090_3_acct_trans   本币交易更新联机金额
 *  5.  L090_4_accx_trans   外币交易更新联机金额
 *  6.  L090_5_busns_trans  交易更新公司卡联机金额
 *  7.  L090_6_oct_proc     逾期账户处理
 *
 *  Edit History:
 *  版本  日期          修改人        修改内容
 *  1.0   20110809     武传宝         创建
 *  1.1   20120307      武传宝        修改 2.2011.5.000
 *                                    CASE-0305-2011-0352 实现人行征信数据T+1报送
 *                                    修改 2.2011.6.000
 *                                      XYK-6403-2011-089   通信代应收本息拆分
 *  1.2   20121105    武传宝        修改 2.2012.4
 *                                    CHG-111201-0018 息费不占用额度
 *  1.3   20130306    张贻华        修改  2.0 2012.6.000 CS-6320-2012-0095
 *                                      东亚银行金贷卡项目需求
 *  1.4   20130506    张贻华        修改  2.0 2013.02.000 CHG-130228-0010
 *                                      V2.0 批量调整等与联机交易发生死锁的问题
 *                                    修改  2.0 2013.02.000 CHG-130225-0018
 *                                      东亚/泰安贷记卡综合改造
 *  1.5   20130712    张贻华        修改
 *                              2.0 2013.03.000 CHG-130702-0011 银盛单币种改造
 *                              2.0 2013.03.000 CHG-130418-0024 泰安商行借贷合一卡二期
 *  1.6   20131021   张贻华      修改   2.0 2013.05.000 CHG-130828-0002
 *  1.7   20131114   张贻华      修改   2.0 2013.05.000 CHG-130509-0001公司卡分期功能
 *  1.8   20131204   张贻华      修改   2.0 2013.05.000 CHG-130128-0007 发卡系统冲正交易修改的需求
 *  1.9   20131216   张贻华      修改   2.0 2013.06.000 CHG-130819-0038/CS-6440-2013-0059版本合并接口及数据结构程序修改
 *  2.0   20131216   张贻华      修改   N2.2014.01.000  CHG-131127-0029 银盛利息计算改造
 *  2.1   20140210   张贻华      修改   2.0 2013.06.000/ N2.2014.02.000 CHG-130909-0051浙江农信-CHGS等受理网点号保存
 *  2.2   20140210   张贻华      修改   2.0 2013.06.000/ N2.2014.02.000 CHG-131030-0014 公司卡分期退货优化
 *  2.3   20140303   张贻华      修改   2.0 2013.06.000/ N2.2014.02.000 CHG-140122-0003 本期息费不占额度
 *  2.4   20140515   张贻华      修改   N2.2014.03.000 CHG-140410-0029 信用卡信贷处理_单笔核算信用卡模式
 *******************************************************************************/
#include "L090_acct_post.h"

/********************************************************************************
* 以下为内部使用子函数的声明
********************************************************************************/
int L090_1_pay_ageing(  event_tdef      *ptTblEvent,
                        UN9_2            L090_minageamt,
                        CH              *pL090_ageing_g,
                        UN11_2          *pL090_over_held,
                        SN2             *pL090_mthsodue);

void L090_3_acct_init(acct_tdef       *ptTblAcct,
                        L090_bal_sdef   *ptL090_bal);

void L090_3_accx_init(accx_tdef       *ptTblAccx,
                        L090_bal_sdef   *ptL090_bal);

int L090_4_acct_trans(  event_tdef      *ptTblEvent,
                        acct_tdef       *ptTblAcct,
                        prmcn_tdef   *ptTblPrmcn,
                        L090_bal_sdef   *ptL090_bal);

int L090_4_accx_trans(  event_tdef      *ptTblEvent,
                        accx_tdef       *ptTblAccx,
                        L090_bal_sdef   *ptL090_bal);

int L090_5_busns_trans( SN3              L090_curr,
                        L090_bal_sdef   *ptL090_bal,
                        event_tdef      *ptTblEvent,
                        busns_tdef      *ptTblBusns);

int L090_6_oct_proc(    banks_tdef      *ptTblBanks,
                        event_tdef      *ptTblEvent,
                        acct_tdef       *ptTblAcct,
                        accx_tdef       *ptTblAccx,
                        UN1              L090_exist_accx);

void L090_7_acct_set(L090_bal_sdef   *ptL090_bal,
                       acct_tdef       *ptTblAcct,
                       acctflag_tdef   *ptTblAcctflag);

void L090_7_accx_set(L090_bal_sdef   *ptL090_bal,
                       accx_tdef       *ptTblAccx,
                       accxflag_tdef   *ptTblAccxflag);

/********************************************************************************
 *  L090_acct_post
 *  对交易流水进行联机入账
 *
 *  形参：
 *      输入：
 *          banks_tdef      *ptTblBanks          数据库表BANKS/当前银行记录
 *          event_tdef      *ptTblEvent          数据库表EVENT/当前待入账交易流水
 *          UN1              L090_switch         是否账单日调用
 *                                               0:否,需更新账户 1:是，不更新账户
 *          CH              *pL090_card_nbr      当前卡片卡号,非附卡交易可传空
 *          SN11_2           L090_cur_bal        当前账户余额（币种为账户币种）
 *          SN11_2           L090_local_bal      当前账户余额（币种为本币）
 *          UN1             *pL090_exist_accx    0：ACCX不存在 1: ACCX存在
 *          acct_tdef       *ptTblAcct           数据库表ACCT/入账后账户记录
 *          accx_tdef       *ptTblAccx           数据库表ACCX/入账后外币账户记录
 *      输出：
 *          ptL090_bal          当前账户联机交易笔数和金额
 *
 *  返回值：
 *         -1 - 数据库调用出错
 *          0 - 成功
 *          1 - 输入参数错误
 *          2 - 未找到CARD记录
 *          3 - 未找到PRMCN记录
 *          4 - 未找到BUSNS记录
 *          5 - 交易类型不合法
 *          6 - 创建ACCX记录失败
 *          7 - 为找到ACCX记录
 *          8 - 冲减逾期金额出错
 *          9 - 未找到EVENT记录
 *         10 - 子函数调用错误
 *******************************************************************************/

int L090_acct_post( banks_tdef      *ptTblBanks,
                    event_tdef      *ptTblEvent,
                    UN1              L090_switch,
                    CH              *pL090_card_nbr,
                    SN11_2           L090_cur_bal,
                    SN11_2           L090_local_bal,
                    UN1             *pL090_exist_accx,
                    acct_tdef       *ptTblAcct,
                    accx_tdef       *ptTblAccx,
                    L090_bal_sdef   *ptL090_bal)
{
    UN1     unTemp_flag     = 0;
    UN1     unScard_flag    = 0;    /*附卡交易时是否单独统计发生额*/
    SN6     snResult        = 0;
    UN8     unPur_mmyy      = 0;
    SN11_2  snLatest_bal    = 0.00;
    SN11_2  snLatest_loc    = 0.00;
    SN11_2  snConv_amt      = 0.00;
    SN10    snCred_limit     = 0;
    SN9_2   snMinAge = 0.00; /*本币最小逾期金额*/
    SN9_2   snMinAgex = 0.00;/*外币最小逾期金额*/
    SN11_2 snPay = 0.00;
    SN11_2 snCre  = 0.00;
    SN11_2 snQer  = 0.00;
    chgs_tdef tTblChgs = {0};
    /*ADD CHG-140122-0003 本期息费不占额度*/
    UN1 unAddamt_flag = 0;  /*判断超限是否加本笔交易金额，0-加;1-不加*/

    card_tdef   tTblCard;
    busns_tdef  tTblBusns;
    prmcn_tdef  tTblPrmcn;

    acctflag_tdef   tTblAcctflag;
    accxflag_tdef   tTblAccxflag;
    cardflag_tdef   tTblCardflag;

    memset(&tTblPrmcn, 0x00, sizeof(tTblPrmcn));/*add for CHG-140410-0029*/

    snResult = CmdDC(DC_DAYNUM, &GLB_BUSINESS_DAY);
    if(snResult != 0)
    {
        CmdLog(LOGWAN, "CmdDC error snResult[%ld]", snResult);
        return 1;
    }
    unPur_mmyy = atoi(GLB_DC_DATE.dc_ddmmyy) % 10000;

    /*输入参数检查--必填性检查*/
    if(NULL == ptTblBanks)
    {
        CmdLog(LOGWAN, "Input ptTblBanks is NULL");
        return 1;
    }
    else if(0 == CmdGL(ptTblBanks->ROWID))
    {
        CmdLog(LOGWAN, "Input ptTblBanks->ROWID is empty");
        return 1;
    }

    if(NULL == ptTblAcct)
    {
        CmdLog(LOGWAN, "Input ptTblAcct is NULL");
        return 1;
    }
    else if(0 == CmdGL(ptTblAcct->ROWID))
    {
        CmdLog(LOGWAN, "Input ptTblAcct->ROWID is empty");
        return 1;
    }

    if(NULL == ptTblEvent)
    {
        CmdLog(LOGWAN, "Input ptTblEvent is NULL");
        return 1;
    }

    if((L090_switch != 0) && (L090_switch != 1))
    {
        CmdLog(LOGWAN, "Input L090_switch is wrong");
        return 1;
    }

    /*外币ACCX必须存在*/
    if(NULL == pL090_exist_accx)
    {
        CmdLog(LOGWAN, "Input pL090_exist_accx is NULL");
        return 1;
    }

    if(NULL == pL090_card_nbr)
    {
        CmdLog(LOGWAN, "Input pL090_card_nbr is NULL");
        return 1;
    }

    switch(*pL090_exist_accx)
    {
    case 0:
        break;
    case 1:
        if(NULL == ptTblAccx)
        {
            CmdLog(LOGWAN, "Input ptTblAccx is NULL");
            return 1;
        }
        else if(0 == CmdGL(ptTblAccx->ROWID))
        {
            CmdLog(LOGWAN, "Input ptTblAccx->ROWID is empty");
            return 1;
        }
        break;
    default:
        CmdLog(LOGWAN, "Input pL090_exist_accx[%d] is wrong", *pL090_exist_accx);
        return 1;
    }

    /*L090_switch 为1且为外币交易时必须输入ACCX记录*/
    if((1 == L090_switch) && (ptTblEvent->CURRNCY_CD != ptTblAcct->CURR_NUM))
    {
        if(*pL090_exist_accx != 1)
        {
            CmdLog(LOGWAN, "Input accx is not exist");
            return 1;
        }
    }

    if(NULL == ptL090_bal)
    {
        CmdLog(LOGWAN, "Output ptL090_bal is NULL");
        return 1;
    }

    /*===================================输入检查结束=================================*/

    if (L090_switch != 1)
    {
        unScard_flag = 0;
        /*remove for CHG-130228-0010 by zyh 20130506*/
        /*
        snResult = Pacctsec(ptTblEvent->BANK, ptTblEvent->ACCTNBR, ptTblAcct);
        if (snResult != 0 && snResult != DB_NOT_FOUND)
        {
            CmdLog(LOGWAN, "Pacctsec error snResult[%ld]", snResult);
            return -1;
        }
        if (DB_NOT_FOUND == snResult)
        {
            CmdLog(LOGWAN, "Pacctsec 未找到本币账户 bank[%d] xaccount[%ld]",
                   ptTblEvent->BANK, ptTblEvent->ACCTNBR);
            return 1;
        }
        if (1 == *pL090_exist_accx)
        {
            snResult = Paccxsec(ptTblAcct->BANK, ptTblAcct->XACCOUNT, ptTblAccx);
            if (snResult != 0 && snResult != DB_NOT_FOUND)
            {
                CmdLog(LOGWAN, "Paccxsec error snResult[%ld]", snResult);
                return -1;
            }
        }*/

        /*查询主附卡额度是否分开(若为主卡默认为不分开)*/
        if (CmdGL(pL090_card_nbr) != 0)
        {
            memset(&tTblCard, 0x00, sizeof(tTblCard));
            snResult = Pcardnumbsec(pL090_card_nbr,0,&tTblCard);
            if (snResult != 0 && snResult != DB_NOT_FOUND)
            {
                CmdLog(LOGWAN, "Pcardnumbsec error snResult[%ld]", snResult);
                return -1;
            }
            if (DB_NOT_FOUND == snResult)
            {
                CmdLog(LOGWAN, "Pcardnumbsec 找不到卡片记录  Card_nbr[%s]", pL090_card_nbr);
                return 2;
            }

            if ((tTblCard.CARDHOLDER != 1) && (tTblCard.LIMIT_X !=0) )
            {
                unScard_flag = 1;
            }
        }

        /*add for CHG-130228-0010 by zyh bagein 20130506*/
        snResult = Pacctsec(ptTblEvent->BANK, ptTblEvent->ACCTNBR, ptTblAcct);
        if (snResult != 0 && snResult != DB_NOT_FOUND)
        {
            CmdLog(LOGWAN, "Pacctsec error snResult[%ld]", snResult);
            return -1;
        }
        if (DB_NOT_FOUND == snResult)
        {
            CmdLog(LOGWAN, "Pacctsec 未找到本币账户 bank[%d] xaccount[%ld]",
                   ptTblEvent->BANK, ptTblEvent->ACCTNBR);
            return 1;
        }
        if (1 == *pL090_exist_accx)
        {
            snResult = Paccxsec(ptTblAcct->BANK, ptTblAcct->XACCOUNT, ptTblAccx);
            if (snResult != 0 && snResult != DB_NOT_FOUND)
            {
                CmdLog(LOGWAN, "Paccxsec error snResult[%ld]", snResult);
                return -1;
            }
        }
        /*add for CHG-130228-0010 by zyh end 20130506*/

        /*查询PRMCN记录内容*/
        memset(&tTblPrmcn, 0x00, sizeof(tTblPrmcn));
        snResult = Pprmcnbnk(ptTblAcct->BANK, ptTblAcct->CATEGORY,&tTblPrmcn);
        if (snResult != 0 && snResult != DB_NOT_FOUND)
        {
            CmdLog(LOGWAN, "Pprmcnbnk error snResult[%ld]", snResult);
            return -1;
        }
        if (DB_NOT_FOUND == snResult)
        {
            CmdLog(LOGWAN, "Pprmcnbnk 未找到Prmcn记录  bank[%d] category[%d]",
                   ptTblAcct->BANK, ptTblAcct->CATEGORY);
            return 3;
        }

    }
    if (CmdGL(ptTblAcct->BUSINESS) != 0)
    {
        memset(&tTblBusns, 0x00, sizeof(tTblBusns));
        snResult = Pbusnssec(ptTblAcct->BANK,ptTblAcct->BUSINESS,&tTblBusns);
        if (snResult != 0 && snResult != DB_NOT_FOUND)
        {
            CmdLog(LOGWAN, "Pbusnssec error snResult[%ld]", snResult);
            return -1;
        }
        if (DB_NOT_FOUND == snResult)
        {
            CmdLog(LOGWAN, "Pbusnssec 未找到公司Busns记录  bank[%d] business[%s]",
                   ptTblAcct->BANK, ptTblAcct->BUSINESS);
            return 4;
        }
    }

    /*本币交易处理*/
    if (ptTblEvent->CURRNCY_CD == ptTblAcct->CURR_NUM)
    {
        /*非批处理需对统计金额与笔数初始化*/
        if (L090_switch != 1)
        {
            memset(ptL090_bal, 0x00, sizeof(L090_bal_sdef));
            L090_3_acct_init(ptTblAcct, ptL090_bal);

            if (1 == unScard_flag)
            {
                ptL090_bal->snScard_tamt = tTblCard.TOTAL_AMT;         /*附卡非还款交易发生额*/
            }
        }
        snResult = L090_4_acct_trans(ptTblEvent, ptTblAcct,&tTblPrmcn,ptL090_bal);
        if (snResult != 0)
        {
            CmdLog(LOGINF, "L090_3_acct_trans error snResult[%ld]", snResult);
            return ((snResult < 0)?-1:5);
        }

        /*附卡本币交易发生额*/
        if (1 == unScard_flag)
        {
            if (!((ptTblEvent->TRANS_TYPE >= 7000) && (ptTblEvent->TRANS_TYPE < 8000)))
            {
                ptL090_bal->snScard_tamt += ptTblEvent->BILL_AMT;
            }
        }

    }
    /*外币交易处理*/
    else
    {
        if (L090_switch != 1)
        {
            snResult = Paccxsec(ptTblAcct->BANK,ptTblAcct->XACCOUNT,ptTblAccx);
            if (snResult != 0 && snResult != DB_NOT_FOUND)
            {
                CmdLog(LOGWAN, "Paccxsec error snResult[%ld]", snResult);
                return -1;
            }
            if (DB_NOT_FOUND == snResult)
            {
                /*ADD CASE-0305-2011-0352 新增ptTblBanks*/
                snResult = L082_create_accx(ptTblBanks, ptTblAcct, ptTblAccx);
                if (snResult < 0)
                {
                    CmdLog(LOGINF, "L082_create_accx error snResult[%ld] XACCOUNT[%ld]",
                           snResult, ptTblAcct->XACCOUNT);
                    return -1;
                }
                if (0 == snResult)
                {
                    *pL090_exist_accx = 1;
                }
            }
            memset(ptL090_bal, 0x00, sizeof(L090_bal_sdef));
            L090_3_accx_init(ptTblAccx, ptL090_bal);

            if (1 == unScard_flag)
            {
                ptL090_bal->snScard_tamt = tTblCard.TOTAL_AMX;         /*附卡非还款交易发生额*/
            }
        }

        snResult = L090_4_accx_trans(ptTblEvent, ptTblAccx, ptL090_bal);
        if (snResult != 0)
        {
            CmdLog(LOGINF, "L090_4_accx_trans error snResult[%ld]", snResult);
            return ((snResult < 0)?-1:5);
        }

        /*附卡外币交易发生额*/
        if (1 == unScard_flag)
        {
            if (!((ptTblEvent->TRANS_TYPE >= 7000) && (ptTblEvent->TRANS_TYPE < 8000)))
            {
                ptL090_bal->snScard_tamt += ptTblEvent->BILL_AMT;
            }
        }
    }

    /*若无需更新此处结束*/
    if (1 == L090_switch)
    {
        return 0;
    }

    /*BEG ADD CHG-140122-0003 本期息费不占额度*/
    /*读取PRMCN*/
    if ((0 == CmdGL(tTblPrmcn.ROWID)) && (0 == tTblPrmcn.BANK) && (2 == ptTblBanks->INTFEE_YN))
    {
        memset(&tTblPrmcn, 0x00, sizeof(tTblPrmcn));
        snResult = Pprmcnbnk(ptTblAcct->BANK, ptTblAcct->CATEGORY,&tTblPrmcn);
        if (snResult != 0 && snResult != DB_NOT_FOUND)
        {
            CmdLog(LOGWAN, "Pprmcnbnk error snResult[%ld]", snResult);
            return -1;
        }
        if (DB_NOT_FOUND == snResult)
        {
            CmdLog(LOGWAN, "Pprmcnbnk 未找到Prmcn记录  bank[%d] category[%d]",
                   ptTblAcct->BANK, ptTblAcct->CATEGORY);
            return 3;
        }
    }

    /*PRMCN.INTFEE_YN=2且费用交易时不累计EVENT.BILL_AMT金额*/
    if (2 == tTblPrmcn.INTFEE_YN)
    {
        /*OTHER_FEES*/
        if ((ptTblEvent->TRANS_TYPE >= 3000) && (ptTblEvent->TRANS_TYPE < 3130))
        {
            unAddamt_flag = 1;
        }
        /*CARD_FEES*/
        else if ((ptTblEvent->TRANS_TYPE >= 3130) && (ptTblEvent->TRANS_TYPE < 3500))
        {
            unAddamt_flag = 1;
        }
        /*CASH_ADFEE*/
        else if (((ptTblEvent->TRANS_TYPE >= 4000) && (ptTblEvent->TRANS_TYPE < 4300)) ||
                 ((ptTblEvent->TRANS_TYPE >= 4500) && (ptTblEvent->TRANS_TYPE < 4900)))
        {
            unAddamt_flag = 1;
        }
        /*PEN_CHRG*/
        else if ((ptTblEvent->TRANS_TYPE == 3500) || (ptTblEvent->TRANS_TYPE == 3550) ||
            (ptTblEvent->TRANS_TYPE == 3501) || (ptTblEvent->TRANS_TYPE == 3551))
        {
            unAddamt_flag = 1;
        }
    }
    /*END ADD CHG-140122-0003 本期息费不占额度*/

    /*此部分需要更新以下表格*/
    memset(&tTblAcctflag, 0x00, sizeof(tTblAcctflag));
    memset(&tTblAccxflag, 0x00, sizeof(tTblAccxflag));
    memset(&tTblCardflag, 0x00, sizeof(tTblCardflag));

    /*本币交易处理*/
    if (ptTblEvent->CURRNCY_CD == ptTblAcct->CURR_NUM)
    {
        /*add for CHG-130418-0024 begin*/
        if((CmdDblCmp(ptTblEvent->BILL_AMT,0.00,2) < 0)
            &&('1' == ptTblBanks->OVER_FLAG[3])
            &&('O' ==ptTblAcct->CLOSE_CODE[0]))
        {
            snCre += CmdROU(ptTblAcct->INT_EARNED, 2);
            snCre += ptTblAcct->CRED_ADJ+ptTblAcct->CRED_VOUCH+ptTblAcct->DUTY_CREDT\
                       +ptTblAcct->PAYMT_CLRD+ptTblAcct->PAYMT_UNCL;
            if(CmdDblCmp(ptTblAcct->OTHER_FEES,0.00, 2) < 0)
            {
                snCre += ptTblAcct->OTHER_FEES;
            }
            if(CmdDblCmp(ptTblAcct->FEES_TAXES,0.00, 2) < 0)
            {
                snCre += ptTblAcct->FEES_TAXES;
            }
            snPay = snCre;
            snQer = ptTblAcct->QUERY_AMT;
            if(CmdDblCmp(ptTblAcct->QUERY_AMT,ptTblAcct->STM_QRYAMT,2) < 0)
            {
                snQer = ptTblAcct->STM_QRYAMT;
            }

            snPay = CmdROU((snPay-snQer),2) + ptTblEvent->BILL_AMT;

            if(CmdDblCmp((ptTblAcct->STM_MINDUE + snPay),0.00,2) <= 0)
            {
                /*记录日志*/
                CmdA2A("L090", sizeof(tTblChgs.ENTRY_TYPE), tTblChgs.ENTRY_TYPE);
                tTblChgs.BANK = GLB_BANK;
                tTblChgs.XACCOUNT = ptTblAcct->XACCOUNT;
                tTblChgs.INP_DAY = GLB_REL_DAY;
                tTblChgs.EMPLY_NBR = 999999;
                tTblChgs.BRANCH = ptTblAcct->BRANCH;
                tTblChgs.SEC_LEVEL = 9;
                CmdA2A(ptTblAcct->CLOSE_CODE, sizeof(tTblChgs.PRIOR_VALU), tTblChgs.PRIOR_VALU);
                memset(tTblChgs.NEW_VALU,' ',(sizeof(tTblChgs.NEW_VALU)-1));
                CmdA2A("AC04", sizeof(tTblChgs.ENTRY_CODE), tTblChgs.ENTRY_CODE);
                CmdA2A(ptTblAcct->BRNCH_CODE, sizeof(tTblChgs.BRNO), tTblChgs.BRNO);/*add for CHG-130909-0051*/
                snResult = L034_chgs_create(&tTblChgs);
                if(snResult != 0)
                {
                    CmdLog(LOGINF, "L034_chgs_create error snResult[%ld]", snResult);
                }

               // CmdA2A("O", sizeof(ptTblAcct->CLOSE_CODE), ptTblAcct->CLOSE_CODE);
                memset( ptTblAcct->CLOSE_CODE,' ',(sizeof( ptTblAcct->CLOSE_CODE)-1));
                tTblAcctflag.CLOSE_CODE = 1;
                ptTblAcct->CLOSE_CHDY = GLB_REL_DAY;
                tTblAcctflag.CLOSE_CHDY = 1;
                ptTblAcct->ETL_DAY= GLB_REL_DAY;
                tTblAcctflag.ETL_DAY  = 1;
            }

        }
        /*add for CHG-130418-0024 end*/

        /*add for CHG-130225-0018 by zyh begin 20130506*/
        snMinAge = tTblPrmcn.MIN_AGEAMT;
        if((1 == tTblPrmcn.AGE_FLAG)&&(ptTblAcct->MIN_AGEAMT > tTblPrmcn.MIN_AGEAMT))
        {
            snMinAge = ptTblAcct->MIN_AGEAMT;
        }
        /*add for CHG-130225-0018 by zyh end 20130506*/

        /*初始化冲减逾期金额的参数并冲减逾期,修改ACCT上逾期字段*/
        snResult = L090_1_pay_ageing(ptTblEvent,
                                     snMinAge,
                                     /*tTblPrmcn.MIN_AGEAMT,*/
                                     ptTblAcct->AGEING_G,
                                     &ptTblAcct->ODUE_HELD,
                                     &ptTblAcct->MTHS_ODUE);
        if(snResult != 0)
        {
            CmdLog(LOGINF, "L090_1_pay_ageing error snResult[%ld]", snResult);
            return ((snResult < 0)?-1:10);
        }

        tTblAcctflag.AGEING_G = 1;
        tTblAcctflag.ODUE_HELD = 1;
        tTblAcctflag.MTHS_ODUE = 1;

        /*更新公司信息*/
        if (CmdGL(ptTblAcct->BUSINESS) != 0)
        {
            snResult = L090_5_busns_trans(ptTblAcct->CURR_NUM,
                                          ptL090_bal,
                                          ptTblEvent,
                                          &tTblBusns);
            if (snResult != 0)
            {
                CmdLog(LOGINF, "L090_5_busns_trans error snResult[%ld]", snResult);
                return ((snResult < 0)?-1:10);
            }
        }

        /*该条件仅为了排版*/
        if (0 == 0)
        {
            if (ptL090_bal->unLasttrday < ptTblEvent->VAL_DATE)
            {
                ptL090_bal->unLasttrday = ptTblEvent->VAL_DATE;
            }
            if (ptL090_bal->unLasttrday < ptTblAcct->STM_CLOSDY)
            {
                ptL090_bal->unLasttrday = ptTblAcct->STM_CLOSDY;
            }
            if (ptL090_bal->unTodayrel < GLB_BUSINESS_DAY)
            {
                ptL090_bal->snTodayamt = 0;
                ptL090_bal->unTodayrel = GLB_BUSINESS_DAY;
            }
            if (ptL090_bal->unTodayrel == GLB_BUSINESS_DAY)
            {
                if (!(ptTblEvent->TRANS_TYPE >= 7000 && ptTblEvent->TRANS_TYPE < 8000)
                    && !(ptTblEvent->TRANS_TYPE >= 2000 && ptTblEvent->TRANS_TYPE < 3000))
                {
                    ptL090_bal->snTodayamt += ptTblEvent->BILL_AMT;
                }
            }

            /*更新最大借记/贷记金额*/
            /*BEG CHG CHG-140122-0003 本期息费不占额度*/
            /*snLatest_bal = L090_cur_bal + ptTblEvent->BILL_AMT;
            snLatest_loc = L090_local_bal + ptTblEvent->BILL_AMT;*/
            if (unAddamt_flag != 1)
            {
                snLatest_bal = L090_cur_bal + ptTblEvent->BILL_AMT;
                snLatest_loc = L090_local_bal + ptTblEvent->BILL_AMT;
            }
            else
            {
                snLatest_bal = L090_cur_bal;
                snLatest_loc = L090_local_bal;
            }
            /*END CHG CHG-140122-0003 本期息费不占额度*/
            /*del for CHG-131127-0029 by zyh 20131216 begin*/
            /* if (snLatest_bal >= 0)
            {
                if ((int)snLatest_bal > ptTblAcct->HI_DEBIT)
                {
                    ptL090_bal->unHidebit = snLatest_bal;
                    ptL090_bal->unHidebmmyy = unPur_mmyy;
                }
            }*/
            /*del for CHG-131127-0029 by zyh 20131216 end*/
            /* DEL FOR CS-6320-2012-0095
            else
            {
                snLatest_bal = fabs(snLatest_bal);
                if ((int)snLatest_bal > ptTblAcct->HI_CREDIT)
                {
                    ptL090_bal->unHicredt = snLatest_bal;
                    ptL090_bal->unHicrdmmyy = unPur_mmyy;
                }
            }*/
            /*超限标志处理*/

            L049_acct_crlim(ptTblAcct,&snCred_limit,&unTemp_flag);
            if (snLatest_loc > snCred_limit)
            {
                ptL090_bal->snOlimit_amt  = snLatest_loc - snCred_limit;
                ptL090_bal->unOlimit_flag = 1;
                /* 删除HI_OLIMIT
                if (ptL090_bal->snOlimit_amt > ptTblAcct->HI_OLIMIT)
                {
                    ptL090_bal->unHiolimit = ptL090_bal->snOlimit_amt;
                    ptL090_bal->unHiolimmyy = unPur_mmyy;
                }
                */
                if (L090_local_bal < snCred_limit)
                {
                    ptL090_bal->unNbrolimit++;
                }
            }
        }

        /*更新数据库字段*/
        if (1 == unScard_flag)
        {
            tTblCard.TOTAL_AMT = ptL090_bal->snScard_tamt;         /*附卡非还款交易发生额*/
            tTblCardflag.TOTAL_AMT = 1;
            tTblCard.ETL_DAY = GLB_BUSINESS_DAY;
            tTblCardflag.ETL_DAY = 1;
        }

        L090_7_acct_set(ptL090_bal, ptTblAcct, &tTblAcctflag);

        snResult = CardUpdate(&tTblCard,&tTblCardflag);
        if (snResult != 0)
        {
            CmdLog(LOGWAN, "CardUpdate error snResult[%ld]", snResult);
            return -1;
        }

        snResult = AcctUpdate(ptTblAcct,&tTblAcctflag);
        if (snResult != 0)
        {
            CmdLog(LOGWAN, "AcctUpdate error snResult[%ld]", snResult);
            return -1;
        }

    }
    /*外币交易处理*/
    else
    {
        memset(&tTblAccxflag, 0x00, sizeof(tTblAccxflag));

        /*add for CHG-130225-0018 by zyh begin 20130506*/
        snMinAgex = tTblPrmcn.MIN_AGEAMX;
        if((1 == tTblPrmcn.AGE_FLAG)&&(ptTblAccx->MIN_AGEAMT > tTblPrmcn.MIN_AGEAMX))
        {
            snMinAgex = ptTblAccx->MIN_AGEAMT;
        }
        /*add for CHG-130225-0018 by zyh end  20130506*/
        /*初始化冲减逾期金额的参数并冲减逾期,修改ACCX上逾期字段*/
        snResult = L090_1_pay_ageing(ptTblEvent,
                                     snMinAgex,
                                     /*tTblPrmcn.MIN_AGEAMX,*/
                                     ptTblAccx->AGEING_G,
                                     &ptTblAccx->ODUE_HELD,
                                     &ptTblAccx->MTHS_ODUE);
        if(snResult != 0)
        {
            CmdLog(LOGINF, "L090_1_pay_ageing error snResult[%ld]", snResult);
            return ((snResult < 0)?-1:10);
        }
        tTblAccxflag.AGEING_G = 1;
        tTblAccxflag.ODUE_HELD = 1;
        tTblAccxflag.MTHS_ODUE = 1;

        /*更新公司信息*/
        if (CmdGL(ptTblAcct->BUSINESS)!=0)
        {
            snResult = L090_5_busns_trans(ptTblAcct->CURR_NUM,
                                          ptL090_bal,
                                          ptTblEvent,
                                          &tTblBusns);
            if (snResult != 0)
            {
                CmdLog(LOGINF, "L090_5_busns_trans error snResult[%ld]", snResult);
                return ((snResult < 0)?-1:10);
            }
        }

        /*仅为排版*/
        if (0 == 0)
        {
            /*更新上一次交易日期与今日金额*/
            if (ptL090_bal->unLasttrday < ptTblEvent->VAL_DATE)
            {
                ptL090_bal->unLasttrday = ptTblEvent->VAL_DATE;
            }
            if (ptL090_bal->unTodayrel < GLB_BUSINESS_DAY)
            {
                ptL090_bal->snTodayamt = 0;
                ptL090_bal->unTodayrel = GLB_BUSINESS_DAY;
            }
            if (ptL090_bal->unTodayrel == GLB_BUSINESS_DAY)
            {
                if (!(ptTblEvent->TRANS_TYPE >= 7000 && ptTblEvent->TRANS_TYPE < 8000)
                    && !(ptTblEvent->TRANS_TYPE >= 2000 && ptTblEvent->TRANS_TYPE < 3000))
                {
                    ptL090_bal->snTodayamt += ptTblEvent->BILL_AMT;
                }
            }
            /*更新最大借记/贷记金额*/
            L083_conversion(ptTblAccx->CURR_NUM, GLB_BUSINESS_DAY, 0,
                            ptTblEvent->BILL_AMT, 0, 0, 0, &snConv_amt);
            /*BEG CHG CHG-140122-0003 本期息费不占额度*/
            /*snLatest_bal = L090_cur_bal + ptTblEvent->BILL_AMT;
            snLatest_loc = L090_local_bal + snConv_amt;*/
            if (unAddamt_flag != 1)
            {
                snLatest_bal = L090_cur_bal + ptTblEvent->BILL_AMT;
                snLatest_loc = L090_local_bal + snConv_amt;
            }
            else
            {
                snLatest_bal = L090_cur_bal;
                snLatest_loc = L090_local_bal;
            }
            /*END CHG CHG-140122-0003 本期息费不占额度*/
            /*del for CHG-131127-0029 by zyh 20131216 begin*/
            /*if (snLatest_bal >= 0)
            {
                if ((int)snLatest_bal > ptTblAccx->HI_DEBIT)
                {
                    ptL090_bal->unHidebit = snLatest_bal;
                    ptL090_bal->unHidebmmyy = unPur_mmyy;
                }
            }*/
            /*del for CHG-131127-0029 by zyh 20131216 end*/
            /* DEL FOR CS-6320-2012-0095
            else
            {

                snLatest_bal = fabs(snLatest_bal);
                if ((int)snLatest_bal > ptTblAccx->HI_CREDIT)
                {
                    ptL090_bal->unHicredt = snLatest_bal;
                    ptL090_bal->unHicrdmmyy = unPur_mmyy;
                }
            }*/

            /*超限标志处理*/
            L049_acct_crlim(ptTblAcct, &snCred_limit, &unTemp_flag);
            if (snLatest_loc > snCred_limit)
            {
                ptL090_bal->snOlimit_amt  = snLatest_loc - snCred_limit;
                ptL090_bal->unOlimit_flag = 1;
                if (L090_local_bal < snCred_limit)
                {
                    ptL090_bal->unNbrolimit++;
                }
            }
        }

        /*更新数据库信息*/
        if (1 == unScard_flag)
        {
            tTblCard.TOTAL_AMX = ptL090_bal->snScard_tamt;         /*附卡非还款交易发生额*/
            tTblCardflag.TOTAL_AMX = 1;
            tTblCard.ETL_DAY = GLB_BUSINESS_DAY;
            tTblCardflag.ETL_DAY = 1;
        }

        L090_7_accx_set(ptL090_bal, ptTblAccx, &tTblAccxflag);

        snResult = CardUpdate(&tTblCard,&tTblCardflag);
        if (snResult != 0)
        {
            CmdLog(LOGWAN, "CardUpdate error snResult[%ld]", snResult);
            return -1;
        }

        snResult = AccxUpdate(ptTblAccx,&tTblAccxflag);
        if (snResult != 0)
        {
            CmdLog(LOGWAN, "AccxUpdate error snResult[%ld]", snResult);
            return -1;
        }
    }

    if (ptTblAcct->OCT_DAYIN > 0)
    {
        snResult = L090_6_oct_proc(ptTblBanks,
                                   ptTblEvent,
                                   ptTblAcct,
                                   ptTblAccx,
                                   *pL090_exist_accx);

        if (snResult != 0)
        {
            CmdLog(LOGINF, "L090_6_oct_proc error snResult[%ld]", snResult);
            return ((snResult < 0)?-1:8);
        }
    }

    return 0;
}

/********************************************************************************
 *  L090_1_pay_ageing
 *  函数对贷记交易（除本月退货交易），冲减账户的0~6期逾期金额和逾期月份。
 *  本函数同时支持本币、外币贷记交易对逾期金额的冲减。
 *
 *  形参：
 *      输入：
 *          ptTblEvent          数据库表EVENT/当前待入账交易流水
 *          L090_minageamt      最小逾期金额
 *          pL090_ageing_g      1~6期逾期金额（对应ACCT或ACCX的AGEING_G字段）
 *          pL090_over_held     小额逾期金额
 *          pL090_mthsodue      逾期月份
 *
 *      输出：
 *          pL090_ageing_g      1~6期逾期金额（对应ACCT或ACCX的AGEING_G字段）
 *          pL090_over_held     小额逾期金额
 *          pL090_mthsodue      逾期月份
 *
 *  返回值：
 *******************************************************************************/

int L090_1_pay_ageing( event_tdef  *ptTblEvent,
                        UN9_2        L090_minageamt,
                        CH          *pL090_ageing_g,
                        UN11_2      *pL090_over_held,
                        SN2         *pL090_mthsodue)
{
    UN11_2  unArr_ageing[6];
    SN6     snResult = 0;

    L021_ageing_a2n(pL090_ageing_g,unArr_ageing);

    /*非撤销/非冲正/非当月退货的贷记交易*/
    if ((ptTblEvent->BILL_AMT < 0)
        && (ptTblEvent->REV_IND[0] != '1')
        && (strcmp(ptTblEvent->BANKACCT, "XXXXX") != 0))
    {
        snResult = L081_pay_ageing(ptTblEvent->BILL_AMT,
                                   L090_minageamt,
                                   unArr_ageing,
                                   pL090_over_held,
                                   pL090_mthsodue);
        if(snResult != 0)
        {
            CmdLog(LOGINF, "L081_pay_ageing error snResult[%ld]", snResult);
            return ((snResult < 0)?-1:10);
        }
    }
    L021_ageing_n2a(unArr_ageing,pL090_ageing_g);
    return 0;
}


/********************************************************************************
 *  L090_4_acct_trans
 *  (1)对本币交易，根据交易流水计算当前账户联机交易笔数和金额；
 *  (2)根据交易流水和账户情况，修改账户交易的各项交易日期、最高交易情况；
 *  (3)根据账户当前余额，判断账户是否超限；
 *
 *  形参：
 *      输入：
 *          ptTblEvent      数据库表EVENT/待入账交易流水记录
 *          ptTblAcct       数据库表ACCT/当前账户记录
 *      输出：
 *          ptL090_bal          当前账户联机交易笔数和金额
 *
 *  返回值：
 *          0 - 成功
 *          1 - 交易类型不合法
 *          2 - 币种不正确
 *          3 - 日期转换失败
 *******************************************************************************/

int L090_4_acct_trans(  event_tdef      *ptTblEvent,
                        acct_tdef       *ptTblAcct,
                        prmcn_tdef   *ptTblPrmcn,/*add for CHG-140410-0029 */
                        L090_bal_sdef   *ptL090_bal)
{
    UN1     unUpd_error = 1; /*默认不成功*/
    UN8     unPur_mmyy = 0;
    SN6     snResult = 0;
    SN11_2  TmpsnL063_cup_od_amt = 0;
    UN1    unLoanCrvjtFlag = 0;/*单笔核算账户退货交易标识add for CHG-140410-0029*/

    snResult = CmdDC(DC_DAYNUM, &GLB_BUSINESS_DAY);
    if(snResult != 0)
    {
        CmdLog(LOGWAN, "CmdDC error snResult[%ld]", snResult);
        return 3;
    }
    unPur_mmyy = atoi(GLB_DC_DATE.dc_ddmmyy) % 10000;

    if (ptTblEvent->CURRNCY_CD != ptTblAcct->CURR_NUM)
    {
        CmdLog(LOGWAN, "币种不正确  Event.rowid[%s] Acct.rowid[%s]",
               ptTblEvent->ROWID, ptTblAcct->ROWID);
        return  2;
    }

    /*累加统计金额*/
    if ((strcmp(ptTblEvent->TRANS_SRC, "CADJT") == 0)
        || (strcmp(ptTblEvent->TRANS_SRC, "CADJ1") == 0)
        || (strcmp(ptTblEvent->TRANS_SRC, "FEEJT") == 0))
    {
        if (ptTblEvent->BILL_AMT >= 0)
        {
            ptL090_bal->snDebitadj += ptTblEvent->BILL_AMT;
            unUpd_error = 0;
        }
        else
        {
            ptL090_bal->snCredtadj += ptTblEvent->BILL_AMT;
            unUpd_error = 0;
        }
    }
    else
    {
        if ((ptTblEvent->TRANS_TYPE >= 1000) && (ptTblEvent->TRANS_TYPE < 2000))
        {
            /*消费类交易*/
            ptL090_bal->snPurchases += ptTblEvent->BILL_AMT;
            if (ptTblEvent->BILL_AMT > ptTblAcct->HI_PURCHSE)
            {
                ptL090_bal->unHipurchse = ptTblEvent->BILL_AMT;
                ptL090_bal->unHipurmmyy = unPur_mmyy;
            }
            ptL090_bal->unNbrpurch++;
            unUpd_error = 0;
        }
        else if ((ptTblEvent->TRANS_TYPE >= 2000) && (ptTblEvent->TRANS_TYPE < 3000))
        {
            /*预借现金类交易*/
            if ((ptTblEvent->TRANS_TYPE >= 2300) && (ptTblEvent->TRANS_TYPE < 2500))
            {
                ptL090_bal->snCredtadj += ptTblEvent->BILL_AMT;
            }
            else
            {
                /*del for CHG-131127-0029 by zyh 20131216 begin*/
                /*if (ptTblAcct->CASH1STAC == 0)
                {
                    ptL090_bal->unCash1stac = GLB_BUSINESS_DAY;
                }*/
                /*del for CHG-131127-0029 by zyh 20131216 end*/
                ptL090_bal->snCashadvce += ptTblEvent->BILL_AMT;
                ptL090_bal->snCashadvce -= ptTblEvent->OTH_AMT;
                if (ptTblEvent->BILL_AMT > ptTblAcct->HI_CASHADV)
                {
                    ptL090_bal->unHicashadv  = ptTblEvent->BILL_AMT;
                    ptL090_bal->unHicashmmyy = unPur_mmyy;
                }
                ptL090_bal->unNbrcashad++;
            }
            unUpd_error = 0;
        }
        else if ((ptTblEvent->TRANS_TYPE >= 3000) && (ptTblEvent->TRANS_TYPE < 3130))
        {
            /*费用类交易*/
            ptL090_bal->snCardfees += ptTblEvent->BILL_AMT;
            ptL090_bal->unNbrfeedty++;
            unUpd_error = 0;
        }
        else if ((ptTblEvent->TRANS_TYPE >= 3130) && (ptTblEvent->TRANS_TYPE < 3500))
        {
            ptL090_bal->snOtherfees += ptTblEvent->BILL_AMT;
            ptL090_bal->unNbrothers++;
            unUpd_error = 0;
        }
        else if (ptTblEvent->TRANS_TYPE == 3510)
        {
            ptL090_bal->snCredtadj += ptTblEvent->BILL_AMT;
            unUpd_error = 0;
        }
        else if ((ptTblEvent->TRANS_TYPE == 3500) || (ptTblEvent->TRANS_TYPE == 3550))
        {
            ptL090_bal->snPen_chrg += ptTblEvent->BILL_AMT;
            ptL090_bal->unNbrothers++;
            unUpd_error = 0;
        }
        else if ((ptTblEvent->TRANS_TYPE >= 3600) && (ptTblEvent->TRANS_TYPE < 3700))
        {
            ptL090_bal->snCredtadj += ptTblEvent->BILL_AMT;
            unUpd_error = 0;
        }
        else if ((ptTblEvent->TRANS_TYPE >= 4000) && (ptTblEvent->TRANS_TYPE < 4900))
        {
            if ((ptTblEvent->TRANS_TYPE >= 4300) && (ptTblEvent->TRANS_TYPE < 4500))
            {
                ptL090_bal->snCredtadj += ptTblEvent->BILL_AMT;
            }
            else
            {
                ptL090_bal->snCashadfee += ptTblEvent->BILL_AMT;
                ptL090_bal->unNbrfeedty++;
            }
            unUpd_error = 0;
        }
        else if ((ptTblEvent->TRANS_TYPE >= 4900) && (ptTblEvent->TRANS_TYPE < 4990))
        {
            ptL090_bal->snFees_taxes += ptTblEvent->BILL_AMT;
            unUpd_error = 0;
        }
        else if ((ptTblEvent->TRANS_TYPE >= 4990) && (ptTblEvent->TRANS_TYPE < 5000))
        {
            ptL090_bal->snOtherfees += ptTblEvent->BILL_AMT;
            ptL090_bal->unNbrothers++;
            unUpd_error = 0;
        }
        else if ((ptTblEvent->TRANS_TYPE >= 5000) && (ptTblEvent->TRANS_TYPE < 5100))
        {
            /*ADD XYK-6403-2011-089 新增TRANS_SRC判断，账单日不再调用L090统计所有利息均走消费*/
            ptL090_bal->snPurchases += ptTblEvent->BILL_AMT;
            unUpd_error = 0;
        }
        else if ((ptTblEvent->TRANS_TYPE >= 5500) && (ptTblEvent->TRANS_TYPE < 5600))
        {
            ptL090_bal->snInt_earned += ptTblEvent->BILL_AMT;
            unUpd_error = 0;
        }
        else if ((ptTblEvent->TRANS_TYPE >= 6000) && (ptTblEvent->TRANS_TYPE < 7000))
        {
            if (strcmp(ptTblEvent->BANKACCT, "XXXXX") == 0)
            {
                ptL090_bal->snPurchases += ptTblEvent->BILL_AMT;
            }
            else
            {
                ptL090_bal->snCredvouch += ptTblEvent->BILL_AMT;
            }
            /*add for CHG-140410-0029 begin*/
            /*读取PRMCN*/
            if ((0 == CmdGL(ptTblPrmcn->ROWID)) && (0 == ptTblPrmcn->BANK) )
            {
                snResult = Pprmcnbnk(ptTblAcct->BANK, ptTblAcct->CATEGORY,ptTblPrmcn);
                if (snResult != 0 && snResult != DB_NOT_FOUND)
                {
                    CmdLog(LOGWAN, "Pprmcnbnk error snResult[%ld]", snResult);
                    return -1;
                }
                if (DB_NOT_FOUND == snResult)
                {
                    CmdLog(LOGWAN, "Pprmcnbnk 未找到Prmcn记录  bank[%d] category[%d]",
                           ptTblAcct->BANK, ptTblAcct->CATEGORY);
                    return 3;
                }
            }
            /*单笔核算账户，退货当成当日还款进行处理，需累加当日还款金额*/
            if(ptTblPrmcn->LOAN_TYPE != 0)
            {
                /*ptL090_bal->snPaymttday += ptTblEvent->BILL_AMT;*/
                unLoanCrvjtFlag = 1;/*单笔核算账户退货交易标识add for CHG-140410-0029*/
            }
            /*add for CHG-140410-0029 end*/
            unUpd_error = 0;
        }
        else if ((ptTblEvent->TRANS_TYPE >= 7000) && (ptTblEvent->TRANS_TYPE < 8000))
        {
            ptL090_bal->snPaymtclrd += ptTblEvent->BILL_AMT;
            ptL090_bal->unNbrpaymnt++;
            unUpd_error = 0;
        }
        else if (ptTblEvent->TRANS_TYPE >= 8000)
        {
            if (ptTblEvent->TRANS_TYPE == 8950)
            {
                ptL090_bal->snPurchases += ptTblEvent->BILL_AMT;
            }
            else if (ptTblEvent->TRANS_TYPE == 8952)
            {
                ptL090_bal->snCashadvce += ptTblEvent->BILL_AMT;
            }
            else
            {
                if (ptTblEvent->BILL_AMT < 0)
                {
                    ptL090_bal->snCredtadj += ptTblEvent->BILL_AMT;
                }
                else
                {
                    ptL090_bal->snDebitadj += ptTblEvent->BILL_AMT;
                }
            }
            unUpd_error = 0;
        }

        /*ADD FOR CS-6320-2012-0095 BEGIN*/
        if ((1050 == ptTblEvent->TRANS_TYPE) || (5050 ==ptTblEvent->TRANS_TYPE))
        {
            ptL090_bal->snMp_instal  +=  ptTblEvent->BILL_AMT;
            unUpd_error = 0;
        }
        if((5000 == ptTblEvent->TRANS_TYPE)&&(strcmp(ptTblEvent->TRANS_SRC,"STMTUPDATE")!= 0))
        {
            ptL090_bal->snMp_instal  +=  ptTblEvent->BILL_AMT;
            unUpd_error = 0;
        }
        /*ADD FOR CS-6320-2012-0095 END*/
    }

    /*ADD FOR CS-6320-2012-0095 BEGIN*/
    /*由于接口问题，此处只在还款时调用L063,其他的交易由授权调用*/
    if((ptTblEvent->TRANS_TYPE >= 7000)&&(ptTblEvent->TRANS_TYPE < 8000))
    /*(ptTblEvent->TRANS_TYPE >= 1000)&&(ptTblEvent->TRANS_TYPE < 3000))*/
    {
        snResult = L063_acca_updt(ptTblEvent,ptTblAcct,"", &TmpsnL063_cup_od_amt);
        if(snResult != 0)
        {
            CmdLog(LOGWAN, "L063_acca_updt error snResult[%ld]", snResult);
            return -1;
        }
    }
    /*ADD FOR CS-6320-2012-0095 END*/

    /*更新上次还款日期/金额/今日还款/今日取现*/
    if (0 == unUpd_error)
    {
        if( ((ptTblEvent->TRANS_TYPE >= 7000) && (ptTblEvent->TRANS_TYPE < 8000))
            ||(1 == unLoanCrvjtFlag))/*单笔核算账户退货交易标识add for CHG-140410-0029*/
        {
            if (ptTblEvent->BILL_AMT < 0)
            {
                if (ptTblEvent->INP_DATE == GLB_BUSINESS_DAY)
                {
                    if (ptTblEvent->INP_DATE == ptTblAcct->LASTPAYDAY)
                    {
                        ptL090_bal->snPaymttday += ptTblEvent->BILL_AMT;
                    }
                    if (ptTblEvent->INP_DATE > ptTblAcct->LASTPAYDAY)
                    {
                        ptL090_bal->snPaymttday = ptTblEvent->BILL_AMT;
                    }
                }
                if (ptTblEvent->INP_DATE >= ptTblAcct->LASTPAYDAY)
                {
                    ptL090_bal->snLastpayamt = ptTblEvent->BILL_AMT;
                    ptL090_bal->unLastpayday = ptTblEvent->INP_DATE;
                }
            }
            if (ptTblEvent->BILL_AMT > 0)
            {
                if (ptTblEvent->INP_DATE == ptTblEvent->VAL_DATE)
                {
                    ptL090_bal->snPaymttday += ptTblEvent->BILL_AMT;
                }
                else
                {
                    ptL090_bal->snCashtday += ptTblEvent->BILL_AMT;
                }
                if (ptTblEvent->VAL_DATE == ptTblAcct->LASTPAYDAY)
                {
                    ptL090_bal->snLastpayamt = 0;
                }
            }
        }
        if ((ptTblEvent->TRANS_TYPE >= 2000) && (ptTblEvent->TRANS_TYPE < 3000))
        {
            if (ptTblEvent->INP_DATE == ptTblEvent->VAL_DATE)
            {
                if (ptTblEvent->INP_DATE <= ptL090_bal->unLasttrday)
                {
                    ptL090_bal->snCashtday += ptTblEvent->BILL_AMT;
                    ptL090_bal->snCashtday -= ptTblEvent->OTH_AMT;
                }
                else
                {
                    ptL090_bal->snCashtday = ptTblEvent->BILL_AMT;
                }
            }
        }
        else
        {
            if (ptTblEvent->VAL_DATE > ptL090_bal->unLasttrday)
            {
                ptL090_bal->snCashtday = 0;
            }
        }

        /*BEG ADD CHG-111201-0018*/
        if(ptTblEvent->VAL_DATE > ptL090_bal->unLasttrday)
        {
          ptL090_bal->snIntfetday = 0.00;
        }

        if ((ptTblEvent->TRANS_TYPE >= 3000) && (ptTblEvent->TRANS_TYPE < 6000))
        {
            if((ptTblEvent->TRANS_TYPE >= 5000) && (ptTblEvent->TRANS_TYPE < 5100))
            {
            }
            else
            {
                if(ptTblEvent->INP_DATE == ptTblEvent->VAL_DATE)
                {
                    ptL090_bal->snIntfetday += ptTblEvent->BILL_AMT;
                }
            }
        }
        /*END ADD CHG-111201-0018*/

        ptL090_bal->unNbrtrans++;
    }
    else
    {
        CmdLog(LOGWAN, "交易类型[%d]错误", ptTblEvent->TRANS_TYPE);
        return 1;
    }
    return 0;
}

/********************************************************************************
 *  L090_4_accx_trans
 *  (1)对外币交易，根据交易流水计算当前账户联机交易笔数和金额；
 *  (2)根据交易流水和账户情况，修改账户交易的各项交易日期、最高交易情况；
 *  (3)根据账户当前余额，判断账户是否超限；
 *
 *  形参：
 *      输入：
 *          ptTblEvent          数据库表EVENT/待入账交易流水记录
 *          ptTblAccx           数据库表ACCX/当前外币账户记录
 *      输出：
 *          ptL090_bal          当前账户联机交易笔数和金额
 *
 *  返回值：
 *          0 - 成功
 *          1 - 交易类型不正确
 *          2 - 币种不正确
 *          3 - 日期转换失败
 *******************************************************************************/

int L090_4_accx_trans(  event_tdef      *ptTblEvent,
                        accx_tdef       *ptTblAccx,
                        L090_bal_sdef   *ptL090_bal)
{
    UN1     unUpd_error = 1; /*默认不成功*/
    UN8     unPur_mmyy = 0;
    SN6     snResult = 0;

    snResult = CmdDC(DC_DAYNUM,&GLB_BUSINESS_DAY);
    if(snResult != 0)
    {
        CmdLog(LOGWAN, "CmdDC error snResult[%ld]", snResult);
        return 3;
    }
    unPur_mmyy = atoi(GLB_DC_DATE.dc_ddmmyy) % 10000;

    if (ptTblEvent->CURRNCY_CD != ptTblAccx->CURR_NUM)
    {
        CmdLog(LOGWAN, "币种不正确  Event.rowid[%s] Accx.rowid[%s]",
               ptTblEvent->ROWID, ptTblAccx->ROWID);
        return  2;
    }

    /*页面息费调整交易*/
    if ((strcmp(ptTblEvent->TRANS_SRC,"CADJT") == 0)
        || (strcmp(ptTblEvent->TRANS_SRC,"CADJ1") == 0)
        || (strcmp(ptTblEvent->TRANS_SRC,"FEEJT") == 0))
    {
        if (ptTblEvent->BILL_AMT >= 0)
        {
            ptL090_bal->snDebitadj += ptTblEvent->BILL_AMT;
            unUpd_error = 0;
        }
        else
        {
            ptL090_bal->snCredtadj += ptTblEvent->BILL_AMT;
            unUpd_error = 0;
        }
    }
    else
    {
        if ((ptTblEvent->TRANS_TYPE >= 1000) && (ptTblEvent->TRANS_TYPE < 2000))
        {
            /*消费类交易*/
            ptL090_bal->snPurchases += ptTblEvent->BILL_AMT;
            if (ptTblEvent->BILL_AMT > ptTblAccx->HI_PURCHSE)
            {
                ptL090_bal->unHipurchse = ptTblEvent->BILL_AMT;
                ptL090_bal->unHipurmmyy = unPur_mmyy;
            }
            ptL090_bal->unNbrpurch++;
            unUpd_error = 0;
        }
        else if ((ptTblEvent->TRANS_TYPE >= 2000) && (ptTblEvent->TRANS_TYPE < 3000))
        {
            /*预借现金类交易*/
            if ((ptTblEvent->TRANS_TYPE >= 2300 ) && (ptTblEvent->TRANS_TYPE < 2600))
            {
                ptL090_bal->snCredtadj += ptTblEvent->BILL_AMT;
            }
            else
            {
                ptL090_bal->snCashadvce += ptTblEvent->BILL_AMT;
                ptL090_bal->snCashadvce -= ptTblEvent->OTH_AMT;
                if (ptTblEvent->BILL_AMT > ptTblAccx->HI_CASHADV)
                {
                    ptL090_bal->unHicashadv  = ptTblEvent->BILL_AMT;
                    ptL090_bal->unHicashmmyy = unPur_mmyy;
                }
                ptL090_bal->unNbrcashad++;
            }
            unUpd_error = 0;
        }
        else if ((ptTblEvent->TRANS_TYPE >= 3000) && (ptTblEvent->TRANS_TYPE < 3130))
        {
            /*费用类交易*/
            ptL090_bal->snCardfees += ptTblEvent->BILL_AMT;
            ptL090_bal->unNbrfeedty++;
            unUpd_error = 0;
        }
        else if ((ptTblEvent->TRANS_TYPE >= 3130) && (ptTblEvent->TRANS_TYPE < 3500))
        {
            ptL090_bal->snOtherfees += ptTblEvent->BILL_AMT;
            ptL090_bal->unNbrothers++;
            unUpd_error = 0;
        }
        else if (ptTblEvent->TRANS_TYPE == 3510)
        {
            ptL090_bal->snCredtadj += ptTblEvent->BILL_AMT;
            unUpd_error = 0;
        }
        else if ((ptTblEvent->TRANS_TYPE == 3501) || (ptTblEvent->TRANS_TYPE == 3551))
        {
            ptL090_bal->snPen_chrg += ptTblEvent->BILL_AMT;
            ptL090_bal->unNbrothers++;
            unUpd_error = 0;
        }
        else if ((ptTblEvent->TRANS_TYPE >= 3600) && (ptTblEvent->TRANS_TYPE < 3700))
        {
            ptL090_bal->snCredtadj += ptTblEvent->BILL_AMT;
            unUpd_error = 0;
        }
        else if ((ptTblEvent->TRANS_TYPE >= 4000) && (ptTblEvent->TRANS_TYPE < 4900))
        {
            if ((ptTblEvent->TRANS_TYPE >= 4300) && (ptTblEvent->TRANS_TYPE < 4600))
            {
                ptL090_bal->snCredtadj += ptTblEvent->BILL_AMT;
            }
            else
            {
                ptL090_bal->snCashadfee += ptTblEvent->BILL_AMT;
                ptL090_bal->unNbrfeedty++;
            }
            unUpd_error = 0;
        }
        else if ((ptTblEvent->TRANS_TYPE >= 4900) && (ptTblEvent->TRANS_TYPE < 4990))
        {
            ptL090_bal->snFees_taxes += ptTblEvent->BILL_AMT;
            unUpd_error = 0;
        }
        else if ((ptTblEvent->TRANS_TYPE >= 5000) && (ptTblEvent->TRANS_TYPE < 5100))
        {
            /*ADD XYK-6403-2011-089 新增TRANS_SRC判断，账单日不再调用L090统计所有利息均走消费*/
            ptL090_bal->snPurchases += ptTblEvent->BILL_AMT;
            unUpd_error = 0;
        }
        else if ((ptTblEvent->TRANS_TYPE >= 5500) && (ptTblEvent->TRANS_TYPE < 5600))
        {
            ptL090_bal->snInt_earned += ptTblEvent->BILL_AMT;
            unUpd_error = 0;
        }
        else if ((ptTblEvent->TRANS_TYPE >= 6000) && (ptTblEvent->TRANS_TYPE < 7000))
        {
            ptL090_bal->snCredvouch += ptTblEvent->BILL_AMT;
            unUpd_error = 0;
        }
        else if ((ptTblEvent->TRANS_TYPE >= 7000) && (ptTblEvent->TRANS_TYPE < 8000))
        {
            ptL090_bal->snPaymtclrd += ptTblEvent->BILL_AMT;
            ptL090_bal->unNbrpaymnt++;
            unUpd_error = 0;
        }
        else if (ptTblEvent->TRANS_TYPE >= 8000)
        {
            if (ptTblEvent->TRANS_TYPE == 8951)
            {
                ptL090_bal->snPurchases += ptTblEvent->BILL_AMT;
            }
            else if (ptTblEvent->TRANS_TYPE == 8953)
            {
                ptL090_bal->snCashadvce += ptTblEvent->BILL_AMT;
            }
            else
            {
                if (ptTblEvent->BILL_AMT < 0)
                {
                    ptL090_bal->snCredtadj += ptTblEvent->BILL_AMT;
                }
                else
                {
                    ptL090_bal->snDebitadj += ptTblEvent->BILL_AMT;
                }
            }
            unUpd_error = 0;
        }
    }

    /*累加当日金额*/
    if (unUpd_error == 0)
    {
        if ((ptTblEvent->TRANS_TYPE >= 7000) && (ptTblEvent->TRANS_TYPE < 8000))
        {
            if (ptTblEvent->BILL_AMT < 0)
            {
                ptL090_bal->snPaymtxday += ptTblEvent->BILL_AMT;
                if (ptTblEvent->INP_DATE == GLB_BUSINESS_DAY)
                {
                    if (ptTblEvent->INP_DATE == ptTblAccx->LASTPAYDAY)
                    {
                        ptL090_bal->snPaymttday += ptTblEvent->BILL_AMT;
                    }
                    if (ptTblEvent->INP_DATE > ptTblAccx->LASTPAYDAY)
                    {
                        ptL090_bal->snPaymttday = ptTblEvent->BILL_AMT;
                    }
                }
                if (ptTblEvent->INP_DATE >= ptTblAccx->LASTPAYDAY)
                {
                    ptL090_bal->snLastpayamt = ptTblEvent->BILL_AMT;
                    ptL090_bal->unLastpayday = ptTblEvent->INP_DATE;
                }
            }
            if (ptTblEvent->BILL_AMT > 0)
            {
                if (ptTblEvent->INP_DATE == ptTblEvent->VAL_DATE)
                {
                    ptL090_bal->snPaymtxday += ptTblEvent->BILL_AMT;
                    ptL090_bal->snPaymttday += ptTblEvent->BILL_AMT;
                }
                else
                {
                    ptL090_bal->snCashtday += ptTblEvent->BILL_AMT;
                }
                if (ptTblEvent->VAL_DATE == ptTblAccx->LASTPAYDAY)
                {
                    ptL090_bal->snLastpayamt = 0;
                }
            }
        }
        if ((ptTblEvent->TRANS_TYPE >= 2000) && (ptTblEvent->TRANS_TYPE < 3000))
        {
            if (ptTblEvent->INP_DATE == ptTblEvent->VAL_DATE)
            {
                if (ptTblEvent->INP_DATE <= ptL090_bal->unLasttrday)
                {
                    ptL090_bal->snCashtday += ptTblEvent->BILL_AMT;
                    ptL090_bal->snCashtday -= ptTblEvent->OTH_AMT;
                }
                else
                {
                    ptL090_bal->snCashtday = ptTblEvent->BILL_AMT;
                }
            }
        }
        else
        {
            if (ptTblEvent->VAL_DATE > ptL090_bal->unLasttrday)
            {
                ptL090_bal->snCashtday = 0;
            }
        }

        /*BEG ADD CHG-111201-0018*/
		if(ptTblEvent->VAL_DATE > ptL090_bal->unLasttrday)
		{
		    ptL090_bal->snIntfetday = 0.00;
		}

        if ((ptTblEvent->TRANS_TYPE >= 3000) && (ptTblEvent->TRANS_TYPE < 6000))
        {
            if((ptTblEvent->TRANS_TYPE >= 5000) && (ptTblEvent->TRANS_TYPE < 5100))
            {
            }
			else
			{
			    if(ptTblEvent->INP_DATE == ptTblEvent->VAL_DATE)
			    {
			        ptL090_bal->snIntfetday += ptTblEvent->BILL_AMT;
			    }
			}
        }
		/*END ADD CHG-111201-0018*/

        ptL090_bal->unNbrtrans++;
    }
    else
    {
        CmdLog(LOGWAN, "交易类型[%d]错误", ptTblEvent->TRANS_TYPE);
        return 1;
    }
    return 0;
}

/********************************************************************************
 *  L090_5_busns_trans
 *  对公司卡账户，根据交易流水修改公司各联机金额，并至公司修改记录标志位
 *
 *  形参：
 *      输入：
 *          L090_curr           本币币种
 *          ptL090_bal          公司卡授权金额
 *          ptTblEvent          数据库表EVENT/待入账交易流水
 *          ptTblBusns          数据库表BUSNS/账户对应公司记录
 *      输出：
 *          ptTblBusns          数据库表BUSNS/账户对应公司记录
 *
 *  返回值：
 *          0 - 成功
 *          1 - 失败
 *******************************************************************************/

int L090_5_busns_trans( SN3              L090_curr,
                        L090_bal_sdef   *ptL090_bal,
                        event_tdef      *ptTblEvent,
                        busns_tdef      *ptTblBusns)
{
    SN6     snResult = 0;
    busnsflag_tdef  tTblBusnsflag;

    memset(&tTblBusnsflag, 0x00, sizeof(tTblBusnsflag));
    /*本币统计处理*/
    if (ptTblEvent->CURRNCY_CD == L090_curr)
    {
        if ((ptTblEvent->TRANS_TYPE >= 1000 && ptTblEvent->TRANS_TYPE < 2000)
            || (ptTblEvent->TRANS_TYPE == 5050)
            || ((strcmp(ptTblEvent->BANKACCT,"XXXXX") == 0)
                && ((ptTblEvent->TRANS_TYPE >= 6000) && (ptTblEvent->TRANS_TYPE < 7000))))
        {
            /*消费类交易/退货类交易/分期应收利息*/
            if(ptTblEvent->TRANS_TYPE != 1050)/*add for CHG-130509-0001 by zyh 20131114*/
            {
                ptTblBusns->BAL_PUR += ptTblEvent->BILL_AMT;
                tTblBusnsflag.BAL_PUR = 1;
            }
        }
        else if ((ptTblEvent->TRANS_TYPE >= 2000 && ptTblEvent->TRANS_TYPE < 2300)
                 || (ptTblEvent->TRANS_TYPE >= 2500 && ptTblEvent->TRANS_TYPE < 3000))
        {
            /*预借现金类(除跨账单日撤销交易/跨行转账撤销交易)*/
            ptTblBusns->BAL_CASH += ptTblEvent->BILL_AMT;
            tTblBusnsflag.BAL_CASH = 1;
        }
        else if ((ptTblEvent->TRANS_TYPE >= 7000) && (ptTblEvent->TRANS_TYPE < 8000))
        {
            /*还款类交易*/
            ptTblBusns->BAL_PAY += ptTblEvent->BILL_AMT;
            tTblBusnsflag.BAL_PAY = 1;
        }
        else
        {
            if((ptTblEvent->TRANS_TYPE != 6050)/*add for CHG-131030-0014 by zyh 20140210*/
            &&(ptTblEvent->TRANS_TYPE != 8950)
            &&(ptTblEvent->TRANS_TYPE != 8952)
            &&(ptTblEvent->TRANS_TYPE != 8956))/*add for CHG-130509-0001 by zyh 20131114*/
            {
                /*所有其他类的交易发生额累加*/
                ptTblBusns->BAL_OTHER += ptTblEvent->BILL_AMT;
                tTblBusnsflag.BAL_OTHER = 1;
            }
        }
    }
    else
    {
        /*外币统计处理*/
        if ((ptTblEvent->TRANS_TYPE >= 1000) && (ptTblEvent->TRANS_TYPE < 2000))
        {
            /*消费类交易*/
            if(ptTblEvent->TRANS_TYPE != 1051)/*add for CHG-130509-0001 by zyh 20131114*/
            {
                ptTblBusns->BAL_PURX += ptTblEvent->BILL_AMT;
                tTblBusnsflag.BAL_PURX = 1;
            }
        }
        else if (((ptTblEvent->TRANS_TYPE >= 2000) && (ptTblEvent->TRANS_TYPE < 2300))
                || ((ptTblEvent->TRANS_TYPE >= 2600) && ptTblEvent->TRANS_TYPE < 3000))
        {
            /*预借现金类(除跨账单日撤销交易/跨行转账撤销交易)*/
            ptTblBusns->BAL_CASHX += ptTblEvent->BILL_AMT;
            tTblBusnsflag.BAL_CASHX = 1;
        }
        else if ((ptTblEvent->TRANS_TYPE >= 7000) && (ptTblEvent->TRANS_TYPE < 8000))
        {
            /*还款类交易*/
            ptTblBusns->BAL_PAYX += ptTblEvent->BILL_AMT;
            tTblBusnsflag.BAL_PAYX = 1;
        }
        else
        {
            if((ptTblEvent->TRANS_TYPE != 6051)/*add for CHG-131030-0014 by zyh 20140210*/
            &&(ptTblEvent->TRANS_TYPE != 8951)
            &&(ptTblEvent->TRANS_TYPE != 8953)
            &&(ptTblEvent->TRANS_TYPE != 8957))/*add for CHG-130509-0001 by zyh 20131114*/
            {
                /*所有其他类的交易发生额累加*/
                ptTblBusns->BAL_OTHERX += ptTblEvent->BILL_AMT;
                tTblBusnsflag.BAL_OTHERX = 1;
            }
        }
    }

    snResult = BusnsUpdate(ptTblBusns,&tTblBusnsflag);
    if (snResult != 0)
    {
        CmdLog(LOGWAN, "BusnsUpdate error snResult[%ld]", snResult);
        return -1;
    }

    return 0;
}

/********************************************************************************
 *  L090_6_oct_proc
 *  贷记交易或还款撤销对催收账户考察出催情况和承诺还款情况。
 *
 *  形参：
 *      输入：
 *          ptTblBanks          数据库表BANKS/当前银行记录
 *          ptTblEvent          数据库表EVENT/当前交易流水记录
 *          ptTblAcct           数据库表ACCT/当前本币账户
 *          ptTblAccx           数据库表ACCX/当前外币账户
 *          L090_exist_accx     是否存在外币账户  0 不存在 1存在
 *      输出：
 *
 *  返回值：
 *         -1 - 数据库失败
 *          0 - 成功
 *          1 - 调用L040出错
 *******************************************************************************/

int L090_6_oct_proc(    banks_tdef      *ptTblBanks,
                        event_tdef      *ptTblEvent,
                        acct_tdef       *ptTblAcct,
                        accx_tdef       *ptTblAccx,
                        UN1             L090_exist_accx)
{
    SN6     snResult  = 0;
    SN11_2  snSum_amt = 0;
    SN11_2  snSum_amx = 0;
    UN11_2  unArr_ageing[6];

    oacc_tdef       tTblOacc;
    ocltr_tdef      tTblOcltr;
    time_sdef       tTime;
    oaccflag_tdef   tTblOaccflag;
    ocltrflag_tdef  tTblOcltrflag;
    acctflag_tdef   tTblAcctflag;
    L040_bal_sdef   tL040_bal;
    L040_avail_sdef tL040_avail;

    if (ptTblAcct->OCT_DAYIN != 0)
    {
        memset(&tTblOacc, 0x00, sizeof(tTblOacc));
        memset(&tTblOaccflag, 0x00, sizeof(tTblOaccflag));
        memset(&tTblAcctflag, 0x00, sizeof(tTblAcctflag));
        snResult = Poaccactv(ptTblAcct->BANK, 0, 0, ptTblAcct->XACCOUNT, &tTblOacc);
        if ((snResult != 0) && (snResult != DB_NOT_FOUND))
        {
            CmdLog(LOGWAN, "Poaccactv error snResult[%ld]", snResult);
            return -1;
        }
        /*无催收记录则将ACCT中入催日期清除*/
        if (snResult == DB_NOT_FOUND)
        {
            ptTblAcct->OCT_DAYIN = 0;
            tTblAcctflag.OCT_DAYIN = 1;

            ptTblAcct->ETL_DAY = GLB_BUSINESS_DAY;
            tTblAcctflag.ETL_DAY = 1;
        }
        else
        {
            /*贷记交易*/
            if (ptTblEvent->BILL_AMT < 0)
            {
                /*非撤销/非冲正/非退货的贷记交易冲减催收承诺还款未还金额*/
                snSum_amt = 0;
                if ((ptTblEvent->REV_IND[0] != '1')
                    && (strcmp(ptTblEvent->BANKACCT,"XXXXX") != 0))
                {
                    snSum_amt = ((0 == ptTblEvent->CURRNCY_CD) || (ptTblAcct->CURR_NUM == ptTblEvent->CURRNCY_CD))? \
                                 tTblOacc.PPAY_UNPD : tTblOacc.PPAYX_UNPD;
                    snSum_amt += ptTblEvent->BILL_AMT;
                    snSum_amt = (snSum_amt>0)?snSum_amt:0;
                    if ((0 == ptTblEvent->CURRNCY_CD) || (ptTblAcct->CURR_NUM == ptTblEvent->CURRNCY_CD))
                    {
                        tTblOacc.PPAY_UNPD = snSum_amt;
                        tTblOaccflag.PPAY_UNPD = 1;
                    }
                    else
                    {
                        tTblOacc.PPAYX_UNPD = snSum_amt;
                        tTblOaccflag.PPAYX_UNPD = 1;
                    }
                    tTblOacc.ETL_DAY = GLB_BUSINESS_DAY;
                    tTblOaccflag.ETL_DAY = 1;
                }

                /*累加逾期金额还清则出催*/
                snSum_amt = 0;

                /*已止付账户需还清当前余额*/
                if (tTblOacc.CARD_STAT[0] == 'B')
                {
                    memset(&tL040_bal, 0x00, sizeof(tL040_bal));
                    memset(&tL040_avail, 0x00, sizeof(tL040_avail));
                    snResult = L040_account_bal(ptTblBanks,ptTblAcct,ptTblAccx,
                                     NULL,L090_exist_accx,0,0,&tL040_bal,&tL040_avail);
                    if(snResult != 0)
                    {
                        CmdLog(LOGINF, "L040_account_bal error snResult[%ld]", snResult);
                        return ((snResult < 0)?-1:8);
                    }
                    snSum_amt = tL040_bal.snLocal_bal_int;
                }
                else
                {
                    /*其他非逾期仅需无逾期*/
                    L021_ageing_a2n(ptTblAcct->AGEING_G,unArr_ageing);
                    snSum_amt = L021_ageing_sumn(unArr_ageing);

                    if (1 == L090_exist_accx)
                    {
                        L021_ageing_a2n(ptTblAccx->AGEING_G,unArr_ageing);
                        snSum_amx = L021_ageing_sumn(unArr_ageing);
                    }
                }

                /*全额还清进行出催处理*/
               /* if ((snSum_amt <= 0) && (snSum_amx <= 0))*/
                if((CmdDblCmp(snSum_amt, 0.00, 2) <= 0)
                    &&(CmdDblCmp(snSum_amx, 0.00, 2) <= 0))
                {
                    ptTblAcct->OCT_DAYIN = 0;
                    tTblAcctflag.OCT_DAYIN = 1;

                    ptTblAcct->OCT_COUNT++;
                    tTblAcctflag.OCT_COUNT = 1;

                    ptTblAcct->ETL_DAY = GLB_BUSINESS_DAY;
                    tTblAcctflag.ETL_DAY = 1;

                    tTblOacc.DAY_OUTOCT = GLB_BUSINESS_DAY;
                    tTblOaccflag.DAY_OUTOCT = 1;

                    memset(&tTime, 0x00, sizeof(tTime));
                    CmdMT(&tTime);
                    tTblOacc.TIM_OUTOCT = tTime.time_HHMMSSss_N;
                    tTblOaccflag.TIM_OUTOCT = 1;

                    tTblOacc.ETL_DAY = GLB_BUSINESS_DAY;
                    tTblOaccflag.ETL_DAY = 1;

                    /*出催后更新催收员工作量内容*/
                    memset(&tTblOcltr, 0x00, sizeof(tTblOcltr));
                    memset(&tTblOcltrflag, 0x00, sizeof(tTblOcltrflag));
                    snResult = Pocltr(tTblOacc.BANK,tTblOacc.CURR_COLL,&tTblOcltr);
                    if ((snResult != 0) && (snResult != DB_NOT_FOUND))
                    {
                        CmdLog(LOGWAN, "Pocltr error snResult[%ld]", snResult);
                        return -1;
                    }
                    if (snResult == 0)
                    {
                        if (tTblOcltr.UPD_DAY != GLB_BUSINESS_DAY)
                        {
                            tTblOcltr.TOT_REASGN = 0;
                            tTblOcltrflag.TOT_REASGN = 1;
                            tTblOcltr.TOT_ASSIGN = 0;
                            tTblOcltrflag.TOT_ASSIGN = 1;
                            tTblOcltr.TOT_PTPADD = 0;
                            tTblOcltrflag.TOT_PTPADD = 1;
                            tTblOcltr.TOT_PTPCHG = 0;
                            tTblOcltrflag.TOT_PTPCHG = 1;
                            tTblOcltr.TOT_WORKED = 0;
                            tTblOcltrflag.TOT_WORKED = 1;
                            tTblOcltr.TOT_OUTOCT = 0;
                            tTblOcltrflag.TOT_OUTOCT = 1;
                            tTblOcltr.TOT_PDFULL = 0;
                            tTblOcltrflag.TOT_PDFULL = 1;
                            tTblOcltr.UPD_DAY = GLB_BUSINESS_DAY;
                            tTblOcltrflag.UPD_DAY = 1;
                        }
                        tTblOcltr.TOT_PDFULL++;
                        tTblOcltrflag.TOT_PDFULL = 1;
                        if (tTblOcltr.TOT_ACCTS > 0)
                        {
                            tTblOcltr.TOT_ACCTS--;
                            tTblOcltrflag.TOT_ACCTS = 1;
                        }
                        snResult = OcltrUpdate(&tTblOcltr,&tTblOcltrflag);
                        if (snResult != 0)
                        {
                            CmdLog(LOGWAN, "OcltrUpdate error snResult[%ld]", snResult);
                            return -1;
                        }
                    }
                }
            }
            /*借记交易*/
            if (ptTblEvent->BILL_AMT > 0)
            {
                /*还款撤销增加催收承诺还款未还金额*/
                if ((ptTblEvent->TRANS_TYPE >= 7000) && (ptTblEvent->TRANS_TYPE < 8000))
                {
                    if (tTblOacc.DAY_PPAY >= GLB_BUSINESS_DAY)
                    {
                        if ((0 == ptTblEvent->CURRNCY_CD) || (ptTblAcct->CURR_NUM == ptTblEvent->CURRNCY_CD))/*156 mod for CHG-130702-0011*/
                        {
                            tTblOacc.PPAY_UNPD += ptTblEvent->BILL_AMT;
                            tTblOaccflag.PPAY_UNPD = 1;
                        }
                        else
                        {
                            tTblOacc.PPAYX_UNPD += ptTblEvent->BILL_AMT;
                            tTblOaccflag.PPAYX_UNPD = 1;
                        }
                        tTblOacc.ETL_DAY = GLB_BUSINESS_DAY;
                        tTblOaccflag.ETL_DAY = 1;
                    }
                }
            }
        }

        snResult = OaccUpdate(&tTblOacc,&tTblOaccflag);
        if (snResult != 0)
        {
            CmdLog(LOGWAN, "OaccUpdate error snResult[%ld]", snResult);
            return -1;
        }

        snResult = AcctUpdate(ptTblAcct,&tTblAcctflag);
        if (snResult != 0)
        {
            CmdLog(LOGWAN, "AcctUpdate error snResult[%ld]", snResult);
            return -1;
        }
    }
    return 0;
}

/********************************************************************************
 *  L090_3_acct_init
 *  使用账户金额初始化临时结构体金额
 *
 *  形参：
 *      输入：acct_tdef       *ptTblAcct
 *      输出：L090_bal_sdef   *ptL090_bal
 *
 *  返回值：
 *******************************************************************************/

void L090_3_acct_init(acct_tdef       *ptTblAcct,
                        L090_bal_sdef   *ptL090_bal)
{
    ptL090_bal->snPurchases     = ptTblAcct->PURCHASES;        /*消费金额*/
    ptL090_bal->unNbrpurch      = ptTblAcct->NBR_PURCH;        /*消费笔数*/

    ptL090_bal->snCashadvce     = ptTblAcct->CASH_ADVCE;       /*取现金额*/
    ptL090_bal->unNbrcashad     = ptTblAcct->NBR_CASHAD;       /*取现笔数*/

    ptL090_bal->snPaymtclrd     = ptTblAcct->PAYMT_CLRD;       /*还款金额*/
    ptL090_bal->unNbrpaymnt     = ptTblAcct->NBR_PAYMNT;       /*还款笔数*/

    ptL090_bal->snCashadfee     = ptTblAcct->CASH_ADFEE;       /*取现费用*/
    ptL090_bal->snCardfees      = ptTblAcct->CARD_FEES;        /*卡片年费金额*/
    ptL090_bal->unNbrfeedty     = ptTblAcct->NBR_FEEDTY;       /*卡片费用笔数*/

    ptL090_bal->snOtherfees     = ptTblAcct->OTHER_FEES;       /*其他费用金额*/
    ptL090_bal->snPen_chrg      = ptTblAcct->PEN_CHRG;         /*滞纳金金额*/
    ptL090_bal->unNbrothers     = ptTblAcct->NBR_OTHERS;       /*其他费用笔数*/

    ptL090_bal->snDebitadj      = ptTblAcct->DEBIT_ADJ;        /*借记调整金额*/
    ptL090_bal->snCredtadj      = ptTblAcct->CRED_ADJ;         /*贷记调整金额*/
    ptL090_bal->snFees_taxes    = ptTblAcct->FEES_TAXES;       /*费用税金金额*/
    ptL090_bal->snInt_earned    = ptTblAcct->INT_EARNED;       /*存款利息*/
    ptL090_bal->snCredvouch     = ptTblAcct->CRED_VOUCH;       /*退货金额*/
    ptL090_bal->unNbrtrans      = ptTblAcct->NBR_TRANS;        /*交易笔数*/
    /*ptL090_bal->unCash1stac     = ptTblAcct->CASH1STAC; */       /*首次取现日期*//*del for CHG-131127-0029 by zyh 20131216 */

    ptL090_bal->snCashtday      = ptTblAcct->CASH_TDAY;        /*今日取现金额*/
    ptL090_bal->snPaymttday     = ptTblAcct->PAYMT_TDAY;       /*今日还款金额*/
	ptL090_bal->snIntfetday	 = ptTblAcct->INTFE_TDAY;       /*今日息费发生额 CHG-111201-0018*/
    ptL090_bal->snTodayamt      = ptTblAcct->TODAY_AMT;        /*今日非取现还款发生额*/
    ptL090_bal->unTodayrel      = ptTblAcct->TODAY_REL;        /*今日日期*/

    ptL090_bal->snLastpayamt    = ptTblAcct->LASTPAYAMT;       /*上次还款金额*/
    ptL090_bal->unLastpayday    = ptTblAcct->LASTPAYDAY;       /*上次还款日期*/
    ptL090_bal->unLasttrday     = ptTblAcct->LAST_TRDAY;       /*上次交易日期*/

    ptL090_bal->unHipurchse     = ptTblAcct->HI_PURCHSE;       /*最高消费金额*/
    ptL090_bal->unHipurmmyy     = ptTblAcct->HI_PURMMYY;       /*最高消费月份*/

    ptL090_bal->unHicashadv     = ptTblAcct->HI_CASHADV;       /*最高取现金额*/
    ptL090_bal->unHicashmmyy    = ptTblAcct->HI_CASMMYY;       /*最高取现月份*/

    /*ptL090_bal->unHidebit       = ptTblAcct->HI_DEBIT;*/         /*最高借记交易金额*//*del for CHG-131127-0029 by zyh 20131216 */
    /*del for CHG-130819-0038 by zyh 20131216*/
    /*ptL090_bal->unHidebmmyy     = ptTblAcct->HI_DEBMMYY;*/       /*最高借记交易月份*/

    /*ptL090_bal->unHicredt       = ptTblAcct->HI_CREDIT;    *//*最高贷记贷记金额*//*DEL FOR CS-6320-2012-0095*/
   /* ptL090_bal->unHicrdmmyy     = ptTblAcct->HI_CRDMMYY;  */ /*最高贷记交易月份*//*DEL FOR CS-6320-2012-0095*/
    ptL090_bal->snMp_instal= ptTblAcct->MP_INSTAL; /*当期新增分期金额*//*ADD FOR CS-6320-2012-0095*/
    /* 删除HI_OLIMIT
    ptL090_bal->unHiolimit      = ptTblAcct->HI_OLIMIT;
    */
   /* ptL090_bal->unHiolimmyy     = ptTblAcct->HI_OLIMMYY;*//*del for CS-6440-2013-0059 by zyh 20131216*/

    ptL090_bal->unNbrolimit     = ptTblAcct->NBR_OLIMIT;       /*超限次数*/
}

/********************************************************************************
 *  L090_3_accx_init
 *  使用账户金额初始化临时结构体金额
 *
 *  形参：
 *      输入：accx_tdef       *ptTblAccx
 *      输出：L090_bal_sdef   *ptL090_bal
 *
 *  返回值：
 *******************************************************************************/
void L090_3_accx_init(accx_tdef       *ptTblAccx,
                        L090_bal_sdef   *ptL090_bal)
{
    ptL090_bal->snPurchases     = ptTblAccx->PURCHASES;        /*消费金额*/
    ptL090_bal->unNbrpurch      = ptTblAccx->NBR_PURCH;        /*消费笔数*/

    ptL090_bal->snCashadvce     = ptTblAccx->CASH_ADVCE;       /*取现金额*/
    ptL090_bal->unNbrcashad     = ptTblAccx->NBR_CASHAD;       /*取现笔数*/

    ptL090_bal->snPaymtclrd     = ptTblAccx->PAYMT_CLRD;       /*还款金额*/
    ptL090_bal->unNbrpaymnt     = ptTblAccx->NBR_PAYMNT;       /*还款笔数*/

    ptL090_bal->snCashadfee     = ptTblAccx->CASH_ADFEE;       /*取现费用*/
    ptL090_bal->snCardfees      = ptTblAccx->CARD_FEES;        /*卡片年费金额*/
    ptL090_bal->unNbrfeedty     = ptTblAccx->NBR_FEEDTY;       /*卡片费用笔数*/

    ptL090_bal->snOtherfees     = ptTblAccx->OTHER_FEES;       /*其他费用金额*/
    ptL090_bal->snPen_chrg      = ptTblAccx->PEN_CHRG;         /*滞纳金金额*/
    ptL090_bal->unNbrothers     = ptTblAccx->NBR_OTHERS;       /*其他费用笔数*/

    ptL090_bal->snDebitadj      = ptTblAccx->DEBIT_ADJ;        /*借记调整金额*/
    ptL090_bal->snCredtadj      = ptTblAccx->CRED_ADJ;         /*贷记调整金额*/
    ptL090_bal->snFees_taxes    = ptTblAccx->FEES_TAXES;       /*费用税金金额*/
    ptL090_bal->snInt_earned    = ptTblAccx->INT_EARNED;       /*存款利息*/
    ptL090_bal->snCredvouch     = ptTblAccx->CRED_VOUCH;       /*退货金额*/
    ptL090_bal->unNbrtrans      = ptTblAccx->NBR_TRANS;        /*交易笔数*/

    ptL090_bal->snCashtday      = ptTblAccx->CASH_TDAY;        /*今日取现金额*/
    ptL090_bal->snPaymttday     = ptTblAccx->PAYMT_TDAY;       /*今日还款金额*/
    ptL090_bal->snPaymtxday     = ptTblAccx->PAYMTX_TDY;       /*今日外币还款金额*/
	ptL090_bal->snIntfetday	 = ptTblAccx->INTFE_TDAY;       /*今日息费发生额 CHG-111201-0018*/
    ptL090_bal->snTodayamt      = ptTblAccx->TODAY_AMT;        /*今日非取现还款发生额*/
    ptL090_bal->unTodayrel      = ptTblAccx->TODAY_REL;        /*今日日期*/

    ptL090_bal->snLastpayamt    = ptTblAccx->LASTPAYAMT;       /*上次还款金额*/
    ptL090_bal->unLastpayday    = ptTblAccx->LASTPAYDAY;       /*上次还款日期*/
    ptL090_bal->unLasttrday     = ptTblAccx->LAST_TRDAY;       /*上次交易日期*/

    ptL090_bal->unHipurchse     = ptTblAccx->HI_PURCHSE;       /*最高消费金额*/
    ptL090_bal->unHipurmmyy     = ptTblAccx->HI_PURMMYY;       /*最高消费月份*/

    ptL090_bal->unHicashadv     = ptTblAccx->HI_CASHADV;       /*最高取现金额*/
    ptL090_bal->unHicashmmyy    = ptTblAccx->HI_CASMMYY;       /*最高取现月份*/

    /*ptL090_bal->unHidebit       = ptTblAccx->HI_DEBIT; */        /*最高借记交易金额*//*del for CHG-131127-0029 by zyh 20131216 */
    /*ptL090_bal->unHidebmmyy     = ptTblAccx->HI_DEBMMYY; */      /*最高借记交易月份*//*del for CHG-131127-0029 by zyh 20131216 */

    /*ptL090_bal->unHicredt       = ptTblAccx->HI_CREDIT; */      /*最高贷记贷记金额*//*del for CHG-131127-0029 by zyh 20131216 */
    /*ptL090_bal->unHicrdmmyy     = ptTblAccx->HI_CRDMMYY;  最高贷记交易月份*//*del for CHG-130828-0002*/

    ptL090_bal->unNbrolimit     = ptTblAccx->NBR_OLIMIT;       /*超限次数*/
}

/********************************************************************************
 *  L090_7_acct_set
 *  更新临时变量到账户
 *
 *  形参：
 *      输入：L090_bal_sdef   *ptL090_bal
 *      输出：acct_tdef       *ptTblAcct
 *            acctflag_tdef   *ptTblAcctflag
 *
 *  返回值：
 *******************************************************************************/
void L090_7_acct_set(L090_bal_sdef   *ptL090_bal,
                       acct_tdef       *ptTblAcct,
                       acctflag_tdef   *ptTblAcctflag)
{

    time_sdef  stTime = {0};
    date_sdef  stDate = {0};

    ptTblAcct->PURCHASES        = ptL090_bal->snPurchases;       /*消费金额*/
    ptTblAcctflag->PURCHASES      = 1;
    ptTblAcct->NBR_PURCH        = ptL090_bal->unNbrpurch;        /*消费笔数*/
    ptTblAcctflag->NBR_PURCH      = 1;

    ptTblAcct->CASH_ADVCE       = ptL090_bal->snCashadvce;       /*取现金额*/
    ptTblAcctflag->CASH_ADVCE     = 1;
    ptTblAcct->NBR_CASHAD       = ptL090_bal->unNbrcashad;       /*取现笔数*/
    ptTblAcctflag->NBR_CASHAD     = 1;

    ptTblAcct->PAYMT_CLRD       = ptL090_bal->snPaymtclrd;       /*还款金额*/
    ptTblAcctflag->PAYMT_CLRD     = 1;
    ptTblAcct->NBR_PAYMNT       = ptL090_bal->unNbrpaymnt;       /*还款笔数*/
    ptTblAcctflag->NBR_PAYMNT     = 1;

    ptTblAcct->CASH_ADFEE       = ptL090_bal->snCashadfee;       /*取现费用*/
    ptTblAcctflag->CASH_ADFEE     = 1;
    ptTblAcct->CARD_FEES        = ptL090_bal->snCardfees;        /*卡片年费金额*/
    ptTblAcctflag->CARD_FEES      = 1;
    ptTblAcct->NBR_FEEDTY       = ptL090_bal->unNbrfeedty;       /*卡片费用笔数*/
    ptTblAcctflag->NBR_FEEDTY     = 1;

    ptTblAcct->OTHER_FEES       = ptL090_bal->snOtherfees;       /*其他费用金额*/
    ptTblAcctflag->OTHER_FEES     = 1;
    ptTblAcct->PEN_CHRG         = ptL090_bal->snPen_chrg;        /*滞纳金金额*/
    ptTblAcctflag->PEN_CHRG       = 1;
    ptTblAcct->NBR_OTHERS       = ptL090_bal->unNbrothers;       /*其他费用笔数*/
    ptTblAcctflag->NBR_OTHERS     = 1;

    ptTblAcct->DEBIT_ADJ        = ptL090_bal->snDebitadj;        /*借记调整金额*/
    ptTblAcctflag->DEBIT_ADJ      = 1;
    ptTblAcct->CRED_ADJ         = ptL090_bal->snCredtadj;        /*贷记调整金额*/
    ptTblAcctflag->CRED_ADJ       = 1;
    ptTblAcct->FEES_TAXES       = ptL090_bal->snFees_taxes;      /*费用税金金额*/
    ptTblAcctflag->FEES_TAXES     = 1;
    ptTblAcct->INT_EARNED       = ptL090_bal->snInt_earned;      /*存款利息*/
    ptTblAcctflag->INT_EARNED     = 1;
    ptTblAcct->CRED_VOUCH       = ptL090_bal->snCredvouch;       /*退货金额*/
    ptTblAcctflag->CRED_VOUCH     = 1;
    ptTblAcct->NBR_TRANS        = ptL090_bal->unNbrtrans;        /*交易笔数*/
    ptTblAcctflag->NBR_TRANS      = 1;
    /*del for CHG-131127-0029 by zyh 20131216 begin*/
    /*ptTblAcct->CASH1STAC        = ptL090_bal->unCash1stac;*/       /*首次取现日期*/
    /*ptTblAcctflag->CASH1STAC      = 1;*/
    /*del for CHG-131127-0029 by zyh 20131216 end*/
    ptTblAcct->CASH_TDAY        = ptL090_bal->snCashtday;        /*今日取现金额*/
    ptTblAcctflag->CASH_TDAY      = 1;
    ptTblAcct->PAYMT_TDAY       = ptL090_bal->snPaymttday;       /*今日还款金额*/
    ptTblAcctflag->PAYMT_TDAY     = 1;
    /*BEG ADD CHG-111201-0018*/
    ptTblAcct->INTFE_TDAY       = ptL090_bal->snIntfetday;       /*今日息费金额*/
    ptTblAcctflag->INTFE_TDAY     = 1;
    /*END ADD CHG-111201-0018*/
    ptTblAcct->TODAY_AMT        = ptL090_bal->snTodayamt;        /*今日非取现还款发生额*/
    ptTblAcctflag->TODAY_AMT      = 1;
    ptTblAcct->TODAY_REL        = ptL090_bal->unTodayrel;        /*今日日期*/
    ptTblAcctflag->TODAY_REL      = 1;

    ptTblAcct->LASTPAYAMT       = ptL090_bal->snLastpayamt;      /*上次还款金额*/
    ptTblAcctflag->LASTPAYAMT     = 1;
    ptTblAcct->LASTPAYDAY       = ptL090_bal->unLastpayday;      /*上次还款日期*/
    ptTblAcctflag->LASTPAYDAY     = 1;
    ptTblAcct->LAST_TRDAY       = ptL090_bal->unLasttrday;       /*上次交易日期*/
    ptTblAcctflag->LAST_TRDAY     = 1;

    ptTblAcct->HI_PURCHSE       = ptL090_bal->unHipurchse;       /*最高消费金额*/
    ptTblAcctflag->HI_PURCHSE     = 1;
    ptTblAcct->HI_PURMMYY       = ptL090_bal->unHipurmmyy;       /*最高消费月份*/
    ptTblAcctflag->HI_PURMMYY     = 1;

    ptTblAcct->HI_CASHADV       = ptL090_bal->unHicashadv;       /*最高取现金额*/
    ptTblAcctflag->HI_CASHADV     = 1;
    ptTblAcct->HI_CASMMYY       = ptL090_bal->unHicashmmyy;      /*最高取现月份*/
    ptTblAcctflag->HI_CASMMYY     = 1;
    /*del for CHG-131127-0029 by zyh 20131216 begin*/
    /*ptTblAcct->HI_DEBIT         = ptL090_bal->unHidebit;*/         /*最高借记交易金额*/
    /*ptTblAcctflag->HI_DEBIT       = 1;*/
    /*del for CHG-131127-0029 by zyh 20131216 end*/
    /*del for CHG-130819-0038 by zyh 20131216 begin*/
    /*ptTblAcct->HI_DEBMMYY       = ptL090_bal->unHidebmmyy;*/      /*最高借记交易月份*/
    /*ptTblAcctflag->HI_DEBMMYY     = 1;*/
    /*del for CHG-130819-0038 by zyh 20131216 end*/
    /* ptTblAcct->HI_CREDIT        = ptL090_bal->unHicredt;   *//*DEL FOR CS-6320-2012-0095*/      /*最高贷记贷记金额*/
    /* ptTblAcctflag->HI_CREDIT      = 1;*//*DEL FOR CS-6320-2012-0095*/
    /*ptTblAcct->HI_CRDMMYY       = ptL090_bal->unHicrdmmyy;*//*DEL FOR CS-6320-2012-0095*/        /*最高贷记交易月份*/
    /* ptTblAcctflag->HI_CRDMMYY     = 1;*//*DEL FOR CS-6320-2012-0095*/
    ptTblAcct->MP_INSTAL= ptL090_bal->snMp_instal;/*当期新增分期金额*//*ADD FOR CS-6320-2012-0095*/
    ptTblAcctflag->MP_INSTAL = 1;
    /* 删除HI_OLIMIT
    ptTblAcct->HI_OLIMIT        = ptL090_bal->unHiolimit;
    ptTblAcctflag->HI_OLIMIT      = 1;
    */
    /*del for CS-6440-2013-0059 by zyh 20131216 begin*/
    /* ptTblAcct->HI_OLIMMYY       = ptL090_bal->unHiolimmyy;    */   /*最高超限月份*/
    /*ptTblAcctflag->HI_OLIMMYY     = 1;*/
    /*del for CS-6440-2013-0059 by zyh 20131216 end*/
    ptTblAcct->NBR_OLIMIT       = ptL090_bal->unNbrolimit;       /*超限次数*/
    ptTblAcctflag->NBR_OLIMIT     = 1;

    ptTblAcct->ETL_DAY          = GLB_BUSINESS_DAY;
    ptTblAcctflag->ETL_DAY        = 1;

    /*add for CHG-130128-0007 by zyh begin 20131204*/
    memset(&stTime, 0x00, sizeof(stTime));
    CmdMT(&stTime);
    CmdMD(&stDate);
    ptTblAcct->LASTTRNDT = stDate.date_CCYYMMDD_N *100000000 +stTime.time_HHMMSSss_N ;
    ptTblAcctflag->LASTTRNDT = 1 ;
    /*add for CHG-130128-0007 by zyh end 20131204*/
}

/********************************************************************************
 *  L090_7_accx_set
 *  更新临时变量到账户
 *
 *  形参：
 *      输入：L090_bal_sdef   *ptL090_bal
 *      输出：accx_tdef       *ptTblAccx
 *            accxflag_tdef   *ptTblAccxflag
 *
 *  返回值：
 *******************************************************************************/
void L090_7_accx_set(L090_bal_sdef   *ptL090_bal,
                       accx_tdef       *ptTblAccx,
                       accxflag_tdef   *ptTblAccxflag)
{
    time_sdef  stTime = {0};
    date_sdef  stDate = {0};

    ptTblAccx->PURCHASES        = ptL090_bal->snPurchases;       /*消费金额*/
    ptTblAccxflag->PURCHASES      = 1;
    ptTblAccx->NBR_PURCH        = ptL090_bal->unNbrpurch;        /*消费笔数*/
    ptTblAccxflag->NBR_PURCH      = 1;

    ptTblAccx->CASH_ADVCE       = ptL090_bal->snCashadvce;       /*取现金额*/
    ptTblAccxflag->CASH_ADVCE     = 1;
    ptTblAccx->NBR_CASHAD       = ptL090_bal->unNbrcashad;       /*取现笔数*/
    ptTblAccxflag->NBR_CASHAD     = 1;

    ptTblAccx->PAYMT_CLRD       = ptL090_bal->snPaymtclrd;       /*还款金额*/
    ptTblAccxflag->PAYMT_CLRD     = 1;
    ptTblAccx->NBR_PAYMNT       = ptL090_bal->unNbrpaymnt;       /*还款笔数*/
    ptTblAccxflag->NBR_PAYMNT     = 1;

    ptTblAccx->CASH_ADFEE       = ptL090_bal->snCashadfee;       /*取现费用*/
    ptTblAccxflag->CASH_ADFEE     = 1;
    ptTblAccx->CARD_FEES        = ptL090_bal->snCardfees;        /*卡片年费金额*/
    ptTblAccxflag->CARD_FEES      = 1;
    ptTblAccx->NBR_FEEDTY       = ptL090_bal->unNbrfeedty;       /*卡片费用笔数*/
    ptTblAccxflag->NBR_FEEDTY     = 1;

    ptTblAccx->OTHER_FEES       = ptL090_bal->snOtherfees;       /*其他费用金额*/
    ptTblAccxflag->OTHER_FEES     = 1;
    ptTblAccx->PEN_CHRG         = ptL090_bal->snPen_chrg;        /*滞纳金金额*/
    ptTblAccxflag->PEN_CHRG       = 1;
    ptTblAccx->NBR_OTHERS       = ptL090_bal->unNbrothers;       /*其他费用笔数*/
    ptTblAccxflag->NBR_OTHERS     = 1;

    ptTblAccx->DEBIT_ADJ        = ptL090_bal->snDebitadj;        /*借记调整金额*/
    ptTblAccxflag->DEBIT_ADJ      = 1;
    ptTblAccx->CRED_ADJ         = ptL090_bal->snCredtadj;        /*贷记调整金额*/
    ptTblAccxflag->CRED_ADJ       = 1;
    ptTblAccx->FEES_TAXES       = ptL090_bal->snFees_taxes;      /*费用税金金额*/
    ptTblAccxflag->FEES_TAXES     = 1;
    ptTblAccx->INT_EARNED       = ptL090_bal->snInt_earned;      /*存款利息*/
    ptTblAccxflag->INT_EARNED     = 1;
    ptTblAccx->CRED_VOUCH       = ptL090_bal->snCredvouch;       /*退货金额*/
    ptTblAccxflag->CRED_VOUCH     = 1;
    ptTblAccx->NBR_TRANS        = ptL090_bal->unNbrtrans;        /*交易笔数*/
    ptTblAccxflag->NBR_TRANS      = 1;

    ptTblAccx->CASH_TDAY        = ptL090_bal->snCashtday;        /*今日取现金额*/
    ptTblAccxflag->CASH_TDAY      = 1;
    ptTblAccx->PAYMT_TDAY       = ptL090_bal->snPaymttday;       /*今日还款金额*/
    ptTblAccxflag->PAYMT_TDAY     = 1;
    ptTblAccx->PAYMTX_TDY       = ptL090_bal->snPaymtxday;       /*今日外币还款金额*/
    ptTblAccxflag->PAYMTX_TDY     = 1;
    /*BEG ADD CHG-111201-0018*/
    ptTblAccx->INTFE_TDAY       = ptL090_bal->snIntfetday;       /*今日息费金额*/
    ptTblAccxflag->INTFE_TDAY     = 1;
    /*END ADD CHG-111201-0018*/
    ptTblAccx->TODAY_AMT        = ptL090_bal->snTodayamt;        /*今日非取现还款发生额*/
    ptTblAccxflag->TODAY_AMT      = 1;
    ptTblAccx->TODAY_REL        = ptL090_bal->unTodayrel;        /*今日日期*/
    ptTblAccxflag->TODAY_REL      = 1;

    ptTblAccx->LASTPAYAMT       = ptL090_bal->snLastpayamt;      /*上次还款金额*/
    ptTblAccxflag->LASTPAYAMT     = 1;
    ptTblAccx->LASTPAYDAY       = ptL090_bal->unLastpayday;      /*上次还款日期*/
    ptTblAccxflag->LASTPAYDAY     = 1;
    ptTblAccx->LAST_TRDAY       = ptL090_bal->unLasttrday;       /*上次交易日期*/
    ptTblAccxflag->LAST_TRDAY     = 1;

    ptTblAccx->HI_PURCHSE       = ptL090_bal->unHipurchse;       /*最高消费金额*/
    ptTblAccxflag->HI_PURCHSE     = 1;
    ptTblAccx->HI_PURMMYY       = ptL090_bal->unHipurmmyy;       /*最高消费月份*/
    ptTblAccxflag->HI_PURMMYY     = 1;

    ptTblAccx->HI_CASHADV       = ptL090_bal->unHicashadv;       /*最高取现金额*/
    ptTblAccxflag->HI_CASHADV     = 1;
    ptTblAccx->HI_CASMMYY       = ptL090_bal->unHicashmmyy;      /*最高取现月份*/
    ptTblAccxflag->HI_CASMMYY     = 1;
    /*del for CHG-131127-0029 by zyh 20131216 begin*/
    /*ptTblAccx->HI_DEBIT         = ptL090_bal->unHidebit;         最高借记交易金额
    ptTblAccxflag->HI_DEBIT       = 1;
    ptTblAccx->HI_DEBMMYY       = ptL090_bal->unHidebmmyy;       最高借记交易月份
    ptTblAccxflag->HI_DEBMMYY     = 1;*/
    /*del for CHG-131127-0029 by zyh 20131216 end*/
    /*DEL FOR CS-6320-2012-0095 BEGIN*/
    /*ptTblAccx->HI_CREDIT        = ptL090_bal->unHicredt ;  */     /*最高贷记贷记金额*/
    /* ptTblAccxflag->HI_CREDIT      = 1;*/
    /* ptTblAccx->HI_CRDMMYY       = ptL090_bal->unHicrdmmyy; */      /*最高贷记交易月份*/
    /*ptTblAccxflag->HI_CRDMMYY     = 1;*/
    /*DEL FOR CS-6320-2012-0095 END*/
    ptTblAccx->NBR_OLIMIT       = ptL090_bal->unNbrolimit;       /*超限次数*/
    ptTblAccxflag->NBR_OLIMIT     = 1;

    ptTblAccx->ETL_DAY          = GLB_BUSINESS_DAY;
    ptTblAccxflag->ETL_DAY        = 1;

    /*add for CHG-130128-0007 by zyh begin 20131204*/
    memset(&stTime, 0x00, sizeof(stTime));
    CmdMT(&stTime);
    CmdMD(&stDate);
    ptTblAccx->LASTTRNDT = stDate.date_CCYYMMDD_N *100000000 +stTime.time_HHMMSSss_N ;
    ptTblAccxflag->LASTTRNDT = 1 ;
    /*add for CHG-130128-0007 by zyh end 20131204*/
}
