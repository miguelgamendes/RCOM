#define ESCAPE 0x7D
#define XOR_ESCAPE 0x5D
#define XOR_FLAG 0x5E
#define STF_XOR_VALUE 0x20



int stuffing(unsigned char* buf, int length, unsigned char** stufBuf)
{
	unsigned i;
	unsigned j = 0;
	(*stufBuf)=malloc(length*2+2);
	(*stufBuf)[j++]=F;
	for(i=0; i<length;i++)
	{
		if(buf[i] == F  || buf[i] == ESCAPE)
		{
			(*stufBuf)[j++] = ESCAPE;
			if(buf[i] == F) (*stufBuf)[j++] = XOR_FLAG;
			else (*stufBuf)[j++] = XOR_ESCAPE;
		}
		else
			(*stufBuf)[j++] = buf[i];
	}

	(*stufBuf)[j++]=F;
	return j;
}





int destuffing(unsigned char* buf, int length, unsigned char** unstBuf)
{
	unsigned i = 1;
	unsigned j=0;
	(*unstBuf)=malloc(length-2);
	while(i < length-1)
	{
		if(buf[i] == ESCAPE){
			if(buf[i+1] == XOR_FLAG){
				(*unstBuf)[j++] = F;
				i += 2;
			}
			else if(buf[i+1] == XOR_ESCAPE){
				(*unstBuf)[j++] = ESCAPE;
				i += 2;
			}
			else 
				return -1;

		}
		else
			(*unstBuf)[j++] = buf[i++];
	}
	return j;
}
