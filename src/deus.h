#ifndef DEUS_COMMANDS_H
#define DEUS_COMMANDS_H

#include <linux/ioctl.h>
#include <linux/types.h>

#define IOCTL_NUMBER 101

#define IOCTL_REGISTER_MASTER 	_IOW(IOCTL_NUMBER, 1, char*)
struct event_handling_result {
    char* data; 
    size_t size;
};
#define IOCTL_SEND_EVENT_RESULT 	_IOW(IOCTL_NUMBER, 2, struct event_handling_result)

#define DEUS_EVENT_TYPE_READ 1
#define DEUS_EVENT_TYPE_WRITE 2
struct deus_event_params {
    char *buffer; 
    size_t len; 
    loff_t *offset;
};
/*
Returns integer indicating type of event. See DEUS_EVENT_TYPE_* macros in this file 
*/
#define IOCTL_MASTER_WAIT_EVENT _IOR(IOCTL_NUMBER, 3, struct deus_event_params*)




#endif //  DEUS_COMMANDS_H
