#include <linux/kernel.h>
#include <linux/syscalls.h>
#include <linux/string.h>
#include <linux/uaccess.h>
#include <linux/init_task.h>
#include <linux/sched.h>
#include <linux/sched/signal.h>
#include <linux/pgtable.h>
#include <linux/mm_types.h>
#include <linux/mm.h>

SYSCALL_DEFINE3(project2, int, len, void* __user, vaddr, void* __user, paddr)
{
    if(len < 0){
    	return len;
    }
    printk("start\n");
    long err = 0;
    unsigned long vaddr_k[len];
    unsigned long paddr_k[len];
    printk("from start\n");
    printk("from success\n");
    int i = 0;
    for(; i < len; i++){
        printk("vaddr_k[%d]: %lx\n", i, vaddr_k[i]);
    }
    i = 0;
    for(; i < len; i++){
        printk("v to p: %d\n", i);
        pmd_t *pmd = pmd_off(current -> mm, vaddr_k[i]);
        printk("pmd success\n");
        pte_t *pte = pte_offset_map(pmd, vaddr_k[i]);
        printk("pte success\n");
        if (pte_none(*pte)){
            err++;
            printk("pte_none err\n", i);
        }
        printk("pt start: %lx\n", (pte_val(*pte) & PAGE_MASK));
        printk("pt offset: %lx\n", (vaddr_k[i] & ~PAGE_MASK));
        printk("pte: %lx\n", pte_val(*pte));
        printk("pfn: %lx\n", pte_pfn(*pte));
        printk("page: %lx\n", pfn_to_page(pte_pfn(*pte)));
        printk("cheat?: %lx\n", virt_to_phys(vaddr_k[i]));
        //paddr_k[i] = (pte_val(*pte) & PAGE_MASK) | (vaddr_k[i] & ~PAGE_MASK);
        paddr_k[i] = page_to_phys(pte_page(*pte)) + (vaddr_k[i] & ~PAGE_MASK);
        printk("%d paddr_k success\n", i);
    }
    i = 0;
    for(; i < len; i++){
        printk("paddr_k[%d]: %lx\n", i, paddr_k[i]);
    }
    printk("to start\n");
    copy_to_user(paddr, &paddr_k, sizeof(unsigned long)*len);
    printk("to success\n");
    printk("return %d\n", err);
    return err;
}
