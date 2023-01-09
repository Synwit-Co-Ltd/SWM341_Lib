#include <stdint.h>
#include <string.h>


#define cb_item_t	uint8_t		// item type

#define CB_ITEM_N	1024		// item number，实际可存入数据少一个，留一个空位表示满，以区别于空


typedef struct
{
    cb_item_t buf[CB_ITEM_N];
    int       wrptr;            // 写指针
    int       rdptr;            // 读指针
} CircleBuffer_t;


/* Element Count in buffer */
static int CirBuf_Count(CircleBuffer_t *cirbuf)
{
    return (cirbuf->wrptr - cirbuf->rdptr) & (CB_ITEM_N - 1);
}

/* Space available in buffer */
static int CirBuf_Space(CircleBuffer_t *cirbuf)
{
    return (cirbuf->rdptr - cirbuf->wrptr - 1) & (CB_ITEM_N - 1);
}

static int CirBuf_Empty(CircleBuffer_t *cirbuf)
{
    return CirBuf_Count(cirbuf) == 0;
}

static int CirBuf_Full(CircleBuffer_t *cirbuf)
{
    return CirBuf_Space(cirbuf) == 0;
}

static void CirBuf_Clear(CircleBuffer_t *cirbuf)
{
    cirbuf->rdptr = cirbuf->wrptr;
}

static int CirBuf_Write(CircleBuffer_t *cirbuf, cb_item_t *buf, int num)
{
    int n;

    if(cirbuf->wrptr >= cirbuf->rdptr)
    {
        n = CB_ITEM_N - cirbuf->wrptr;
        if(cirbuf->rdptr == 0)
            n -= 1;
        
        n = (n >= num) ? num : n;

        memcpy(&cirbuf->buf[cirbuf->wrptr], buf, n * sizeof(cb_item_t));

        cirbuf->wrptr = (cirbuf->wrptr + n) % CB_ITEM_N;

        if((n == num) || CirBuf_Full(cirbuf))
            return n;
        else
            return n + CirBuf_Write(cirbuf, &buf[n], num - n);
    }
    else
    {
        n = CirBuf_Space(cirbuf);

        n = (n >= num) ? num : n;

        memcpy(&cirbuf->buf[cirbuf->wrptr], buf, n * sizeof(cb_item_t));

        cirbuf->wrptr += n;

        return n;
    }
}

static int CirBuf_Read(CircleBuffer_t *cirbuf, cb_item_t *buf, int num)
{
    int n;

    if(cirbuf->wrptr >= cirbuf->rdptr)
    {
        n = CirBuf_Count(cirbuf);

        n = (n >= num) ? num : n;

        memcpy(buf, &cirbuf->buf[cirbuf->rdptr], n * sizeof(cb_item_t));

        cirbuf->rdptr += n;

        return n;
    }
    else
    {
        n = CB_ITEM_N - cirbuf->rdptr;

        n = (n >= num) ? num : n;

        memcpy(buf, &cirbuf->buf[cirbuf->rdptr], n * sizeof(cb_item_t));

        cirbuf->rdptr = (cirbuf->rdptr + n) % CB_ITEM_N;

        if((n == num) || CirBuf_Empty(cirbuf))
            return n;
        else
            return n + CirBuf_Read(cirbuf, &buf[n], num - n);
    }
}
