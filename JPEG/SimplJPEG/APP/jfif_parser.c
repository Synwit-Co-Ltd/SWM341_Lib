#include <string.h>
#include "SWM341.h"

#include "jfif_parser.h"


int jfif_parse(const uint8_t buf[], uint32_t len, jfif_info_t * jfif_info)
{
    uint32_t index = 0;
    uint16_t marker;
	uint16_t blklen;
	uint32_t i, j;
	
	memset(jfif_info, 0x00, sizeof(jfif_info_t));

    do {
        marker = buf[index++];
        marker = (marker << 8) | buf[index++];

        switch(marker)
        {
        case JFIF_MKR_SOI:
            break;

        case JFIF_MKR_APP0: case JFIF_MKR_APP1: case JFIF_MKR_APP2: case JFIF_MKR_APP3:
		case JFIF_MKR_APP4: case JFIF_MKR_APP5: case JFIF_MKR_APP6: case JFIF_MKR_APP7:
		case JFIF_MKR_APP8: case JFIF_MKR_APP9: case JFIF_MKR_APPa: case JFIF_MKR_APPb:
		case JFIF_MKR_APPc: case JFIF_MKR_APPd: case JFIF_MKR_APPe: case JFIF_MKR_APPf:
            index += (buf[index] << 8) | buf[index+1];
            break;

        case JFIF_MKR_DQT:
			blklen = (buf[index] << 8) | buf[index+1];
			
			index += 2;
			blklen -= 2;
			while(blklen)
			{
				uint8_t idx = buf[index] & 0x0F;
				uint8_t fmt = buf[index] >> 4;		// table element precision, 0 8-bit   1 16-bit
				
				if(fmt != 0)
					return JFIF_NO_SUPPORT;
				
				if(idx >= JFIF_QTAB_MAX)
					return JFIF_NO_SUPPORT;
				
				for(j = 0; j < 64; j++)
					jfif_info->QTable[idx][j] = buf[index+1+j];
				jfif_info->QTableCnt++;
				
				index += 65;
				blklen -= 65;
			}
            break;

		case JFIF_MKR_SOF0:
			if(buf[index+2] != 8)
				return JFIF_NO_SUPPORT;
		
			jfif_info->Height = (buf[index+3] << 8) | buf[index+4];
			jfif_info->Width  = (buf[index+5] << 8) | buf[index+6];
			
			jfif_info->CompCnt = buf[index+7];
			if(jfif_info->CompCnt != 3)
				return JFIF_NO_SUPPORT;
			
			index += 8;
			for(i = 0; i < jfif_info->CompCnt; i++)
			{
				jfif_info->CompInfo[i].id      = buf[index];
				jfif_info->CompInfo[i].hfactor = buf[index+1] >> 4;
				jfif_info->CompInfo[i].vfactor = buf[index+1] & 0x0F;
				jfif_info->CompInfo[i].qtab_id = buf[index+2];
				
				index += 3;
			}
            break;
		
		case JFIF_MKR_DHT:
			blklen = (buf[index] << 8) | buf[index+1];
			
			index += 2;
			blklen -= 2;
			while(blklen)
			{
				uint8_t idx = buf[index] & 0x0F;
				uint8_t typ = buf[index] >> 4;		// 0 DC   1 AC
				
				if(idx >= JFIF_HTAB_MAX)
					return JFIF_NO_SUPPORT;
				
				uint8_t  indx = 0;
				uint16_t code = 0;
				for(i = 1; i < 17; i++)
				{
					for(j = 0; j < buf[index+i]; j++)
					{
						if(typ == 0)
						{
							jfif_info->HTable[idx].DC.codeVal[indx]  = buf[index+17+indx];
							jfif_info->HTable[idx].DC.codeLen[indx]  = i;
							jfif_info->HTable[idx].DC.codeWord[indx] = code;
						}
						else
						{
							jfif_info->HTable[idx].AC.codeVal[indx]  = buf[index+17+indx];
							jfif_info->HTable[idx].AC.codeLen[indx]  = i;
							jfif_info->HTable[idx].AC.codeWord[indx] = code;
						}
						
						indx += 1;
						code += 1;
					}
					
					code <<= 1;
				}
				jfif_info->HTableCnt++;
				
				index += 17+indx;
				blklen -= 17+indx;
			}
            break;
		
        case JFIF_MKR_SOS:
            if(buf[index+2] != 3)
				return JFIF_NO_SUPPORT;
			
			index += 3;
			for(i = 0; i < jfif_info->CompCnt; i++)
			{
				jfif_info->CompInfo[i].htab_id_dc = buf[index+1] >> 4;
				jfif_info->CompInfo[i].htab_id_ac = buf[index+1] & 0x0F;
				
				index += 2;
			}
			
			if(((buf[index] << 16) | (buf[index+1] << 8) | buf[index+2]) != 0x003F00)
				return JFIF_FORMAT_ERR;
			index += 3;
			
			jfif_info->CodeAddr = (uint32_t)&buf[index];
			jfif_info->CodeLen = len - index;
			
			if(jfif_info->HTableCnt == 3)
			{
				if((jfif_info->CompInfo[0].htab_id_dc == jfif_info->CompInfo[1].htab_id_dc) &&
				   (jfif_info->CompInfo[0].htab_id_dc == jfif_info->CompInfo[2].htab_id_dc))	// use same DC table
				{
					if(jfif_info->CompInfo[0].htab_id_dc == 0)
						memcpy(&jfif_info->HTable[1].DC, &jfif_info->HTable[0].DC, sizeof(jfif_info->HTable[0].DC));
					else
						memcpy(&jfif_info->HTable[0].DC, &jfif_info->HTable[1].DC, sizeof(jfif_info->HTable[0].DC));
					
					jfif_info->CompInfo[0].htab_id_dc = jfif_info->CompInfo[0].htab_id_ac;
					jfif_info->CompInfo[1].htab_id_dc = jfif_info->CompInfo[1].htab_id_ac;
					jfif_info->CompInfo[2].htab_id_dc = jfif_info->CompInfo[2].htab_id_ac;
				}
				else																			// use same AC table
				{
					if(jfif_info->CompInfo[0].htab_id_ac == 0)
						memcpy(&jfif_info->HTable[1].AC, &jfif_info->HTable[0].AC, sizeof(jfif_info->HTable[0].AC));
					else
						memcpy(&jfif_info->HTable[0].AC, &jfif_info->HTable[1].AC, sizeof(jfif_info->HTable[0].AC));
					
					jfif_info->CompInfo[0].htab_id_ac = jfif_info->CompInfo[0].htab_id_dc;
					jfif_info->CompInfo[1].htab_id_ac = jfif_info->CompInfo[1].htab_id_dc;
					jfif_info->CompInfo[2].htab_id_ac = jfif_info->CompInfo[2].htab_id_dc;
				}
				
				jfif_info->HTableCnt += 1;
			}
			
			jfif_info->HTableCnt /= 2;
			
			return JFIF_RES_OK;

		case JFIF_MKR_DRI:
			jfif_info->RestartInterval = (buf[index+2] << 8) | buf[index+3];
			index += (buf[index] << 8) | buf[index+1];
			break;
		
		case JFIF_MKR_COM:
            index += (buf[index] << 8) | buf[index+1];
            break;
		
        default:
            return JFIF_FORMAT_ERR;
        }
    } while(index < len);
	
	return JFIF_FORMAT_ERR;
}
