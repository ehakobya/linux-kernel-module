/**
 * HakobyanLKM.c
 * 
 * Author: Edgar Hakobyan
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/list.h>
#include <linux/tty.h> 

#define KERN_INFO       KERN_SOH "6"    /* informational */

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Edgar Hakobyan");
MODULE_DESCRIPTION("Assignment 5");

static int inputPID = 0;

module_param(inputPID, int, 0); 
MODULE_PARM_DESC(inputPID, "Input parameter for displaying processes with PID greater than input.");

/**
* @brief This function is called when the module is loaded into the kernel
*/
static int __init ModuleInit(void) {

       struct task_struct *task;
       struct task_struct *child;

       for_each_process(task) {
              if (task->pid > inputPID) {
                     // display details of processes that have pid greater than the input
                     printk(KERN_INFO "Loaded Kernel Module");

                     printk(KERN_INFO "%25s%10s%10s%10s%10s%15s\n", "PROCESS","PID","STATE","PRIO","ST_PRIO","NORM_PRIO");
					 
					 printk(KERN_INFO "%25s%10d%10ld%10d%10d%15d\n",task->comm,task->pid,task->__state,task->prio,
                                                               task->static_prio, task->normal_prio);

			//Iterates through each process again looking for children
			for_each_process(child){
				//If task is a childs parent
				if(child->real_parent->pid == task->pid) {
					printk(KERN_INFO "%25s\n", "CHILD");
					printk(KERN_INFO "%25s%10d%10ld%10d%10d%15d\n", child->comm,child->pid,child->__state,
						child->prio,child->static_prio, child->normal_prio);					
				}
			}

			//Print parameters for the real parent process
			printk(KERN_INFO "%25s\n","PARENT");
			printk(KERN_INFO "%25s%10d%10ld%10d%10d%15d\n", task->real_parent->comm,
				task->real_parent->pid, task->real_parent->__state,task->real_parent->prio,
				task->real_parent->static_prio,	task->real_parent->normal_prio);
			
			//Print a separator between tasks
			printk(KERN_INFO "\n\n");
              }
       }

       return 0;
}

/**
* @brief This function is called when the module is removed from the kernel
*/
static void __exit ModuleExit(void) {
       printk("Removed Kernel Module\n");
}

module_init(ModuleInit);
module_exit(ModuleExit);