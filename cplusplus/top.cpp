#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct        //定义一个cpu occupy的结构体
{
    /*cpu user nice system idle iowait irq softirq*/
    char name[20];      //定义一个char类型的数组名name有20个元素
    unsigned long user; //定义一个无符号的int类型的user
    unsigned long nice; //定义一个无符号的int类型的nice
    unsigned long system;//定义一个无符号的int类型的system
    unsigned long idle; //定义一个无符号的int类型的idle
    unsigned long wa; //定义一个无符号的int类型的idle
    unsigned long ir; //定义一个无符号的int类型的ir
    unsigned long sr; //定义一个无符号的int类型的softirq
}CPU_OCCUPY;

typedef struct       //定义一个mem occupy的结构体
{
    char name[20];      //定义一个char类型的数组名name有20个元素
    unsigned long total;
    char name2[20];
    unsigned long free;
}MEM_OCCUPY;

int get_memoccupy (MEM_OCCUPY *mem) //对无类型get函数含有一个形参结构体类弄的指针O
{
    FILE *fd;
    int n;
    char buff[256];
    MEM_OCCUPY *m;
    m=mem;

    fd = fopen ("/proc/meminfo", "r");

    fgets (buff, sizeof(buff), fd);
    fgets (buff, sizeof(buff), fd);
    fgets (buff, sizeof(buff), fd);
    fgets (buff, sizeof(buff), fd);
    sscanf (buff, "%s %u %s", m->name, &m->total, m->name2);

    fgets (buff, sizeof(buff), fd); //从fd文件中读取长度为buff的字符串再存到起始地址为buff这个空间里
    sscanf (buff, "%s %u", m->name2, &m->free, m->name2);

    fclose(fd);     //关闭文件fd
    return 0;
}

double cal_cpuoccupy (CPU_OCCUPY *o, CPU_OCCUPY *n)
{
    unsigned long od, nd;
    unsigned long id, sd;
    double cpu_use = 0.0;

    od = (unsigned long) (o->user + o->nice + o->system + o->idle + o->wa + o->ir + o->sr);
    nd = (unsigned long) (n->user + n->nice + n->system + n->idle + n->wa + n->ir + n->sr);

    id = (unsigned long) (n->idle - o->idle);
    sd = (unsigned long) (n->system - o->system);
    if(nd > od) {
        cpu_use = (double)(id * 1.0) * 100.0/(double)(nd-od);
        cpu_use = 100.0 - cpu_use;
    } else if (nd == od){
        cpu_use = 0;
    } else {
        cpu_use = -1;
    }
    return cpu_use;
}

int get_cpuoccupy (CPU_OCCUPY *cpust) //对无类型get函数含有一个形参结构体类弄的指针O
{
    FILE *fd;
    int n;
    char buff[1024];
    CPU_OCCUPY *cpu_occupy;
    cpu_occupy=cpust;

    fd = fopen ("/proc/stat", "r");
    fgets (buff, sizeof(buff), fd);
    //printf("proc/stat: %s\n", buff);
    sscanf (buff, "%s %lu %lu %lu %lu %lu %lu", cpu_occupy->name, &cpu_occupy->user,
        &cpu_occupy->nice,&cpu_occupy->system, &cpu_occupy->idle, &cpu_occupy->wa,
        &cpu_occupy->ir, &cpu_occupy->sr);
    /*printf("cpu us=%lu, ni=%lu, sys=%lu, idle=%lu, wa=%lu, ir=%lu, sr=%lu\n", cpu_occupy->user,
                                                        cpu_occupy->nice,
                                                        cpu_occupy->system,
                                                        cpu_occupy->idle,
                                                        cpu_occupy->wa,
                                                        cpu_occupy->ir,
                                                        cpu_occupy->sr);
   */
    fclose(fd);
    return 0;
}

int main()
{
    CPU_OCCUPY cpu_stat1;
    CPU_OCCUPY cpu_stat2;
    MEM_OCCUPY mem_stat;
    double cpu;

    //获取内存
    get_memoccupy ((MEM_OCCUPY *)&mem_stat);

    while(1) {
        //第一次获取cpu使用情况
        get_cpuoccupy((CPU_OCCUPY *)&cpu_stat1);
        sleep(2);

        //第二次获取cpu使用情况
        get_cpuoccupy((CPU_OCCUPY *)&cpu_stat2);

        //计算cpu使用率
        cpu = cal_cpuoccupy ((CPU_OCCUPY *)&cpu_stat1, (CPU_OCCUPY *)&cpu_stat2);
        printf("cpu.usage=%.2lf\n", cpu);
    }

    return 0;
}
