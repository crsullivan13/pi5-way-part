#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/arm-smccc.h>

#define L3_CONFIG_SMC 0x82000000

//void set_cluster_thread_sid_el1(uint64_t value);
//static void configure_l3_partitions();

// //select between the L3 cache partitions allocated
// void set_cluster_thread_sid_el1(uint64_t value) {
//     asm volatile("msr S3_0_C15_C4_0, %0" :: "r" (value));
// }


static void configure_l3_partitions(void) {
    struct arm_smccc_res res;

    arm_smccc_smc(L3_CONFIG_SMC, 0x00008601, 0x0, 0x0, 0x00060002, 0, 0, 0, &res);

    if ( res.a0 == 0 ) {
        printk(KERN_INFO "L3 cache partition setup successful\n");
    } else {
        printk(KERN_ERR "L3 cache partition setup failed: %lx\n", res.a0);
    }
}

static int __init cache_part_init(void) {
    configure_l3_partitions();
    return 0;
}

static void __exit cache_part_exit(void) {
    printk(KERN_INFO "L3 cache partition exit\n");
}

module_init(cache_part_init);
module_exit(cache_part_exit);


MODULE_LICENSE("GPL");
MODULE_AUTHOR("csul");
MODULE_DESCRIPTION("L3 Cache Partitioning Setup Module");