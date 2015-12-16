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
 *  ���ڶԽ�����ˮ�����������ˡ��������º�����
 *  1.  L090_acct_post      ԭ������
 *  2.  L090_1_pay_ageing   �������˻�����
 *  4.  L090_3_acct_trans   ���ҽ��׸����������
 *  5.  L090_4_accx_trans   ��ҽ��׸����������
 *  6.  L090_5_busns_trans  ���׸��¹�˾���������
 *  7.  L090_6_oct_proc     �����˻�����
 *
 *  Edit History:
 *  �汾  ����          �޸���        �޸�����
 *  1.0   20110809     �䴫��         ����
 *  1.1   20120307      �䴫��        �޸� 2.2011.5.000
 *                                    CASE-0305-2011-0352 ʵ��������������T+1����
 *                                    �޸� 2.2011.6.000
 *                                      XYK-6403-2011-089   ͨ�Ŵ�Ӧ�ձ�Ϣ���
 *  1.2   20121105    �䴫��        �޸� 2.2012.4
 *                                    CHG-111201-0018 Ϣ�Ѳ�ռ�ö��
 *  1.3   20130306    ���ݻ�        �޸�  2.0 2012.6.000 CS-6320-2012-0095
 *                                      �������н������Ŀ����
 *  1.4   20130506    ���ݻ�        �޸�  2.0 2013.02.000 CHG-130228-0010
 *                                      V2.0 �������������������׷�������������
 *                                    �޸�  2.0 2013.02.000 CHG-130225-0018
 *                                      ����/̩�����ǿ��ۺϸ���
 *  1.5   20130712    ���ݻ�        �޸�
 *                              2.0 2013.03.000 CHG-130702-0011 ��ʢ�����ָ���
 *                              2.0 2013.03.000 CHG-130418-0024 ̩�����н����һ������
 *  1.6   20131021   ���ݻ�      �޸�   2.0 2013.05.000 CHG-130828-0002
 *  1.7   20131114   ���ݻ�      �޸�   2.0 2013.05.000 CHG-130509-0001��˾�����ڹ���
 *  1.8   20131204   ���ݻ�      �޸�   2.0 2013.05.000 CHG-130128-0007 ����ϵͳ���������޸ĵ�����
 *  1.9   20131216   ���ݻ�      �޸�   2.0 2013.06.000 CHG-130819-0038/CS-6440-2013-0059�汾�ϲ��ӿڼ����ݽṹ�����޸�
 *  2.0   20131216   ���ݻ�      �޸�   N2.2014.01.000  CHG-131127-0029 ��ʢ��Ϣ�������
 *  2.1   20140210   ���ݻ�      �޸�   2.0 2013.06.000/ N2.2014.02.000 CHG-130909-0051�㽭ũ��-CHGS����������ű���
 *  2.2   20140210   ���ݻ�      �޸�   2.0 2013.06.000/ N2.2014.02.000 CHG-131030-0014 ��˾�������˻��Ż�
 *  2.3   20140303   ���ݻ�      �޸�   2.0 2013.06.000/ N2.2014.02.000 CHG-140122-0003 ����Ϣ�Ѳ�ռ���
 *  2.4   20140515   ���ݻ�      �޸�   N2.2014.03.000 CHG-140410-0029 ���ÿ��Ŵ�����_���ʺ������ÿ�ģʽ
 *******************************************************************************/
#include "L090_acct_post.h"

/********************************************************************************
* ����Ϊ�ڲ�ʹ���Ӻ���������
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
 *  �Խ�����ˮ������������
 *
 *  �βΣ�
 *      ���룺
 *          banks_tdef      *ptTblBanks          ���ݿ��BANKS/��ǰ���м�¼
 *          event_tdef      *ptTblEvent          ���ݿ��EVENT/��ǰ�����˽�����ˮ
 *          UN1              L090_switch         �Ƿ��˵��յ���
 *                                               0:��,������˻� 1:�ǣ��������˻�
 *          CH              *pL090_card_nbr      ��ǰ��Ƭ����,�Ǹ������׿ɴ���
 *          SN11_2           L090_cur_bal        ��ǰ�˻�������Ϊ�˻����֣�
 *          SN11_2           L090_local_bal      ��ǰ�˻�������Ϊ���ң�
 *          UN1             *pL090_exist_accx    0��ACCX������ 1: ACCX����
 *          acct_tdef       *ptTblAcct           ���ݿ��ACCT/���˺��˻���¼
 *          accx_tdef       *ptTblAccx           ���ݿ��ACCX/���˺�����˻���¼
 *      �����
 *          ptL090_bal          ��ǰ�˻��������ױ����ͽ��
 *
 *  ����ֵ��
 *         -1 - ���ݿ���ó���
 *          0 - �ɹ�
 *          1 - �����������
 *          2 - δ�ҵ�CARD��¼
 *          3 - δ�ҵ�PRMCN��¼
 *          4 - δ�ҵ�BUSNS��¼
 *          5 - �������Ͳ��Ϸ�
 *          6 - ����ACCX��¼ʧ��
 *          7 - Ϊ�ҵ�ACCX��¼
 *          8 - ������ڽ�����
 *          9 - δ�ҵ�EVENT��¼
 *         10 - �Ӻ������ô���
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
    UN1     unScard_flag    = 0;    /*��������ʱ�Ƿ񵥶�ͳ�Ʒ�����*/
    SN6     snResult        = 0;
    UN8     unPur_mmyy      = 0;
    SN11_2  snLatest_bal    = 0.00;
    SN11_2  snLatest_loc    = 0.00;
    SN11_2  snConv_amt      = 0.00;
    SN10    snCred_limit     = 0;
    SN9_2   snMinAge = 0.00; /*������С���ڽ��*/
    SN9_2   snMinAgex = 0.00;/*�����С���ڽ��*/
    SN11_2 snPay = 0.00;
    SN11_2 snCre  = 0.00;
    SN11_2 snQer  = 0.00;
    chgs_tdef tTblChgs = {0};
    /*ADD CHG-140122-0003 ����Ϣ�Ѳ�ռ���*/
    UN1 unAddamt_flag = 0;  /*�жϳ����Ƿ�ӱ��ʽ��׽�0-��;1-����*/

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

    /*����������--�����Լ��*/
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

    /*���ACCX�������*/
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

    /*L090_switch Ϊ1��Ϊ��ҽ���ʱ��������ACCX��¼*/
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

    /*===================================���������=================================*/

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
            CmdLog(LOGWAN, "Pacctsec δ�ҵ������˻� bank[%d] xaccount[%ld]",
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

        /*��ѯ����������Ƿ�ֿ�(��Ϊ����Ĭ��Ϊ���ֿ�)*/
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
                CmdLog(LOGWAN, "Pcardnumbsec �Ҳ�����Ƭ��¼  Card_nbr[%s]", pL090_card_nbr);
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
            CmdLog(LOGWAN, "Pacctsec δ�ҵ������˻� bank[%d] xaccount[%ld]",
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

        /*��ѯPRMCN��¼����*/
        memset(&tTblPrmcn, 0x00, sizeof(tTblPrmcn));
        snResult = Pprmcnbnk(ptTblAcct->BANK, ptTblAcct->CATEGORY,&tTblPrmcn);
        if (snResult != 0 && snResult != DB_NOT_FOUND)
        {
            CmdLog(LOGWAN, "Pprmcnbnk error snResult[%ld]", snResult);
            return -1;
        }
        if (DB_NOT_FOUND == snResult)
        {
            CmdLog(LOGWAN, "Pprmcnbnk δ�ҵ�Prmcn��¼  bank[%d] category[%d]",
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
            CmdLog(LOGWAN, "Pbusnssec δ�ҵ���˾Busns��¼  bank[%d] business[%s]",
                   ptTblAcct->BANK, ptTblAcct->BUSINESS);
            return 4;
        }
    }

    /*���ҽ��״���*/
    if (ptTblEvent->CURRNCY_CD == ptTblAcct->CURR_NUM)
    {
        /*�����������ͳ�ƽ���������ʼ��*/
        if (L090_switch != 1)
        {
            memset(ptL090_bal, 0x00, sizeof(L090_bal_sdef));
            L090_3_acct_init(ptTblAcct, ptL090_bal);

            if (1 == unScard_flag)
            {
                ptL090_bal->snScard_tamt = tTblCard.TOTAL_AMT;         /*�����ǻ���׷�����*/
            }
        }
        snResult = L090_4_acct_trans(ptTblEvent, ptTblAcct,&tTblPrmcn,ptL090_bal);
        if (snResult != 0)
        {
            CmdLog(LOGINF, "L090_3_acct_trans error snResult[%ld]", snResult);
            return ((snResult < 0)?-1:5);
        }

        /*�������ҽ��׷�����*/
        if (1 == unScard_flag)
        {
            if (!((ptTblEvent->TRANS_TYPE >= 7000) && (ptTblEvent->TRANS_TYPE < 8000)))
            {
                ptL090_bal->snScard_tamt += ptTblEvent->BILL_AMT;
            }
        }

    }
    /*��ҽ��״���*/
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
                /*ADD CASE-0305-2011-0352 ����ptTblBanks*/
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
                ptL090_bal->snScard_tamt = tTblCard.TOTAL_AMX;         /*�����ǻ���׷�����*/
            }
        }

        snResult = L090_4_accx_trans(ptTblEvent, ptTblAccx, ptL090_bal);
        if (snResult != 0)
        {
            CmdLog(LOGINF, "L090_4_accx_trans error snResult[%ld]", snResult);
            return ((snResult < 0)?-1:5);
        }

        /*������ҽ��׷�����*/
        if (1 == unScard_flag)
        {
            if (!((ptTblEvent->TRANS_TYPE >= 7000) && (ptTblEvent->TRANS_TYPE < 8000)))
            {
                ptL090_bal->snScard_tamt += ptTblEvent->BILL_AMT;
            }
        }
    }

    /*��������´˴�����*/
    if (1 == L090_switch)
    {
        return 0;
    }

    /*BEG ADD CHG-140122-0003 ����Ϣ�Ѳ�ռ���*/
    /*��ȡPRMCN*/
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
            CmdLog(LOGWAN, "Pprmcnbnk δ�ҵ�Prmcn��¼  bank[%d] category[%d]",
                   ptTblAcct->BANK, ptTblAcct->CATEGORY);
            return 3;
        }
    }

    /*PRMCN.INTFEE_YN=2�ҷ��ý���ʱ���ۼ�EVENT.BILL_AMT���*/
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
    /*END ADD CHG-140122-0003 ����Ϣ�Ѳ�ռ���*/

    /*�˲�����Ҫ�������±��*/
    memset(&tTblAcctflag, 0x00, sizeof(tTblAcctflag));
    memset(&tTblAccxflag, 0x00, sizeof(tTblAccxflag));
    memset(&tTblCardflag, 0x00, sizeof(tTblCardflag));

    /*���ҽ��״���*/
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
                /*��¼��־*/
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

        /*��ʼ��������ڽ��Ĳ������������,�޸�ACCT�������ֶ�*/
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

        /*���¹�˾��Ϣ*/
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

        /*��������Ϊ���Ű�*/
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

            /*���������/���ǽ��*/
            /*BEG CHG CHG-140122-0003 ����Ϣ�Ѳ�ռ���*/
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
            /*END CHG CHG-140122-0003 ����Ϣ�Ѳ�ռ���*/
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
            /*���ޱ�־����*/

            L049_acct_crlim(ptTblAcct,&snCred_limit,&unTemp_flag);
            if (snLatest_loc > snCred_limit)
            {
                ptL090_bal->snOlimit_amt  = snLatest_loc - snCred_limit;
                ptL090_bal->unOlimit_flag = 1;
                /* ɾ��HI_OLIMIT
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

        /*�������ݿ��ֶ�*/
        if (1 == unScard_flag)
        {
            tTblCard.TOTAL_AMT = ptL090_bal->snScard_tamt;         /*�����ǻ���׷�����*/
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
    /*��ҽ��״���*/
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
        /*��ʼ��������ڽ��Ĳ������������,�޸�ACCX�������ֶ�*/
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

        /*���¹�˾��Ϣ*/
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

        /*��Ϊ�Ű�*/
        if (0 == 0)
        {
            /*������һ�ν�����������ս��*/
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
            /*���������/���ǽ��*/
            L083_conversion(ptTblAccx->CURR_NUM, GLB_BUSINESS_DAY, 0,
                            ptTblEvent->BILL_AMT, 0, 0, 0, &snConv_amt);
            /*BEG CHG CHG-140122-0003 ����Ϣ�Ѳ�ռ���*/
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
            /*END CHG CHG-140122-0003 ����Ϣ�Ѳ�ռ���*/
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

            /*���ޱ�־����*/
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

        /*�������ݿ���Ϣ*/
        if (1 == unScard_flag)
        {
            tTblCard.TOTAL_AMX = ptL090_bal->snScard_tamt;         /*�����ǻ���׷�����*/
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
 *  �����Դ��ǽ��ף��������˻����ף�������˻���0~6�����ڽ��������·ݡ�
 *  ������ͬʱ֧�ֱ��ҡ���Ҵ��ǽ��׶����ڽ��ĳ����
 *
 *  �βΣ�
 *      ���룺
 *          ptTblEvent          ���ݿ��EVENT/��ǰ�����˽�����ˮ
 *          L090_minageamt      ��С���ڽ��
 *          pL090_ageing_g      1~6�����ڽ���ӦACCT��ACCX��AGEING_G�ֶΣ�
 *          pL090_over_held     С�����ڽ��
 *          pL090_mthsodue      �����·�
 *
 *      �����
 *          pL090_ageing_g      1~6�����ڽ���ӦACCT��ACCX��AGEING_G�ֶΣ�
 *          pL090_over_held     С�����ڽ��
 *          pL090_mthsodue      �����·�
 *
 *  ����ֵ��
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

    /*�ǳ���/�ǳ���/�ǵ����˻��Ĵ��ǽ���*/
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
 *  (1)�Ա��ҽ��ף����ݽ�����ˮ���㵱ǰ�˻��������ױ����ͽ�
 *  (2)���ݽ�����ˮ���˻�������޸��˻����׵ĸ�������ڡ���߽��������
 *  (3)�����˻���ǰ���ж��˻��Ƿ��ޣ�
 *
 *  �βΣ�
 *      ���룺
 *          ptTblEvent      ���ݿ��EVENT/�����˽�����ˮ��¼
 *          ptTblAcct       ���ݿ��ACCT/��ǰ�˻���¼
 *      �����
 *          ptL090_bal          ��ǰ�˻��������ױ����ͽ��
 *
 *  ����ֵ��
 *          0 - �ɹ�
 *          1 - �������Ͳ��Ϸ�
 *          2 - ���ֲ���ȷ
 *          3 - ����ת��ʧ��
 *******************************************************************************/

int L090_4_acct_trans(  event_tdef      *ptTblEvent,
                        acct_tdef       *ptTblAcct,
                        prmcn_tdef   *ptTblPrmcn,/*add for CHG-140410-0029 */
                        L090_bal_sdef   *ptL090_bal)
{
    UN1     unUpd_error = 1; /*Ĭ�ϲ��ɹ�*/
    UN8     unPur_mmyy = 0;
    SN6     snResult = 0;
    SN11_2  TmpsnL063_cup_od_amt = 0;
    UN1    unLoanCrvjtFlag = 0;/*���ʺ����˻��˻����ױ�ʶadd for CHG-140410-0029*/

    snResult = CmdDC(DC_DAYNUM, &GLB_BUSINESS_DAY);
    if(snResult != 0)
    {
        CmdLog(LOGWAN, "CmdDC error snResult[%ld]", snResult);
        return 3;
    }
    unPur_mmyy = atoi(GLB_DC_DATE.dc_ddmmyy) % 10000;

    if (ptTblEvent->CURRNCY_CD != ptTblAcct->CURR_NUM)
    {
        CmdLog(LOGWAN, "���ֲ���ȷ  Event.rowid[%s] Acct.rowid[%s]",
               ptTblEvent->ROWID, ptTblAcct->ROWID);
        return  2;
    }

    /*�ۼ�ͳ�ƽ��*/
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
            /*�����ཻ��*/
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
            /*Ԥ���ֽ��ཻ��*/
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
            /*�����ཻ��*/
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
            /*ADD XYK-6403-2011-089 ����TRANS_SRC�жϣ��˵��ղ��ٵ���L090ͳ��������Ϣ��������*/
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
            /*��ȡPRMCN*/
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
                    CmdLog(LOGWAN, "Pprmcnbnk δ�ҵ�Prmcn��¼  bank[%d] category[%d]",
                           ptTblAcct->BANK, ptTblAcct->CATEGORY);
                    return 3;
                }
            }
            /*���ʺ����˻����˻����ɵ��ջ�����д������ۼӵ��ջ�����*/
            if(ptTblPrmcn->LOAN_TYPE != 0)
            {
                /*ptL090_bal->snPaymttday += ptTblEvent->BILL_AMT;*/
                unLoanCrvjtFlag = 1;/*���ʺ����˻��˻����ױ�ʶadd for CHG-140410-0029*/
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
    /*���ڽӿ����⣬�˴�ֻ�ڻ���ʱ����L063,�����Ľ�������Ȩ����*/
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

    /*�����ϴλ�������/���/���ջ���/����ȡ��*/
    if (0 == unUpd_error)
    {
        if( ((ptTblEvent->TRANS_TYPE >= 7000) && (ptTblEvent->TRANS_TYPE < 8000))
            ||(1 == unLoanCrvjtFlag))/*���ʺ����˻��˻����ױ�ʶadd for CHG-140410-0029*/
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
        CmdLog(LOGWAN, "��������[%d]����", ptTblEvent->TRANS_TYPE);
        return 1;
    }
    return 0;
}

/********************************************************************************
 *  L090_4_accx_trans
 *  (1)����ҽ��ף����ݽ�����ˮ���㵱ǰ�˻��������ױ����ͽ�
 *  (2)���ݽ�����ˮ���˻�������޸��˻����׵ĸ�������ڡ���߽��������
 *  (3)�����˻���ǰ���ж��˻��Ƿ��ޣ�
 *
 *  �βΣ�
 *      ���룺
 *          ptTblEvent          ���ݿ��EVENT/�����˽�����ˮ��¼
 *          ptTblAccx           ���ݿ��ACCX/��ǰ����˻���¼
 *      �����
 *          ptL090_bal          ��ǰ�˻��������ױ����ͽ��
 *
 *  ����ֵ��
 *          0 - �ɹ�
 *          1 - �������Ͳ���ȷ
 *          2 - ���ֲ���ȷ
 *          3 - ����ת��ʧ��
 *******************************************************************************/

int L090_4_accx_trans(  event_tdef      *ptTblEvent,
                        accx_tdef       *ptTblAccx,
                        L090_bal_sdef   *ptL090_bal)
{
    UN1     unUpd_error = 1; /*Ĭ�ϲ��ɹ�*/
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
        CmdLog(LOGWAN, "���ֲ���ȷ  Event.rowid[%s] Accx.rowid[%s]",
               ptTblEvent->ROWID, ptTblAccx->ROWID);
        return  2;
    }

    /*ҳ��Ϣ�ѵ�������*/
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
            /*�����ཻ��*/
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
            /*Ԥ���ֽ��ཻ��*/
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
            /*�����ཻ��*/
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
            /*ADD XYK-6403-2011-089 ����TRANS_SRC�жϣ��˵��ղ��ٵ���L090ͳ��������Ϣ��������*/
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

    /*�ۼӵ��ս��*/
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
        CmdLog(LOGWAN, "��������[%d]����", ptTblEvent->TRANS_TYPE);
        return 1;
    }
    return 0;
}

/********************************************************************************
 *  L090_5_busns_trans
 *  �Թ�˾���˻������ݽ�����ˮ�޸Ĺ�˾��������������˾�޸ļ�¼��־λ
 *
 *  �βΣ�
 *      ���룺
 *          L090_curr           ���ұ���
 *          ptL090_bal          ��˾����Ȩ���
 *          ptTblEvent          ���ݿ��EVENT/�����˽�����ˮ
 *          ptTblBusns          ���ݿ��BUSNS/�˻���Ӧ��˾��¼
 *      �����
 *          ptTblBusns          ���ݿ��BUSNS/�˻���Ӧ��˾��¼
 *
 *  ����ֵ��
 *          0 - �ɹ�
 *          1 - ʧ��
 *******************************************************************************/

int L090_5_busns_trans( SN3              L090_curr,
                        L090_bal_sdef   *ptL090_bal,
                        event_tdef      *ptTblEvent,
                        busns_tdef      *ptTblBusns)
{
    SN6     snResult = 0;
    busnsflag_tdef  tTblBusnsflag;

    memset(&tTblBusnsflag, 0x00, sizeof(tTblBusnsflag));
    /*����ͳ�ƴ���*/
    if (ptTblEvent->CURRNCY_CD == L090_curr)
    {
        if ((ptTblEvent->TRANS_TYPE >= 1000 && ptTblEvent->TRANS_TYPE < 2000)
            || (ptTblEvent->TRANS_TYPE == 5050)
            || ((strcmp(ptTblEvent->BANKACCT,"XXXXX") == 0)
                && ((ptTblEvent->TRANS_TYPE >= 6000) && (ptTblEvent->TRANS_TYPE < 7000))))
        {
            /*�����ཻ��/�˻��ཻ��/����Ӧ����Ϣ*/
            if(ptTblEvent->TRANS_TYPE != 1050)/*add for CHG-130509-0001 by zyh 20131114*/
            {
                ptTblBusns->BAL_PUR += ptTblEvent->BILL_AMT;
                tTblBusnsflag.BAL_PUR = 1;
            }
        }
        else if ((ptTblEvent->TRANS_TYPE >= 2000 && ptTblEvent->TRANS_TYPE < 2300)
                 || (ptTblEvent->TRANS_TYPE >= 2500 && ptTblEvent->TRANS_TYPE < 3000))
        {
            /*Ԥ���ֽ���(�����˵��ճ�������/����ת�˳�������)*/
            ptTblBusns->BAL_CASH += ptTblEvent->BILL_AMT;
            tTblBusnsflag.BAL_CASH = 1;
        }
        else if ((ptTblEvent->TRANS_TYPE >= 7000) && (ptTblEvent->TRANS_TYPE < 8000))
        {
            /*�����ཻ��*/
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
                /*����������Ľ��׷������ۼ�*/
                ptTblBusns->BAL_OTHER += ptTblEvent->BILL_AMT;
                tTblBusnsflag.BAL_OTHER = 1;
            }
        }
    }
    else
    {
        /*���ͳ�ƴ���*/
        if ((ptTblEvent->TRANS_TYPE >= 1000) && (ptTblEvent->TRANS_TYPE < 2000))
        {
            /*�����ཻ��*/
            if(ptTblEvent->TRANS_TYPE != 1051)/*add for CHG-130509-0001 by zyh 20131114*/
            {
                ptTblBusns->BAL_PURX += ptTblEvent->BILL_AMT;
                tTblBusnsflag.BAL_PURX = 1;
            }
        }
        else if (((ptTblEvent->TRANS_TYPE >= 2000) && (ptTblEvent->TRANS_TYPE < 2300))
                || ((ptTblEvent->TRANS_TYPE >= 2600) && ptTblEvent->TRANS_TYPE < 3000))
        {
            /*Ԥ���ֽ���(�����˵��ճ�������/����ת�˳�������)*/
            ptTblBusns->BAL_CASHX += ptTblEvent->BILL_AMT;
            tTblBusnsflag.BAL_CASHX = 1;
        }
        else if ((ptTblEvent->TRANS_TYPE >= 7000) && (ptTblEvent->TRANS_TYPE < 8000))
        {
            /*�����ཻ��*/
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
                /*����������Ľ��׷������ۼ�*/
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
 *  ���ǽ��׻򻹿���Դ����˻������������ͳ�ŵ���������
 *
 *  �βΣ�
 *      ���룺
 *          ptTblBanks          ���ݿ��BANKS/��ǰ���м�¼
 *          ptTblEvent          ���ݿ��EVENT/��ǰ������ˮ��¼
 *          ptTblAcct           ���ݿ��ACCT/��ǰ�����˻�
 *          ptTblAccx           ���ݿ��ACCX/��ǰ����˻�
 *          L090_exist_accx     �Ƿ��������˻�  0 ������ 1����
 *      �����
 *
 *  ����ֵ��
 *         -1 - ���ݿ�ʧ��
 *          0 - �ɹ�
 *          1 - ����L040����
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
        /*�޴��ռ�¼��ACCT������������*/
        if (snResult == DB_NOT_FOUND)
        {
            ptTblAcct->OCT_DAYIN = 0;
            tTblAcctflag.OCT_DAYIN = 1;

            ptTblAcct->ETL_DAY = GLB_BUSINESS_DAY;
            tTblAcctflag.ETL_DAY = 1;
        }
        else
        {
            /*���ǽ���*/
            if (ptTblEvent->BILL_AMT < 0)
            {
                /*�ǳ���/�ǳ���/���˻��Ĵ��ǽ��׳�����ճ�ŵ����δ�����*/
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

                /*�ۼ����ڽ��������*/
                snSum_amt = 0;

                /*��ֹ���˻��軹�嵱ǰ���*/
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
                    /*���������ڽ���������*/
                    L021_ageing_a2n(ptTblAcct->AGEING_G,unArr_ageing);
                    snSum_amt = L021_ageing_sumn(unArr_ageing);

                    if (1 == L090_exist_accx)
                    {
                        L021_ageing_a2n(ptTblAccx->AGEING_G,unArr_ageing);
                        snSum_amx = L021_ageing_sumn(unArr_ageing);
                    }
                }

                /*ȫ�����г��ߴ���*/
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

                    /*���ߺ���´���Ա����������*/
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
            /*��ǽ���*/
            if (ptTblEvent->BILL_AMT > 0)
            {
                /*��������Ӵ��ճ�ŵ����δ�����*/
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
 *  ʹ���˻�����ʼ����ʱ�ṹ����
 *
 *  �βΣ�
 *      ���룺acct_tdef       *ptTblAcct
 *      �����L090_bal_sdef   *ptL090_bal
 *
 *  ����ֵ��
 *******************************************************************************/

void L090_3_acct_init(acct_tdef       *ptTblAcct,
                        L090_bal_sdef   *ptL090_bal)
{
    ptL090_bal->snPurchases     = ptTblAcct->PURCHASES;        /*���ѽ��*/
    ptL090_bal->unNbrpurch      = ptTblAcct->NBR_PURCH;        /*���ѱ���*/

    ptL090_bal->snCashadvce     = ptTblAcct->CASH_ADVCE;       /*ȡ�ֽ��*/
    ptL090_bal->unNbrcashad     = ptTblAcct->NBR_CASHAD;       /*ȡ�ֱ���*/

    ptL090_bal->snPaymtclrd     = ptTblAcct->PAYMT_CLRD;       /*������*/
    ptL090_bal->unNbrpaymnt     = ptTblAcct->NBR_PAYMNT;       /*�������*/

    ptL090_bal->snCashadfee     = ptTblAcct->CASH_ADFEE;       /*ȡ�ַ���*/
    ptL090_bal->snCardfees      = ptTblAcct->CARD_FEES;        /*��Ƭ��ѽ��*/
    ptL090_bal->unNbrfeedty     = ptTblAcct->NBR_FEEDTY;       /*��Ƭ���ñ���*/

    ptL090_bal->snOtherfees     = ptTblAcct->OTHER_FEES;       /*�������ý��*/
    ptL090_bal->snPen_chrg      = ptTblAcct->PEN_CHRG;         /*���ɽ���*/
    ptL090_bal->unNbrothers     = ptTblAcct->NBR_OTHERS;       /*�������ñ���*/

    ptL090_bal->snDebitadj      = ptTblAcct->DEBIT_ADJ;        /*��ǵ������*/
    ptL090_bal->snCredtadj      = ptTblAcct->CRED_ADJ;         /*���ǵ������*/
    ptL090_bal->snFees_taxes    = ptTblAcct->FEES_TAXES;       /*����˰����*/
    ptL090_bal->snInt_earned    = ptTblAcct->INT_EARNED;       /*�����Ϣ*/
    ptL090_bal->snCredvouch     = ptTblAcct->CRED_VOUCH;       /*�˻����*/
    ptL090_bal->unNbrtrans      = ptTblAcct->NBR_TRANS;        /*���ױ���*/
    /*ptL090_bal->unCash1stac     = ptTblAcct->CASH1STAC; */       /*�״�ȡ������*//*del for CHG-131127-0029 by zyh 20131216 */

    ptL090_bal->snCashtday      = ptTblAcct->CASH_TDAY;        /*����ȡ�ֽ��*/
    ptL090_bal->snPaymttday     = ptTblAcct->PAYMT_TDAY;       /*���ջ�����*/
	ptL090_bal->snIntfetday	 = ptTblAcct->INTFE_TDAY;       /*����Ϣ�ѷ����� CHG-111201-0018*/
    ptL090_bal->snTodayamt      = ptTblAcct->TODAY_AMT;        /*���շ�ȡ�ֻ������*/
    ptL090_bal->unTodayrel      = ptTblAcct->TODAY_REL;        /*��������*/

    ptL090_bal->snLastpayamt    = ptTblAcct->LASTPAYAMT;       /*�ϴλ�����*/
    ptL090_bal->unLastpayday    = ptTblAcct->LASTPAYDAY;       /*�ϴλ�������*/
    ptL090_bal->unLasttrday     = ptTblAcct->LAST_TRDAY;       /*�ϴν�������*/

    ptL090_bal->unHipurchse     = ptTblAcct->HI_PURCHSE;       /*������ѽ��*/
    ptL090_bal->unHipurmmyy     = ptTblAcct->HI_PURMMYY;       /*��������·�*/

    ptL090_bal->unHicashadv     = ptTblAcct->HI_CASHADV;       /*���ȡ�ֽ��*/
    ptL090_bal->unHicashmmyy    = ptTblAcct->HI_CASMMYY;       /*���ȡ���·�*/

    /*ptL090_bal->unHidebit       = ptTblAcct->HI_DEBIT;*/         /*��߽�ǽ��׽��*//*del for CHG-131127-0029 by zyh 20131216 */
    /*del for CHG-130819-0038 by zyh 20131216*/
    /*ptL090_bal->unHidebmmyy     = ptTblAcct->HI_DEBMMYY;*/       /*��߽�ǽ����·�*/

    /*ptL090_bal->unHicredt       = ptTblAcct->HI_CREDIT;    *//*��ߴ��Ǵ��ǽ��*//*DEL FOR CS-6320-2012-0095*/
   /* ptL090_bal->unHicrdmmyy     = ptTblAcct->HI_CRDMMYY;  */ /*��ߴ��ǽ����·�*//*DEL FOR CS-6320-2012-0095*/
    ptL090_bal->snMp_instal= ptTblAcct->MP_INSTAL; /*�����������ڽ��*//*ADD FOR CS-6320-2012-0095*/
    /* ɾ��HI_OLIMIT
    ptL090_bal->unHiolimit      = ptTblAcct->HI_OLIMIT;
    */
   /* ptL090_bal->unHiolimmyy     = ptTblAcct->HI_OLIMMYY;*//*del for CS-6440-2013-0059 by zyh 20131216*/

    ptL090_bal->unNbrolimit     = ptTblAcct->NBR_OLIMIT;       /*���޴���*/
}

/********************************************************************************
 *  L090_3_accx_init
 *  ʹ���˻�����ʼ����ʱ�ṹ����
 *
 *  �βΣ�
 *      ���룺accx_tdef       *ptTblAccx
 *      �����L090_bal_sdef   *ptL090_bal
 *
 *  ����ֵ��
 *******************************************************************************/
void L090_3_accx_init(accx_tdef       *ptTblAccx,
                        L090_bal_sdef   *ptL090_bal)
{
    ptL090_bal->snPurchases     = ptTblAccx->PURCHASES;        /*���ѽ��*/
    ptL090_bal->unNbrpurch      = ptTblAccx->NBR_PURCH;        /*���ѱ���*/

    ptL090_bal->snCashadvce     = ptTblAccx->CASH_ADVCE;       /*ȡ�ֽ��*/
    ptL090_bal->unNbrcashad     = ptTblAccx->NBR_CASHAD;       /*ȡ�ֱ���*/

    ptL090_bal->snPaymtclrd     = ptTblAccx->PAYMT_CLRD;       /*������*/
    ptL090_bal->unNbrpaymnt     = ptTblAccx->NBR_PAYMNT;       /*�������*/

    ptL090_bal->snCashadfee     = ptTblAccx->CASH_ADFEE;       /*ȡ�ַ���*/
    ptL090_bal->snCardfees      = ptTblAccx->CARD_FEES;        /*��Ƭ��ѽ��*/
    ptL090_bal->unNbrfeedty     = ptTblAccx->NBR_FEEDTY;       /*��Ƭ���ñ���*/

    ptL090_bal->snOtherfees     = ptTblAccx->OTHER_FEES;       /*�������ý��*/
    ptL090_bal->snPen_chrg      = ptTblAccx->PEN_CHRG;         /*���ɽ���*/
    ptL090_bal->unNbrothers     = ptTblAccx->NBR_OTHERS;       /*�������ñ���*/

    ptL090_bal->snDebitadj      = ptTblAccx->DEBIT_ADJ;        /*��ǵ������*/
    ptL090_bal->snCredtadj      = ptTblAccx->CRED_ADJ;         /*���ǵ������*/
    ptL090_bal->snFees_taxes    = ptTblAccx->FEES_TAXES;       /*����˰����*/
    ptL090_bal->snInt_earned    = ptTblAccx->INT_EARNED;       /*�����Ϣ*/
    ptL090_bal->snCredvouch     = ptTblAccx->CRED_VOUCH;       /*�˻����*/
    ptL090_bal->unNbrtrans      = ptTblAccx->NBR_TRANS;        /*���ױ���*/

    ptL090_bal->snCashtday      = ptTblAccx->CASH_TDAY;        /*����ȡ�ֽ��*/
    ptL090_bal->snPaymttday     = ptTblAccx->PAYMT_TDAY;       /*���ջ�����*/
    ptL090_bal->snPaymtxday     = ptTblAccx->PAYMTX_TDY;       /*������һ�����*/
	ptL090_bal->snIntfetday	 = ptTblAccx->INTFE_TDAY;       /*����Ϣ�ѷ����� CHG-111201-0018*/
    ptL090_bal->snTodayamt      = ptTblAccx->TODAY_AMT;        /*���շ�ȡ�ֻ������*/
    ptL090_bal->unTodayrel      = ptTblAccx->TODAY_REL;        /*��������*/

    ptL090_bal->snLastpayamt    = ptTblAccx->LASTPAYAMT;       /*�ϴλ�����*/
    ptL090_bal->unLastpayday    = ptTblAccx->LASTPAYDAY;       /*�ϴλ�������*/
    ptL090_bal->unLasttrday     = ptTblAccx->LAST_TRDAY;       /*�ϴν�������*/

    ptL090_bal->unHipurchse     = ptTblAccx->HI_PURCHSE;       /*������ѽ��*/
    ptL090_bal->unHipurmmyy     = ptTblAccx->HI_PURMMYY;       /*��������·�*/

    ptL090_bal->unHicashadv     = ptTblAccx->HI_CASHADV;       /*���ȡ�ֽ��*/
    ptL090_bal->unHicashmmyy    = ptTblAccx->HI_CASMMYY;       /*���ȡ���·�*/

    /*ptL090_bal->unHidebit       = ptTblAccx->HI_DEBIT; */        /*��߽�ǽ��׽��*//*del for CHG-131127-0029 by zyh 20131216 */
    /*ptL090_bal->unHidebmmyy     = ptTblAccx->HI_DEBMMYY; */      /*��߽�ǽ����·�*//*del for CHG-131127-0029 by zyh 20131216 */

    /*ptL090_bal->unHicredt       = ptTblAccx->HI_CREDIT; */      /*��ߴ��Ǵ��ǽ��*//*del for CHG-131127-0029 by zyh 20131216 */
    /*ptL090_bal->unHicrdmmyy     = ptTblAccx->HI_CRDMMYY;  ��ߴ��ǽ����·�*//*del for CHG-130828-0002*/

    ptL090_bal->unNbrolimit     = ptTblAccx->NBR_OLIMIT;       /*���޴���*/
}

/********************************************************************************
 *  L090_7_acct_set
 *  ������ʱ�������˻�
 *
 *  �βΣ�
 *      ���룺L090_bal_sdef   *ptL090_bal
 *      �����acct_tdef       *ptTblAcct
 *            acctflag_tdef   *ptTblAcctflag
 *
 *  ����ֵ��
 *******************************************************************************/
void L090_7_acct_set(L090_bal_sdef   *ptL090_bal,
                       acct_tdef       *ptTblAcct,
                       acctflag_tdef   *ptTblAcctflag)
{

    time_sdef  stTime = {0};
    date_sdef  stDate = {0};

    ptTblAcct->PURCHASES        = ptL090_bal->snPurchases;       /*���ѽ��*/
    ptTblAcctflag->PURCHASES      = 1;
    ptTblAcct->NBR_PURCH        = ptL090_bal->unNbrpurch;        /*���ѱ���*/
    ptTblAcctflag->NBR_PURCH      = 1;

    ptTblAcct->CASH_ADVCE       = ptL090_bal->snCashadvce;       /*ȡ�ֽ��*/
    ptTblAcctflag->CASH_ADVCE     = 1;
    ptTblAcct->NBR_CASHAD       = ptL090_bal->unNbrcashad;       /*ȡ�ֱ���*/
    ptTblAcctflag->NBR_CASHAD     = 1;

    ptTblAcct->PAYMT_CLRD       = ptL090_bal->snPaymtclrd;       /*������*/
    ptTblAcctflag->PAYMT_CLRD     = 1;
    ptTblAcct->NBR_PAYMNT       = ptL090_bal->unNbrpaymnt;       /*�������*/
    ptTblAcctflag->NBR_PAYMNT     = 1;

    ptTblAcct->CASH_ADFEE       = ptL090_bal->snCashadfee;       /*ȡ�ַ���*/
    ptTblAcctflag->CASH_ADFEE     = 1;
    ptTblAcct->CARD_FEES        = ptL090_bal->snCardfees;        /*��Ƭ��ѽ��*/
    ptTblAcctflag->CARD_FEES      = 1;
    ptTblAcct->NBR_FEEDTY       = ptL090_bal->unNbrfeedty;       /*��Ƭ���ñ���*/
    ptTblAcctflag->NBR_FEEDTY     = 1;

    ptTblAcct->OTHER_FEES       = ptL090_bal->snOtherfees;       /*�������ý��*/
    ptTblAcctflag->OTHER_FEES     = 1;
    ptTblAcct->PEN_CHRG         = ptL090_bal->snPen_chrg;        /*���ɽ���*/
    ptTblAcctflag->PEN_CHRG       = 1;
    ptTblAcct->NBR_OTHERS       = ptL090_bal->unNbrothers;       /*�������ñ���*/
    ptTblAcctflag->NBR_OTHERS     = 1;

    ptTblAcct->DEBIT_ADJ        = ptL090_bal->snDebitadj;        /*��ǵ������*/
    ptTblAcctflag->DEBIT_ADJ      = 1;
    ptTblAcct->CRED_ADJ         = ptL090_bal->snCredtadj;        /*���ǵ������*/
    ptTblAcctflag->CRED_ADJ       = 1;
    ptTblAcct->FEES_TAXES       = ptL090_bal->snFees_taxes;      /*����˰����*/
    ptTblAcctflag->FEES_TAXES     = 1;
    ptTblAcct->INT_EARNED       = ptL090_bal->snInt_earned;      /*�����Ϣ*/
    ptTblAcctflag->INT_EARNED     = 1;
    ptTblAcct->CRED_VOUCH       = ptL090_bal->snCredvouch;       /*�˻����*/
    ptTblAcctflag->CRED_VOUCH     = 1;
    ptTblAcct->NBR_TRANS        = ptL090_bal->unNbrtrans;        /*���ױ���*/
    ptTblAcctflag->NBR_TRANS      = 1;
    /*del for CHG-131127-0029 by zyh 20131216 begin*/
    /*ptTblAcct->CASH1STAC        = ptL090_bal->unCash1stac;*/       /*�״�ȡ������*/
    /*ptTblAcctflag->CASH1STAC      = 1;*/
    /*del for CHG-131127-0029 by zyh 20131216 end*/
    ptTblAcct->CASH_TDAY        = ptL090_bal->snCashtday;        /*����ȡ�ֽ��*/
    ptTblAcctflag->CASH_TDAY      = 1;
    ptTblAcct->PAYMT_TDAY       = ptL090_bal->snPaymttday;       /*���ջ�����*/
    ptTblAcctflag->PAYMT_TDAY     = 1;
    /*BEG ADD CHG-111201-0018*/
    ptTblAcct->INTFE_TDAY       = ptL090_bal->snIntfetday;       /*����Ϣ�ѽ��*/
    ptTblAcctflag->INTFE_TDAY     = 1;
    /*END ADD CHG-111201-0018*/
    ptTblAcct->TODAY_AMT        = ptL090_bal->snTodayamt;        /*���շ�ȡ�ֻ������*/
    ptTblAcctflag->TODAY_AMT      = 1;
    ptTblAcct->TODAY_REL        = ptL090_bal->unTodayrel;        /*��������*/
    ptTblAcctflag->TODAY_REL      = 1;

    ptTblAcct->LASTPAYAMT       = ptL090_bal->snLastpayamt;      /*�ϴλ�����*/
    ptTblAcctflag->LASTPAYAMT     = 1;
    ptTblAcct->LASTPAYDAY       = ptL090_bal->unLastpayday;      /*�ϴλ�������*/
    ptTblAcctflag->LASTPAYDAY     = 1;
    ptTblAcct->LAST_TRDAY       = ptL090_bal->unLasttrday;       /*�ϴν�������*/
    ptTblAcctflag->LAST_TRDAY     = 1;

    ptTblAcct->HI_PURCHSE       = ptL090_bal->unHipurchse;       /*������ѽ��*/
    ptTblAcctflag->HI_PURCHSE     = 1;
    ptTblAcct->HI_PURMMYY       = ptL090_bal->unHipurmmyy;       /*��������·�*/
    ptTblAcctflag->HI_PURMMYY     = 1;

    ptTblAcct->HI_CASHADV       = ptL090_bal->unHicashadv;       /*���ȡ�ֽ��*/
    ptTblAcctflag->HI_CASHADV     = 1;
    ptTblAcct->HI_CASMMYY       = ptL090_bal->unHicashmmyy;      /*���ȡ���·�*/
    ptTblAcctflag->HI_CASMMYY     = 1;
    /*del for CHG-131127-0029 by zyh 20131216 begin*/
    /*ptTblAcct->HI_DEBIT         = ptL090_bal->unHidebit;*/         /*��߽�ǽ��׽��*/
    /*ptTblAcctflag->HI_DEBIT       = 1;*/
    /*del for CHG-131127-0029 by zyh 20131216 end*/
    /*del for CHG-130819-0038 by zyh 20131216 begin*/
    /*ptTblAcct->HI_DEBMMYY       = ptL090_bal->unHidebmmyy;*/      /*��߽�ǽ����·�*/
    /*ptTblAcctflag->HI_DEBMMYY     = 1;*/
    /*del for CHG-130819-0038 by zyh 20131216 end*/
    /* ptTblAcct->HI_CREDIT        = ptL090_bal->unHicredt;   *//*DEL FOR CS-6320-2012-0095*/      /*��ߴ��Ǵ��ǽ��*/
    /* ptTblAcctflag->HI_CREDIT      = 1;*//*DEL FOR CS-6320-2012-0095*/
    /*ptTblAcct->HI_CRDMMYY       = ptL090_bal->unHicrdmmyy;*//*DEL FOR CS-6320-2012-0095*/        /*��ߴ��ǽ����·�*/
    /* ptTblAcctflag->HI_CRDMMYY     = 1;*//*DEL FOR CS-6320-2012-0095*/
    ptTblAcct->MP_INSTAL= ptL090_bal->snMp_instal;/*�����������ڽ��*//*ADD FOR CS-6320-2012-0095*/
    ptTblAcctflag->MP_INSTAL = 1;
    /* ɾ��HI_OLIMIT
    ptTblAcct->HI_OLIMIT        = ptL090_bal->unHiolimit;
    ptTblAcctflag->HI_OLIMIT      = 1;
    */
    /*del for CS-6440-2013-0059 by zyh 20131216 begin*/
    /* ptTblAcct->HI_OLIMMYY       = ptL090_bal->unHiolimmyy;    */   /*��߳����·�*/
    /*ptTblAcctflag->HI_OLIMMYY     = 1;*/
    /*del for CS-6440-2013-0059 by zyh 20131216 end*/
    ptTblAcct->NBR_OLIMIT       = ptL090_bal->unNbrolimit;       /*���޴���*/
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
 *  ������ʱ�������˻�
 *
 *  �βΣ�
 *      ���룺L090_bal_sdef   *ptL090_bal
 *      �����accx_tdef       *ptTblAccx
 *            accxflag_tdef   *ptTblAccxflag
 *
 *  ����ֵ��
 *******************************************************************************/
void L090_7_accx_set(L090_bal_sdef   *ptL090_bal,
                       accx_tdef       *ptTblAccx,
                       accxflag_tdef   *ptTblAccxflag)
{
    time_sdef  stTime = {0};
    date_sdef  stDate = {0};

    ptTblAccx->PURCHASES        = ptL090_bal->snPurchases;       /*���ѽ��*/
    ptTblAccxflag->PURCHASES      = 1;
    ptTblAccx->NBR_PURCH        = ptL090_bal->unNbrpurch;        /*���ѱ���*/
    ptTblAccxflag->NBR_PURCH      = 1;

    ptTblAccx->CASH_ADVCE       = ptL090_bal->snCashadvce;       /*ȡ�ֽ��*/
    ptTblAccxflag->CASH_ADVCE     = 1;
    ptTblAccx->NBR_CASHAD       = ptL090_bal->unNbrcashad;       /*ȡ�ֱ���*/
    ptTblAccxflag->NBR_CASHAD     = 1;

    ptTblAccx->PAYMT_CLRD       = ptL090_bal->snPaymtclrd;       /*������*/
    ptTblAccxflag->PAYMT_CLRD     = 1;
    ptTblAccx->NBR_PAYMNT       = ptL090_bal->unNbrpaymnt;       /*�������*/
    ptTblAccxflag->NBR_PAYMNT     = 1;

    ptTblAccx->CASH_ADFEE       = ptL090_bal->snCashadfee;       /*ȡ�ַ���*/
    ptTblAccxflag->CASH_ADFEE     = 1;
    ptTblAccx->CARD_FEES        = ptL090_bal->snCardfees;        /*��Ƭ��ѽ��*/
    ptTblAccxflag->CARD_FEES      = 1;
    ptTblAccx->NBR_FEEDTY       = ptL090_bal->unNbrfeedty;       /*��Ƭ���ñ���*/
    ptTblAccxflag->NBR_FEEDTY     = 1;

    ptTblAccx->OTHER_FEES       = ptL090_bal->snOtherfees;       /*�������ý��*/
    ptTblAccxflag->OTHER_FEES     = 1;
    ptTblAccx->PEN_CHRG         = ptL090_bal->snPen_chrg;        /*���ɽ���*/
    ptTblAccxflag->PEN_CHRG       = 1;
    ptTblAccx->NBR_OTHERS       = ptL090_bal->unNbrothers;       /*�������ñ���*/
    ptTblAccxflag->NBR_OTHERS     = 1;

    ptTblAccx->DEBIT_ADJ        = ptL090_bal->snDebitadj;        /*��ǵ������*/
    ptTblAccxflag->DEBIT_ADJ      = 1;
    ptTblAccx->CRED_ADJ         = ptL090_bal->snCredtadj;        /*���ǵ������*/
    ptTblAccxflag->CRED_ADJ       = 1;
    ptTblAccx->FEES_TAXES       = ptL090_bal->snFees_taxes;      /*����˰����*/
    ptTblAccxflag->FEES_TAXES     = 1;
    ptTblAccx->INT_EARNED       = ptL090_bal->snInt_earned;      /*�����Ϣ*/
    ptTblAccxflag->INT_EARNED     = 1;
    ptTblAccx->CRED_VOUCH       = ptL090_bal->snCredvouch;       /*�˻����*/
    ptTblAccxflag->CRED_VOUCH     = 1;
    ptTblAccx->NBR_TRANS        = ptL090_bal->unNbrtrans;        /*���ױ���*/
    ptTblAccxflag->NBR_TRANS      = 1;

    ptTblAccx->CASH_TDAY        = ptL090_bal->snCashtday;        /*����ȡ�ֽ��*/
    ptTblAccxflag->CASH_TDAY      = 1;
    ptTblAccx->PAYMT_TDAY       = ptL090_bal->snPaymttday;       /*���ջ�����*/
    ptTblAccxflag->PAYMT_TDAY     = 1;
    ptTblAccx->PAYMTX_TDY       = ptL090_bal->snPaymtxday;       /*������һ�����*/
    ptTblAccxflag->PAYMTX_TDY     = 1;
    /*BEG ADD CHG-111201-0018*/
    ptTblAccx->INTFE_TDAY       = ptL090_bal->snIntfetday;       /*����Ϣ�ѽ��*/
    ptTblAccxflag->INTFE_TDAY     = 1;
    /*END ADD CHG-111201-0018*/
    ptTblAccx->TODAY_AMT        = ptL090_bal->snTodayamt;        /*���շ�ȡ�ֻ������*/
    ptTblAccxflag->TODAY_AMT      = 1;
    ptTblAccx->TODAY_REL        = ptL090_bal->unTodayrel;        /*��������*/
    ptTblAccxflag->TODAY_REL      = 1;

    ptTblAccx->LASTPAYAMT       = ptL090_bal->snLastpayamt;      /*�ϴλ�����*/
    ptTblAccxflag->LASTPAYAMT     = 1;
    ptTblAccx->LASTPAYDAY       = ptL090_bal->unLastpayday;      /*�ϴλ�������*/
    ptTblAccxflag->LASTPAYDAY     = 1;
    ptTblAccx->LAST_TRDAY       = ptL090_bal->unLasttrday;       /*�ϴν�������*/
    ptTblAccxflag->LAST_TRDAY     = 1;

    ptTblAccx->HI_PURCHSE       = ptL090_bal->unHipurchse;       /*������ѽ��*/
    ptTblAccxflag->HI_PURCHSE     = 1;
    ptTblAccx->HI_PURMMYY       = ptL090_bal->unHipurmmyy;       /*��������·�*/
    ptTblAccxflag->HI_PURMMYY     = 1;

    ptTblAccx->HI_CASHADV       = ptL090_bal->unHicashadv;       /*���ȡ�ֽ��*/
    ptTblAccxflag->HI_CASHADV     = 1;
    ptTblAccx->HI_CASMMYY       = ptL090_bal->unHicashmmyy;      /*���ȡ���·�*/
    ptTblAccxflag->HI_CASMMYY     = 1;
    /*del for CHG-131127-0029 by zyh 20131216 begin*/
    /*ptTblAccx->HI_DEBIT         = ptL090_bal->unHidebit;         ��߽�ǽ��׽��
    ptTblAccxflag->HI_DEBIT       = 1;
    ptTblAccx->HI_DEBMMYY       = ptL090_bal->unHidebmmyy;       ��߽�ǽ����·�
    ptTblAccxflag->HI_DEBMMYY     = 1;*/
    /*del for CHG-131127-0029 by zyh 20131216 end*/
    /*DEL FOR CS-6320-2012-0095 BEGIN*/
    /*ptTblAccx->HI_CREDIT        = ptL090_bal->unHicredt ;  */     /*��ߴ��Ǵ��ǽ��*/
    /* ptTblAccxflag->HI_CREDIT      = 1;*/
    /* ptTblAccx->HI_CRDMMYY       = ptL090_bal->unHicrdmmyy; */      /*��ߴ��ǽ����·�*/
    /*ptTblAccxflag->HI_CRDMMYY     = 1;*/
    /*DEL FOR CS-6320-2012-0095 END*/
    ptTblAccx->NBR_OLIMIT       = ptL090_bal->unNbrolimit;       /*���޴���*/
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
