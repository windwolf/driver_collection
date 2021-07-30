//-ĺżŤélog2 ççŽćł
#include "../inc/shared.h"

uint32_t fast_log2(uint32_t _val)
{
    uint32_t ret;
    float fdata = (float)_val;
    //unsigned int uData = (fdata>>23)&0xFF;		//ç´ćĽç§ťĺ¨ĺşé
    uint32_t data = *(uint32_t *)&fdata; //(unsigned int&)fdata ä¸(unsigned int*)&fdata; ä¸č
    unsigned int udata = (data >> 23) & 0xFF;
    ret = (int)udata - 127; //-ć çŹŚĺˇĺ°ćçŹŚĺ

    return ret;
}

void event_handler_add(Events *events, Event *event)
{
    Event *parent = events->event;
    if (parent == NULL)
    {
        events->event = event;
        return;
    }
    while (parent->next != NULL)
    {
        parent = parent->next;
    }
    parent->next = event;
};