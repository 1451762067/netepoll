
/* ------------------------------------------------------------------
 * FileName : msgtool.c (1.100)
 * Function : receive from queue and send to queue
 * ------------------------------------------------------------------ */

#include    <stdio.h>
#include    <stdlib.h>
#include    <sys/types.h>
#include    <sys/ipc.h>
#include    <sys/msg.h>
#include    <memory.h>

#define     MSGSIZE    8192
#define     PERMS      0600

typedef struct queue QUEUE;
struct queue
{
    long msgtype;
    char text[MSGSIZE];
};

/*
 * Delete the Queue, Return 0 if success, (-1) otherwise.
 * Param: Key ------ queue key
 */
int clr_que(key_t Key)
{
    int  qid;
    
    if ((qid = msgget(Key,0)) < 0)
        return 0;
    return(msgctl(qid, IPC_RMID, NULL));
}

/*
 * Get the Queue Info, Return 0 if success, (-1) otherwise.
 * Param: Key ------ queue key
 */
int get_qinfo(key_t Key, struct msqid_ds *Info)
{
    int qid;

    if ((qid = msgget(Key, PERMS | IPC_CREAT)) < 0)
        return -1;
    return(msgctl(qid, IPC_STAT, Info));
}

/*
 * Receiving message from the queue, 
 * Return length of the message if success, (-1) otherwise.
 * Param: Key ------ queue key
 *        Buf ------ buffer for the message
 *        Msgtype -- message type,
 *            = 0 for the first message of the queue
 *            > 0 for the first selected message
 *            < 0 we seldom use this
 *        Flag ----- IPC_NOWAIT for immediatly response
 *                   0 for completing reading
 */
int recv_qmsg(key_t Key, char *Buf, long *Msgtype, int Flag)
{
    int     qid;
    int     len;     /* message length */
    QUEUE   msgbuf; /* message queue */
    
    if ((qid = msgget(Key,PERMS | IPC_CREAT)) < 0)
        return -1;
    if ((len = msgrcv(qid, &msgbuf, sizeof(msgbuf.text), *Msgtype, Flag))<0)
        return -1;
    memcpy(Buf,msgbuf.text,len);
    *Msgtype = msgbuf.msgtype;
    return(len);
}

/*
 * Sending message to the queue, Return 0 if success,(-1) otherwise.
 * Param: Key ------ queue key
 *        Buf ------ buffer for the message
 *        Msgtype -- message type(must be > 0)                *
 *        Flag ----- IPC_NOWAIT for immediatly response
 *                   0 for completing reading
 */
int send_qmsg(key_t Key, long Msgtype, char *Buf, int Len, int Flag)
{
    int   qid;
    QUEUE msgbuf;  /* message queue */

    if ((qid = msgget(Key, PERMS | IPC_CREAT)) < 0)
        return -1;
    if (Len > sizeof(msgbuf.text))
        return -1;
    msgbuf.msgtype = Msgtype;
    memcpy(msgbuf.text,Buf,Len);
    return(msgsnd(qid,&msgbuf,Len,Flag));
}
