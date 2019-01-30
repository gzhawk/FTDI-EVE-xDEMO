/* 
    Hardware Abstract Layer for EVE 
    Author: Hawk
    Email : hawk.gao@ftdichip.com	
    Date  : 2013/Oct
 */
#include "platform.h"
#include "EVE_Gpu.h"
#include "EVE_HAL.h"
#include "EVE_APP.h"

FTU32 * mcuCMDBuf = 0;
FTU32 mcuCMDBufSize = 0;
FTU32 mcuCMDindex = 0; 
FTU32 mcuDLindex = 0;

#if defined(USED_CMD_BUF)
FTU8 mem_pool[USE_STATIC_MEM_LEN+4] = {0};
#endif


STATIC FTU32 cmdWait (FTVOID)
{
    while (HAL_Read32(REG_CMD_WRITE) != HAL_Read32(REG_CMD_READ)) {
        if (0xFFF == HAL_Read32(REG_CMD_READ)) {
            FTPRINT("\nco-processor error!");
            UI_END();
            return 0;
        }
    }
    
    return HAL_Read32(REG_CMD_READ);
}

STATIC FTVOID dlpWait (FTVOID)
{
    while (HAL_Read8(REG_DLSWAP)) ;

    mcuDLindex = 0;
}

FTVOID HAL_CoReset (FTVOID)
{
#if defined(DEF_BT81X)
	FTU16 p_addr = HAL_Read16(PATCH_ADDR);
#endif	

    /*
       Bit 2 - 0 :
       Bit 0 for coprocessor engine,
       Bit 1 for touch engine,
       Bit 2 for audio engine.
       Write 1 to reset the corresponding engine.
       Write 0 to go back normal working status.
     */

	/* Set REG_CPURESET to 1, to hold the coprocessor in the reset condition */
    HAL_Write8(REG_CPURESET, 1);
    while (!HAL_Read8(REG_CPURESET));

    /* Set REG_CMD_READ and REG_CMD_WRITE to zero */
    HAL_Write32(REG_CMD_READ,0);
    HAL_Write32(REG_CMD_WRITE,0);
	HAL_Write32(REG_CMD_DL, 0);
    
    mcuCMDBuf[mcuCMDBufSize/4] = REG_FLAG_CLN;
    mcuCMDindex = 0; 
        
    /* j1 will set the pclk to 0 for that error case */
	HAL_Write32(REG_PCLK, 2);

	/* Set REG_CPURESET to 0, to restart the coprocessor */
    HAL_Write8(REG_CPURESET, 0);
    while (HAL_Read8(REG_CPURESET));

#if defined(DEF_BT81X)
    HAL_Write16(PATCH_ADDR, p_addr);
#endif
}

FTU32 HAL_CalResultAddr (FTVOID)
{
    if (mcuCMDBuf) {
        return RAM_CMD + mcuCMDindex - 4;
    } else {	
        return RAM_CMD + HAL_Read32(REG_CMD_WRITE) - 4;
    }
}

FTVOID HAL_McuCmdBufInit (FTVOID)
{
    mcuDLindex = 0;
    FTPRINT("\nCMD buffer: ");
#if defined(USED_CMD_BUF)
    FTPRINT("yes");
    mcuCMDBuf = (FTU32 *)&mem_pool[0];
    mcuCMDBufSize = USE_STATIC_MEM_LEN;
    mcuCMDBuf[mcuCMDBufSize/4] = REG_FLAG_CLN;
    mcuCMDindex = 0; 
#else
    FTPRINT("no");
    mcuCMDBuf = 0;
    mcuCMDBufSize = 0;
    mcuCMDindex = 0;
#endif
}

FTU32 HAL_CmdBufSize (FTVOID)
{
    return mcuCMDBufSize;
}

/*
 * SampleApp smart way to find the cmd fifo free space is:
 * (used space) = ((mcu/eve writen pointer) - (eve executed pointer)) & ((eve cmd fifo length) - 1);
 * (free space) = ((eve cmd fifo length) - 4) - (used space); 
 * refer to Ft_Gpu_Cmdfifo_Freespace in SampleApp for the detail
 * I like my stupid way, so keep using this way
   |---------desLen-------|
   |>>>>>>!>>>>>>>>>>>>>>>|
   pDes  desNow        pDes+desLen

   the loop space may be located in EVE's cmd buffer or EVE's RAM_G
   so make the address as input variable
 */
FTU32 HAL_EVELoopMemWr (FTU32 pDes, FTU32 desNow, FTU32 desLen, FTU8 * inSrc, FTU32 inLen)
{
    FTU32 l;

    if (inLen >= desLen) {
        FTPRINT("\neve loop wr: inLen >= desLen");
        return desNow;
    } else if (inLen == 0) {
        FTPRINT("\neve loop wr: inLen == 0");
        return desNow;
    }

    if (desNow + inLen > desLen) {
        l = desLen - desNow;
        HAL_Write8Src(pDes+desNow,inSrc,l);
        HAL_Write8Src(pDes,&inSrc[l],inLen-l);
        return inLen - l;
    } else {
        HAL_Write8Src(pDes+desNow,inSrc,inLen);
        if (desNow + inLen != desLen) {
            return desNow + inLen;
        } else {
            return 0;
        }
    }
}

FTVOID HAL_CmdToReg (FTU32 Cmd)
{
    FTU32 w = HAL_Read32(REG_CMD_WRITE);

    HAL_Write32(RAM_CMD + w,Cmd);
    w += 4;
    if (w >= CMDBUF_SIZE) {
        w = 0;
    }
    HAL_Write16(REG_CMD_WRITE,(FTU16)w);
}

FTVOID HAL_CmdBufIn (FTU32 Cmd)
{
    if (mcuCMDBuf) {
        if (mcuCMDBufSize <= mcuCMDindex) {
            FTPRINT("\nmcu cmd buf: mcuCMDBufSize <= mcuCMDindex");
            return;
        }
        /* mcuCMDBuf would be used both DLP and CMD for the buffering
           set the CMD tag, to indicate CMD is using
           when tag not match, previous command may using DLP*/
        if (REG_FLAG_CLN == mcuCMDBuf[mcuCMDBufSize/4]) {
            mcuCMDBuf[mcuCMDBufSize/4] = RAM_CMD;
        } else if (mcuCMDBuf[mcuCMDBufSize/4] != RAM_CMD) {
            FTPRINT("\nmcu cmd buf: tag != RAM_CMD");
            return;
        }
        mcuCMDBuf[mcuCMDindex/4] = Cmd;
        mcuCMDindex += 4;
        if (mcuCMDindex == mcuCMDBufSize) {

            HAL_Write16(REG_CMD_WRITE,
                    (FTU16)HAL_EVELoopMemWr(RAM_CMD,HAL_Read32(REG_CMD_WRITE),
                        CMDBUF_SIZE,(FTU8 *)mcuCMDBuf,
                        mcuCMDBufSize));

            mcuCMDindex = 0;
        }
    } else {
        HAL_CmdToReg(Cmd);
    }
}
/* 
 * It's not smart at all
 * you may refer to SampleApp for better solution
 * I'll leave my stupid routine here
 */
FTVOID HAL_CmdBufInStr (FTC8 *pstr)
{
	FTU32 i = 1,tmp = 0;
	FTU8 *p = (FTU8 *)&tmp, little_endian;
    /*
     * one of the reason I'm doing this is:
     * there are two loop buffer needs to be handled
     * 1. mcu cmd buf
     * 2. eve cmd buf
     * both of them got and 'loop free space' need to be consider
     * before doing any continous filling
     * and for string, string become another 'buffer' space
     * 3. string buf
     * when continous filling into whatever mcu buffer or eve buffer
     * to prevent too many 'if there are still free space ahead'
     * judgement in program, I make the action to 4 byte a shot
     */
    little_endian = *(FTU8 *)&i;
    for (i = 0; pstr[i] != '\0'; i++) {
        if (little_endian) {
		    p[i%4] = pstr[i];
        } else {
		    p[3 - i%4] = pstr[i];
        }
		if (i%4 != 3) {
			continue;
		}

        HAL_CmdBufIn(tmp);

        /* 
         make sure:
         1. not 4 byte align length str would have '\0' with the tmp
         2. 4 byte align length str would have '\0'
         once it break from the while loop
         */
		tmp = 0;
    }

    /*
     when the str length is :
     1. 4 byte align, here is to give '\0'
     2. none 4 byte align, here is to give the tmp with '\0'
     */
    HAL_CmdBufIn(tmp);

    return;
}

FTU8 COUNT_ARGS(FTC8 * str)
{
	FTU8 count = 0;
	FTC8 *tmp = str;

	tmp = strstr(tmp, "%");
	while (tmp)
	{
		if (*(tmp + 1) == '%') {
			tmp += 2;
		}
		else {
			count++;
			tmp++;
		}
		tmp = strstr(tmp, "%");
	}
	return count;
}

FTVOID CoCmd_TEXT(FTU32 x, FTU32 y, FTU32 font, FTU32 opt, FTC8 * s, ...)
{
	va_list args;
	FTU8 i, num=0;
	va_start(args, s);
	
#if defined(DEF_BT81X)
    //Only check % characters if option OPT_FORMAT is set 
	num = (opt & OPT_FORMAT) ? (COUNT_ARGS(s)) : (0); 
#endif
	HAL_CmdBufIn(CMD_TEXT);
	HAL_CmdBufIn((((FTU32)y << 16) | (x & 0xffff)));
	HAL_CmdBufIn((((FTU32)opt << 16) | (font & 0xffff)));
	HAL_CmdBufInStr(s);
	for (i = 0; i < num; i++)
	{
	    HAL_CmdBufIn((FTU32)va_arg(args, FTU32));
	}
	va_end(args);
}

FTVOID HAL_DlpBufIn (FTU32 Dlp)
{
    if (mcuCMDBuf) {
        if (EVE_DLP_SIZE <= (mcuDLindex + mcuCMDindex)) {
            FTPRINT("\nmcu dlp buf: EVE_DLP_SIZE <= index");
            return;
        }
        /* mcuCMDBuf would be used both DLP and CMD for the buffering
           set the DLP tag, to indicate DLP is using
           when tag not match, previous command may using CMD*/
        if (REG_FLAG_CLN == mcuCMDBuf[mcuCMDBufSize/4]) {
            mcuCMDBuf[mcuCMDBufSize/4] = RAM_DL;
        } else if (mcuCMDBuf[mcuCMDBufSize/4] != RAM_DL) {
            FTPRINT("\nmcu dlp buf: tag != RAM_DL");
            return;
        }
        mcuCMDBuf[mcuCMDindex/4] = Dlp;
        mcuCMDindex += 4;
        if (mcuCMDindex == mcuCMDBufSize) {
            HAL_Write8Src(RAM_DL+mcuDLindex,
                    (FTU8 *)mcuCMDBuf,mcuCMDBufSize);
            mcuCMDindex = 0;
            mcuDLindex += mcuCMDBufSize;
        }
    } else {
        HAL_Write32(RAM_DL+mcuDLindex,Dlp);
        mcuDLindex += 4;
    }
}

FTVOID HAL_CmdWait (FTU16 CmdWrAddr)
{
    HAL_Write16(REG_CMD_WRITE, CmdWrAddr);
    cmdWait();
}

FTVOID HAL_DlWait (FTVOID)
{
    HAL_Write32(REG_DLSWAP,DLSWAP_FRAME);
    dlpWait();
}

FTVOID HAL_BufToReg (FTU32 reg, FTU32 padNum)
{
    if (reg == RAM_CMD) {
        if (mcuCMDBuf) {
            if (mcuCMDBuf[mcuCMDBufSize/4] != reg) {
                FTPRINT("\nmcu to eve: cmd tag invalid");
                return;
            }
			/* sometime cmd buffer already flush into eve in HAL_CmdBufIn
			   so judge the mcuCMDindex before doing the flush */
			if (mcuCMDindex) {
				HAL_CmdWait((FTU16)HAL_EVELoopMemWr(reg,HAL_Read32(REG_CMD_WRITE),
							CMDBUF_SIZE,(FTU8 *)mcuCMDBuf,
							mcuCMDindex));
			}
        } else {
            HAL_CmdWait((FTU16)HAL_Read32(REG_CMD_WRITE));
        }
    } else {
        if (mcuCMDBuf) {
            if (mcuCMDBuf[mcuCMDBufSize/4] != reg) {
                FTPRINT("\nmcu to eve: dlp tag invalid");
                return;
            }
			/* sometime cmd buffer already flush into eve in HAL_DlpBufIn
			   so judge the mcuCMDindex before doing the flush */
			if (mcuCMDindex) {
				HAL_Write8Src(reg+mcuDLindex,(FTU8 *)mcuCMDBuf,mcuCMDindex);
			}
        }
        HAL_DlWait();
    }

    if (mcuCMDBuf) {
        mcuCMDBuf[mcuCMDBufSize/4] = REG_FLAG_CLN;
        mcuCMDindex = 0;
    }
}

FTVOID HAL_CmdExeNow(FTU32 * pCL, FTU32 l)
{
    FTU32 i = 0;
    
    /* make sure command buffer clean */
    HAL_CmdWait((FTU16)HAL_Read32(REG_CMD_WRITE));

    while (i < l) {
	    HAL_CmdToReg(pCL[i]);
        i++;
    }

    /* make sure command successful executed */
    HAL_CmdWait((FTU16)HAL_Read32(REG_CMD_WRITE));
}

