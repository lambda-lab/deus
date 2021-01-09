#ifndef DEUS_DRIVER_H
#define DEUS_DRIVER_H

#include "deus.h"
#include <linux/init.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/wait.h>
#include <linux/interrupt.h>
#include <linux/workqueue.h>
#include <linux/jiffies.h>

#include <linux/uaccess.h>

struct master_node {
    char* name; 
    int majorNumber; 
    struct class*  charClass; ///< The device-driver class struct pointer
    struct device* charDevice; ///< The device-driver device struct pointer
    struct file* masterFile; 
    struct semaphore sem;

};

struct deus_event {
    int type;
    struct deus_event_params params; 
    struct event_handling_result result;
};

int driver_prepare(void);

// master registration and device creation 
/*
    Create new master node and add it to list of master nodes with its name
    Returns: identifier for given master node which is referenced in other functions 
*/
int register_master(char* name); 
/*
    Create device for given master id. Returns number of devices created (0 or 1). 
*/
int create_master_node(int id);

/*
    Removes given master node from master node list. 
    Returns: number of master nodes removed. 
*/
int unregister_master(int id); 

// event handling 
// Returns first event in FIFO
struct deus_event* wait_deus_event(int master_id); 

// Adds event at the end of FIFO
int send_deus_event(int master_id, int type, struct deus_event_params params);





#endif // DEUS_DRIVER_H