#include <kpm.h>
#include <sys/fs.h>

KPM_MODULE("cpuspoof");

static const char fake_cpuinfo[] =
"processor\t: 0\n"
"model name\t: Snapdragon 8 Gen2\n"
"cpu MHz\t\t: 3200\n"
"Hardware\t: Qualcomm SM8475\n";

static long (*original_read)(struct file *file, char __user *buf, size_t count, loff_t *ppos);

static long hooked_read(struct file *file, char __user *buf, size_t count, loff_t *ppos)
{
    if(strcmp(file->f_path.dentry->d_name.name,"cpuinfo")==0){
        size_t len = strlen(fake_cpuinfo);
        if(*ppos >= len) return 0;
        if(count > len - *ppos) count = len - *ppos;
        copy_to_user(buf, fake_cpuinfo + *ppos, count);
        *ppos += count;
        return count;
    }
    return original_read(file,buf,count,ppos);
}

int kpm_main(void)
{
    KPM_HOOK_SYMBOL_FS(read, original_read, hooked_read);
    return 0;
}
