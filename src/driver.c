
#include "driver.h"

#define MAX_MASTER_NODES 256

MODULE_LICENSE("Dual MIT/GPL");


static int     dev_open(struct inode * my_inode, struct file * my_file) {
    return 0; 
}
static int     dev_release(struct inode * my_inode, struct file * my_file) {
    return 0; 

}
static ssize_t dev_read(struct file * my_file, char * buffer, size_t len, loff_t * offset) {
    return 0; 

}
static ssize_t dev_write(struct file * my_file, const char * buffer, size_t len, loff_t * off) {
    return 0; 

}

static struct file_operations fops =
{
   .open = dev_open,
   .read = dev_read,
   .write = dev_write,
   .release = dev_release,
};

static struct semaphore master_nodes_sem;
int master_node_count = 0;
struct master_node master_nodes[MAX_MASTER_NODES];

int driver_prepare(void) {
    sema_init(&master_nodes_sem, 1);
    return 0; 
}

int register_master(char* name) {
    down(&master_nodes_sem);
    if (master_node_count == MAX_MASTER_NODES) {
        return -1; 
    } 
    sprintf(master_nodes[master_node_count].name, "%s", name);
    sema_init(&master_nodes[master_node_count].sem, 1);
    master_node_count++;
    up(&master_nodes_sem);
    return master_node_count - 1;
}

int create_master_node(int id) {
    if (id >= MAX_MASTER_NODES) {
        printk(KERN_ALERT "Too high master node id\n");
        return 0;
    }
    down(&master_nodes[id].sem);
    master_nodes[id].majorNumber = register_chrdev(0, master_nodes[id].name, &fops);
    if (master_nodes[id].majorNumber<0){
        printk(KERN_ALERT 
            "deus: failed to register a major number for master %s\n", 
            master_nodes[id].name
        );
        up(&master_nodes[id].sem);
        return 0;
    }
    master_nodes[id].charClass = class_create(THIS_MODULE, master_nodes[id].name);
    if (IS_ERR(master_nodes[id].charClass)){                // Check for error and clean up if there is
        unregister_chrdev(master_nodes[id].majorNumber, master_nodes[id].name);
        printk(KERN_ALERT "Failed to register device class\n");
        up(&master_nodes[id].sem);
        return 0;
    }
    printk(KERN_INFO "%s: device class registered correctly\n", master_nodes[id].name);

    master_nodes[id].charDevice = device_create(
        master_nodes[id].charClass, 
        NULL, 
        MKDEV(master_nodes[id].majorNumber, 0), 
        NULL, 
        master_nodes[id].name
    );
    if (IS_ERR(master_nodes[id].charDevice)){               // Clean up if there is an error
        class_destroy(master_nodes[id].charClass);           // Repeated code but the alternative is goto statements
        unregister_chrdev(master_nodes[id].majorNumber, master_nodes[id].name);
        printk(KERN_ALERT "Failed to create the device\n");
        up(&master_nodes[id].sem);
        return 0;
    }
    printk(KERN_INFO "%s: device class created correctly\n", master_nodes[id].name);
    up(&master_nodes[id].sem);
    return 1;
}

int unregister_master(int id) {
    return 0;
}


struct deus_event* wait_deus_event(int master_id) {
    return NULL;
}

int send_deus_event(int master_id, int type, struct deus_event_params params) {
    return -1;
}

